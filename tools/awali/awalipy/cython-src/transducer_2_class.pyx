# This file is part of Awali.
# Copyright 2016-2023 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
#
# Awali is a free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

cdef Transducer _Transducer(basic_automaton_t tdc):
    """Wraps a basic_automaton_t (C++ class) in a Transducer (python class).

    Args:
        tdc (dyn::basic_automaton_t):  the transducer to wrap.

    Returns:
        Transducer: the wrapped transducer.

    Notes:
        Internal function.
        Argument <tdc> is not verified to actually be transducer.
        Use with caution.
    """
    a= Transducer([],"hack!!")
    a._set_cpp_class(tdc)
    return a


## ========================================================================= ##
## ========================================================================= ##
## ========================================================================= ##
## ========================================================================= ##
cdef class Transducer(_BasicAutomaton):

    cdef str semiring #this is a hack to delete at some point

    cdef simple_transducer_t _to_cpp_class(self):
        return (<simple_transducer_t>self._this)

    def __init__(self, list alphabets, weightset=None):
        """
        Usage: Transducer(alphabets [,weightset='B'])

        Description: builds an empty (weighted) transducer whose transitions are labelled in <alphabet> and weighted in <semiring>

        Parameters:
            - alphabets: list of str
            - semiring: str of {'B', 'Z', 'Z-min-plus', 'Z-max-plus', 'Q', 'R', 'R-max-prod', 'F2' }, default is 'B'; or an instance of class WeightSet.
        """
        if weightset is None:
            self._this = make_simple_transducer1(alphabets)
        else:
            if weightset != "hack!!":
                if isinstance(weightset, WeightSet):
                  weightset= str(weightset)
                self._this = make_simple_transducer2(alphabets, weightset)
            else:
                return # returning an empty shell but it's OK!
        self._recompute_state_names();
                


    def __call__(self, other):
        return self.eval(other).aut_to_exp()


    def __repr__(self):
        return self.__str__()

    def __str__(self):
        result=  "Transducer ("
        result+= self.get_static_context()+"):"
        result+= "\tWeight Set: "+str(self.get_weightset())
        result+= "\tAlphabets: "+str(self.alphabets())+"\n"
        result+= "States:{"
        weight_one= self.get_weightset().one()
        states = self.states();
        strlist=[]
        for s in states:
            c = '('
            e = ''
            str_s= ""
            str_s+= str(s)
            if self.is_initial(s):
                str_s+= "(i"
                c = ','
                e = ')'
                weight= self.get_initial_weight(s)
                if (weight != weight_one):
                    str_s+= ':'+weight
            if self.is_final(s):
                str_s+= c+'f'
                e = ')'
                weight= self.get_final_weight(s)
                if (weight != weight_one):
                    str_s+= ':'+weight
            str_s+= e
            result+= "\t"+str_s
        result+="\t}\n"
        result+="Transitions:{"
        transitions = self.transitions()
        for t in transitions:
            result+= '\t'
            result+= str(self.src_of(t))
            result+= '--'
            weight= self.weight_of(t)
            if (weight != weight_one):
                result+= '<'+weight+'>'
            result+='('
            e=''
            for i in self.label_of(t):
                result+=e
                result+=i
                e=','
            result+=')'
            result+='-->'
            result+= str(self.dst_of(t))
        result+="\t\t}"
        return result




## ========================================================================= ##
## == Non-method equivalent function in fused_Automaton_or_Transducer.pyx == ##
## ========================================================================= ##
    def eval(self, RatExp_or_Str_or_Aut other):
        """
        Usages:
            eval(tdc, word)
            eval(tdc, exp)
            eval(tdc, aut)

        Descriptions:  returns an automaton or a transducer whose \
behaviour is the image(s) of the second argument (either <word>, <exp> or \
<automaton>) by <tdc>.

        Args:
            tdc (Transducer)
            word (str)
            exp (RatExp)
            aut (Automaton)

        Returns:  Automaton or Transducer, having one less tape than <tdc>
        """
        if isinstance(other, str):
            return self.eval(RatExp(other, self.get_weightset()))
        if isinstance(other, RatExp):
            return self.eval(other.exp_to_aut("derived_term"))
        if isinstance(other, Automaton):
            return _Automaton(eval_tdc_(Automaton._to_cpp_class(other), self._to_cpp_class()))
        raise TypeError('Expression to evaluate should be of one of the following types: str, RatExp or Automaton')


## ========================================================================= ##
    def set_transition(self, src, dst, list labels, str new_weight=None):
        """
        Usages:  tdc.set_transition(src, dst, labels [, new_weight=tdc.get_weightset().one() ] )

        Description:  set as <new_weight> the weight of the transition \
going from <src> to <dst> and labelled by <label> (resp. by  <labels>); \
creates the transition if necessary; overwrites the previous weight if the \
transition already existed.

        Args:
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            label (str)
            labels (list of str)
            new_weight (str, optional)
                defaults tdc.get_weightset().one()
        """
        if new_weight is None:
            return self._to_cpp_class().set_transition3(self._id_or_name(src), self._id_or_name(dst), labels)
        else:
            return self._to_cpp_class().set_transition4(self._id_or_name(src), self._id_or_name(dst), labels, new_weight)


## ========================================================================= ##
    def add_transition(self, src, dst, list labels, str weight=None):
        """
        Usages:  tdc.add_transition(src, dst, labels [, weight=tdc.get_weightset().one() ] )

        Description:  add <weight> to the weight of the transition \
going from <src> to <dst> and labelled by <labels>; creates the \
transition if necessary.

        Args:
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            labels (list of str)
            weight (str, optional)
                defaults to tdc.get_weightset().one()

        Returns:  str, the new weight of the transition
        """
        if weight is None:
            return self._to_cpp_class().add_transition3(self._id_or_name(src), self._id_or_name(dst), labels)
        else:
            return self._to_cpp_class().add_transition4(self._id_or_name(src), self._id_or_name(dst), labels, weight)


## ========================================================================= ##
    def has_transition(self, source_or_transition, dst=None, list labels=None):
        """
        Usages:
            tdc.has_transition(tr_id)
            tdc.has_transition(src, dst, labels)

        Description:  Returns True if the transition given as argument, or \
determined by the arguments, exists in <tdc/self>.

        Arguments:
            tr_id (int), identifier of the transition
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            labels (list of str), labels of the transition

        Returns:
            bool, True if the transition exists.
        """
        if ((dst is None) != (labels is None)): #xor
            _bad_arg_number_("Transducer", "has_transition", [1,3])
        if dst is None:
            return _BasicAutomaton.has_transition(self, source_or_transition)
        else:
            return  self._to_cpp_class().has_transition3(self._id_or_name(source_or_transition), self._id_or_name(dst), labels)


## ========================================================================= ##
    def del_transition(self, trans_or_src, dst= None, list labels= None):
        """
        Usages:
            [a]  tdc.del_transition(tr_num)
            [b]  tdc.del_transition(src, dst)
            [c]  tdc.del_transition(src, dst, labels)

        Description:
            [a,c]  removes from <tdc/self> the unique transition \
determined either by an identifier <tr_num>, or by a source <src>, \
a destination <dst> and a tuple of <labels>
            [b]  removes from <tdc/self> every transition going from <src> to <dst>

        Args:
            tr_num (int), represents a transition
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            labels (list of str)
        """
        if labels is None:
            _BasicAutomaton.del_transition(self, trans_or_src, dst)
        else:
            self._to_cpp_class().del_transition3(self._id_or_name(trans_or_src), self._id_or_name(dst), labels)


## ========================================================================= ##
    def get_transition(self, src, dst, list labels):
        """
        Usages:  tdc.get_transition(src_id, dst_id, labels)

        Description:  returns the identifier of the transition of <tdc/self> \
going from <src> to <dst> and labelled by <labels.

        Args:
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            labels (list of str)

        Returns:
            int, identifier of a transition
        """
        return self._to_cpp_class().get_transition(self._id_or_name(src), self._id_or_name(dst), labels)


## ========================================================================= ##
    def label_of(self, int tr_id):
        """
        Usage:  tdc.label_of(tr_id)

        Description:  Returns the labels of a transition.

        Args:  tr_id (int), identifier of a transition

        Returns:  list of str
        """
        return self._to_cpp_class().label_of(tr_id)


## ========================================================================= ##
    def incoming(self, stt, list labels=None):
        """
        Usage: tdc.incoming(stt [, labels ])

        Description:  Returns the identifiers of all incoming transitions of \
<stt> in <tdc/self>; \
if <labels> is provided, returns only the identifiers of the \
    transitions labelled by <labels>.

        Args:
            stt (int or str): state identifier (int) or state name (str)
            labels (list of str, optional)

        Returns:  list of int, identifiers of transitions
        """
        if labels is None:
            return _BasicAutomaton.incoming(self, self._id_or_name(stt))
        else:
            return self._to_cpp_class().incoming2(self._id_or_name(stt), labels)


## ========================================================================= ##
    def outgoing(self, stt, list labels=None):
        """
        Usage:  tdc.outgoing(stt [,labels])

        Description:  returns the identifiers of all outgoing transitions of \
<stt> within <tdc/self>; \
if <labels> is provided, returns only the identifiers of the \
transitions labelled by <labels>.

        Args:
            stt (int or str): state identifier (int) or state name (str)
            labels (list of str, optional)

        Returns:  list of int, identifiers of transitions
        """
        if labels is None:
            return _BasicAutomaton.outgoing(self, self._id_or_name(stt))
        else:
            return self._to_cpp_class().outgoing2(self._id_or_name(stt), labels)


## ========================================================================= ##
    def predecessors(self, stt, list labels=None):
        """
        Usage:  tdc.predecessors(stt [, labels])

        Description:  returns the identifiers of all the states that are \
predecessors of <stt> within <tdc/self>; \
if <labels> is provided, returns only the identifiers of the \
states s such that there exists a transition from s to <stt> labelled by <labels>.

        Args:
            stt (int or str): state identifier (int) or state name (str)
            labels (list of str, optional)

        Returns:  list of int, identifiers of states
        """
        if labels is None:
            return _BasicAutomaton.predecessors(self, self._id_or_name(stt))
        else:
            return self._to_cpp_class().predecessors2(self._id_or_name(stt), labels)


## ========================================================================= ##
    def successors(self, stt, list labels=None):
        """
        Usage:  tdc.successors(stt [, labels])

        Description:  returns the identifiers of all the states that are \
successors of <stt> within <tdc/self>; \
if <labels> is provided, returns only the identifiers of the \
states s such that there exists a transition going from <stt> to s and \
labelled by <labels>.

        Args:
            stt (int or str): state identifier (int) or state name (str)
            labels (list of str, optional)

        Returns:  list of int, identifiers of states
        """
        if labels is None:
            return _BasicAutomaton.successors(self, self._id_or_name(stt))
        else:
            return self._to_cpp_class().successors2(self._id_or_name(stt), labels)


### ========================================================================= ##
    #def copy(self):
        #"""
        #Usage:  tdc.copy()

        #Description:  builds a copy of <tdc/self>

        #Returns:  Transducer
        #"""
        #return _Transducer(copy_(self._this))





    def lift(self): return _Transducer(lift_(self._this))



## ========================================================================= ##
    def has_letter(self, string letter, int tape_num):
        """
        Usage:  tdc.has_letter(letter, tape_num)

        Description:  returns True if <letter> belongs to the alphabet of the <tape_num>-th tape of <tdc/self>

        Returns: bool
        """
        return self._to_cpp_class().has_letter(letter, tape_num)


## ========================================================================= ##
    def unpack_transition(self, int tr_id):
        """
        usage:  tdc.unpack_transition(tr_id)

        Description:   returns the details of the transition idenfitied by <tr_identifier> as a Tuple containing, in that order, the source-state id, the weight, the labels and the destination-state id.

        Args:  tr_id(int), identifier of a transition

        Returns: Tuple (int, str, list of str, int)
        """
        return (self.src_of(tr_id), self.label_of(tr_id), \
                self.weight_of(tr_id), self.dst_of(tr_id))

## ========================================================================= ##
## ====== Non-method equivalent function in transducer_3_function.pyx ====== ##
## ========================================================================= ##
    def num_tapes(self):
        """
        Usage:  tdc.num_tapes()

        Description: returns the number of tapes of <tdc/self>

        Returns: int
        """
        return self._to_cpp_class().num_tapes()



## ========================================================================= ##
    def alphabets(self):
        """
        Usage:  tdc.alphabets()

        Description:  returns the alphabets of the different tapes of <tdc/self>

        Returns:  list of str
        """
        res=[]
        for x in self._to_cpp_class().alphabets():
            res.append(str(x))
        return res;


## ========================================================================= ##
    def domain(self):
        """
        Usage:  tdc.domain()

        Description:  return the automaton accepting the domain of the function realized by <tdc/self>

        Returns: Automaton
        """
        return _Automaton(domain_(self._to_cpp_class()))


## ========================================================================= ##
    def image(self):
        """
        Usage:  tdc.domain()

        Description:  return the automaton accepting the image of the function realized by <tdc/self>

        Precondition: <tdc> must be a two tape transducer.

        Returns: Automaton
        """
        return _Automaton(image_(self._to_cpp_class()))


## ========================================================================= ##
    def images(self):
        """
        Usage:  tdc.domain()

        Description:  returns the automaton or the transducer accepting the images of the function realized by <tdc/self>

        Returns: Automaton or Transducer
        """
        return _BasicAutomaton_(images_(self._to_cpp_class()))


## ========================================================================= ##
    def projection(self, int tape_num):
        """
        Usage:  tdc.projection(i)

        Description:  returns the automaton accepting the language of the <tape_num>-th tape of <tdc/self>

        Args:  tape_num (int)
        Returns: Automaton
        """
        return _Automaton(projection_(self._to_cpp_class(), tape_num))


## ========================================================================= ##
    def inverse(self):
        """
        Usage:  tdc.inverse()

        Description:  returns a transducer that realizes the inverse relation of the one realized by <tdc>; effectively permuting the two tapes of <tdc>

        Precondition: <tdc> must be a two-tape Transducer

        Returns: Transducer
        """
        return _Transducer(inverse_(self._to_cpp_class()))


## ========================================================================= ##
    def compose(self, Transducer other_tdc):
        """
        Usage:  tdc.compose(other_tdc)

        Description:  returns the transducer realizing the composition of the relations realized by <tdc/self> and <other_tdc>

        Args:  other_tdc (Transducer)

        Precondition:  the alphabet of first tape of <other> must be equal to the one of the last tape of <tdc/self>

        Returns Automaton
        """
        return _Transducer(compose_(self._to_cpp_class(), other_tdc._to_cpp_class()))


## ========================================================================= ##
    def is_functional(self):
        """
        Usage:  tdc.is_functional()

        Description:  returns True if the relation realized by <tdc/self> is a function, that is if every word from its domain has one image.

        Returns: bool
        """
        return is_functional_(self._to_cpp_class())


## ========================================================================= ##
    def synchronize(self):  return _Transducer(synchronize_(self._to_cpp_class()))


## ========================================================================= ##
    def is_synchronizable(self):  return is_synchronizable_(self._to_cpp_class())















