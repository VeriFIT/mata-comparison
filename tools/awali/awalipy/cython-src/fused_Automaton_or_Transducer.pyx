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

#This file contains the functions acting *either* on Automata or Transducers
#but possibly not in the same manner.  Functions acting indifferently on
#automata and transducers are contained in the files starting by
#``basic_automaton''. These function uses the fused type
#Automaton_or_Transducer for their first argument and usually feature an
#argument `label' whose type depends on the type of the first argument (str
#if Automaton, list if Transducer).

ctypedef fused Automaton_or_Transducer:
    Automaton
    Transducer


## ========================================================================= ##
def eval(Automaton_or_Transducer aut_or_tdc, object arg):
    """
    Usage:
        [a] eval(aut, word)
        [b] eval(tdc, word)
        [c] eval(tdc, exp)
        [d] eval(tdc, aut)

    Descriptions:
        [a] returns the weight of <word> in the automaton <aut/self>
        [b,c,d] returns an automaton or a transducer whose behaviour is \
the image(s) of the second argument (either <word>, <exp> or <aut>) \
by <tdc>.

    Args:
        aut (Automaton)
        tdc (Transducer)
        word (str)
        exp (RatExp)

    Returns:
        [a] str
        [b,c,d] Automaton or Transducer, having one less tape than <tdc>
    """
    _deprecated("eval",since="2.0")
    return aut_or_tdc.eval(arg)


## ========================================================================= ##
def set_transition(Automaton_or_Transducer aut_or_tdc, int src_id, int dst_id, label, str new_weight=None):
    """
    Usage:
        set_transition(aut, src_id, dst_id, label [, weight=aut.get_weightset().one() ] )
        set_transition(tdc, src_id, dst_id, labels [, weight=tdc.get_weightset().one() ] )

    Description:  in the automaton <aut> (resp. in the transducer <tdc>), \
set as <new_weight> the weight of the transition going from <src_id> \
to <dst_id> and labelled by <label> (resp. by  <labels>); \
overwrites the eventual previous weight; \
creates the transition if necessary.

    Args:
        aut (Automaton)
        tdc (Transducer)
        src_id (int), identifier of a state
        dst_id (int), identifier of a state
        label (str)
        labels (list of str)
        new_weight (str, optional)
            defaults to aut.get_weightset().one() or tdc.get_weightset().one()
    """
    _deprecated("set_transition",since="2.0")
    return aut_or_tdc.set_transition(src_id, dst_id, label, new_weight)


## ========================================================================= ##
def add_transition(Automaton_or_Transducer aut_or_tdc, int src_id, int dst_id, label, str weight=None):
    """
    Usage:
        add_transition(aut, src_id, dst_id, label [, weight=aut.get_weightset().one() ] )
        add_transition(tdc, src_id, dst_id, labels [, weight=tdc.get_weightset().one() ] )

    Description:  add in <aut> (resp. in <tdc>) some weight to the transition \
going from <src_id> to <dst_id> and labelled by <label> (resp. by  <labels>); creates the \
transition if necessary.

    Args:
        aut (Automaton)
        tdc (Transducer)
        src_id (int), identifier of a state
        dst_id (int), identifier of a state
        label (str)
        labels (list of str)
        weight (str, optional)
            defaults to aut.get_weightset().one() or tdc.get_weightset().one()

    Returns:  str, the new weight of the transition
    """
    _deprecated("add_transitions",since="2.0")
    return aut_or_tdc.add_transition(src_id, dst_id, label, weight)



## ========================================================================= ##
def has_transition(Automaton_or_Transducer aut_or_tdc, int tr_or_src_id, object dst_id=None, object label=None):
    """
    Usage:
        has_transition(aut, tr_id)
        has_transition(tdc, tr_id)
        has_transition(aut, src_id, dst_id, label)
        has_transition(tdc, src_id, dst_id, labels)

    Description:  Returns True if the transition given as argument, or \
determined by the arguments, exists (in <aut> or <tdc>).

    Arguments:
        aut (Automaton)
        tdc (Transducer)
        tr_id (int), identifier of the transition
        src_id (int), identifier of the source state
        dst_id (int), identifier of the destination state
        label (str), label of the transition
        labels (list of str), labels of the transition

    Returns:
        bool, True if the transition exists.
    """
    _deprecated("has_transition",since="2.0")
    if ((dst_id is None) != (label is None)):
        _bad_arg_number_("awali", "has_transition", [2,4])
    return aut_or_tdc.has_transition(tr_or_src_id, dst_id, label)


## ========================================================================= ##
def del_transition(Automaton_or_Transducer aut_or_tdc, int tr_or_src_id, object dst_id=None, object label=None):
    """
    Usage:
        [a]  del_transition(aut, tr_num)
        [b]  del_transition(aut, src_id, dst_id)
        [c]  del_transition(aut, src_id, dst_id, label)
        [d]  del_transition(tdc, tr_num)
        [e]  del_transition(tdc, src_id, dst_id)
        [f]  del_transition(tdc, src_id, dst_id, labels)

    Description:
        [a,c]  removes from the automaton <aut> the unique transition \
determined either by an identifier <tr_num>, or by a source <src_id>, \
a destination <dst_id> and a <label>
        [b,e]  removes from <aut> or <tdc> every transition going from \
<src_id> to <dst_id>
        [d,f]  removes from the transducer <tdc> the unique transition \
determined either by an identifier <tr_num>, or by a source <src_id>, \
a destination <dst_id> and a tuple of <labels>

    Args:
        aut (Automaton)
        tdc (Transducer)
        tr_num (int), represents a transition
        src_id (int), identifier of the source state
        dst_id (int), identifier of the destination state
        label (str), a label
        labels (list of str)
    """
    _deprecated("del_transition",since="2.0")
    aut_or_tdc.del_transition(tr_or_src_id, dst_id, label)


## ========================================================================= ##
def get_transition(Automaton_or_Transducer aut_or_tdc, int src_id, int dst_id, label):
    """
    Usage:
        get_transition(aut, src_id, dst_id, label)
        get_transition(tdc, src_id, dst_id, labels)

    Description:  returns the identifier of the transition of <aut> (resp. \
<tdc>) going from <src_id> to <dst_id> and labelled by <label> (resp. \
<labels>).

    Args:
        aut (Automaton)
        tdc (Transducer)
        src_id (int), identifier of the source state
        dst_id (int), identifier of the destination state
        label (str)
        labels (list of str)

    Returns:
        int, identifier of a transition
    """
    _deprecated("get_transition",since="2.0")
    return aut_or_tdc.get_transition(src_id, dst_id, label)


## ========================================================================= ##
def label_of(Automaton_or_Transducer aut_or_tdc, int tr_id):
    """
    Usage:
        [a] label_of(aut, tr_id)
        [b] label_of(tdc, tr_id)

    Description:  returns the label or labels of a transition.

    Args:
        aut (Automaton)
        tdc (Transducer)
        tr_id (int), identifier of a transition

    Returns:
        [a] str
        [b] list of str
    """
    _deprecated("label_of",since="2.0")
    return aut_or_tdc.label_of(tr_id);


## ========================================================================= ##
def incoming(Automaton_or_Transducer aut_or_tdc, int stt_id, label=None):
    """
    Usage:
        incoming(aut, stt_id [,label])
        incoming(tdc, stt_id [,labels])

    Description:  returns the identifiers of all incoming transitions of \
<stt_id> in the automaton <aut> (resp. in the transducer <tdc>); \
if <label> (resp. <labels>) is provided, returns only the identifiers of the \
transitions labelled by <label> (resp. <labels>).

    Args:
        aut (Automaton)
        tdc (Automaton)
        stt_id (int), identifier of a state
        label (str, optional)
        labels (list of str, optional)

    Returns:  list of int, identifiers of transitions
    """
    _deprecated("incoming",since="2.0")
    return aut_or_tdc.incoming(stt_id, label)


## ========================================================================= ##
def outgoing(Automaton_or_Transducer aut_or_tdc, int stt_id, label=None):
    """
    Usage:
        outgoing(aut, stt_id [,label])
        outgoing(tdc, stt_id [,labels])

    Description:  returns the identifiers of all outgoing transitions of \
<stt_id> in the automaton <aut> (resp. in the transducer <tdc>); \
if <label> (resp. <labels>) is provided, returns only the identifiers of the \
transitions labelled by <label> (resp. <labels>).

    Args:
        aut (Automaton)
        tdc (Automaton)
        stt_id (int), identifier of a state
        label (str, optional)
        labels (list of str, optional)

    Returns:  list of int, identifiers of transitions
    """
    _deprecated("outgoing",since="2.0")
    return aut_or_tdc.outgoing(stt_id, label)

## ========================================================================= ##
def outin(Automaton_or_Transducer aut_or_tdc, int stt_id, label=None):
    """
    Usage:
        outgoing(aut, stt_id [,label])
        outgoing(tdc, stt_id [,labels])

    Description:  returns the identifiers of all outgoing transitions of \
<stt_id> in the automaton <aut> (resp. in the transducer <tdc>); \
if <label> (resp. <labels>) is provided, returns only the identifiers of the \
transitions labelled by <label> (resp. <labels>).

    Args:
        aut (Automaton)
        tdc (Automaton)
        stt_id (int), identifier of a state
        label (str, optional)
        labels (list of str, optional)

    Returns:  list of int, identifiers of transitions
    """
    _deprecated("outin",since="2.0")
    return aut_or_tdc.outgoing(stt_id, label)


## ========================================================================= ##
def predecessors(Automaton_or_Transducer aut_or_tdc, int stt_id, label=None):
    """
    Usage:
        predecessors(aut, stt_id [, label])
        predecessors(tdc, stt_id [, labels])

    Description:  returns the identifiers of all the states that are \
predecessors of <stt_id> in the automaton <aut> (resp. in the transducer <tdc>); \
if <label> (resp. <labels>) is provided, returns only the identifiers of the \
states s such that there exists a transition going  from s to <stt_id> and \
labelled by <label> (resp. <labels>).

    Args:
        aut (Automaton)
        tdc (Automaton)
        stt_id (int), identifier of a state
        label (str, optional)
        labels (list of str, optional)

    Returns:  list of int, identifiers of states
    """
    _deprecated("predecessors",since="2.0")
    return aut_or_tdc.predecessors(stt_id, label)


## ========================================================================= ##
def successors(Automaton_or_Transducer aut_or_tdc, int stt_id, label=None):
    """
    Usage:
        successors(aut, stt_id [, label])
        successors(tdc, stt_id [, labels])

    Description:  returns the identifiers of all the states that are \
successors of <stt_id> in the automaton <aut> (resp. in the transducer <tdc>); \
if <label> (resp. <labels>) is provided, returns only the identifiers of the \
states s such that there exists a transition going from <stt_id> to s and \
labelled by <label> (resp. <labels>).

    Args:
        aut (Automaton)
        tdc (Automaton)
        stt_id (int), identifier of a state
        label (str, optional)
        labels (list of str, optional)

    Returns:  list of int, identifiers of states
    """
    _deprecated("successors",since="2.0")
    return aut_or_tdc.successors(stt_id, label)


### ========================================================================= ##
#def copy(Automaton_or_Transducer aut_or_tdc):
    #"""
    #Usage:
        #[a] copy(aut)
        #[b] copy(tdc)

    #Description:  returns a copy of <aut> or <tdc>

    #Returns:
        #[a] Automaton
        #[b] Transducer
    #"""
    #return aut_or_tdc.copy()


## ========================================================================= ##
def has_letter(Automaton_or_Transducer aut_or_tdc, str letter, tape_num = None):
    """
    Usage:
        [a] has_letter(aut, letter)
        [b] has_letter(tdc, letter, tape_num)

    Description:
        [a] tests whether <letter> belongs to the alphabet of <aut>
        [b] tests whether <letter> belongs to the alphabet of the <tape_num>-th tape of <tdc>

    Args:
        aut (Automaton)
        tdc (Transducer)
        letter (str)
        tape_num (int), the number of a tape of <tdc>

    Returns:
        bool
    """
    _deprecated("has_letter",since="2.0")
    if tape_num is None:
        return Automaton.has_letter(aut_or_tdc, letter)
    else:
        return Transducer.has_letter(aut_or_tdc, letter, tape_num)



