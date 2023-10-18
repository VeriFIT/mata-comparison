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


cdef _BasicAutomaton _BasicAutomaton_(basic_automaton_t aut_or_tdc):
    """Wraps a basic_automaton_t (C++ class) in an Automaton or a Transducer (python class).

    Args:
        aut_or_tdc (dyn::basic_automaton_t):  the automaton or transducer to wrap.

    Returns:
        Transducer: the wrapped transducer.

    Notes:
        Internal function.
        Argument <tdc> is not verified to actually be transducer.
        Use with caution.
    """
    if (aut_or_tdc.is_transducer()):
        a= Transducer([], "hack!!")
    else:
        a = Automaton(empty_shell=True)
    a._set_cpp_class(aut_or_tdc)
    return a

## ========================================================================= ##
## ========================================================================= ##
cdef class _BasicAutomaton:
    """
    Internal wrapper for C++ class basic_automaton_t.
    This class is not fit to be used directly.
    """

## ========================================================================= ##
    cdef basic_automaton_t _this

## ========================================================================= ##
    cdef dict _state_name_map


## ========================================================================= ##
    cdef void _set_cpp_class(self, basic_automaton_t other):
        """
        Modify wrapped object. Internal use only.
        This seems to be quite fragile. Modify the code with caution.
        """
        self._this.rewrap_into_me_(other)
        self._recompute_state_names()

## ========================================================================= ##
    cdef void _set_cpp_class_bis(self, _BasicAutomaton other):
        """
        Modify wrapped object. Internal use only.
        This seems to be quite fragile. Modify the code with caution.
        """
        self._set_cpp_class(other._this)

## ========================================================================= ##
    def _register_state_name(self, i):
        if (self.has_explicit_name(i)):
                name = self.get_state_name(i)
                if name in self._state_name_map:
                    if type(self._state_name_map[name]) is list:
                        self._state_name_map[name].append(i)
                    else:
                        self._state_name_map[name] = [self._state_name_map[name],i]
                else:
                    self._state_name_map[name] = i

## ========================================================================= ##
    def _unregister_state_name(self, i):
        if (self.has_explicit_name(i)):
                name = self.get_state_name(i)
                if type(self._state_name_map[name]) is list:
                    l = [x for x in self._state_name_map[name] if x != i]
                    if len (l) == 1:
                        self._state_name_map[name] = l[0]
                    else:
                        self._state_name_map[name] = l
                else:
                    del self._state_name_map[name]

## ========================================================================= ##
    def _recompute_state_names(self):
        self._state_name_map = dict()
        for i in self.states():
            self._register_state_name(i)
            
## ========================================================================= ##
    def _id_or_name(self, id):
        if type(id) is int:
            return id
        elif type(id) is str:
            state = self.get_state_by_name(id);
            if type(state) is list:
                raise ValueError("The name \""+id+"\" is associated with multiple states: "+ str(state))
            return state
        else:
            raise TypeError("Expected a state id (int) or state name (str). Got type: "+ str(type(id)))

            


## ========================================================================= ##
    def __init__(self):
        self._recompute_state_names();


## ========================================================================= ##
## == Non-method equivalent function in fused_Automaton_or_Transducer.pyx == ##
## ========================================================================= ##
## These methods are overridden in derived class (Automaton and Transducer)
## and hence are probably not visible for users.

    def del_transition(self, transition_or_source, dest_or_none=None):
        if dest_or_none is None:
            self._this.del_transition1(transition_or_source)
        else:
            self._this.del_transition2(self._id_or_name(transition_or_source), self._id_or_name(dest_or_none))

    def predecessors(self, stt):  return self._this.predecessors1(self._id_or_name(stt))
    def successors(self, stt):  return self._this.successors1(self._id_or_name(stt))
    def incoming(self, stt):  return self._this.incoming1(self._id_or_name(stt))
    def outgoing(self, stt):  return self._this.outgoing1(self._id_or_name(stt))


## ========================================================================= ##
## ===== Non-method equivalent function in basic_automaton_function.pyx ==== ##
## ========================================================================= ##

    def display(self, horizontal=True, history=False):
        """
        Usage: aut_or_tdc.display( [horizontal=True, [, history=False] ] )

        Description: displays <aut_or_tdc>. If <horizontal> is True, the layout is horizontal, otherwise it is vertical.  If <history> is True, history of states is not written in states.

        Args:
            horizontal (bool, optional),
                defaults to True
            history (bool, optional)
                defaults to False


        Note:  requires the program graphviz for automatic generation of graphs, and a software for pdf-visualization.
        """
        if run_from_jupyter_notebook():
            #f = tempfile.NamedTemporaryFile(suffix=".svg")
            #fname = f.name
            #f.write(self.svg(history,horizontal))
            #f.close()
            from IPython.display import Image, display, SVG
            display(SVG(self._this.svg(history, horizontal)))
        else:
            pdfdisplay(self._this, horizontal, history);
#          else:
#              f = tempfile.NamedTemporaryFile(suffix=".gv")
#              f.close()
#              fnameGV = f.name
#              fname, _ = os.path.splitext(fnameGV)
#              if run_from_ipython_notebook():
#                  filenameOut = fname + ".svg"
#              else:
#                  filenameOut = fname + ".pdf"
#          foo = open(fnameGV, "w")
#          foo.write(self.dot(history, horizontal))
#          foo.close()
#          if run_from_ipython_notebook():
#              from IPython.display import Image, display, SVG
#              callstr = "dot -Tsvg %s -o %s" % (fnameGV, filenameOut)
#          else:
#              callstr = "dot -Tpdf %s -o %s" % (fnameGV, filenameOut)
#          result = subprocess.call(callstr, shell=True)
#          if result:
#              print("Need graphviz to visualize objects")
#              return
#          if run_from_ipython_notebook():
#              display(SVG(filename=filenameOut))
#          elif sys.platform.startswith('win32'): #WINDOWS
#              os.system("start %s" % filenameOut)
#          elif sys.platform.startswith('darwin'): #MAC
#              os.system("open %s" % filenameOut)
#          else: #LINUX/FREEBSD
#              os.system("xdg-open %s" % filenameOut)


#  ## ========================================================================= ##
#      cpdef bytes dot(self, bool history=False, bool horizontal=True):
#          """
#          Usage:  aut.dot( [history=False, horizontal=True] )
#  
#          Description:  returns a string-representation of <aut/self> under the format dot; suitable for file-writing.
#  
#          Args:
#              history (bool, optional), True if the automaton history is to be written into the file,
#                  defaults to False
#              horizontal (bool, optional), True if the option is set to get a left-right layout instead of a top-down layout,
#                  defaults to True
#  
#          Returns:  str
#          """
#          return self._this.dot(history, horizontal)
#  

# ## ========================================================================= ##
#     cpdef bytes svg(self, bool history=False, bool horizontal=True):
#         """
#         Usage:  aut.svg( [history=False, horizontal=True] )
#   
#         Description:  returns a string-representation of <aut/self> under the format svg; suitable for file-writing.
#   
#         Args:
#             history (bool, optional), True if the automaton history is to be written into the file,
#                 defaults to False
#             horizontal (bool, optional), True if the option is set to get a left-right layout instead of a top-down layout,
#                 defaults to True
#   
#         Returns:  str
#         """
#         return 
#   
#  ## ========================================================================= ##
#      cpdef str fado(self):
#          """
#          Usage:  aut.fado()
#  
#          Description:  returns a string-representation of <aut/self> under the format fado; suitable for file-writing.
#  
#          Returns:  str
#          """
#          return self._this.fado()
#  
#  ## ========================================================================= ##
#      cpdef str grail(self):
#          """
#          Usage:  aut.grail()
#  
#          Description:  returns a string-representation of <aut/self> under the format grail; suitable for file-writing.
#  
#          Returns:  str
#          """
#          return self._this.grail()
#  
#  
#  ## ========================================================================= ##
#      cpdef str json(self):
#          """
#          Usage:  aut.json()
#  
#          Description:  returns a string-representation of <aut/self> under the format JSON; suitable for file-writing.
#  
#          Returns:  str
#          """
#          return self._this.json()
#  

## ========================================================================= ##
    def save(self, str s, format = None, history = None):
        """
        Usage:  automaton.save( filename [, format='json' [, history=False]] )

        Description:  writes <automaton> in file <filename> using <format>.

        Args:
          filename (str)
          format (str, optional),
              admissible values are 'dot', 'fado', 'json', 'svg', 'pdf', 'grail'
              defaults to 'json'
          history(bool, optional)
              Whether the history of states is put in states
              Only meaningfull when using format "pdf" or "svg"

        Note: only automata saved in format 'json' may be loaded back later on.
        """
        if format == None:
            save_automaton2_(self._this, s)
        elif history == None:
            save_automaton3_(self._this, s, format)
        else:
            save_automaton4_(self._this, s, format, history)

## ========================================================================= ##
    def states(self):
        """
        Usage:  aut_or_tdc.states()

        Description:  returns the list of the identifiers of all the states of <aut_or_tdc/self>.

        Returns:  list of int
        """
        return self._this.states()


## ========================================================================= ##
    def num_states(self):
        """
        Usage:  aut_or_tdc.num_states()

        Description:  returns the number of states of <aut_or_tdc/self>

        Returns:  int
        """
        return self._this.num_states()


## ========================================================================= ##
    def has_state(self, int stt_id):
        """
        Usage:  aut_or_tdc.has_state(stt_id)

        Description:  returns True if <stt_id> is indeed the identifier of a state of <aut_or_tdc/self>.

        Args:  stt_id (int)

        Returns:  bool
        """
        return self._this.has_state(stt_id)


## ========================================================================= ##
    def initial_states(self):
        """
        Usage:  aut_or_tdc.initial_states()

        Description:  returns the list of the identifiers of the initial states of <aut_or_tdc/self>.

        Returns:  list of int
        """
        return self._this.initial_states()


## ========================================================================= ##
    def num_initials(self):
        """
        Usage:  aut_or_tdc.num_initials()

        Description: returns the number of initial states of <aut_or_tdc/self>

        Returns:  int
        """
        return self._this.num_initials()


## ========================================================================= ##
    def is_initial(self, stt):
        """
        Usage:  aut_or_tdc.is_initial(stt)

        Description:  returns True if the state identified by <stt> is initial in <aut_or_tdc>

        Args:  stt (int or str): state identifier (int) or state name (str)

        Returns: bool
        """
        return self._this.is_initial(self._id_or_name(stt));


## ========================================================================= ##
    def get_initial_weight(self, stt):
        """
        Usage:  aut_or_tdc.get_initial_weight(stt):

        Description:  returns the initial weight of the state identified by <stt> in <aut_or_tdc/self>; returns the neutral additive element of the weight-set if the state is not initial

        Args:  stt (int or str): state identifier (int) or state name (str)

        Returns:  str
        """
        return self._this.get_initial_weight(self._id_or_name(stt))


## ========================================================================= ##
    def set_initial(self, stt, str weight= None):
        """
        Usage:  aut_or_tdc.set_initial(stt [, weight=aut.get_weightset().one() ] )

        Description:  sets in <aut_or_tdc/self> the initial weight to the state identified by <stt>; overwrites the eventual previous initial weight

        Args:
            stt (int or str): state identifier (int) or state name (str)
            weight (str, optional)
                defaults to aut.get_weightset().one()

        Returns:  str, the new initial weight
        """
        if weight is None:
            self._this.set_initial1(self._id_or_name(stt))
        else:
            self._this.set_initial2(self._id_or_name(stt), weight)


## ========================================================================= ##
    def add_initial(self, stt, str weight= None):
        """
        Usage:  aut_or_tdc.add_initial(stt [, weight=aut.get_weightset().one() ] )

        Description:  adds in <aut_or_tdc/self> some initial weight to the state identified by <stt>

        Args:
             stt (int or str): state identifier (int) or state name (str)
            weight (str, optional)
                defaults to aut.get_weightset().one()

        Returns:  str, the new initial weight
        """
        if (weight is None):
            return self.add_initial(self._id_or_name(stt), self.get_weightset().one())
        else:
            return self._this.add_initial(self._id_or_name(stt), weight)


## ========================================================================= ##
    def unset_initial(self, stt):
        """
        Usage:  aut_or_tdc.unset_initial(stt)

        Description:  makes the state identified by <stt> non-initial, that is, sets its initial weight to aut.weight_zero().

        Args:   stt (int or str): state identifier (int) or state name (str)
        """
        self._this.unset_initial(self._id_or_name(stt))


## ========================================================================= ##
    def final_states(self):
        """
        Usage:  aut_or_tdc.final_states()

        Description:  returns the list of the identifiers of the initial states of <aut_or_tdc/self>.

        Returns:  list of int
        """
        return self._this.final_states()


## ========================================================================= ##
    def num_finals(self):
        """
        Usage:  aut_or_tdc.num_finals()

        Description: returns the number of final states of <aut_or_tdc/self>

        Returns:  int
        """
        return self._this.num_finals()


## ========================================================================= ##
    def is_final(self, stt):
        """
        Usage:  aut_or_tdc.is_final(stt):

        Description:  returns True if the state identified by <stt> is final in <aut_or_tdc/self>

        Args:  stt (int or str): state identifier (int) or state name (str)

        Returns:  bool
        """
        return self._this.is_final(self._id_or_name(stt))


## ========================================================================= ##
    cpdef str get_final_weight(self, stt):
        """
        Usage:  aut_or_tdc.get_final_weight(stt):

        Description:  returns the final weight of the state identified by <stt> in <aut_or_tdc/self>; returns the neutral additive element of the weight-set if the state is not final

        Args:  stt (int)

        Returns: str
        """
        return self._this.get_final_weight(self._id_or_name(stt))


## ========================================================================= ##
    def set_final(self, stt, str weight=None):
        """
        Usage:  aut_or_tdc.set_final(stt [, weight=aut.get_weightset().one() ] )

        Description:  sets in <aut_or_tdc/self> the final weight to the state identified by <stt>; overwrites the eventual previous final weight

        Args:
             stt (int or str): state identifier (int) or state name (str)
            weight (str, optional)
                defaults to aut.get_weightset().one()
        """
        if weight is None:
            self._this.set_final1(self._id_or_name(stt))
        else:
            self._this.set_final2(self._id_or_name(stt), weight)


## ========================================================================= ##
    cpdef str add_final(self, stt, str weight=None):
        """
        Usage:  aut_or_tdc.add_final(stt [, weight=aut.get_weightset().one() ] )

        Description:  adds in <aut_or_tdc/self> some final weight to the state identified by <stt>

        Args:
             stt (int or str): state identifier (int) or state name (str)
            weight (str, optional)
                defaults to aut.get_weightset().one()

        Returns:  str, the new final weight
        """
        if (weight is None):
            weight = self.get_weightset().one()
        return self._this.add_final(self._id_or_name(stt), weight)


## ========================================================================= ##
    def unset_final(self, stt):
        """
        Usage:  aut_or_tdc.unset_final(stt)

        Description:  makes the state identified by <stt> non-final, that is, sets its final weight to aut.weight_zero().

        Args:   stt (int or str): state identifier (int) or state name (str)

        Returns:  str, the new final weight
        """
        self._this.unset_final(self._id_or_name(stt))


## ========================================================================= ##
    def add_state(self, name = None):
        """
        Usage:  aut_or_tdc.add_state([name])

        Description:  adds a new state to <aut_or_tdc>, eventually name <name> and returns its identifier.

        Args:
          name (str, optional),
            no default value

        Returns:  int
        """
        if (name is None):
          return self._this.add_state0()
        else:
          i = self._this.add_state1(name)
          self._register_state_name(i)
          return i


## ========================================================================= ##
    def del_state(self, stt):
        """
        Usage:  aut_or_tdc.del_state(stt)

        Description:  deletes from <aut_or_tdc/self> the state identified by <stt> and all its incoming and outgoing transitions

        Args:   stt (int or str): state identifier (int) or state name (str)
        """
        stt_id = self._id_or_name(stt)
        self._unregister_state_name(stt_id)
        self._this.del_state(stt_id)


## ========================================================================= ##
    def transitions(self):
        """
        Usage:  aut_or_tdc.transitions()

        Description: returns the idenfitiers of all transitions of <aut_or_tdc/self>

        Returns:  list of int
        """
        return self._this.transitions()


## ========================================================================= ##
    def num_transitions(self):
        """
        Usage:  aut_or_tdc.num_transitions()

        Description: returns the number of transitions of <aut_or_tdc/self>.

        Returns: int.
        """
        return self._this.num_transitions()


## ========================================================================= ##
    def has_transition(self, int tr_id):
        """
        Usage:  aut_or_tdc.has_transition(tr_id)

        Description:  returns True if <tr_id> is the identifier of a transition of <aut_or_tdc/self>

        Args:  tr_id (int)

        Returns:  bool
        """
        return self._this.has_transition1(tr_id)


## ========================================================================= ##
    def src_of(self, int tr_id):
        """
        Usage:  aut_or_tdc.src_of(tr_id)

        Description: returns the identifier of the source  state of the transition identified by <tr_id>

        Args: tr_id (int)

        Returns: int
        """
        return self._this.src_of(tr_id)


## ========================================================================= ##
    def dst_of(self, int tr_id):
        """
        Usage:  aut_or_tdc.dst_of(tr_id)

        Description: returns the identifier of the destination state of the transition identified by <tr_id>

        Args: tr_id (int)

        Returns: int
        """
        return self._this.dst_of(tr_id)


## ========================================================================= ##
    cpdef str weight_of(self, int tr_id):
        """
        Usage:  aut_or_tdc.weight_of(tr_id)

        Description: returns the weight of the transition identified by <tr_id>

        Args: tr_id (int)

        Returns: str
        """
        return self._this.weight_of(tr_id)


## ========================================================================= ##
    def set_eps_transition(self, int source, int destin, str weight=None):
        """
        Usage:  aut_or_tdc.set_eps_transition(src_id, dst_id [, new_weight=aut_or_tdc.weigh_one()] )

        Description:  sets as <new_weight> the weight of the epsilon-transition \
    going from <src_id> to <dst_id>; \
    overwrites the eventual previous weight; \
    creates the transition if necessary.

        Args:
            src_id (int), representing a state
            dst_id (int), representing a state
            label (str)
            new_weight (str, optional),
                defaults to aut_or_tdc.get_weightset().one().

        Note:  by default, transducers allow epsilon transitions but automata do not
        """
        if weight is None:
            return self._this.set_eps_transition2(source, destin)
        else:
            return self._this.set_eps_transition3(source, destin, weight)


## ========================================================================= ##
    cpdef str add_eps_transition(self, int src, int dst, str weight=None):
        """
        Usage:  aut_or_tdc.add_eps_transition(src_id, dst_id [, extra_weight=aut_or_tdc.get_weightset().one() ] )

        Description:  adds in <aut_or_tdc> some <extra_weight> to the epsilon-transition \
    going from <src_id> to <dst_id>; creates the \
    transition if necessary.

        Args:
            src_id (int), identifier of a state
            dst_id (int), identifier of a state
            extra_weight (str, optional)
                defaults to aut_or_tdc.get_weightset().one()

        Returns:  str, the new weight of the transition

        Note:  by default, transducers allow epsilon transitions but automata do not
        """
        if weight is None:
            return self._this.add_eps_transition2(src, dst)
        else:
            return self._this.add_eps_transition3(src, dst, weight)


## ========================================================================= ##
    def set_weight(self, int tr_id, str new_weight):
        """
        Usage:  aut_or_tdc.set_weight(tr_id, new_weight)

        Description:  sets as <new_weight> weight of the transition identified by <tr_id>

        Args:
            tr_id (int), identifier of a transition
            new_weight (str)
        """
        return self._this.set_weight(tr_id, new_weight)


## ========================================================================= ##
    cpdef str add_weight(self, int tr_id, str extra_weight):
        """
        Usage:  aut_or_tdc.add_weight(tr_id, extra_weight)

        Description:  adds some <extra_weight> to the transition identified by <tr_id>

        Args:
            tr_id (int), identifier of a transition
            extra_weight (str)

        Returns: str, the new weight of the transition
        """
        return self._this.add_weight(tr_id, extra_weight)


## ========================================================================= ##
    cpdef str lmul_weight(self, int tr_id, str weight_factor):
        """
        Usage:  aut_or_tdc.lmul_weight(tr_id, weight_factor)

        Description:  multiplies on the left the weight of the transition identified by <tr_id> by some <weight_factor>

        Args:
            tr_id (int), identifier of a transition
            weight_factor (str)

        Returns: str, the new weight of the transition
        """
        return self._this.lmul_weight(tr_id, weight_factor)


## ========================================================================= ##
    cpdef str rmul_weight(self, int tr_id, str weight_factor):
        """
        Usage:  aut_or_tdc.lmul_weight(tr_id, weight_factor)

        Description:  multiplies on the right the weight of the transition identified by <tr_id> by some <weight_factor>

        Args:
            tr_id (int), identifier of a transition
            weight_factor (str)

        Returns: str, the new weight of the transition
        """
        return self._this.rmul_weight(tr_id, weight_factor)


## ========================================================================= ##
    def outin(self, int src, int dst):
        """
        Usage:  aut_or_tdc.outin(src_id, dst_id)

        Description:  returns the list of the ids of all transitions going from <src_id> to <dst_id>

        Args:
            src_id (int), identifier of a state
            dst_id (int), identifier of a state

        Returns: list of int
        """
        return self._this.outin(src,dst)



## ========================================================================= ##
    def get_weightset(self):
        """
        Usage:   aut_or_tdc.get_weightset()

        Description:  return the weightset of given automaton

        Returns:  WeightSet
        """
        return _WeightSet(self._this.get_weightset());

## ========================================================================= ##
    cpdef str get_static_context(self):
        """
        Usage:  aut_or_tdc.get_static_context()

        Description:  returns the complete static context of <aut_or_tdc>

        Returns:  str

        Note:  this function is mostly for debugging purposes since there is no way to use contexts at the python layer.
        """
        return self._this.get_static_context()


## ========================================================================= ##
    def accessible_states(self):
        """
        Usage:  aut_or_tdc.accessible_states()

        Description:  returns the list of identifier of the accessible states of <aut_or_tdc/self>

        Returns:  list of int

        Note:  a state is accessible if it is the endpoint of a path starting from an initial state
        """
        return accessible_states_(self._this)


## ========================================================================= ##
    def num_accessible_states(self):
        """
        Usage:   aut_or_tdc.num_accessible_states()

        Description:  returns the number of accessible states of <aut_or_tdc/self>

        Returns:  int

        Note:  a state is accessible if it is the endpoint of a path starting from an initial state
        """
        return num_accessible_states_(self._this)


## ========================================================================= ##
    def is_accessible(self):
        """
        Usage:  aut_or_tdc.is_accessible()

        Description:  returns true if every state of <aut_or_tdc/self> is accessible

        Returns:  bool

        Note:  a state is accessible if it is the endpoint of a path starting from an initial state
        """
        return is_accessible_(self._this)

## ========================================================================= ##
    def promote(self, weightset):
        """
        Usage:  aut_or_tdc.promote(weightset)

        Description:  returns a copy of <aut_or_tdc/self>, except its weightset is <weightset>

        Args: 
            weightset (str or WeightSet), the weightset to promote tp

        Precondition:  <weightset> must be a supersemiring of the weightset of <aut_or_tdc/self>. (Use method `characteristic` to pass from B to any other semiring.)

        Returns:  Automaton or Transducer 
        """
        if isinstance(weightset, WeightSet):
            weightset = str(weightset)
        return _BasicAutomaton_(promote_automaton_(self._this,weightset))

## ========================================================================= ##
    def accessible(self):
        """
        Usage:  aut_or_tdc.accessible()

        Description:  returns a copy of <aut_or_tdc/self> in which states that are not accessible are deleted

        Returns:  Automaton or Transducer

        Note:  a state is accessible if it is the endpoint of a path starting from an initial state
        """
        return _BasicAutomaton_(accessible_(self._this))


## ========================================================================= ##
    def accessible_here(self):
        """
        Usage:  aut_or_tdc.accessible_here()

        Description:  deletes from <aut_or_tdc> every non-accessible state.

        Note:  a state is accessible if it is the endpoint of a path starting from an initial state
        """
        accessible_here_(self._this)

## ========================================================================= ##
    def coaccessible_states(self):
        """
        Usage:  aut_or_tdc.coaccessible_states()

        Description:  returns the list of identifier of the co-accessible states of <aut_or_tdc/self>

        Returns: list of int

        Note:  a state is co-accessible if it is the starting point of a path ending in a final state
        """
        return coaccessible_states_(self._this)


## ========================================================================= ##
    def num_coaccessible_states(self):
        """
        Usage:   aut_or_tdc.num_coaccessible_states()

        Description:  returns the number of co-accessible states of <aut_or_tdc/self>

        Returns:  int

        Note:  a state is co-accessible if it is the starting point of a path ending in a final state
        """
        return num_coaccessible_states_(self._this)


## ========================================================================= ##
    def is_coaccessible(self):
        """
        Usage:  aut_or_tdc.is_coaccessible()

        Description:  returns true if every state of <aut_or_tdc/self> is co-accessible

        Returns:  bool

        Note:  a state is co-accessible if it is the starting point of a path ending in a final state
        """
        return is_coaccessible_(self._this)


## ========================================================================= ##
    def coaccessible(self):
        """
        Usage:  aut_or_tdc.accessible()

        Description:  returns a copy of <aut_or_tdc/self> in which are deleted the states that are not co-accessible

        Returns:  Automaton or Transducer

        Note:  a state is co-accessible if it is the starting point of a path ending in a final state
        """
        return  _BasicAutomaton_(coaccessible_(self._this))


## ========================================================================= ##
    def coaccessible_here(self):
        """
        Usage:  aut_or_tdc.coaccessible_here()

        Description:  deletes from <aut_or_tdc> every state that is not co-accessible

        Note:  a state is co-accessible if it is the starting point of a path ending in a final state
        """
        coaccessible_here_(self._this)


## ========================================================================= ##
    def useful_states(self):
        """
        Usage:  aut_or_tdc.coaccessible_states()

        Description:  returns the list of identifier of the states that are both accessible and co-accessible

        Returns: list of int

        Note:
            a state is accessible if it is the endpoint of a path starting from an initial state
            a state is co-accessible if it is the starting point of a path ending in a final state
        """
        return useful_states_(self._this)


## ========================================================================= ##
    def num_useful_states(self):
        """
        Usage:   aut_or_tdc.num_useful_states()

        Description:  returns the number of states that are both accessible and co-accessible

        Returns:  int

        Note:
            a state is accessible if it is the endpoint of a path starting from an initial state
            a state is co-accessible if it is the starting point of a path ending in a final state
        """
        return num_useful_states_(self._this)


## ========================================================================= ##
    def is_trim(self):
        """
        Usage:  aut_or_tdc.is_trim()

        Description:  returns true if every state of <aut_or_tdc/self> is both accessible and co-accessible

        Returns:  bool

        Note:
            a state is accessible if it is the endpoint of a path starting from an initial state
            a state is co-accessible if it is the starting point of a path ending in a final state
        """
        return is_trim_(self._this)


## ========================================================================= ##
    def trim(self):
        """
        Usage:  aut_or_tdc.trim()

        Description:  returns a copy of <aut_or_tdc/self> in which are deleted the states that are not both accessible and co-accessible

        Returns:  Automaton or Transducer

        Note:
            a state is accessible if it is the endpoint of a path starting from an initial state
            a state is co-accessible if it is the starting point of a path ending in a final state
        """
        return _BasicAutomaton_(trim_(self._this))


## ========================================================================= ##
    def trim_here(self):
        """
        Usage:  aut_or_tdc.trim_here()

        Description:  deletes from <aut_or_tdc> every state that is not both accessible and co-accessible

        Note:
            a state is accessible if it is the endpoint of a path starting from an initial state
            a state is co-accessible if it is the starting point of a path ending in a final state
        """
        trim_here_(self._this)


## ========================================================================= ##
    def concatenate(self, _BasicAutomaton other):
        """
        Usage:  aut_or_tdc.concatenate(other)

        Description:  computes the concatenation of <aut_or_tdc> with <other>.
        If needs be, the standard of each operand is computed beforehand.

        arg:
          other (Automaton or Transducer)

        Returns: Automaton or Transducer

        Preconditions:  The respective weightsets and labelsets of <aut_or_tdc> and <other> must be compatible.
        """
        return _BasicAutomaton_(concatenate_(self._this, other._this))

## ========================================================================= ##
    def concatenate_here(self, _BasicAutomaton other):
        """
        Usage:  aut_or_tdc.concatenate(other)

        Description:  computes the concatenation of <aut_or_tdc> with <other>.  The result is stored into <aut_or_tdc1>.  If needs be, the standard of each operand is computed beforehand.

        arg:
          other (Automaton or Transducer)

        Returns: Automaton or Transducer

        Preconditions:  The respective weightsets and labelsets of <aut_or_tdc> and <other> must be compatible.
        """
        self._set_cpp_class_bis(self.concatenate(other))


## ========================================================================= ##
    def lift(self):
        """
        Usage:  aut_or_tdc.lift()

        Description:  returns a copy of <aut_or_tdc/self> in which transitions have no labels and weights are rational expressions

        Returns:  Automaton or Transducer
        """
        return _BasicAutomaton_(lift_(self._this))


## ========================================================================= ##
    def lift_here(self):
        """
        Usage:  aut_or_tdc.lift_here()

        Description:  modifies the context of <aut_or_tdc/self>, transitions have no labels any longer and weights are now rational expressions

        Args:  aut_or_tdc (Automaton or Transducer)

        Returns:  Automaton or Transducer

        Note:  convenience function written at the python layer
        """
        self._set_cpp_class(lift_(self._this))


## ========================================================================= ##
    def transpose(self):
        """
        Usage:  aut_or_tdc.transpose()

        Description:  returns a copy of <aut_or_tdc/self> in which transitions are reversed, every previously final state is now initial and every previously initial state is now final

        Returns:  Automaton or Transducer

        Requirement:
        """
        return _BasicAutomaton_(transpose_(self._this))


## ========================================================================= ##
    def transpose_here(self):
        """
        Usage:  aut_or_tdc.transpose_here()

        Description:  reverses the transitions, makes final every state which was previously initial and makes initial every state which was previously final

        Returns:  Automaton or Transducer
        """
        transpose_here_(self._this)


## ========================================================================= ##
    def copy(self):
        """
        Usage:  aut_or_tdc.copy()

        Description:  builds a copy of <aut_or_tdc/self>

        Returns:  Automaton or Transducer
        """
        return _BasicAutomaton_(copy_(self._this))

## ========================================================================= ##
    def sccs(self):
        """
        Usage:  aut_or_tdc.sccs()

        Description:  computes the sccs of <aut_or_tdc>

        Returns:  List of list of int
        """
        return sccs_(self._this)

## ========================================================================= ##
    def scc_of(self, stt):
        """
        Usage:  aut_or_tdc.scc_of(stt)

        Description:  computes the scc of <aut_or_tdc> that contains <stt>.

        Args:   stt (int or str): state identifier (int) or state name (str)

        Returns:  List of int
        """
        return scc_of_(self._this, self._id_or_name(stt))

## ========================================================================= ##
    def condensation(self):
        """
        Usage:  aut_or_tdc.condensation()

        Description:  computes the strong components of <aut_or_tdc>, and merge every connected component into a single state.  The result is a graph that describes the strong-connexity relation in <aut_or_tdc>.

        Returns:  an Automaton or Transducer

        Precise definition:
        1) The context of the condensation is the same as <aut_or_tdc>, that is an Automaton or a Transducer, weighted or not.  However, the behavior of the condensation has no particular meaning.
        2) Each vertex in the condensation is a strong component of <aut_or_tdc> .
        3) There is an edge in the condensation from some vertex S to some vertex T in the condensation if there is one in <aut_or_tdc> from some s in S to some t in T.
        4) If, in the previous item, there are multiple pairs (s,t), then one edge is created in the condensation for each distinct label, and the weight of that edge is equal to the sum of the weights of the corresponding edges in <aut_or_tdc> . (If you use a weightset that is not positive, such as Z, you should probably use `characteristic` before `condensation` to avoid strange behavior.)
        5) A vertex S in the condensation is initial (resp. final) if and only if some state s in S is initial (resp. final) in <aut_or_tdc>.  Initial and final weights are added if there are several state s.

        Precondition: This function works for all weightsets, but the result might be not what you want if the weightset is not Positive.  Indeed, if it is possible that the sum of two weights is equal the additive neutral element, then some edges might be missing from the result. In that case, it is recommended to use `characteristic` prior to `condensation`.
        """
        return _BasicAutomaton_(condensation_(self._this))


## ========================================================================= ##
    def strip_history(self):
        """
        Usage: aut_or_tdc.strip_history()

        Description: removes the history from <aut_or_tdc>.
        """
        self._this.strip_history()

## ========================================================================= ##
    def aut_to_exp(self, str method=None):
        """
        Usage:  aut_or_tdc.aut_to_exp([method="min_inout_degree"])

        Description:  computes the rational expression of the language accepted by <aut_or_tdc/self> by state elimination.

        Argument:
            method (str, optional): govers the order in which states are eliminated
              - "min_inout_degree": always eliminate the state with minimal (in degree)x(out degree).
              - "min_id": eliminates state by increasing identifier (hence in an arbitrary order)

        Returns:
            RatExp, (weighted) expression of the language accepted by <aut_or_tdc/self>.
        """
        if method == None:
            return _RatExp(aut_to_exp1_(self._this))
        else:
            return _RatExp(aut_to_exp2_(self._this, method))
            
## ========================================================================= ##
    def has_explicit_name(self, int stt_id):
        """
        Usage:  aut_or_tdc.has_explicit_name(stt_id)

        Description:  returns True if <stt_id> has an explicit name.

        Args:  stt_id (int), identifier of a state

        Returns:  bool

        Note: states always has a name; if it is not explicit, it is built from its identifier.
        """
        return self._this.has_explicit_name(stt_id)

## ========================================================================= ##
    def get_state_name(self, int stt_id):
        """
        Usage:  aut_or_tdc.get_state_name(stt_id)

        Description:  returns the name of the state

        Args:  stt_id (int), identifier of a state

        Returns:  str
        """
        return self._this.get_state_name(stt_id)

## ========================================================================= ##
    def get_state_by_name(self, str name):
        """
        Usage:  aut_or_tdc.get_state_by_name(name)

        Argument: name (str): name to the state

        Returns: int or list, the state or the list of states that are named <name>

        Warning: Nothing ensures that state names are unique.
        """
        return self._state_name_map[name]

## ========================================================================= ##
    def set_state_name(self, int stt_id, str name):
        """
        Usage:  aut_or_tdc.set_state_name(stt_id, name)

        Description: changes the name of <stt_id> to <name>

        Argument: name (str): new name of the state <stt_id>

        See also: get_state_by_name and get_state_name
        """
        self._unregister_state_name(stt_id);
        self._this.set_state_name(stt_id, name);
        self._register_state_name(stt_id);


