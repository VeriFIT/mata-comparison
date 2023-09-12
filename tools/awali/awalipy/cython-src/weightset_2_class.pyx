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

cdef WeightSet _WeightSet(weightset_t ws):
  """Builds a WeightSet (python class) wrapping a given weightset_t (C++ class).

  Args:
    ws (dyn::weight_set_t) the weightset to wrap.

  Notes:
    Internal function, use at your own risk.
  """
  w = WeightSet("Hack!!")
  w._this = ws
  return w


cdef class WeightSet:
  """
  WeightSet for weighting transitionss of an Automaton or a Transducer.
  Wraps a `weightset_t` (C++ class).
  """

  cdef weightset_t _this

## ========================================================================= ##
  def __repr__(self):
    return self.__str__()

## ========================================================================= ##
  def __str__(self):
    return self._this.name()

## ========================================================================= ##
  def __richcmp__(self,other,op):
    if op == 2:#Py_EQ:
      return str(self) == str(other)
    if op == 3:
      return str(self) != str(other)
#    """
#    Usage:  ws1 == ws2

#    Description: Tests equality of <ws1> and <ws2>. Returns true if both are instances of WeightSet and have the same string representation.

#    Returns: bool
#    """
#    if isinstance(other, WeightSet):
#      return str(self) == str(other);
#    else:
#      return False

### ========================================================================= ##
#  def __ne__(self,other):
#    return not (self == other)

## ========================================================================= ##
  def __init__(self, ws):
    """
    Usage: Weighset(ws)

    Description:  builds a WeightSet from a string representation of it.

    Argument:  ws (str), valid values include \"B\", \"Z\", \"Z-min-plus\", \"Z/<n>Z\", \"Q\"

    See:  function `awalipy.available_weighsets()` for a complete list of available weightsets.
    """
    if (ws == "Hack!!"):
      pass
    else:
      self._this = make_simple_weightset(ws)

## ========================================================================= ##
  def zero(self):
    """
    Usage: ws.zero()

    Description: return the neutral element of addition.

    Returns: str
    """
    return self._this.zero();
## ========================================================================= ##
  def one(self):
    """
    Usage: ws.one()

    Description: return the neutral element of multiplication.

    Returns: str
    """
    return self._this.one();
## ========================================================================= ##
  def mul(self, string l, string r):
    """
    Usage: ws.mul(weight1, weight2)

    Description:  computes the result of mutiplication <weight1>*<weight2>, according to this semiring of weights.

    Arguments:
      weight1 (str), left weight to multiply
      weight2 (str), right weight to multiply
    Returns:  str

    Note:  Depending on represented weightset, multiplication may be non-commutative.
    """
    return self._this.mul_weights(l,r)

## ========================================================================= ##
  def add(self, string l, string r):
    """
    Usage: ws.add(weight1, weight2)

    Description:  computes the result of addition <weight1>+<weight2>, according to this semiring of weights.

    Arguments:
      weight1 (str), left weight to add
      weight2 (str), right weight to add
    Returns:  str
    """
    return self._this.add_weights(l,r)

## ========================================================================= ##
  def div(self, string l, string r):
    """
    Usage: ws.div(weight1, weight2)

    Description:  computes, when it is possible, the result of division <weight1>/<weight2>, according to this semiring of weights.

    Arguments:
      weight1 (str), dividend
      weight2 (str), divisor

    Returns:  str

    Note:  Fails if asked division is not possible in this semiring
    """
    return self._this.div_weights(l,r)
