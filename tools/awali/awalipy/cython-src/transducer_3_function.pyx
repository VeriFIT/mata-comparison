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

def make_transducer(list alphabets, str semiring=None):
    """
    Usage: Automaton(alphabets [, semiring='B'])

    Description: builds an empty (weighted) automaton whose transitions are labelled in <alphabet> and weighted in <semiring>

    Parameters:
        - alphabets: list of str
        - semiring: str of {'B', 'Z', 'Z-min-plus', 'Z-max-plus', 'Q', 'R', 'R-max-prod', 'F2' }, default is 'B'
    """   
    _deprecated("get_weightset",since="2.0",replace="the constructor Transducer")
    return Transducer(alphabets, semiring)


## ========================================================================= ##
def num_tapes(Transducer tdc):
    """
    Usage:  num_tapes(tdc)

    Description: returns the number of tapes of <tdc>

    Args:  tdc (Transducer)

    Returns: int
    """
    _deprecated("num_tapes",since="2.0")
    return tdc.num_tapes()


## ========================================================================= ##
def alphabets(Transducer tdc):
    """
    Usage:  alphabets(tdc)

    Description:  returns the alphabets of the different tapes of <tdc>

    Args:  tdc (Transducer)

    Returns:  list of str
    """
    _deprecated("alphabets",since="2.0")
    return tdc.alphabets()


## ========================================================================= ##
def domain(Transducer tdc):
    """
    Usage:  domain(tdc)

    Description:  return the automaton accepting the domain of the function realized by <tdc>

    Args:  tdc (Transducer)

    Returns: Automaton
    """
    _deprecated("domain",since="2.0")
    return tdc.domain()


## ========================================================================= ##
def image(Transducer tdc):
    """
    Usage:  image(tdc)

    Description:  return the automaton accepting the image of the function realized by <tdc>

    Args:  tdc (Transducer)

    Precondition: <tdc> must be a two tape transducer.

    Returns: Automaton
    """
    _deprecated("image",since="2.0")
    return tdc.image()


## ========================================================================= ##
def images(Transducer tdc):
    """
    Usage:  domain(tdc)

    Description:  returns the automaton or the transducer accepting the images of the function realized by <tdc>

    Args:  tdc (Transducer)

    Returns: Automaton or Transducer
    """
    _deprecated("images",since="2.0")
    return tdc.images()


## ========================================================================= ##
def projection(Transducer tdc, int i):
    """
    Usage:  projection(tdc, tape_num)

    Description:  returns the automaton accepting the language of the <tape_num>-th tape of <tdc>

    Args:
        tdc (Transducer)
        tape_num (int)

    Returns: Automaton
    """
    _deprecated("projection",since="2.0")
    return tdc.projection(i)


## ========================================================================= ##
def inverse(Transducer tdc):
    """
    Usage:  inverse(tdc)

    Description:  returns a transducer that realizes the inverse relation of the one realized by <tdc>; effectively permuting the two tapes of <tdc>

    Args:  tdc (Transducer)

    Precondition: <tdc> must be a two-tape Transducer

    Returns: Transducer
    """
    _deprecated("inverse",since="2.0")
    return tdc.inverse()


## ========================================================================= ##
def compose(Transducer tdc1, Transducer tdc2):
    """
    Usage:  compose(tdc1, tdc2)

    Description:  returns the transducer realizing the composition of the relations realized by <tdc1> and <tdc2>

    Args:
        tdc1 (Transducer)
        tdc2 (Transducer)

    Precondition:  the alphabet of first tape of <tdc2> must be equal to the one of the last tape of <tdc1>

    Returns Automaton
    """
    _deprecated("compose",since="2.0")
    return tdc1.compose(tdc2)


## ========================================================================= ##
def is_functional(Transducer tdc):
    """
    Usage:  is_functional(tdc)

    Description:  returns True if the relation realized by <tdc> is a function, that is if every word from its domain has one image.

    Args:  tdc (Transducer)

    Returns: bool
    """
    _deprecated("is_functional",since="2.0")
    return tdc.is_functional()


## ========================================================================= ##
def synchronize(Transducer tdc):  
    _deprecated("is_functional",since="2.0")
    return tdc.synchronize()


## ========================================================================= ##
def is_synchronizable(Transducer tdc):     
    _deprecated("is_functional",since="2.0")
    return tdc.is_synchronizable()


## ========================================================================= ##



#def get_transition(Transducer tdc, int src, int dst, vector[str] labels):
    #return tdc.get_transition(src, dst, labels)

#cpdef bool has_transition_tdc(Transducer tdc, int src, int dst, vector[string] labels): #3
    #return tdc.has_transition(src, dst, labels)


        #void del_transition3(int src, int dst, vector[string] label)
        #vector[string] label_of(transition_t t)
        #void set_transition4(int src, int dst, vector[string] label, string weight)
        #void set_transition3(int src, int dst, vector[string] label)
        #string add_transition4(int src, int dst, vector[string] label, string weight)
        #string add_transition3(int src, int dst, vector[string] label)
        #vector[int] successors2(int src, vector[string] label)
        #vector[int] predecessors2(int src, vector[string] label)
        #vector[transition_t] incoming2(int s, vector[string] label)
        #vector[transition_t] outgoing2(int s, vector[string] label)
        #vector[transition_t] in2(int s, vector[string] label)
        #vector[transition_t] out2(int s, vector[string] label)
        #vector[string]
        #has_letter(string l, unsigned tape_num)





