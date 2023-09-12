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

cdef RatExp _RatExp(simple_ratexp_t ratexp):
    r= RatExp("","hack!!")
    r._this= ratexp
    return r

#cdef RatExp _eventually_build_RatExp(expression, weightset=None):
  #if type(expression) is RatExp:
    #return expression
  #else:
    #return RatExp(expression, weightset)

  ##

cdef _RatExpKind_is_closed = False
cdef class RatExpKind:
    cdef int _enum_value
    cdef str _enum_name
    of = {}
    instances = []
    def __init__(self,n,i):
        if (_RatExpKind_is_closed):
            raise RuntimeError("RatExpKind is not instantiable.  Please use RatExpKind.instances or RatExpKind.of to get a value.");
        else:
            self._enum_value = i
            self._enum_name = n
            RatExpKind.of[i]=self
            RatExpKind.of[n]=self
            RatExpKind.instances.append(self);
    def __str__(self):
        return self._enum_name
    def __int__(self):
        return self._enum_value
    def __repr__(self):
        return str(self._enum_name)+"~"+str(self._enum_value)
    def value(self):
        return self._enum_value
    def name(self):
        return self._enum_name
    ZERO = RatExpKind("ZERO",KIND_ZERO)
    ONE = RatExpKind("ONE",KIND_ONE)
    ATOM = RatExpKind("ATOM",KIND_ATOM)
    SUM = RatExpKind("SUM",KIND_SUM)
    PROD = RatExpKind("PROD",KIND_PROD)
    STAR = RatExpKind("STAR",KIND_STAR)

_RatExpKind_is_closed = True

cdef class RatExp:
    ZERO = RatExpKind.ZERO
    ONE = RatExpKind.ONE
    ATOM = RatExpKind.ATOM
    SUM = RatExpKind.SUM
    PROD = RatExpKind.PROD
    STAR = RatExpKind.STAR

## ========================================================================= ##
    cdef simple_ratexp_t _this



## ========================================================================= ##
    cdef void _set_cpp_class(self, simple_ratexp_t other):
        """
        Modify wrapped object. Internal use only.
        This seems to be quite fragile. Modify the code with caution.
        """
        self._this.rewrap_into_me_(other)

## ========================================================================= ##
    cdef simple_ratexp_t _to_cpp_class(self):
        return (<simple_ratexp_t>self._this)

## ========================================================================= ##
    def __init__(self, expression, weightset = None, str alphabet = None):
        if type(expression) is RatExp:
          self._this= (<RatExp>expression)._this.copy();
        elif type(expression) is not str:
          raise TypeError ("Argument 'expression' has incorrect type (expected awalipy.RatExp or str, got "+type(expression)+")")
        else:
          n = len(expression)
          if (n > 1) and (expression[0] == '<') \
                                   and (expression[n-1] == '>') :
            expression+= '\\e'
          if weightset is None:
              if alphabet is None:
                  self._this = make_simple_ratexp1(expression)
              else:
                  self._this = make_simple_ratexp2bis(expression, alphabet)
          else:
              if weightset == "hack!!":
                  pass
              else:
                if isinstance(weightset, WeightSet):
                    weightset = str(weightset);
                if alphabet is None:
                    self._this = make_simple_ratexp2(expression, weightset)
                else:
                    self._this = make_simple_ratexp3(expression, weightset, alphabet)

## ========================================================================= ##
    def copy(self):
      return _RatExp(self._this.copy())

## ========================================================================= ##
    def alphabet(self):
      return self._this.alphabet()

## ========================================================================= ##
    def __add__(self, RatExp other):
        if isinstance(self, RatExp):
            return self.add(other)
        else:
            self.__add__(other);

    def __invert__(self):
        return self.star()

    def __xor__(self, other):
        if type(self) is RatExp:
          return self.mult(other)
        else:
          return RatExp(self, other.get_weightset()).mult(other)


    def __str__(self):
        return self._this.print_()

    def __repr__(self):
        return self._this.print_()


## ========================================================================= ##
    def add(self, re):
        """
        Usage. re.add(other_re)

        Description:  returns the sum (ie union) of <re/self> and <other_re>

        Args:  other_re (RatExp)

        Returns: RatExp
        """
        return _RatExp(self._this.add(RatExp(re,self.get_weightset())._this))


## ========================================================================= ##
    def add_here(self, RatExp re):
        """
        Usage:  re.add_here(other_re)

        Description:  computes the sum  (ie union) of <re/self> and <other_re> and reassign the result to <re/self>

        Args:  other_re (RatExp)

        Note:  convenience function written at the python layer
        """
        self._set_cpp_class(self._this.add(re._this))


## ========================================================================= ##
    def mult(self, re):
        """
        Usage:  re.mult(other_re)

        Description:  returns the multiplication (or concatenation) of <re/self> and <other_re>

        Args:  other_re (RatExp)

        Returns: RatExp
        """
        return _RatExp(self._this.mult(RatExp(re,self.get_weightset())._this))



## ========================================================================= ##
    def mult_here(self, RatExp re):
        """
        Usage:  re.mult(other_re)

        Description:  computes the multiplication (or concatenation) of <re/self> and <other_re> and reassign the result to <re/self>

        Args:  other_re (RatExp)

        Note:  convenience function written at the python layer
        """
        self._set_cpp_class(self._this.mult(re._this))


## ========================================================================= ##
    def star(self):
        """
        Usage:  re.star()

        Description:  returns the star of <re/self>

        Returns:  RatExp
        """
        return _RatExp(self._this.star())


## ========================================================================= ##
    def star_here(self):
        """
        Usage:  re.star_here()

        Description:  computes the star of <re/self> and reassign the result to <re/self>

        Note:  convenience function written at the python layer
        """
        return self._set_cpp_class(self._this.star())


## ========================================================================= ##
    def derivation(self, str word, bool breaking=False):
        return _from_map_ratexp_s(derivation_(self._this, word, breaking))



## ========================================================================= ##
    def exp_to_aut(self, str method=None):
        """
        Usage:  re.exp_to_aut( [method="glushkov"] )

        Description:  computes an automaton accepting <re/self>.

        Args:
            method (str, optional), algorithm to use 
                admissible values are "glushkov", "derived_term", "breaking_derived_term", "thompson", "weighted_thompson"
                defaults to "glushkov"

        Returns:  Automaton or Transducer
        """
        if method is None:
            return _BasicAutomaton_(exp_to_aut_(self._this))
        else:
            return _BasicAutomaton_(exp_to_aut_(self._this,method))




## ========================================================================= ##
    def star_normal_form(self):
        """
        Usage:  re.star_normal_form()

        Description:  builds a new equivalent expression in star normal form

        Precondition:  weighset of <re/self> must be B

        Returns:  RatExp
        """
        return _RatExp(star_normal_form_(self._this))


## ========================================================================= ##
    def star_height(self):
        """
        Usage:  re.star_height()

        Description:  computes the star-height of the expression

        Returns:  int
        """
        return star_height_(self._this)


## ========================================================================= ##
    def get_weightset(self):
        """
        Usage:  re.get_weightset()

        Description:  returns the weightset of <re/self>

        Returns:  WeightSet
        """
        return WeightSet(self._this.get_weightset())


## ========================================================================= ##
    def display(self):
        """
        Usage:  re.display
        """
        a = _BasicAutomaton_(display_rat_(self._this))
        a.display(history=True)


## ========================================================================= ##
    def is_valid(self):
        """
        Usage:  ratexp.is_valid()

        Description:  Tests whether epsilon is properly weighted in every sub-expression.

        Returns:  bool
        """
        return self._this.is_valid()


## ========================================================================= ##
    cpdef str constant_term(self):
        """
        Usage:  ratexp.constant_term()

        Description:  Returns the weight of epsilon.

        Returns: str, representing a weight
        """
        return self._this.constant_term()

## ========================================================================= ##
    def expand(self):
        """
        Usage:  ratexp.expand()

        Description:  Distributes union and concatenation as much as possible.

        Returns: RatExp
        """
        return _RatExp(expand_(self._this))

## ========================================================================= ##
    def get_kind(self):
        """
        Usage:  ratexp.get_kind()

        Description:  Returns the kind of this RatExp
        
        Returns: a RatExpKind 

        See also RatExp.ZERO, RatExp.ONE, RatExp.ATOM, RatExp.SUM, RatExp.PROD, RatExp.STAR
        """
        return RatExpKind.of[self._this.get_kind()]



## ========================================================================= ##
    def content(self):
        """
        Usage: ratexp.content()
        
        Description: Computes list representing this RatExp as a node.
        - In all case, the first element is the kind of this RatExp.
        - In all case, the second and last elements are of type str and respectively represent the left and right weight of this RatExp.
        - If this RatExp is an RatExp.ATOM, then returned list is of length 3, and the middle element is of type str and represents the label.
        - If this RatExp is not a RatExp.ATOM, then the returns list if of lengtrh n+2, and the n middle elements are of type RatExp.

        Returns: a list of lenth n+2 where n is the number of subexpressions.

        See also: RatExp.getKind(), RatExp.ZERO, RatExp.ONE, RatExp.ATOM, RatExp.SUM, RatExp.PROD, RatExp.STAR

        """
        res = [self.get_kind()]
        res.append(self._this.lweight())
        for x in self._this.children():
            res.append(_RatExp(x))
        if (self.get_kind() == RatExp.ATOM):
            res.append(self._this.value())
        res.append(self._this.rweight())
        return res

## ========================================================================= ##
    def children(self):
        """
        Usage ratexp.content()

        Description returns a tuple subexpressions of this RatExp.

        Returns: a tuple of RatExp, or a string if <ratexp> is a RatExp.ATOM.
        """
        if (self.get_kind() == RatExp.ATOM):
            return [self._this.value()]
        else:      
            res = []
            for x in self._this.children():
                res.append(_RatExp(x))
            return res

## ========================================================================= ##
    def weight(self):
        """
        Usage ratexp.weight()

        Description: returns the left and right weight of this RatExp.

        Returns: a pair of string.
        """
        return [self._this.lweight(), self._this.rweight()]
