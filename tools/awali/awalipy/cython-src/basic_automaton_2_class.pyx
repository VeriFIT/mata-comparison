# This file is part of Awali.
# Copyright 2016-2021 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
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
    a._this= aut_or_tdc
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
    cdef void _set_cpp_class(self, basic_automaton_t other):
        """
        Modify wrapped object. Internal use only.
        This seems to be quite fragile. Modify the code with caution.
        """
        self._this.rewrap_into_me_(other)

## ========================================================================= ##
    cdef void _set_cpp_class_bis(self, _BasicAutomaton other):
        """
        Modify wrapped object. Internal use only.
        This seems to be quite fragile. Modify the code with caution.
        """
        self._this.rewrap_into_me_(other._this)


## ========================================================================= ##
    def __init__(self):
        pass


## ========================================================================= ##
## == Non-method equivalent function in fused_Automaton_or_Transducer.pyx == ##
## ========================================================================= ##
## These methods are overridden in derived class (Automaton and Transducer)
## and hence are probably not visible for users.

    def del_transition(self, int transition_or_source, dest_or_none=None):
        if dest_or_none is None:
            self._this.del_transition1(transition_or_source)
        else:
            self._this.del_transition2(transition_or_source, dest_or_none)

    def predecessors(self, int stt_id):  return self._this.predecessors1(stt_id)
    def successors(self, int stt_id):  return self._this.successors1(stt_id)
    def incoming(self, int stt_id):  return self._this.incoming1(stt_id)
    def outgoing(self, int stt_id):  return self._this.outgoing1(stt_id)


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
    def save(self, str s, str fmt = 'json'):
        """
        Usage:  automaton.save( filename [, format='json' ] )

        Description:  writes <automaton> in file <filename> using <format>.

        Args:
          filename (str)
          format (str, optional),
              admissible values are 'dot', 'fado', 'grail' and 'json'
              defaults to 'json'
        """
        save_automaton_(self._this, s, fmt)


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
    def is_initial(self, int stt_id):
        """
        Usage:  aut_or_tdc.is_initial(stt_id)

        Description:  returns True if the state identified by <stt_id> is initial in <aut_or_tdc>

        Args:  stt_id (int)

        Returns: bool
        """
        return self._this.is_initial(stt_id);


## ========================================================================= ##
    cpdef str get_initial_weight(self, int stt_id):
        """
        Usage:  aut_or_tdc.get_initial_weight(stt_id):

        Description:  returns the initial weight of the state identified by <stt_id> in <aut_or_tdc/self>; returns the neutral additive element of the weight-set if the state is not initial

        Args:  stt_id (int)

        Returns:  str
        """
        return self._this.get_initial_weight(stt_id)


## ========================================================================= ##
    def set_initial(self, int stt_id, str weight= None):
        """
        Usage:  aut_or_tdc.set_initial(stt_id [, weight=aut.get_weightset().one() ] )

        Description:  sets in <aut_or_tdc/self> the initial weight to the state identified by <stt_id>; overwrites the eventual previous initial weight

        Args:
            stt_id (int), identifier of a state
            weight (str, optional)
                defaults to aut.get_weightset().one()

        Returns:  str, the new initial weight
        """
        if weight is None:
            self._this.set_initial1(stt_id)
        else:
            self._this.set_initial2(stt_id, weight)


## ========================================================================= ##
    cpdef str add_initial(self, int stt_id, str weight= None):
        """
        Usage:  aut_or_tdc.add_initial(stt_id [, weight=aut.get_weightset().one() ] )

        Description:  adds in <aut_or_tdc/self> some initial weight to the state identified by <stt_id>

        Args:
            stt_id (int), identifier of a state
            weight (str, optional)
                defaults to aut.get_weightset().one()

        Returns:  str, the new initial weight
        """
        if (weight is None):
            return self.add_initial(stt_id, self.get_weightset().one())
        else:
            return self._this.add_initial(stt_id, weight)


## ========================================================================= ##
    def unset_initial(self, int stt_id):
        """
        Usage:  aut_or_tdc.unset_initial(stt_id)

        Description:  makes the state identified by <stt_id> non-initial, that is, sets its initial weight to aut.weight_zero().

        Args:  stt_id (int), identifier of a state
        """
        self._this.unset_initial(stt_id)


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
    def is_final(self, int stt_id):
        """
        Usage:  aut_or_tdc.is_final(stt_id):

        Description:  returns True if the state identified by <stt_id> is final in <aut_or_tdc/self>

        Args:  stt_id (int)

        Returns:  bool
        """
        return self._this.is_final(stt_id)


## ========================================================================= ##
    cpdef str get_final_weight(self, int stt_id):
        """
        Usage:  aut_or_tdc.get_final_weight(stt_id):

        Description:  returns the final weight of the state identified by <stt_id> in <aut_or_tdc/self>; returns the neutral additive element of the weight-set if the state is not final

        Args:  stt_id (int)

        Returns: str
        """
        return self._this.get_final_weight(stt_id)


## ========================================================================= ##
    def set_final(self, int stt_id, str weight=None):
        """
        Usage:  aut_or_tdc.set_final(stt_id [, weight=aut.get_weightset().one() ] )

        Description:  sets in <aut_or_tdc/self> the final weight to the state identified by <stt_id>; overwrites the eventual previous final weight

        Args:
            stt_id (int), identifier of a state
            weight (str, optional)
                defaults to aut.get_weightset().one()
        """
        if weight is None:
            self._this.set_final1(stt_id)
        else:
            self._this.set_final2(stt_id, weight)


## ========================================================================= ##
    cpdef str add_final(self, int stt_id, str weight=None):
        """
        Usage:  aut_or_tdc.add_final(stt_id [, weight=aut.get_weightset().one() ] )

        Description:  adds in <aut_or_tdc/self> some final weight to the state identified by <stt_id>

        Args:
            stt_id (int), identifier of a state
            weight (str, optional)
                defaults to aut.get_weightset().one()

        Returns:  str, the new final weight
        """
        if (weight is None):
            weight = self.get_weightset().one()
        return self._this.add_final(stt_id, weight)


## ========================================================================= ##
    def unset_final(self, int stt_id):
        """
        Usage:  aut_or_tdc.unset_final(stt_id)

        Description:  makes the state identified by <stt_id> non-final, that is, sets its final weight to aut.weight_zero().

        Args:  stt_id (int), identifier of a state

        Returns:  str, the new final weight
        """
        self._this.unset_final(stt_id)


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
          return self._this.add_state1(name)


## ========================================================================= ##
    def del_state(self, int stt_id):
        """
        Usage:  aut_or_tdc.del_state(stt_id)

        Description:  deletes from <aut_or_tdc/self> the state identified by <stt_id> and all its incoming and outgoing transitions

        Args:  stt_id (int), a state identifier
        """
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
    def get_state_name(self, int stt_id):
        """
        Usage:  aut_or_tdc.get_state_name(stt_id)

        Description:  returns the name of the state

        Args:  stt_id (int), identifier of a state

        Returns:  str
        """
        return self._this.get_state_name(stt_id)



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
    def scc_of(self, stt_id):
        """
        Usage:  aut_or_tdc.scc_of(stt_id)

        Description:  computes the scc of <aut_or_tdc> that contains <stt_id>.

        Args:  stt_id (int), identifier of a state

        Returns:  List of int
        """
        return scc_of_(self._this, stt_id)

## ========================================================================= ##
    def condensation(self):
        """
        Usage:  aut_or_tdc.condensation()

        Description:  computes the sccs of <aut_or_tdc> condensed as a DAG.

        Returns:  an Automaton or Transducer
        """
        return _BasicAutomaton_(condensation_(self._this))
