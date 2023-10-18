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

## ========================================================================= ##
## ================== Functions with no equivalent method ================== ##
## ========================================================================= ##
def load(str filename, str fmt = 'json', bool internal_examples = False):
    """
    Usage:  load(filename [,fmt='json'] )

    Description:  returns the automaton or transducer stored in file <filename> under the format <format>.

    Args:
        filename (str),
            must be a relative path to a file.
        format (str, optional),
            admissible values are 'json', 'fado' and 'grail',
            defaults to 'json'.

    Returns:  Automaton or Transducer
    """
    if fmt == 'json':
        x = load_aut_or_exp(filename, internal_examples)
        if x.is_aut:
            return _BasicAutomaton_(basic_automaton_t(x.aut))
        else:
            return _RatExp(simple_ratexp_t(x.exp))
    else:
        aut_or_tdc= load_automaton_(filename, fmt)
        return _BasicAutomaton_(aut_or_tdc)

def list_examples():
    """
    Usage:  list_example_automata()

    Description:  displays the list of example of automata and transducers shipped in with Awali.
    """
    dico_aut = {}
    for s in example_automata_():
      dico_aut[s[0]] = s[1]

    dico_exp = {}
    for s in example_ratexps_():
      dico_exp[s[0]] = s[1]

    return {"automata":dico_aut, "ratexps":dico_exp}

def available_weightsets():
  """
    Usage: awalipy.available_weightsets()
    
    Description: provides the list of the weightsets available.

    Returns: a list of strings
  """
  res = []
  for x in available_semirings():
    res.append(str(x));
  return res;


## ========================================================================= ##
## =========== Equivalent method in basic_automaton_2_class.pyx ============ ##
## ========================================================================= ##
def display(_BasicAutomaton aut_or_tdc, horizontal=True, filename=None):
    """
    Usage: aut_or_tdc.display( [horizontal=True, [, filename ] ] )

    Description: displays <aut_or_tdc>. If <horizontal> is True, the layout is horizontal,         otherwise it is vertical.  If the optional argument <filename> is present, the automaton representation is in addition written to file "<filename>.gv" .

    Args:
        aut_or_tdc (Automaton or Transducer)
        horizontal (bool, optional),
            defaults to True
        filename (str, optional)


    Note:  
        requires the program graphviz for automatic generation of graphs, and a software for pdf-visualization.
        This functon is deprecated.
    """
    _deprecated(caller="display",since="2.0")
    aut_or_tdc.display(horizontal, filename)


#  ## ========================================================================= ##
#  def dot(_BasicAutomaton aut_or_tdc, bool history=False, bool horizontal=True):
#      """
#      Usage:  dot(aut_or_tdc)
#  
#      Description:  Returns the description of <aut_or_tdc> using the format dot.
#  
#      Parameter:  aut_or_tdc (Automaton or Transducer)
#  
#      Returns:  str
#      """
#      return aut_or_tdc.dot(history, horizontal)
#  
#  
#  ## ========================================================================= ##
#  def fado(_BasicAutomaton aut_or_tdc):
#      """
#      Usage:  fado(automaton)
#  
#      Description:  Returns the description of <automaton> using the format fado.
#  
#      Args:  automaton (Automaton)
#  
#      Returns: str
#      """
#      return aut_or_tdc.fado()
#  
#  
#  ## ========================================================================= ##
#  def grail(_BasicAutomaton aut_or_tdc):
#      """
#      Usage:  grail(automaton)
#  
#      Description:  Returns the description of <automaton> using the format grail.
#  
#      Args:  automaton: a awali.automaton
#  
#      Returns: str
#      """
#      return aut_or_tdc.grail()
#  
#  
#  ## ========================================================================= ##
#  def json(_BasicAutomaton aut_or_tdc):
#      """
#      Usage:  json(aut_or_tdc)
#  
#      Description:  Returns the description of <automaton> using the format json, which is the default format of Awali.
#  
#      Parameter:  aut_or_tdc: an Automaton or a Transducer
#  
#      Returns: str
#      """
#      return aut_or_tdc.json()


## ========================================================================= ##
def save(_BasicAutomaton aut_or_tdc, str s, str fmt = 'json'):
    """
    Usage:  aut.save( filename [, format='json' ] )

    Description:  writes <aut> in file <filename> using <format>.

    Parameters:
      filename (str)
      format (str, optional),
        admissible values are 'dot', 'fado', 'grail' and 'json',
        defaults to 'json'
    """
    _deprecated("save",since="2.0")
    aut_or_tdc.save(s,fmt)


## ========================================================================= ##
def states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  states(aut_or_tdc)

    Description:  returns the list of the identifiers of all the states of <aut_or_tdc>.

    Returns:  list of int
    """
    _deprecated("states",since="2.0")
    return aut_or_tdc.states()


## ========================================================================= ##
def num_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  num_states(aut_or_tdc)

    Description:  returns the number of states of <aut_or_tdc>

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  int
    """
    _deprecated("num_states",since="2.0")
    return aut_or_tdc.num_states()


## ========================================================================= ##
def has_state(_BasicAutomaton aut_or_tdc, int stt_id):
    """
    Usage:  has_state(aut_or_tdc, stt_id):

    Description:  Returns True if <stt_id> is indeed the identifier of a state of <aut_or_tdc>.

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int)

    Returns:  bool
    """
    _deprecated("has_state",since="2.0")
    return aut_or_tdc.has_state(stt_id)


## ========================================================================= ##
def initial_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  initial_states(aut_or_tdc)

    Description:  returns the list of the identifiers of the initial states of <aut_or_tdc>.

    Returns:  list of int
    """
    _deprecated("initial_states",since="2.0")
    return aut_or_tdc.initial_states()


## ========================================================================= ##
def num_initials(_BasicAutomaton aut_or_tdc):
    """
    Usage:  num_initials(aut_or_tdc)

    Description:  returns the number of initial states of <aut_or_tdc>

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  int
    """
    _deprecated("num_initials",since="2.0")
    return aut_or_tdc.num_initials()


## ========================================================================= ##
def is_initial(_BasicAutomaton aut_or_tdc, int stt_id):
    """
    Usage:  is_initial(aut_or_tdc, stt_id):

    Description:  returns True if the state identified by <stt_id> is initial in <aut_or_tdc>

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int)

    Returns:  bool
    """
    _deprecated("is_initial",since="2.0")
    return aut_or_tdc.is_initial(stt_id);


## ========================================================================= ##
def get_initial_weight(_BasicAutomaton aut_or_tdc, int stt_id):
    """
    Usage:  get_initial_weight(aut_or_tdc, stt_id):

    Description:  returns the initial weight of the state identified by <stt_id> in <aut_or_tdc>; returns the neutral additive element of the weight-set if the state is not initial

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int)

    Returns: str
    """
    _deprecated("get_initial_weight",since="2.0")
    return aut_or_tdc.get_initial_weight(stt_id)


## ========================================================================= ##
def set_initial(_BasicAutomaton aut_or_tdc, int stt_id, str weight=None):
    """
    Usage:  set_initial(aut_or_tdc, stt_id [, weight=aut_or_tdc.get_weightset().one() ] )

    Description:  sets in <aut_or_tdc> the initial weight to the state identified by <stt_id>; overwrites the eventual previous initial weight

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int), identifier of a state
        weight (str, optional)
            defaults to aut_or_tdc.get_weightset().one()

    Returns:  str, the new initial weight
    """
    _deprecated("set_initial",since="2.0")
    aut_or_tdc.set_initial(stt_id, weight)


## ========================================================================= ##
def add_initial(_BasicAutomaton aut_or_tdc, int stt_id, str weight=None):
    """
    Usage:  add_initial(aut_or_tdc, stt_id [, weight=aut_or_tdc.get_weightset().one() ] )

    Description:  adds in <aut_or_tdc> some initial weight to the state identified by <stt_id>

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int), identifier of a state
        weight (str, optional)
            defaults to aut_or_tdc.get_weightset().one()

    Returns:  str, the new initial weight
    """
    _deprecated("add_initial",since="2.0")
    return aut_or_tdc.add_initial(stt_id,weight)


## ========================================================================= ##
def unset_initial(_BasicAutomaton aut_or_tdc, int stt_id):
    """
    Usage:  unset_initial(aut_or_tdc, stt_id)

    Description:  makes the state identified by <stt_id> non-initial, that is, sets its initial weight to aut_or_tdc.get_weightset().zero().

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int), identifier of a state

    Returns:  str, the new initial weight
    """
    _deprecated("unset_initial",since="2.0")
    aut_or_tdc.unset_initial(stt_id)


## ========================================================================= ##
def final_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  final_states(aut_or_tdc)

    Description:  returns the list of the identifiers of the final states of <aut_or_tdc>.

    Returns:  list of int
    """
    _deprecated("final_states",since="2.0")
    return aut_or_tdc.final_states()


## ========================================================================= ##
def num_finals(_BasicAutomaton aut_or_tdc):
    """
    Usage:  num_finals(aut_or_tdc)

    Description: returns the number of final states of <aut_or_tdc>

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  int
    """
    _deprecated("num_finals",since="2.0")
    return aut_or_tdc.num_finals()


## ========================================================================= ##
def is_final(_BasicAutomaton aut_or_tdc, int stt_id):
    """
    Usage:  is_final(aut_or_tdc, stt_id):

    Description:  Returns True if the state identified by <stt_id> is final in <aut_or_tdc>

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int)

    Returns:  bool
    """
    _deprecated("is_final",since="2.0")
    return aut_or_tdc.is_final(stt_id)


## ========================================================================= ##
def get_final_weight(_BasicAutomaton aut_or_tdc, int stt_id):
    """
    Usage:  get_final_weight(aut_or_tdc, stt_id):

    Description:  returns the final weight of the state identified by <stt_id> in <aut_or_tdc>; returns the neutral additive element of the weight-set if the state is not final

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int)

    Returns: str
    """
    _deprecated("get_final_weight",since="2.0")
    return aut_or_tdc.get_final_weight(stt_id)


## ========================================================================= ##
def set_final(_BasicAutomaton aut_or_tdc, int stt_id, str weight=None):
    """
    Usage:  set_final(aut_or_tdc, stt_id [, weight=aut_or_tdc.get_weightset().one() ] )

    Description:  sets in <aut_or_tdc> the final weight to the state identified by <stt_id>; overwrites the eventual previous final weight

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int), identifier of a state
        weight (str, optional)
            defaults to aut_or_tdc.get_weightset().one()

    Returns:  str, the new final weight
    """
    _deprecated("set_final",since="2.0")
    aut_or_tdc.set_final(stt_id, weight)


## ========================================================================= ##
def add_final(_BasicAutomaton aut_or_tdc, int stt_id, str weight=None):
    """
    Usage:  add_final(aut_or_tdc, stt_id [, weight=aut_or_tdc.get_weightset().one() ] )

    Description:  adds in <aut_or_tdc> some final weight to the state identified by <stt_id>

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int), identifier of a state
        weight (str, optional)
            defaults to aut_or_tdc.get_weightset().one()

    Returns:  str, the new final weight
    """
    _deprecated("add_final",since="2.0")
    return aut_or_tdc.add_final(stt_id, weight)


## ========================================================================= ##
def unset_final(_BasicAutomaton aut_or_tdc, int stt_id):
    """
    Usage:  unset_final(aut_or_tdc, stt_id)

    Description:  makes the state identified by <stt_id> non-final, that is, sets its final weight to aut_or_tdc.get_weightset().zero().

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int), identifier of a state

    Returns:  str, the new final weight
    """
    _deprecated("unset_final",since="2.0")
    aut_or_tdc.unset_final(stt_id)


## ========================================================================= ##
def add_state(_BasicAutomaton aut_or_tdc, name= None):
    """
    Usage:  add_state(aut_or_tdc [, name] )

    Description:  adds a new state to <aut_or_tdc> and returns its identifier.

    Args:
        aut_or_tdc (Automaton or Transducer)
        name (str, optional),
            no default value: if not given, the state is unnamed.

    Returns:  int
    """
    _deprecated("add_state",since="2.0")
    return aut_or_tdc.add_state(name)


## ========================================================================= ##
def del_state(_BasicAutomaton aut_or_tdc, int state):
    """
    Usage:  aut_or_tdc.del_state(stt_id)

    Description:  deletes from <aut_or_tdc> the state identified by <stt_id> and all its incoming and outgoing transitions

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int), a state identifier
    """
    _deprecated("del_state",since="2.0")
    aut_or_tdc.del_state(state)


## ========================================================================= ##
def transitions(_BasicAutomaton aut_or_tdc):
    """
    Usage:  transitions(aut_or_tdc)

    Description:  returns the idenfitiers of all transitions of <aut_or_tdc>

    Returns:  list of int
    """
    _deprecated("transitions",since="2.0")
    return aut_or_tdc.transitions()


## ========================================================================= ##
def num_transitions(_BasicAutomaton aut_or_tdc):
    """
    Usage:  num_transitions(aut_or_transition)

    Description:  returns the number of transitions of <aut_or_tdc>.

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  int
    """
    _deprecated("num_transitions",since="2.0")
    return aut_or_tdc.num_transitions()


## ========================================================================= ##
def has_transition(_BasicAutomaton aut_or_tdc, int tr_id):
    """
    Usage:  aut_or_tdc.has_transition(tr_id):

    Description:  returns True if <tr_id> is the identifier of a transition of <aut_or_tdc>

    Args:
        aut_or_tdc (Automaton or Transducer)
        tr_id (int)

    Returns:  bool
    """
    _deprecated("has_transition",since="2.0")
    aut_or_tdc.has_transition(tr_id)


## ========================================================================= ##
def src_of(_BasicAutomaton aut_or_tdc, int transition):
    """
    Usage:  src_of(aut_or_tdc, tr_id)

    Description: returns the identifier of the source state of the transition identified by <tr_id>

    Args: tr_id (int)

    Returns: int
    """
    _deprecated("src_of",since="2.0")
    return aut_or_tdc.src_of(transition)


## ========================================================================= ##
def dst_of(_BasicAutomaton aut_or_tdc, int transition):
    """
    Usage:  dst_of(aut_or_tdc, tr_id)

    Description: returns the identifier of the destination state of the transition identified by <tr_id>

    Args: tr_id (int)

    Returns: int
    """
    _deprecated("dst_of",since="2.0")
    return aut_or_tdc.dst_of(transition)


## ========================================================================= ##
def weight_of(_BasicAutomaton aut_or_tdc, int transition):
    """
    Usage:  weight_of(aut_or_tdc, tr_id)

    Description: returns the weight of the transition identified by <tr_id>

    Args: tr_id (int)

    Returns: str
    """
    _deprecated("weight_of",since="2.0")
    return aut_or_tdc.weight_of(transition)


## ========================================================================= ##
def set_eps_transition(_BasicAutomaton aut_or_tdc, int src_id, int dst_id, str new_weight=None):
    """
    Usage:  set_eps_transition(aut_or_tdc, src_id, dst_id [, new_weight=aut_or_tdc.weigh_one()] )

    Description:  sets as <new_weight> the weight of the epsilon-transition \
going from <src_id> to <dst_id>; \
overwrites the eventual previous weight; \
creates the transition if necessary.

    Args:
        aut_or_tdc (Automaton or Transducer)
            needs to allow epsilon transition
        src_id (int), representing a state
        dst_id (int), representing a state
        label (str)
        new_weight (str, optional),
            defaults to aut_or_tdc.get_weightset().one().

    Note:  by default, transducers allow epsilon transitions but automata do not
    """
    _deprecated("set_eps_transition",since="2.0")
    return aut_or_tdc.set_eps_transition(src_id, dst_id, new_weight)


## ========================================================================= ##
def add_eps_transition(_BasicAutomaton aut_or_tdc, int src_id, int dst_id, str extra_weight=None):
    """
    Usage:  add_eps_transition(aut_or_tdc, src_id, dst_id [, extra_weight=aut_or_tdc.get_weightset().one() ] )

    Description:  adds in <aut_or_tdc> some <extra_weight> to the epsilon-transition \
going from <src_id> to <dst_id>; creates the \
transition if necessary.

    Args:
        aut_or_tdc (Automaton or Transducer)
            needs to allow epsilon transition
        src_id (int), identifier of a state
        dst_id (int), identifier of a state
        extra_weight (str, optional)
            defaults to aut_or_tdc.get_weightset().one()

    Returns:  str, the new weight of the transition

    Note:  by default, transducers allow epsilon transitions but automata do not
    """
    _deprecated("add_eps_transition",since="2.0")
    return aut_or_tdc.add_eps_transition(src_id, dst_id, extra_weight)


## ========================================================================= ##
def set_weight(_BasicAutomaton aut_or_tdc, int transition, str weight=None):
    """
    Usage:  set_weight(aut_or_tdc, tr_id, new_weight)

    Description:  sets as <new_weight> weight of the transition identified by <tr_id>

    Args:
        tr_id (int), identifier of a transition
        new_weight (str)
    """
    _deprecated("set_weight",since="2.0")
    return aut_or_tdc.set_weight(transition, weight)


## ========================================================================= ##
def add_weight(_BasicAutomaton aut_or_tdc, int transition, str weight):
    """
    Usage:  add_weight(aut_or_tdc, tr_id, extra_weight)

    Description:  adds some <extra_weight> to the transition identified by <tr_id>

    Args:
        aut_or_tdc (Automaton or Transducer)
        tr_id (int), identifier of a transition
        extra_weight (str)

    Returns: str, the new weight of the transition
    """
    return aut_or_tdc.add_weight(transition, weight)
    _deprecated("add_weight",since="2.0")


## ========================================================================= ##
def lmul_weight(_BasicAutomaton aut_or_tdc, int transition, str weight):
    """
    Usage:  lmul_weight(aut_or_tdc, tr_id, weight_factor)

    Description:  multiplies on the left the weight of the transition identified by <tr_id> by some <weight_factor>

    Args:
        aut_or_tdc (Automaton or Transducer)
        tr_id (int), identifier of a transition
        weight_factor (str)

    Returns: str, the new weight of the transition
    """
    _deprecated("lmul_weight",since="2.0")
    return aut_or_tdc.lmul_weight(transition, weight)


## ========================================================================= ##
def rmul_weight(_BasicAutomaton aut_or_tdc, int transition, str weight):
    """
    Usage:  rmul_weight(aut_or_tdc, tr_id, weight_factor)

    Description:  multiplies on the right the weight of the transition identified by <tr_id> by some <weight_factor>

    Args:
        aut_or_tdc (Automaton or Transducer)
        tr_id (int), identifier of a transition
        weight_factor (str)

    Returns: str, the new weight of the transition
    """
    _deprecated("rmul_weight",since="2.0")
    return aut_or_tdc.rmul_weight(transition, weight)


## ========================================================================= ##
def outin(_BasicAutomaton aut_or_tdc, int src, int dst):
    """
    Usage:  outin(aut_or_tdc, src_id, dst_id)

    Description:  returns the list of the ids of all transitions going from <src_id> to <dst_id>

    Args:
        aut_or_tdc (Automaton or Transducer)
        src_id (int), identifier of a state
        dst_id (int), identifier of a state

    Returns: list of int
    """
    _deprecated("outin",since="2.0")
    return aut_or_tdc.outin(src,dst)


## ========================================================================= ##
def get_state_name(_BasicAutomaton aut_or_tdc, int state):
    """
    Usage:  get_state_name(aut_or_tdc, stt_id)

    Description:  returns the name of the state

    Args:
        aut_or_tdc (Automaton or Transducer)
        stt_id (int), identifier of a state

    Returns:  str
    """
    _deprecated("get_state_name",since="2.0")
    return aut_or_tdc.get_state_name(state)



## ========================================================================= ##
def get_weightset(_BasicAutomaton aut_or_tdc):
    """
    Usage:   get_weightset(aut_or_tdc)

    Description:  return the name of the weightset of <aut_or_tdc>

    Returns:  str
    """
    _deprecated("get_weightset",since="2.0")
    return aut_or_tdc.get_weightset()


## ========================================================================= ##
def get_static_context(_BasicAutomaton aut_or_tdc):
    """
    Usage:  get_static_context

    Description:  returns the complete static context of <aut_or_tdc>

    Returns:  str

    Note:  this function is mostly for debugging purposes since there is no way to use contexts at the python layer.
    """
    _deprecated("get_static_context",since="2.0")
    return aut_or_tdc.get_static_context()


## ========================================================================= ##
def accessible_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  accessible_states(aut_or_tdc)

    Description:  returns the list of idenfitied of the accessibles states of <aut_or_tdc>

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  list of int
    """
    _deprecated("accessible_states",since="2.0")
    return aut_or_tdc.accessible_states()


## ========================================================================= ##
def num_accessible_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  num_accessible_states(aut_or_tdc)

    Description:  returns the number of accessible states of <aut_or_tdc>

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  int
    """
    _deprecated("num_accessible_states",since="2.0")
    return aut_or_tdc.num_accessible_states()


## ========================================================================= ##
def is_accessible(_BasicAutomaton aut_or_tdc):
    """
    Usage:  is_accessible(aut_or_tdc)

    Description:  returns true if every state of <aut_or_tdc> is accessible

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  bool
    """
    _deprecated("is_accessible",since="2.0")
    return aut_or_tdc.is_accessible()


## ========================================================================= ##
def accessible(_BasicAutomaton aut_or_tdc):
    """
    Usage:  accessible(aut_or_tdc)

    Description:  returns a copy of <aut_or_tdc> in which states that are not accessible are deleted

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  Automaton or Transducer
    """
    _deprecated("accessible",since="2.0")
    return aut_or_tdc.accessible()


## ========================================================================= ##
def accessible_here(_BasicAutomaton aut_or_tdc):
    """
    Usage:  aut_or_tdc.accessible_here()

    Description:  deletes from <aut_or_tdc> every non-accessible state.

    Args:  aut_or_tdc (Automaton or Transducer)
    """
    _deprecated("accessible_here",since="2.0")
    aut_or_tdc.accessible_here()


## ========================================================================= ##
def coaccessible_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  coaccessible_states(aut_or_tdc)

    Description:  returns the list of idenfitied of the co-accessibles states of <aut_or_tdc>

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  list of int
    """
    _deprecated("coaccessible_states",since="2.0")
    return aut_or_tdc.coaccessible_states()


## ========================================================================= ##
def num_coaccessible_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  num_coaccessible_states(aut_or_tdc)

    Description:  returns the number of co-accessible states of <aut_or_tdc>

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  int
    """
    _deprecated("num_coaccessible_states",since="2.0")
    return aut_or_tdc.num_coaccessible_states()


## ========================================================================= ##
def is_coaccessible(_BasicAutomaton aut_or_tdc):
    """
    Usage:  is_coaccessible(aut_or_tdc)

    Description:  returns true if every state of <aut_or_tdc> is co-accessible

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  bool
    """
    _deprecated("is_coaccessible",since="2.0")
    return aut_or_tdc.is_coaccessible()


## ========================================================================= ##
def coaccessible(_BasicAutomaton aut_or_tdc):
    """
    Usage:  coaccessible(aut_or_tdc)

    Description:  returns a copy of <aut_or_tdc> in which are deleted the states that are not co-accessible

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  Automaton or Transducer
    """
    _deprecated("coaccessible",since="2.0")
    return  aut_or_tdc.coaccessible()


## ========================================================================= ##
def coaccessible_here(_BasicAutomaton aut_or_tdc):
    """
    Usage:  aut_or_tdc.coaccessible_here()

    Description:  deletes from <aut_or_tdc> every state that is not co-accessible

    Args:  aut_or_tdc (Automaton or Transducer)
    """
    _deprecated("coaccessible_here",since="2.0")
    aut_or_tdc.coaccessible_here()


## ========================================================================= ##
def useful_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  coaccessible_states(aut_or_tdc)

    Description:  returns the list of identifier of the states that are both accessible and co-accessible

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  list of int
    """
    _deprecated("useful_states",since="2.0")
    return aut_or_tdc.useful_states()


## ========================================================================= ##
def num_useful_states(_BasicAutomaton aut_or_tdc):
    """
    Usage:  num_useful_states(aut_or_tdc)

    Description:  returns the number of states that are both accessible and co-accessible

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  int
    """
    _deprecated("num_useful_states",since="2.0")
    return aut_or_tdc.num_useful_states()


## ========================================================================= ##
def is_trim(_BasicAutomaton aut_or_tdc):
    """
    Usage:  is_trim(aut_or_tdc)

    Description:  returns true if every state of <aut_or_tdc> is both accessible and co-accessible

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  bool
    """
    _deprecated("is_trim",since="2.0")
    return aut_or_tdc.is_trim()


## ========================================================================= ##
def trim(_BasicAutomaton aut_or_tdc):
    """
    Usage:  trim(aut_or_tdc)

    Description:  returns a copy of <aut_or_tdc> in which are deleted the states that are not both accessible and co-accessible

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  Automaton or Transducer
    """
    _deprecated("trim",since="2.0")
    return aut_or_tdc.trim()

## ========================================================================= ##
def concatenate(aut_or_tdc1, aut_or_tdc2):
    """
    Usage:  concatenate(aut_or_tdc1, aut_or_tdc2)

    Description:  computes the concatenation of <aut_or_tdc1> with <aut_or_tdc2>.
    If needs be, the standard of each operand is computed beforehand.

    arg:
      aut_or_tdc1 (Automaton or Transducer)
      aut_or_tdc2 (Automaton or Transducer)

    Returns: Automaton or Transducer

    Preconditions:  The respective weightsets and labelsets of <aut_or_tdc1> and <aut_or_tdc2> must be compatible.
    """
    _deprecated("concatenate",since="2.0")
    return aut_or_tdc1.concatenate(aut_or_tdc2)

## ========================================================================= ##
def concatenate_here(aut_or_tdc1, aut_or_tdc2):
    """
    Usage:  aut_or_tdc.concatenate(other)

    Description:  computes the concatenation of <aut_or_tdc1> with <aut_or_tdc2>.  The result is stored into <aut_or_tdc1>.  If needs be, the standard of each operand is computed beforehand.

    arg:
      aut_or_tdc1 (Automaton or Transducer)
      aut_or_tdc2 (Automaton or Transducer)

    Returns: void

    Preconditions:  The respective weightsets and labelsets of <aut_or_tdc> and <other> must be compatible.
    """
    _deprecated("concatenate_here",since="2.0")
    aut_or_tdc1.concatenate_here(aut_or_tdc2)

## ========================================================================= ##
def trim_here(_BasicAutomaton aut_or_tdc):
    """
    Usage:  trim_here(aut_or_tdc)

    Description:  deletes from <aut_or_tdc> every state that is not both accessible and co-accessible

    Args:  aut_or_tdc (Automaton or Transducer)
    """
    _deprecated("trim_here",since="2.0")
    aut_or_tdc.trim_here()


### ========================================================================= ##
#def lift(_BasicAutomaton aut_or_tdc):
#    """
#    Usage:  lift(aut_or_tdc)
#
#    Description:  returns a copy of <aut_or_tdc> in which transitions have no labels and weights are rational expressions
#
#    Args:  aut_or_tdc (Automaton or Transducer)
#
#    Returns:  Automaton or Transducer
#    """
#    return aut_or_tdc.lift()
#
#
#========================================================================= ##
#def lift_here(_BasicAutomaton aut_or_tdc):
#    """
#    Usage:  lift_here(aut_or_tdc)
#
#    Description:  modifies the context of aut_or_tdc, transitions have no labels any longer and weights are now rational expressions
#
#    Args:  aut_or_tdc (Automaton or Transducer)
#
#    Returns:  Automaton or Transducer
#
#    Note:  convenience function written at the python layer.
#    """
#    return aut_or_tdc.lift_here()


## ========================================================================= ##
def transpose(_BasicAutomaton aut_or_tdc):
    """
    Usage:  transpose(aut_or_tdc)

    Description:  returns a copy of <aut_or_tdc> in which transitions are reversed, every previously final state is now initial and every previously initial state is now final

    Returns:  Automaton or Transducer
    """
    _deprecated("transpose",since="2.0")
    return aut_or_tdc.transpose()


## ========================================================================= ##
def transpose_here(_BasicAutomaton aut_or_tdc):
    """
    Usage:  transpose_here(aut_or_tdc)

    Description:  reverses the transitions, makes final every state which was previously initial and makes initial every state which was previously final

    Returns:  Automaton or Transducer
    """
    _deprecated("transpose_here",since="2.0")
    aut_or_tdc.transpose_here()


## ========================================================================= ##
def copy(_BasicAutomaton aut_or_tdc):
    """
    Usage:  copy(aut_or_tdc)

    Description:  returns a copy of <aut_or_tdc>

    Returns: Automaton or Transducer, of the same type of aut_or_tdc
    """
    _deprecated("copy",since="2.0")
    return aut_or_tdc.copy()


## ========================================================================= ##
def sccs(_BasicAutomaton aut_or_tdc):
    """
    Usage:  sccs(aut_or_tdc)

    Description:  computes the sccs of <aut_or_tdc>

    Args:  aut_or_tdc (Automaton or Transducer)

    Returns:  List of list of int
    """
    _deprecated("sccs",since="2.0")
    return aut_or_tdc.sccs()


## ========================================================================= ##
def scc_of(_BasicAutomaton aut_or_tdc, stt_id):
    """
    Usage:  scc_of(aut_or_tdc, stt_id)

    Description:  computes the scc of <aut_or_tdc> that contains <stt_id>.

    Args:
      aut_or_tdc (Automaton or Transducer)
      stt_id (int), identifier of a state

    Returns:  List of int
    """
    _deprecated("sccs_of",since="2.0")
    return aut_or_tdc.scc_of_(stt_id)

## ========================================================================= ##
def are_isomorphic(_BasicAutomaton aut_or_tdc1, _BasicAutomaton aut_or_tdc2):
    """
    Usage:   are_isomorphic(aut_or_tdc1, aut_or_tdc2)

    Description:  tests whether <aut_or_tdc1> and <aut_or_tdc2> are isomorphic as labelled graph.

    Args:  aut1, aut2 (Automaton):

    Returns:  bool, True if <aut_or_tdc1> and <aut_or_tdc2> are isomorphic.
    """
    return  are_isomorphic_(aut_or_tdc1._this, aut_or_tdc2._this)
