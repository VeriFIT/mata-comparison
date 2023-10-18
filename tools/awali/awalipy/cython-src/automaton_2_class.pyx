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

cdef Automaton _Automaton(basic_automaton_t aut):
    """Wraps a basic_automaton_t (C++ class) in an Automaton (python class).

    Args:
        aut (dyn::basic_automaton_t):  the automaton to wrap.

    Returns:
        Automaton, the wrapped automaton.

    Note:
        Internal function.
        Argument <aut> is not verified to actually be an automaton.
        Use with caution.
    """
    a= Automaton("", empty_shell=True)
    a._set_cpp_class(aut)
    return a

cdef class Automaton(_BasicAutomaton):

    cdef simple_automaton_t _to_cpp_class(self):
        """
        Usage:  aut._to_cpp_class()

        Description:  returns the wrapped object explicitly casted to simple_automaton_t.
        """
        return (<simple_automaton_t>self._this)

    def __init__(self, *args, **kwargs):
        """
        Usage:
            [a] Automaton(aut)
            [b] Automaton(ratexp)
            [c] Automaton(alphabet [, weightset="B"] )
            [d] Automaton(...)

        Description:
            [a] builds a copy of <aut>
            [b] computes a (weighted) automaton accepting the rational expression <ratexp> using the default algorithm
            [c] builds an empty (weighed) automaton over the alphabet and weight-set given
            [d] builds an empty (weighed) automaton over the alphabet and weight-set given then possibly do additional basic operations depending on the optional named argument(s) given.

        Args:
            aut (Automaton)
            ratexp (RatExp)
            alphabet (str), each character of this  will be a letter of the produced-automaton alphabet
            weighset (str or WeightSet, optional)
                admissible values are 'B', 'Z', 'Z-min-plus', 'Z-max-plus', 'Q', 'R', 'R-max-prod', 'F2' and 'Z/<int>Z"                defaults to 'B'

        Named argument authorized for [d] (also works for [c])
            alphabet (str), same as above
            weightset (str or WeightSet, optional), same as above
            size (int), number of states of the newly created automaton
                defaults to 0
            finals (list of (int or (int * str))), states to be rendered final (optionally with weight)
                defaults to []
            initials (list of (int or (int * str)))
                defaults to []
            allow_eps (bool), whether the automaton allows epsilon transition
                defaults to False

        Example for [d]:
            Automaton(alphabet="abc", weightset="Z", size=4, finals=[(2,"10"),3], allow_eps=True)builds an automaton
                over the alphabet {a,b,c},
                whose transitions are weighed on (Z,+,x),
                with four states,
                such that the state n°2 is final with final-weight 10 and the state n°3 is final with final-weight 1 (ie. the multiplicative neutral element of the weight-set)
                and that allows epsilon transitions
            Automaton("abc", "Z", size=4, finals=[(2,"10"),3], allow_eps=True) produces the same result
        """
        if len(args)==1:
            if isinstance(args[0], Automaton):
                self._set_cpp_class_bis(args[0].copy())
                return
            if isinstance(args[0], RatExp):
                self._set_cpp_class_bis(args[0].exp_to_aut())
                return
        alphabet= None
        weightset= None
        size= 0
        initials=[]
        finals=[]
        NFA= False
        if len(args)>0:
            alphabet=args[0]
        if len(args)>1:
            weightset=args[1]
        for k,v in kwargs.items():
            if (k=="alphabet"): alphabet= v
            elif (k=="weightset"):  weightset= v
            elif (k=="size"):  size= v
            elif (k=="initials"):  initials= v
            elif (k=="finals"):  finals= v
            elif (k=="allow_eps"):  NFA= v
            elif (k=="empty_shell"): return
            else: raise ValueError("Unrecognised named argument: "+k)
        if alphabet is None:
            raise ValueError("No alphabet was provided. One must be given, either as a named or positional parameter.")
        if weightset is None:
            if NFA:
                self._this= make_NFA_with_eps1(alphabet)
            else:
                self._this= make_simple_automaton1(alphabet)
        else:
            if isinstance(weightset, WeightSet):
              weightset = str(weightset)
            if NFA:
                self._this= make_NFA_with_eps2(alphabet, weightset)
            else:
                self._this= make_simple_automaton2(alphabet, weightset)
        #Adding states
        for i in range(size):
            self.add_state()
        #Setting initials states
        if isinstance(initials , dict):
            initials= initials.items()
        for t in initials:
            if isinstance(t, tuple):
                self.set_initial(t[0], t[1])
            elif isinstance(t, list):
                self.set_initial(t[0], t[1])
            else:
                self.set_initial(t)
        #Setting final states
        if isinstance(finals , dict): ##If a dictionary was provided, make a list of pair
            finals= finals.items()
        for t in finals:
            if isinstance(t, tuple):
                self.set_final(t[0], t[1])
            elif isinstance(t, list):
                self.set_final(t[0], t[1])
            else:
                self.set_final(t)
        self._recompute_state_names();


    def __call__(self, str word):
        return self.eval(word)


    def __repr__(self):
      return self.__str__()

    def is_eps_allowed(self):
        """
        Usage:  aut.is_eps_allowed()

        Description:  returns True if <self/aut> allows epsilon-transitions.
        """
        return self._to_cpp_class().is_eps_allowed();

    def __str__(self):
        result=  "Automaton (static context: "
        result+= self.get_static_context()+")\n"
        result+= "Weightset: "+str(self.get_weightset())
        result+= "\tAlphabet: "+self.alphabet()
        result+= "\tEpsilon-transitions: "
        if self.is_eps_allowed():
            result += "Allowed\n"
        else:
            result +="Disallowed\n"
        result+= "- States:{"
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
                    str_s+= ':'+str(weight)
            if self.is_final(s):
                str_s+= c+'f'
                e = ')'
                weight= self.get_final_weight(s)
                if (weight != weight_one):
                    str_s+= ':'+str(weight)
            str_s+= e
            result+= "\t"+str_s
            #result+= strlist[-1]
        result+="\t}\n"
        result+="- Transitions:{"
        transitions = self.transitions()
        for t in transitions:
            result+= '\t'
            result+= str(self.src_of(t))
            result+= '-'
            weight= self.weight_of(t)
            if (weight != weight_one):
                result+= '<'+weight+'>'
            result+= self.label_of(t)
            result+='->'
            result+= str(self.dst_of(t))
        result+="\t\t}"
        return result


## ========================================================================= ##
## =========== Non-method equivalent function in fused_type.pyx ============ ##
## ========================================================================= ##
    cpdef str eval(self, str word):
        """
        Usage:  aut.eval(word)

        Description:  returns the weight of <word> in the automaton \
<aut/self>.

        Args:  word (str)
        """
        return eval_(self._to_cpp_class(), word)


## ========================================================================= ##
    def null_state(self):
        """
        Usage:  aut_or_tdc.null_state()

        Description:  Returns the standard phony state id, that is returned when looking for a state and none was found.

        Returns:  the phony state id
        """
        return self._to_cpp_class().null_state()


## ========================================================================= ##
    def null_transition(self):
        """
        Usage:  aut_or_tdc.null_state()

        Description:  Returns the standard phony state id, that is returned when looking for a state and none was found.

        Returns:  the phony state id
        """
        return self._to_cpp_class().null_transition()


## ========================================================================= ##
    def set_transition(self, src, dst, str label, str new_weight=None):
        """
        Usage:
            aut.set_transition(src, dst, label [, new_weight=aut.get_weightset().one() ] )

        Description:  sets as <new_weight> the weight of the transition \
going from <src> to <dstd> and labelled by <label>; \
overwrites the eventual previous weight; \
creates the transition if necessary.

        Args:
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            label (str)
            new_weight (str, optional),
                defaults to aut.get_weightset().one().
        
        Returns: (int), representing a transition
        """
        if new_weight is None:
            return self._to_cpp_class().set_transition3(self._id_or_name(src), self._id_or_name(dst), label)
        else:
            return self._to_cpp_class().set_transition4(self._id_or_name(src), self._id_or_name(dst), label, new_weight)


## ========================================================================= ##
    def add_transition(self, src, dst, str label, str weight=None):
        """
        Usage:  aut.add_transition(src, dst, label [, weight=aut.get_weightset().one() ] )

        Description:  adds in <aut> some weight to the transition \
going from <src> to <dst> and labelled by <label>; creates the \
transition if necessary.

        Args:
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            label (str)
            weight (str, optional)
                defaults to aut.get_weightset().one()

        Returns:  str, the new weight of the transition
        """
        if weight is None:
            return self._to_cpp_class().add_transition3(self._id_or_name(src), self._id_or_name(dst), label)
        else:
            return self._to_cpp_class().add_transition4(self._id_or_name(src), self._id_or_name(dst), label, weight)


## ========================================================================= ##
    def has_transition(self, source_or_transition, object dst=None, str label=None): #3
        """
        Usage:
            aut.has_transition(tr_id)
            aut.has_transition(src, dst, label)

        Description:  Returns True if the transition given as argument, or \
determined by the arguments, exists in <aut/self>.

        Arguments:
            tr_id (int), identifier of the transition
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            label (str), label of the transition

        Returns:
            bool, True if the transition exists.
    """
        if ((dst is None) != (label is None)):
            _bad_arg_number_("Automaton", "has_transition", [1,3])
        if dst is None:
            return _BasicAutomaton.has_transition(self,source_or_transition)
        else:
            return  self._to_cpp_class().has_transition3(self._id_or_name(source_or_transition), self._id_or_name(dst), label)


## ========================================================================= ##
    def del_transition(self, int tr_or_src, dest=None, str label=None):
        """
        Usage:
            [a]  aut.del_transition(tr_id)
            [b]  aut.del_transition(src, dst)
            [c]  aut.del_transition(src, dst, label)

        Description:
            [a,c]  removes from <aut/self> the unique transition \
determined either by an identifier <tr_id>, or by a source <src>, \
a destination <dst> and and a <label>
            [b]  removes from <aut/self> every transition going from \
<src> to <dst>

        Args:
            tr_id (int), identifier of a transition
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            label (str)
        """
        if label is None:
            _BasicAutomaton.del_transition(self, tr_or_src, dest)
        else:
            self._to_cpp_class().del_transition3(tr_or_src, dest, label)


## ========================================================================= ##
    def get_transition(self, src, dst, str label):
        """
        Usage:
            aut.get_transition(src, self._id_or_name(dst), label)

        Description:  returns the identifier of the transition of <aut/self> going from <src> to <dst> and labelled by <label>.

        Args:
            src (int or str): state identifier (int) or state name (str) of the source
            dst (int or str): state identifier (int) or state name (str) of the destination
            label (str)

        Returns:
            int, identifier of a transition
        """
        return self._to_cpp_class().get_transition(self._id_or_name(src), self._id_or_name(dst), label)


## ========================================================================= ##
    cpdef str label_of(self, int transition):
        """
        Usage:  aut.label_of(tr_id)

        Description:  Returns the label of a transition whose identifier is given as argument.

        Args:  tr_id (int), identifier of a transition

        Returns:  str
        """
        return self._to_cpp_class().label_of(transition);


## ========================================================================= ##
    def incoming(self, stt, str label=None):
        """
        Usage:  aut.incoming(stt [,label])

        Description:  Returns the identifiers of all incoming transitions of \
<stt> in <aut/self>; \
if <label> is provided, returns only the identifiers of the \
transitions labelled by <label>.

        Args:
            stt (int or str): state identifier (int) or state name (str)
            label (str, optional)

        Returns:  list of int, identifiers of transitions
        """
        if label is None:
            return _BasicAutomaton.incoming(self, self._id_or_name(stt))
        else:
            return self._to_cpp_class().incoming2(self._id_or_name(stt), label)


## ========================================================================= ##
    def outgoing(self, stt, str label=None):
        """
        Usage:
            outgoing(aut, stt [,label])
            outgoing(tdc, stt [,labels])

        Description:  Returns the identifiers of all outgoing transitions of \
<stt> within <aut/self>; \
if <label> is provided, returns only the identifiers of the \
transitions labelled by <label>.

        Args:
            stt (int or str): state identifier (int) or state name (str)
            label (str, optional)

        Returns:  list of int, identifiers of transitions
        """
        if label is None:
            return _BasicAutomaton.outgoing(self, self._id_or_name(stt))
        else:
            return self._to_cpp_class().outgoing2(self._id_or_name(stt), label)


## ========================================================================= ##
    def predecessors(self,stt, str label=None):
        """
        Usage:  aut.predecessors(stt [, label])

        Description:  returns the identifiers of all the states that are \
predecessors of <stt> within <tdc/self>; \
if  <labels> is provided, returns only the identifiers of the \
states s such that there exists a transition going  from s to <stt> and \
labelled by <labels>.

        Args:
            stt (int or str): state identifier (int) or state name (str)
            label (str, optional)

        Returns:  list of int, identifiers of states
        """
        if label is None:
            return _BasicAutomaton.predecessors(self, self._id_or_name(stt))
        else:
            return self._to_cpp_class().predecessors2(self._id_or_name(stt), label)


## ========================================================================= ##
    def successors(self,stt, str label=None):
        """
        Usage:  aut.successors(stt_id [, label])

        Description:  returns the identifiers of all the states that are \
successors of <stt_id> within <aut/self>; \
if <label> is provided, returns only the identifiers of the \
states s such that there exists a transition going from <stt_id> to s and \
labelled by <label>.

        Args:
            stt (int or str): state identifier (int) or state name (str)
            label (str, optional)

        Returns:  list of int, identifiers of states
        """
        if label is None:
            return _BasicAutomaton.successors(self, self._id_or_name(stt))
        else:
            return self._to_cpp_class().successors2(self._id_or_name(stt), label)


## ========================================================================= ##
    def unpack_transition(self, int tr_id):
        """
        usage:  aut.unpack_transition(tr_id)

        Description:   returns the details of the transition idenfitied by <tr_identifier> as a Tuple containing, in that order, the source-state id, the weight, the label and the destination-state id.

        Args:  tr_id(int), identifier of a transition

        Returns:  Tuple (int, str, str, int)
        """
        return (self.src_of(tr_id), self.label_of(tr_id), \
                self.weight_of(tr_id), self.dst_of(tr_id))


## ========================================================================= ##
    def has_letter(self, str l):
        """
        Usage:  aut.has_letter(letter)

        Description:  tests whether <letter> belongs to the alphabet of <aut/self>

        Args:  letter (str)

        Returns:  bool
        """
        return self._to_cpp_class().has_letter(l)


## ========================================================================= ##
## ======= Non-method equivalent function in automaton_3_function.pyx ====== ##
## ========================================================================= ##
    cpdef str alphabet(self):
        """
        Usage:  aut.alphabet()

        Description:  returns the alphabet of <aut/self>.

        Returns:
            str, the string representation of the alphabet of <aut/self>.
        """
        return self._to_cpp_class().alphabet()


## ========================================================================= ##
    def support(self):
        """
        Usage:  aut.support()

        Description:  returns the support of <aut/self>, that is the automaton on the weight-set <weightset> resulting from setting the weight of every transition of <aut/self> to one (ie. the multiplicative neutral element of <weightset>).

        Returns:
            Automaton:  the support of <aut/self> over the weight-set <weightset>.
        """
        return _Automaton(support_(self._to_cpp_class()))


## ========================================================================= ##
    def characteristic(self, str semiring):
        """
        Usage:  aut.characteristic(weightset)

        Description:  returns the characteristic of <aut/self>, that is the automaton on the weight-set <weightset> resulting from setting the weight of every transition of <aut/self> to one (ie. the multiplicative neutral element of <weightset>).

        Args:
            weightset (str):  weight-set of the returned automaton. Admissible values are {'B', 'Z', 'Z-min-plus', 'Z-max-plus', 'Q', 'R', 'R-max-prod', 'F2' }.

        Returns:
            Automaton, the characteristic of <aut/self> over the weight-set <weightset>.
        """
        return _Automaton(characteristic_(self._to_cpp_class(), semiring))




## ========================================================================= ##
    def partial_identity(self):
        """
        Usage:  aut.partial_identity()

        Description:  return the transducer realising the partial identity of (the language accepted by) <aut/self>.

        Returns:
            Transducer, transducer realising the partial identity of <aut/self>.
        """
        return _Transducer(partial_identity_(self._to_cpp_class()))


## ========================================================================= ##
    def allow_eps_transition(self):
        """
        Usage:  aut.allow_eps_transition(aut)

        Description: returns a copy of aut in which epsilon transition are allowed.

        Returns:
            Automaton: copy of <aut/self> where epsilon transition are allowed.
        """
        return _Automaton(allow_eps_transition_(self._to_cpp_class()))


## ========================================================================= ##
    def allow_eps_transition_here(self):
        """
        Usage:  aut.allow_eps_transition_here()

        Description: allows epsilon transition in <aut/self>.

        Notes:
            This function uses allow_eps_transition, hence requires a complete traversal of aut.
            Convenience function written at the python layer.
        """
        self._set_cpp_class(allow_eps_transition_(self._to_cpp_class()))


## ========================================================================= ##
    def are_equivalent(self, Automaton other):
        """
        Usage:  aut.are_equivalent(other)

        Description:  determines if <aut/self> is equivalent to <other>.

        Args:
            other (Automaton): the automaton whose equivalence to <aut/self> is tested.

        Preconditions:
            The weigh-set of <aut/self> and <other> must be B, Z or a field.

        Returns:
            bool: True if <aut/self> and <other> accept the same language.
        """
        return are_equivalent_(self._to_cpp_class(), other._to_cpp_class())


## ========================================================================= ##
    def determinize(self, bool history=True):
        """
        Usage:  aut.determinize([ history=True ])

        Description: returns a deterministic automaton equivalent to <aut/self>. If <history> is True, then the history of the automaton is preserved.

        Preconditions:
            weight-set of <aut/self> is the boolean semi-ring.

        Args:
            history (bool): whether history of the automaton should be preserved.

        Returns:
            Automaton: a deterministic automaton equivalent to <aut/self>.


        Notes:
            An automaton is deterministic if each state features at most one outgoing transition labelled by a given letter.
        """
        return _Automaton(determinize_(self._to_cpp_class(), history))


## ========================================================================= ##
    def determinize_here(self, bool history=True):
        """
        Usage:  aut.determinize_here([ history=True ])

        Description: determinizes <aut/self>. If <history> is True, then the history of the automaton is preserved.

        Preconditions:
            weight-set of <aut/self> is the boolean semi-ring.

        Args:
            history (bool): whether history of the automaton should be preserved.

        Notes:
            An automaton is deterministic if each state features at most one outgoing transition labelled by a given letter.
            Convenience function written at the python layer.
        """
        self._set_cpp_class(determinize_(self._to_cpp_class(), history))


## ========================================================================= ##
    def is_deterministic(self):
        """
        Usage:  aut.is_deterministic()

        Description:   check whether <aut/self> is deterministic.

        Returns:
            bool: True if <aut/self> is deterministic.

        Notes:
            An automaton is deterministic if each state features at most one outgoing transition labelled by a given letter.
        """
        return is_deterministic_(self._to_cpp_class())


## ========================================================================= ##
    def complement(self):
        """
        Usage:  aut.complement()

        Description:  compute the complement of a deterministic complete automaton.

        Args:
            aut (Automaton): automaton whose complement is computed.
                Preconditions: deterministic, complete.

        Returns:
            Automaton: accepts the complement language of <aut/self>.
        """
        return _Automaton(complement_(self._to_cpp_class()))


## ========================================================================= ##
    def complement_here(self):
        """
        Usage:  aut.complement_here()

        Description:  modify <aut/self> such that it accepts the complement of its previous language.

        Args:
            aut (Automaton): automaton whose complement is computed.
                Preconditions: deterministic, complete.
        """
        complement_here_(self._to_cpp_class())


## ========================================================================= ##
    def complete(self):
        """
        Usage:  aut.complete()

        Description:  returns a copy of <aut/self> which is complete, that is such that every state features an outgoing transition labelled by every letter.

        Returns:
            Automaton: a complete automaton equivalent to <aut/self>.

        Note:
            An automaton is complete if each state features an outgoing transition labelled by each letter.
        """
        return _Automaton(complete_(self._to_cpp_class()))


## ========================================================================= ##
    def complete_here(self):
        """
        Usage:  aut.complete_here()

        Description:  modify <aut/self> such that it becomes complete.

        Args:
            aut (Automaton): automaton whose completion is computed. Preconditions: Boolean semi-ring.

        Note:
            An automaton is complete if each state features an outgoing transition labelled by each letter.
        """
        complete_here_(self._to_cpp_class())


## ========================================================================= ##
    def is_complete(self):
        """
        Usage:  aut.is_complete()

        Description:  modify <aut/self> such that it becomes complete.

        Note:
            An automaton is complete if each state features an outgoing transition labelled by each letter.
        """
        return is_complete_(self._to_cpp_class())


## ========================================================================= ##
    def reduce(self):
        """
        Usage:  aut.reduce()

        Description:  compute the reduced automaton of <aut/self>.

        Preconditions: weight-set is Z or a field.

        Returns:
            Automaton: a reduced automaton equivalent to <aut/self>.

        Notes:
            The computation of the reduced automaton can be seen as the generalisation (to automata whose weight-set is a field) of the minimization process due to J. A. Brzozowski.
              """
        return _Automaton(reduce_(self._to_cpp_class()))


## ========================================================================= ##
    def reduce_here(self):
        """
        Usage:  reduce_here(aut)

        Description:  modify <aut/self> such that it becomes reduced automaton.

        Preconditions: weight-set is a field, or included in a field.

        Notes:
            The computation of the reduced automaton is the generalisation (to automata whose weight-set is a field) of the minimization process due to J. A. Brzozowski.
            If the weigh-set is not a field but is a sub-semi-ring of a clear field (eg Z is included in Q), the automaton weight-set will be generalised to the field and the reduced automaton will be computed as normal.
            Convenience function written at the python layer.
        """
        self._set_cpp_class(reduce_(self._to_cpp_class()))


## ========================================================================= ##
    def left_reduce(self):
        """
        Usage:  aut.left_reduce()

        Description:  compute the 'left reduced' automaton of <aut/self>.

        Preconditions: weight-set is Z or a field.

        Returns:
        Automaton: a controllable automaton equivalent to <aut/self>.

        Notes: To be written
        """
        return _Automaton(left_reduce_(self._to_cpp_class()))


## ========================================================================= ##
    def right_reduce(self):
        """
        Usage:  aut.right_reduce()

        Description:  compute the 'right reduced' automaton of <aut/self>.

        Preconditions: weight-set is Z or a field.

        Returns:
        Automaton: an observable automaton equivalent to <aut/self>.

        Notes: To be written
        """
        return _Automaton(right_reduce_(self._to_cpp_class()))


## ========================================================================= ##
    def is_ambiguous(self):
        """
        Usage:  is_ambiguous(aut)

        Description:  determines if <aut/self> is ambiguous.

        Args:
            aut (Automaton):

        Notes:
            An automaton is ambiguous if there exists two accepting runs for the same word.
        """
        return is_ambiguous_(self._to_cpp_class())


## ========================================================================= ##
    def product(self, Automaton other):
        """
        Usage:  aut.product(other)

        Description:  returns the Hadamard product of <aut/self> with <other>.

        Args:
            other (Automaton): automata whose product is computed.
                Preconditions: same weight-set as <aut/self>

        Returns:
            Automaton: the product of <aut/self> with <other>.

        Notes:  The weight of a given word in the Hadamard product is the weight-set multiplication of the respective weights of this word in the two operand automata.
        """
        return _Automaton(product_(self._to_cpp_class(), other._to_cpp_class()))


## ========================================================================= ##
    def product_here(self, Automaton other):
        """
        Usage:  aut.product_here(other)

        Description:  computes the Hadamard product of <aut/self> with <other> and store the results in <aut/self>.

        Args:
            other (Automaton): automata whose product is computed.
                Preconditions: same weight-set as <aut/self>.

        Notes:
            The weight of a given word in the Hadamard product is the weight-set multiplication of the respective weights of this word in the two operand automata.
            Convenience function written at the python layer.
        """
        self._set_cpp_class(product_(self._to_cpp_class(), other._to_cpp_class()))


## ========================================================================= ##
    def power(self, int n):
        """
        Usage:  aut.power(n)

        Description:  computes the n-th power of <aut/self>, that is the Hadamard product <aut/self> times <aut/self> times <aut/self> ...

        Args:
            aut (Automaton)
            n (int): exponent.
                Preconditions: positive.

        Returns:  Automaton

        Notes:
            The weight of a given word in the Hadamard product is the weight-set multiplication of the respective weights of this word in the two operand automata.
        """
        return _Automaton(power_(self._to_cpp_class(), n))


## ========================================================================= ##
    def power_here(self, int n):
        """
        Usage:  aut.power_here(n)

        Description:  computes the n-th power of <aut/self>, that is the Hadamard product <aut/self> times <aut/self> times <aut/self> ..., and stores the result in <aut/self>.

        Args:
            aut (Automaton), automaton whose power is computed.
            n (int), exponent
                needs to be positive.

        Notes:
            The weight of a given word in the Hadamard product is the weight-set multiplication of the respective weights of this word in the two operand automata.
            Convenience function written at the python layer.
        """
        self._set_cpp_class(power_(self._to_cpp_class(), n))


## ========================================================================= ##
    def shuffle(self, Automaton other):
        """
        Usage:  aut.shuffle(other)

        Description:  computes the automaton accepting the shuffle of the languages of <aut/self> and <other>.

        Args:  other (Automaton)

        Returns:  Automaton

        Notes:  A word u belongs to the shuffle of two languages L, K if u may be factorised as u = v0.w0.v1.w1...vn.wn such that v0.v1...vn belongs to L and w0.w1...wn belongs to K.
        """
        return _Automaton(shuffle_(self._to_cpp_class(), other._to_cpp_class()))


## ========================================================================= ##
    def infiltration(self, Automaton other):
        """
        Usage:  aut.infiltration(other)

        Description:  computes and returns the automaton accepting the infiltration of <aut/self> and <other>.

        Args:  other (Automaton)

        Returns:  Automaton

        Notes:  A word u belongs to the infiltration of two languages L,K if u may be factorised as u = u0.v0.w0.u1.v1.w1...un.vn.wn such that u0.v0.u1.v1...un.vn belongs to L and u0.w0.u1.w1...un.wn belongs to K.
        """
        return _Automaton(infiltration_(self._to_cpp_class(), other._to_cpp_class()))


## ========================================================================= ##
    def sum(self, Automaton other):
        """
        Usage:  aut.sum(other)

        Description:  computes the parallel join of <aut/self> and <other>

        Args:
            other (Automaton)

        Returns:
            Automaton: the sum of <aut/self> and <other>
        """
        return _Automaton(sum_(self._to_cpp_class(), other._to_cpp_class()))


## ========================================================================= ##
    def sum_here(self, Automaton other):
        """
        Usage:  aut.sum_here(other)

        Description:  computes the parallel join of <aut/self> and <other> and stores the result in <aut/self>.

        Args:
            other (Automaton)

        Notes:
            Convenience function written at the python layer.
        """
        self._set_cpp_class(sum_(self._to_cpp_class(), other._to_cpp_class()))


## ========================================================================= ##
    def proper(self, bool backward=True, bool prune=True):
        """
        Usage:  aut.proper([ backward=True [, prune=True ] ])

        Description:  returns a copy of <aut/self> in which the epsilon transition have been removed.

        Args:
            aut (Automaton)
            backward (bool, optional)
            prune (bool, optional)

        Returns:
            Automaton, automaton equivalent to <aut/self> without epsilon transitions and that does not allow future addition of epsilon-transitions.
        """
        return _Automaton(proper_(self._to_cpp_class(), backward, prune))


## ========================================================================= ##
    def proper_here(self, bool backward=True, bool prune=True):
        """
        Usage:  proper_here([  backward=True [, prune=True ] ])

        Description:  perform the epsilon-transition removal algorithm on <aut/self>.

        Args:
            backward (bool, optional)
            prune (bool, optional)

        Note:  Convenience function written at the python layer.
        """
        self._set_cpp_class(proper_(self._to_cpp_class(), backward, prune))


## ========================================================================= ##
    def is_proper(self):
        """
        Usage:  aut.is_proper()

        Description:  tests whether <aut/self> features epsilon transitions.

        Returns:
            bool: True if <aut/self> does not have any epsilon transition.
        """
        return is_proper_(self._to_cpp_class())


## ========================================================================= ##
    def is_valid(self):
        """
        Usage:  aut.is_valid()

        Description: tests whether <aut/self> is valid.

        Returns: bool, True if <aut/self> is valid.

        Notes:
          An automaton is valid if every (infinite) epsilon-path is properly weighted: the sequence of weights along this path is summable.
          Being valid is the precondition for the epsilon-transition-removal algorithm (function proper(..)) to work.
        """
        return is_valid_(self._to_cpp_class())


## ========================================================================= ##
    cpdef dict shortest(self, int n):
        """
        Usage:  aut.shortest(n):

        Description:  looks for the shortest <n> words accepted by <aut/self>, and computes their accepted weight.  This method might return less than <n> mappings if <aut/self> accepts less than <n> words.

        Args:  n (int), number of words to look for.

        Preconditions:
            <aut/self> must be proper.
            <n> must be positive.

        Returns:  dict, mapping the shortest <n> words accepted by <aut/self> to the weights they are respectively accepted with.
        """
        return shortest_(self._to_cpp_class(), n)


## ========================================================================= ##
    def enumerate(self, int max_len):
        """
        Usage:  aut.enumerate(max_len)

        Description:  computes the words shorter than <max_len> accepted by <aut/self> and their weight.

        Args:
            max_len (int), maximal length
                needs to be non-negative

        Returns:
            dict str->str: domain is all the words shorter than <max_len> and accepted by <aut/self>; the image of a word is its weight.
        """
        return _from_map_s_s(enumerate_(self._to_cpp_class(), max_len))


## ========================================================================= ##
    def prefix(self):
        """
        Usage:  aut.prefix()

        Description:  returns a copy of <aut/self> in which every co-accessible state is made final.

        Args:
            aut (Automaton)

        Returns:
            Automaton
        """
        return _Automaton(prefix_(self._to_cpp_class()))


## ========================================================================= ##
    def prefix_here(self):
        """
        Usage:  aut.prefix_here()

        Description:  makes final every state of <aut/self> which is co-accessible.
        """
        prefix_here_(self._to_cpp_class())


## ========================================================================= ##
    def suffix(self):
        """
        Usage:  aut.suffix()

        Description:  returns a copy of <aut/self> in which every accessible state is made initial.

        Returns:  Automaton
        """
        return _Automaton(suffix_(self._to_cpp_class()))


## ========================================================================= ##
    def suffix_here(self):
        """
        Usage:  aut.suffix_here()

        Description:  makes initial every state of <aut/self> which is accessible.

        Returns:
            Automaton
        """
        suffix_here_(self._to_cpp_class())


## ========================================================================= ##
    def factor(self):
        """
        Usage:  aut.factor()

        Description:  returns a copy of <aut/self> in which every accessible state is now initial and every co-accessible state is now final.

        Returns (Automaton)
        """
        return _Automaton(factor_(self._to_cpp_class()))


## ========================================================================= ##
    def factor_here(self):
        """
        Usage:  aut.factor_here()

        Description:  makes initial every accessible state of <aut/self> and initial every co-accessible state of <aut/self>.
        """
        factor_here_(self._to_cpp_class())


## ========================================================================= ##
    def minimal_automaton(self, str method = "moore"):
        """
        Usage:  aut.minimal_automaton([method="moore"])

        Description:  computes and returns the minimal automaton accepting the language of <aut/self>, using determinization if necessary.

        Preconditions:  weightset of <aut/self> must be B.

        Args:  method(str, optional), algorithm to use; admissible values are "moore", "hopcroft", or "brzozowski"; default value is "moore".

        Returns (Automaton)
        """
        return _Automaton(minimal_automaton_(self._to_cpp_class(), method))


## ========================================================================= ##
    def quotient(self, list equiv):
        """
        Usage: aut.quotient(equiv)

        Description: computes and returns the (maximal) quotient of \
<aut/self> respecting <equiv> the partial equivalence relation on the \
state-set of <aut/self>.

        Args:  <equiv> (list of list of int), a partial partition of the state-set of <aut/self>.

        Returns (Automaton)

        Note:  If the partial equivalence relation is invalid, that is \
if two states allegedly equivalent do not have the same out-language, \
then the algorithm will not fail but the resulting automaton will not be a quotient of <aut/self>.
        """
        return _Automaton(quotient_(self._to_cpp_class(), equiv))

## ========================================================================= ##
    def min_quotient(self, str method = "moore"):
        """
        Usage: aut.min_quotient([method="moore"])

        Description: computes and returns the minimal quotient of <aut/self>

        Args:  method(str, optional), algorithm to use; admissible values are "moore" or "hopcroft"; default value is "moore".

Returns (Automaton)
        """
        return _Automaton(min_quotient_(self._to_cpp_class(), method))


## ========================================================================= ##
    def quotient_here(self, list equiv=None):
        """
        Usage:
          [a] aut.quotient_here()
          [b] aut.quotient_here(equiv)

        Description:
          [a] computes the minimal quotient of <aut/self>
          [b] computes the (maximal) quotient of \
<aut/self> respecting <equiv> the partial equivalence relation on the \
state-set of <aut/self>.

        Args:  <equiv> (list of list of int, optional), a partial partition of the state-set of <aut/self>.

        Returns (Automaton)

        Note:
          If the partial equivalence relation is invalid, that is if two \
states allegedly equivalent do not have the same out-language, then the \
algorithm will not fail but the resulting automaton will not be a quotient \
of <aut/self>.
          Convenience function written at the python layer.
        """
        if equiv is None:
          self._set_cpp_class(min_quotient_(self._to_cpp_class()))
        else:
          self._set_cpp_class(quotient_(self._to_cpp_class(), equiv))


## ========================================================================= ##
    def coquotient(self, list equiv):
        """
        Usage: aut.coquotient(equiv)

        Description: computes and returns the (maximal) coquotient of \
<aut/self> respecting <equiv> the partial equivalence relation on the \
state-set of <aut/self>.

        Args:  <equiv> (list of list of int), a partial partition of the state-set of <aut/self>.

        Returns (Automaton)

        Note:  If the partial equivalence relation is invalid, that is \
if two states allegedly equivalent do not have the same in-language, \
then the algorithm will not fail but the resulting automaton will not be a coquotient of <aut/self>.
        """
        return _Automaton(coquotient_(self._to_cpp_class(), equiv))

## ========================================================================= ##
    def min_coquotient(self, str method = "moore"):
        """
        Usage: aut.min_coquotient()

        Description: computes and returns the minimal coquotient of <aut/self>

        Args:  method(str, optional), algorithm to use; admissible values are "moore" or "hopcroft"; default value is "moore".

        Returns (Automaton)
        """
        return _Automaton(min_coquotient_(self._to_cpp_class(), method))


## ========================================================================= ##
    def coquotient_here(self, list equiv=None):
        """
        Usage:
          [a] aut.coquotient_here()
          [b] aut.coquotient_here(, equiv)

        Description:
          [a] computes the minimal coquotient of <aut/self>
          [b] computes the (maximal) coquotient of \
<aut/self> respecting <equiv> the partial equivalence relation on the \
state-set of <aut/self>.

        Args:  <equiv> (list of list of int, optional), a partial partition of the state-set of <aut/self>.

        Returns (Automaton)

        Note:
          If the partial equivalence relation is invalid, that is if two \
states allegedly equivalent do not have the same out-language, then the \
algorithm will not fail but the resulting automaton will not be a coquotient \
of <aut/self>.
          Convenience function written at the python layer.
        """
        if equiv is None:
          self._set_cpp_class(min_coquotient_(self._to_cpp_class()))
        else:
          self._set_cpp_class(coquotient_(self._to_cpp_class(), equiv))

## ========================================================================= ##
    def explore_with_bound(self, str weight):
        """
        Usage: aut.explore_with_bound(weight)
        
        Args: <weight> (str), the string representation of a weight

        Returns: Automaton

        Description:  Computes part of the weighted determinization of <aut>.  Recall that in general, the weighted determinization of <aut> may be infinite; let us call it <det_aut>.  Every state of <det_aut> is associated with a vector (x_1,...,x_q), where the x_i's are weights in <weightset> and q is the number of states in <aut>. 
Aside from sink states, the returned automaton is the subautomaton of <det_aut>
containing the states that satisfy (x_i)² <= (<k>)' for every i.  If a word is such that it would reach a state of <det_aut> that is not in the returned automaton, then that word reaches a non-accepting sink state instead.

        Precondition: 
            - The automaton <aut> must be labelled with letters with no epsilon-transitions allowed.
            - This command requires the weightset to be N, Z, N-oo or N<int>.
        """
        return _Automaton(explore_with_bound(self._to_cpp_class(), weight))


## ========================================================================= ##
    def explore_by_length(self, int i):
        """
        Usage: aut.explore_by_length(length)
        
        Args: <length> (int >=0), the length

        Returns: Automaton

        Description:  Computes part of the weighted determinization of <aut>. Precisely, it computes the part that is reached by words of length less than or equal to <length>. (Recall that in general, the weighted determinization of <aut> may be infinite.)
        Words longer than <length> may reach a non-accepting sink state.

        Precondition: The automaton <aut> must be labelled with letters with no epsilon-transitions allowed.
        """
        return _Automaton(explore_by_length_(self._to_cpp_class(), i))

## ========================================================================= ##
    def matrix_repr(self):
        """
        Usage:  aut.matrix_repr()

        Description:  returns the matrix representation of <aut/self> as an \
initial line-vector, a dictionary that maps letters to matrices, and a final \
column-vector.

        Returns (size-1-list of list of str * (dict str -> list of list of str)  * list of size-1-list of str)
        """
        dict= {}
        initial_vector= []
        final_vector= []
        ws_zero= self.weight_zero()
        for i in self.states():
            if self.is_initial(i):
                initial_vector.append (self.get_initial_weight(i))
            else:
                initial_vector.append (ws_zero)
            if self.is_final(i):
                final_vector.append ([self.get_final_weight(i)])
            else:
                final_vector.append ([ws_zero])
        for a in self.alphabet():
            matrix = []
            for i in self.states():
                line = []
                for j in self.states():
                    if self.has_transition(i,j,a):
                        line.append(self.weight_of(self.get_transition(i,j,a)))
                    else:
                        line.append(ws_zero)
                matrix.append(line)
            dict[a]= matrix
        return ([initial_vector], dict, final_vector)
