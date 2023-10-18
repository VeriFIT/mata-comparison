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
## ================ Equivalent method in ratexp_2_class.pyx ================ ##
## ========================================================================= ##

def add(RatExp ratexp1, RatExp ratexp2):
    """
    Usage: add(ratexp1, ratexp2)

    Description:  returns the sum (or union) of <ratexp1> and <ratexp2>

    Args:
        ratexp1 (RatExp)
        ratexp2 (RatExp)

    Returns: RatExp
    """
    _deprecated("add",since="2.0")
    return ratexp1.add(ratexp2)


## ========================================================================= ##
def add_here(RatExp ratexp1, RatExp ratexp2):
    """
    Usage:  add_here(ratexp1, ratexp2)

    Description:  computes the sum  (or union) of <ratexp1> and <ratexp2> and reassign the result to <ratexp1>

    Args:
        ratexp1 (RatExp)
        ratexp2 (RatExp)

    Note:  convenience function written at the python layer
    """
    _deprecated("add_here",since="2.0")
    ratexp1.add_here(ratexp2)


## ========================================================================= ##
def mult(RatExp ratexp1, RatExp ratexp2):
    """
    Usage:  mult(ratexp1, ratexp2)

    Description:  returns the multiplication (or concatenation) of <ratexp1> and <ratexp2>

    Args:
        ratexp1 (RatExp)
        ratexp2 (RatExp)

    Returns: RatExp
    """
    _deprecated("mult",since="2.0")
    return ratexp1.mult(ratexp2)


## ========================================================================= ##
def mult_here(RatExp ratexp1, RatExp ratexp2):
    """
    Usage:  mult(ratexp1, ratexp2)

    Description:  computes the multiplication (or concatenation) of <ratexp1> and <ratexp2> and reassign the result to <ratexp1>

    Args:
        ratexp1 (RatExp)
        ratexp2 (RatExp)

    Note:  convenience function written at the python layer
    """
    _deprecated("mult_here",since="2.0")
    ratexp1.mult_here(ratexp2)


## ========================================================================= ##
def star(RatExp ratexp):
    """
    Usage:  star(ratexp)

    Description:  returns the star of <ratexp>

    Args:  ratexp (RatExp)

    Returns:  RatExp
    """
    _deprecated("star",since="2.0")
    return ratexp.star()


## ========================================================================= ##
def star_here(RatExp ratexp):
    """
    Usage:  star_here(ratexp)

    Description:  computes the star of <ratexp> and reassign the result to <ratexp>

    Args:  ratexp (RatExp)

    Note:  convenience function written at the python layer
    """
    _deprecated("star_here",since="2.0")
    return ratexp.star_here()


## ========================================================================= ##
def derivation(RatExp ratexp, str word, bool breaking=False):
    return ratexp.derivation_(word, breaking)


## ========================================================================= ##
def derived_term(RatExp ratexp, bool breaking=False, bool keep_history=True):
    """
    Usage:  derived_term(ratexp, [,breaking=False [, keep_history=True] ] )

    Description:  computes an automaton accepting <ratexp> using the derived term algorithm

    Args:
        ratexp (RatExp)
        breaking (bool, optional)
            defaults to False
        keep_history
            defaults to False

    Returns:  Automaton
    """
    _deprecated("derived_term",since="2.0")
    return ratexp.derived_term(breaking, keep_history)


## ========================================================================= ##
def exp_to_aut(RatExp ratexp):
    """
    Usage:  exp_to_aut(ratexp)

    Description:  computes an automaton accepting <ratexp> (using the derived term algorithm)

    Args:  ratexp (RatExp)

    Returns:  Automaton
    """
    _deprecated("exp_to_aut",since="2.0")
    return ratexp.exp_to_aut()


## ========================================================================= ##
def thompson(RatExp ratexp):
    """
    Usage:  thompson(ratexp)

    Description:  computes the Thompson automaton of <ratexp>

    Args:  ratexp (RatExp)

    Returns:  Automaton
    """
    _deprecated("thompson",since="2.0")
    return ratexp.thompson()


## ========================================================================= ##
def standard(RatExp ratexp):
    """
    Usage:  standard(ratexp)

    Description:  builds a standard automaton accepting <ratexp>

    Args:  ratexp (RatExp)

    Returns:  Automaton
    """
    _deprecated("standard",since="2.0")
    return ratexp.standard()


## ========================================================================= ##
def expand(RatExp ratexp):
    """
    Usage:  expand(ratexp)

    Description: builds a new expression by distributing the products over the sums of the input

    Args:  ratexp (RatExp)

    Returns:  RatExp
    """
    _deprecated("expand",since="2.0")
    return ratexp.expand()


## ========================================================================= ##
def star_normal_form(RatExp ratexp):
    """
    Usage:  star_normal_form(ratexp)

    Description:  builds a new equivalent expression in star normal form

    Args:  ratexp (RatExp)

    Precondition:  weighset of <ratexp> must be B

    Returns:  RatExp
    """
    _deprecated("star_normal_form",since="2.0")
    return ratexp.star_normal_form()


## ========================================================================= ##
def star_height(RatExp ratexp):
    """
    Usage:  star_height(ratexp)

    Description:  computes the star-height of the expression

    Args:  ratexp (RatExp)

    Returns:  int
    """
    _deprecated("star_height",since="2.0")
    return ratexp.star_height()


## ========================================================================= ##
def get_weightset(RatExp ratexp):
    """
    Usage:  get_weightset(ratexp)

    Description:  returns the weightset of <ratexp>

    Args:  ratexp (RatExp)

    Returns:  str
    """
    _deprecated("get_weightset",since="2.0")
    return ratexp.get_weightset()


## ========================================================================= ##
def constant_term(RatExp ratexp):
    """
    Usage: constant_term(ratexp)

    Description:  Returns the weight of epsilon in <ratexp>.

    args: ratexp (RatExp)

    Returns: str, representing a weight
    """
    _deprecated("constant_term",since="2.0")
    return ratexp.constant_term()
