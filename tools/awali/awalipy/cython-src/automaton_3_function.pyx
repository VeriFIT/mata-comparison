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

## ========================================================================= ##
def make_random_DFA(unsigned int size, str alph):
    """
    Usage:  make_random_DFA(size, alph)

    Description:  builds a ``random'' automaton on the alphabet <alph> with <size> states. Moreover, the generated automaton is complete, deterministic and trim.

    Args:
        size (int), number of states of the returned automaton.
        alph (str), alphabet of the returned automaton.

    Returns:  Automaton
    """
    return _Automaton(random_DFA_(size, alph))


## ========================================================================= ##
def make_n_ultimate(unsigned int n, str alph='ab'):
    """
    Usage:  make_n_ultimate(n [, alph='ab'])

    Description

    Args:
        n (int) must be positive
        alph (str, optional)
            must be of length at least 2
	    default to 'ab'

    Returns:  Automaton
    """
    return _Automaton(make_n_ultimate_(n, alph))


## ========================================================================= ##
def make_divkbaseb(unsigned int k, unsigned int b, str alph='auto'):
    """
    Usage:  make_divkbaseb(k, b [, alph='auto'])

    Description:  builds the automaton accepting the integers, written in \
integer base <b>, which are multiple of <k>; if alphabet is set to 'auto'
 the alphabet is defined as the alphabet of digits; otherwise, <alph>
 must contain at least b letters (and only the first b will be used).

    Args:
        k (int), the period
        b (int), the base
        alph (str, optional)
            must be of length at least b, or 'auto'
            defaults to 'auto'

    Returns:  Automaton
    """
    return _Automaton(make_divkbaseb_(k, b, alph))


## ========================================================================= ##
def make_double_ring(unsigned int n, list finals, str alph='ab'):
    """
    Usage: make_double_ring(n, [finals=[0] [, alph='ab' ] ] )

    Description:  generates the automaton over the weigh-set 'B' defined as follows:
        it has <n> states {0, 1, ..., (n-1)};
        0 is the unique initial state;
        it features the circuit 0 -> 1 -> ... -> (n-1) -> 0 labelled by the first letter of <alph>;
        it features the circuit (n-1) -> (n-2) -> ... -> 0 -> (n-1) \
labelled by the second letter of <alph>;
        every state of <finals> is a final state..

    Args:
        n (int),
            must be positive
        finals (list of int, optional), the states that will be final
            every integer k in the list must satisfies 0<= k < n-1
            defaults to [0]
        alph (str, optional)
            must be of length 2
            defaults to 'ab'

    Returns:  Automaton
    """
    return _Automaton(make_double_ring_(n, _list_to_vector_of_unsigned(finals), alphabet))


## ========================================================================= ##
def make_ladybird(unsigned int n, str alph='abc'):
    """
    Usage:  make_ladybird(n [, alph='abc'] )

    Description:  generates the `ladybird' automaton with <n>-states; \
it features a three-letter alphabet.

    Args:
	n (int) must be positive
        alphabet (str, optional)
            must be of length 3
            default to 'abc'

    Note:  the `ladybird' automata form a family of non-deterministic \
automata that reach the upper bound for the determinization process: \
the minimal deterministic automaton equivalent to ladybird(n) has 2**n states.
    """
    return _Automaton(make_ladybird_(n, alph))




## ========================================================================= ##
def make_witness(int n, alph='abc'):
    return _Automaton(make_witness_(n, alph))

## ========================================================================= ##
def make_cerny(int n):
    return _Automaton(make_cerny_(n))


## ========================================================================= ##
## =============== Method Function in automaton_2_class.pyx ================ ##
## ========================================================================= ##
def make_automaton(str alphabet, str weightset=None):
    """Usage:  make_automaton(alphabet [, weightset='B'])

    Description:  builds an empty (weighted) automaton whose transitions are labelled in <alphabet> and weighted in <weightset>.

    Args:
        alphabet (str), letters that may label the transitions of the automaton.
        weightset (str, optional), represents the set of transition weights.
            Admissible values are 'B', 'Z', 'Z-min-plus', 'Z-max-plus', 'Q', 'R', 'R-max-prod' and 'F2'.
            Case-insensitive.
            Defaults to 'B'.

    Returns:  Automaton

    Note:  Other values for <weightset> may be admissible if properly added to the code of the awali C++ library.
    """
    _deprecated("make_automaton",since="2.0",replace="constructor for Automaton")
    return Automaton(alphabet, weightset)


## ========================================================================= ##
def alphabet(Automaton aut):
    """Usage:  alphabet(aut)

    Description:  returns the alphabet of <aut> as the string.

    Args:  aut (Automaton)

    Returns:  str
    """
    _deprecated("alphabet",since="2.0")
    return aut.alphabet()


## ========================================================================= ##
def support(Automaton aut):
    """
    Usage:  support(aut)

    Description: Returns the support of <aut>, that is the automaton on the boolean weight-set
    resulting from replacing any non-zero weight of <aut> by True.

    Args:
        aut (Automaton), automaton whose support is returned.

    Returns:
        Automaton: an automaton over the 'B' weight-set.
    """
    _deprecated("support",since="2.0")
    return aut.support()


## ========================================================================= ##
def characteristic(Automaton aut, str weightset):
    """
    Usage: characteristic(aut, weightset)

    Description: returns the characteristic of <aut>, that is the automaton on the weight-set <weightset> resulting from setting the weight of every transition of <aut> to one (ie. the multiplicative neutral element of <weightset>).

    Args:
        aut (Automaton), automaton whose characteristic automaton is return.
        weightset (str), weight-set of the returned automaton
            admissible values are 'B', 'Z', 'Z-min-plus', 'Z-max-plus', 'Q', 'R', 'R-max-prod', 'F2'


    Returns:
        Automaton: the characteristic of <aut> over the weight-set <weightset>.
    """
    _deprecated("characteristic",since="2.0")
    return aut.characteristic(weightset)


## ========================================================================= ##
def aut_to_exp(Automaton aut):
    """
    Usage:  aut_to_exp(aut)

    Description:  computes the rational expression of the language accepted by <aut>.

    Args:
        aut (Automaton), the automaton whose rational expression is returned.

    Returns:
        RatExp, (weighted) expression of the language accepted by <aut>.
    """
    _deprecated("aut_to_exp",since="2.0")
    return aut.aut_to_exp_()



## ========================================================================= ##
def partial_identity(Automaton aut):
    """
    Usage:  partial_identity(aut)

    Description: return the transducer realising the partial identity of (the language accepted by) <aut>.

    Args:
        aut (Automaton), automaton whose partial identity is computed.

    Returns:
        Transducer, transducer realising the partial identity of <aut>.
    """
    _deprecated("partial_identity",since="2.0")
    return aut.partial_identity()


## ========================================================================= ##
def allow_eps_transition(Automaton aut):
    """
    Usage:  allow_eps_transition(aut)

    Description: returns a copy of aut in which epsilon transition are allowed.

    Args:
        aut (Automaton), automaton whose

    Returns:
        Automaton: copy of <aut> where epsilon transition are allowed.
    """
    _deprecated("allow_eps_transition",since="2.0")
    return aut.allow_eps_transition()


## ========================================================================= ##
def allow_eps_transition_here(Automaton aut):
    """
    Usage:  allow_eps_transition_here(aut)

    Description: allows epsilon transition in <aut>.

    Args:
        aut (Automaton), automaton whose

    Note:
        This function uses allow_eps_transition, hence requires a complete traversal of aut.
    """
    _deprecated("allow_eps_transition_here",since="2.0")
    aut.allow_eps_transition_here()


## ========================================================================= ##
def are_equivalent(Automaton aut1, Automaton aut2):
    """
    Usage:  are_equivalent(aut1, aut2)

    Description:  determines if <aut1> and <aut2> are equivalent.

    Args:
        aut1 (Automaton)
        aut2 (Automaton)

    Preconditions:
        The weigh-set of <aut1> and <aut2> must be a field or Z.

    Returns:
        bool, True if <aut1> and <aut2> accept the same language.
    """
    return aut1.are_equivalent(aut2)


## ========================================================================= ##
def determinize(Automaton aut, bool history=True):
    """
    Usage:  determinize(aut [, history=True ])

    Description: returns a deterministic automaton equivalent to <aut>. If <history> is True, then the history of the automaton is preserved.

    Args:
        aut (Automaton), automaton to determinize
            preconditions: weight-set is the boolean semi-ring.
        history (bool), whether history of the automaton should be preserved.

    Returns:
        Automaton, a deterministic automaton equivalent to <aut>.

    Notes:
        An automaton is deterministic if each state features at most one outgoing transition labelled by a given letter.
    """
    _deprecated("determinize",since="2.0")
    return aut.determinize(history)


## ========================================================================= ##
def determinize_here(Automaton aut, bool history=True):
    """
    Usage:  determinize_here(aut [, history=True ])

    Description:  determinizes <aut>. If <history> is True, then the history of the automaton is preserved.

    Args:
        aut (Automaton), automaton to determinize.
            Preconditions: weight-set must be the boolean semi-ring.
        history (bool), whether history of the automaton should be preserved.

    Notes:
        An automaton is deterministic if each state features at most one outgoing transition labelled by a given letter.
    """
    _deprecated("determinize_here",since="2.0")
    aut.determinize_here(history)


## ========================================================================= ##
def is_deterministic(Automaton aut):
    """
    Usage:  is_deterministic(aut)

    Description: check whether <aut> is deterministic.

    Args:
        aut (Automaton), automaton whose determinism will be checked.

    Returns:
        bool, True if <aut> is deterministic.

    Notes:
        An automaton is deterministic if each state features at most one outgoing transition labelled by a given letter.
    """
    _deprecated("is deterministic",since="2.0")
    return aut.is_deterministic()


## ========================================================================= ##
def complement(Automaton aut):
    """
    Usage: complement(aut)

    Description: compute the complement of a deterministic complete automaton.

    Args:
        aut (Automaton), automaton whose complement is computed.
            Preconditions: deterministic, complete.

    Returns:
        Automaton, accepts the complement language of <aut>.
    """
    _deprecated("complement",since="2.0")
    return aut.complement()


## ========================================================================= ##
def complement_here(Automaton aut):
    """
    Usage:  complement_here(aut)

    Description:  modify <aut> such that it accepts the complement of its previous language.

    Args:
        aut (Automaton), automaton whose complement is computed.
            Preconditions: deterministic, complete.
    """
    _deprecated("complement_here",since="2.0")
    aut.complement_here()


## ========================================================================= ##
def complete(Automaton aut):
    """
    Usage:  complete(aut)

    Description:  returns a copy of <aut> which is complete, that is such that every state features an outgoing transition labelled by every letter.

    Args:
        aut (Automaton), automaton whose completion is computed
            preconditions: boolean semi-ring

    Returns:
        Automaton, a complete automaton equivalent to <aut>.

    Note:
        An automaton is complete if each state features an outgoing transition labelled by each letter.
    """
    _deprecated("complete",since="2.0")
    return aut.complete()


## ========================================================================= ##
def complete_here(Automaton aut):
    """
    Usage:  complete_here(aut)

    Description:  modify <aut> such that it becomes complete.

    Args:
        aut (Automaton), automaton whose completion is computed. Preconditions: Boolean semi-ring.

    Note:
        An automaton is complete if each state features an outgoing transition labelled by each letter.
    """
    _deprecated("complete_here",since="2.0")
    aut.complete_here()


## ========================================================================= ##
def is_complete(Automaton aut):
    """
    Usage:  is_complete(aut)

    Description:  modify <aut> such that it becomes complete.

    Args:
        aut (Automaton), automaton whose completion is computed.

    Note:
        An automaton is complete if each state features an outgoing transition labelled by each letter.
    """
    _deprecated("is_complete",since="2.0")
    return aut.is_complete()


## ========================================================================= ##
def reduce(Automaton aut):
    """
    Usage:  reduce(aut)

    Description:  compute the reduced automaton of <aut>.

    Args:
        aut (Automaton), automaton whose reduced automaton is computed.

    Preconditions:  weight-set is Z or a field.

    Returns:  Automaton, a reduced automaton equivalent to <aut>.

    Notes:
        The computation of the reduced automaton can be seen as the generalisation (to automata whose weight-set is a field) of the minimization process due to J. A. Brzozowski: reversal, determinization, reversal, determinization.
    """
    _deprecated("reduce",since="2.0")
    return aut.reduce()


## ========================================================================= ##
def reduce_here(Automaton aut):
    """
    Usage:  reduce_here(aut)

    Description:  modify <aut> such that it becomes reduced automaton.

    Args:
        aut (Automaton), automaton whose reduced automaton is computed.
            Preconditions: deterministic, weight-set is a field.

    Notes:
        The computation of the reduced automaton is the generalisation (to automata whose weight-set is a field) of the minimization process due to J. A. Brzozowski.
        Convenience function written at the python layer.
    """
    _deprecated("reduce_here",since="2.0")
    aut.reduce_here()


## ========================================================================= ##
def left_reduce(Automaton aut):
    """
        Usage:  left_reduce(aut)

        Description:  compute the 'left reduced' automaton of <aut>.

        Args:
           aut (Automaton), automaton whose 'left reduced' automaton is computed.

        Preconditions:  weight-set is Z or a field.

        Returns:  Automaton, a controllable automaton equivalent to <aut>.

    """
    _deprecated("left_reduce",since="2.0")
    return aut.left_reduce()


## ========================================================================= ##
def right_reduce(Automaton aut):
    """
    Usage:  right_reduce(aut)

    Description:  compute the 'right reduced' automaton of <aut>.

    Args:
    aut (Automaton), automaton whose 'right reduced' automaton is computed.

    Preconditions:  weight-set is Z or a field.

    Returns:  Automaton, an observable automaton equivalent to <aut>.

    """
    _deprecated("right_reduce",since="2.0")
    return aut.right_reduce()


## ========================================================================= ##
def is_ambiguous(Automaton aut):
    """
    Usage:  is_ambiguous(aut)

    Description:  determines if <aut> is ambiguous.

    Args:  aut (Automaton)

    Notes:
        An automaton is ambiguous if there exists two accepting runs for the same word.
    """
    _deprecated("is_ambiguous",since="2.0")
    return aut.is_ambiguous()


## ========================================================================= ##
def product(Automaton aut1, Automaton aut2):
    """
    Usage:  product(aut1, aut2)

    Description:  returns the Hadamard product of <aut1> with <aut2>.

    Args:
        aut1, aut2 (Automaton): automata whose product is computed.
            Preconditions: same weight-set.

    Returns:  Automaton:  the product of <aut1> with <aut2>.

    Notes:  The weight of a given word in the Hadamard product is the weight-set multiplication of the respective weights of this word in the two operand automata.
    """
    return aut1.product(aut2)


## ========================================================================= ##
def product_here(Automaton aut1, Automaton aut2):
    """
    Usage:  product_here(aut1, aut2)

    Description:  computes the Hadamard product of <aut1> with <aut2> and store the results in <aut1>.

    Args:
        aut1, aut2 (Automaton): automata whose product is computed.
            Preconditions: same weight-set.

    Notes:
        The weight of a given word in the Hadamard product is the weight-set multiplication of the respective weights of this word in the two operand automata.
        Convenience function written at the python layer.
    """
    _deprecated("is_ambiguous",since="2.0",replace=None)
    aut1.product_here(aut2)


## ========================================================================= ##
def power(Automaton aut, int n):
    """
    Usage:  power(aut, n)

    Description:  computes the n-th power of <aut>, that is the Hadamard product <aut> times <aut> times <aut> ...

    Args:
        aut (Automaton): automaton whose power is computed.
        n (int): exponent
            Preconditions: positive.

    Returns:  Automaton: the n-th power of <aut>.

    Notes:  The weight of a given word in the Hadamard product is the weight-set multiplication of the respective weights of this word in the two operand automata.
    """
    _deprecated("power",since="2.0")
    return aut.power(n)


## ========================================================================= ##
def power_here(Automaton aut, int n):
    """
    Usage:  power_here(aut, n)

    Description:  computes the n-th power of <aut>, that is the Hadamard product <aut> times <aut> times <aut> ..., and stores the result in <aut>.

    Args:
        aut (Automaton): automaton whose power is computed.
        n (int): exponent
            Preconditions: positive.

    Notes:
        The weight of a given word in the Hadamard product is the weight-set multiplication of the respective weights of this word in the two operand automata.
        Convenience function written at the python layer.
    """
    _deprecated("power_here",since="2.0")
    aut.power_here(n)


## ========================================================================= ##
def shuffle(Automaton aut1, Automaton aut2):
    """
    Usage:  shuffle(aut1, aut2)

    Description:  computes the automaton accepting the shuffle of the languages of <aut1> and <aut2>.

    Args:  aut1, aut2 (Automaton)

    Returns:  Automaton, the shuffle of <aut1> and <aut2>.

    Notes:  A word u belongs to the shuffle of two languages L, K if u may be factorised as u = v0.w0.v1.w1...vn.wn such that v0.v1...vn belongs to L and w0.w1...wn belongs to K.
    """
    return aut1.shuffle(aut2)

## ========================================================================= ##
def infiltration(Automaton aut1, Automaton aut2):
    """
    Usage:  infiltration(aut1, aut2)

    Description:  computes and returns the automaton accepting the infiltration of <aut1> and <aut2>.

    Args:  aut1, aut2 (Automaton)

    Returns:  Automaton

    Notes:  A word u belongs to the infiltration of two languages L,K if u may be factorised as u = u0.v0.w0.u1.v1.w1...un.vn.wn such that u0.v0.u1.v1...un.vn belongs to L and u0.w0.u1.w1...un.wn belongs to K.
    """
    return aut1.infiltration(aut2)

## ========================================================================= ##
def union(Automaton aut1, Automaton aut2):
    """
    Usage:  union(aut1, aut2)

    Description:  computes the parallel join of <aut1> and <aut2>

    Args:  aut1, aut2 (Automaton)

    Returns:  Automaton, the union of <aut1> and <aut2>
    """
    return aut1.union(aut2)


## ========================================================================= ##
def union_here(Automaton aut1, Automaton aut2):
    """
    Usage:  union_here(aut1, aut2)

    Description:  computes the parallel join of <aut1> and <aut2> and stores the result in <aut1>.

    Args:  aut1, aut2 (Automaton):

    Notes:  Convenience function written at the python layer.
    """
    return aut1.union_here(aut2)


## ========================================================================= ##
def are_isomorphic(Automaton aut1, Automaton aut2):
    """
    Usage:   are_isomorphic(aut1, aut2)

    Description:  tests whether <aut1> and <aut2> are isomorphic as labelled graph.

    Args:  aut1, aut2 (Automaton):

    Returns:  bool, True if <aut1> and <aut2> are isomorphic.
    """
    return aut1.are_isomorphic(aut2)


## ========================================================================= ##
def proper(Automaton aut, bool backward=True, bool prune=True):
    """
    Usage:  proper(aut [, backward=True [, prune=True ] ] )

    Description:  returns a copy of <aut> in which the epsilon transition have been removed.

    Args:
        aut (Automaton)
        backward (bool, optional)
        prune (bool, optional)

    Returns:  Automaton, automaton equivalent to <aut> without epsilon transitions and that does not allow future addition of epsilon-transitions.
    """
    _deprecated("proper",since="2.0")
    return aut.proper(backward, prune)


## ========================================================================= ##
def proper_here(Automaton aut, bool backward=True, bool prune=True):
    """
    Usage:  proper_here(aut [, backward=True [, prune=True ] ] )

    Description:  perform the epsilon-transition removal algorithm on <aut>.

    Args:
        aut (Automaton)
        backward (bool, optional)
            defaults to True
        prune (bool, optional)
            defaults to True
    """
    _deprecated("proper_here",since="2.0")
    aut.proper_here(backward, prune)


## ========================================================================= ##
def is_proper(Automaton aut):
    """
    Usage:  is_proper(aut)

    Description:  tests whether <aut> features epsilon transitions.

    Args:  aut (Automaton)

    Returns:  bool, True if <aut> does not have any epsilon transition
    """
    _deprecated("is_proper",since="2.0")
    return aut.is_proper()


## ========================================================================= ##
def is_valid(aut):
    """
    Usage:  is_valid(aut_or_exp)

    Description:  [a] tests whether <aut> has a epsilon-circuit labelled by a weight that is not starrable.  In the case of expressions, tests whether epsilon is properly weighted in every sub-expression.

    Args:  aut_or_exp(Automaton or RatExp)

    Returns:  bool, True if the automaton (resp. the expression) is valid.
    """
    _deprecated("is_valid",since="2.0")
    return aut.is_valid()

## ========================================================================= ##
def shortest(Automaton aut, int max_len):
    """
    Usage:  shortest(aut,n):

    Description:  looks for the shortest <n> words accepted by <aut>, and computes their accepted weight.  This method might return less than <n> mappings if <aut> accepts less than <n> words.

    Args:  n (int), number of words to look for.

    Preconditions:
        <aut> must be proper.
        <n> must be positive.

    Returns:  dict, mapping the shortest <n> words accepted by <aut> to the weights they are respectively accepted with.
    """
    _deprecated("shortest",since="2.0")
    return aut.shortest(max_len)


## ========================================================================= ##
def enumerate(Automaton aut, int max_len):
    """
    Usage:  enumerate(aut, max_len)

    Description:  computes the words shorter than <max_len> accepted by <aut> and their weight.

    Args:
        aut (Automaton)
        max_len (int), maximal length
            preconditions: non-negative

    Returns:  dict str->str, domain is all the words shorter than <max_len> and accepted by <aut>; image of a word is its weight.
    """
    _deprecated("enumerate",since="2.0")
    return aut.enumerate(max_len)


## ========================================================================= ##
def prefix(Automaton aut):
    """
    Usage:  prefix(aut)

    Description:  returns a copy of <aut> in which every co-accessible state is made final.

    Args:  aut (Automaton)

    Returns:  Automaton
    """
    _deprecated("prefix",since="2.0")
    return aut.prefix()


## ========================================================================= ##
def prefix_here(Automaton aut):
    """
    Usage:  prefix_here(aut)

    Description:  makes final every state of <aut> which is co-accessible.

    Args:  aut (Automaton)
    """
    _deprecated("prefix_here",since="2.0")
    aut.prefix_here()


## ========================================================================= ##
def suffix(Automaton aut):
    """
    Usage:  suffix(aut)

    Description:  returns a copy of <aut> in which every accessible state is made initial.

    Args:   aut (Automaton)

    Returns:  Automaton
    """
    _deprecated("suffix",since="2.0")
    return aut.suffix()


## ========================================================================= ##
def suffix_here(Automaton aut):
    """
    Usage:  suffix_here(aut)

    Description:  makes initial every state of <aut> which is accessible.

    Args:  aut (Automaton)

    Returns:  Automaton
    """
    _deprecated("suffix_here",since="2.0")
    aut.suffix_here()


## ========================================================================= ##
def factor(Automaton aut):
    """
    Usage:  factor(aut)

    Description:  returns a copy of <aut> in which every accessible state is now initial and every co-accessible state is now final.

    Args:  aut (Automaton)

    Returns:  Automaton
    """
    _deprecated("factor",since="2.0")
    return aut.factor()


## ========================================================================= ##
def factor_here(Automaton aut):
    """
    Usage:  factor_here(aut)

    Description:  makes initial every accessible state of <aut> and initial every co-accessible state of <aut>.

    Args:  aut (Automaton)
    """
    _deprecated("factor_here",since="2.0")
    aut.factor_here()



## ========================================================================= ##
def minimal_automaton(Automaton aut, str method = "moore"):
    """
    Usage:  minimal(aut, [method="moore"])

    Description:  computes and returns the minimal automaton accepting the language of <aut>, using determinization if necessary.

    Args:  
        Aut (Automaton)
        method(str, optional), algorithm to use; admissible values are "moore", "hopcroft", or "brzozowski"; default value is "moore".


    Precondition:  weigh-set of <aut> must be B.

    Returns (Automaton)

    Note:  If <aut> is trim (resp. complete), so will be the returned automaton. Otherwise, the returned automaton is not ensured to be complete, nor trim.
    """
    _deprecated("minimal_automaton",since="2.0")
    return aut.minimal_automaton(method)


## ========================================================================= ##
def quotient(Automaton aut, list equiv=None):
    """
    Usage:  quotient(aut, [equiv])

    Description:  computes and returns the (maximal) quotient of \
<aut> respecting <equiv> the partial equivalence relation on the \
state-set of <aut>. If <equiv> is not provided, the minimal quotient \
of <aut> is computed instead.

    Args:  <equiv> (list of list of int, optional), a partial partition of the state-set of <aut>.

    Returns (Automaton)

    Note:  If the partial equivalence relation is invalid, that is if two states allegedly equivalent do not have the same out-language, then the algorithm will not fail but the resulting automaton will not be a quotient of <aut>.
    """
    _deprecated("quotient",since="2.0")
    return aut.quotient(equiv)

## ========================================================================= ##
def min_quotient(Automaton aut, str method = "moore"):
        """
        Usage: min_quotient(aut, [method="moore"])

        Description: computes and returns the minimal quotient of <aut/self>

    
        Args: 
            aut(Automaton)
            method(str, optional), algorithm to use; admissible values are "moore" or "hopcroft"; default value is "moore".

        Returns (Automaton)
        """
        _deprecated("min_quotient",since="2.0")
        aut.min_quotient(method);

## ========================================================================= ##
def quotient_here(Automaton aut, list equiv=None):
        """
        Usage:
          [a] aut.quotient_here(aut)
          [b] quotient_here(aut, equiv)

        Description:
          [a] computes the minimal quotient of <aut> is computed instead.
          [b] computes the (maximal) quotient of \
<aut> respecting <equiv> the partial equivalence relation on the \
state-set of <aut>.

        Args:  <equiv> (list of list of int, optional), a partial partition of the state-set of <aut>.

        Returns (Automaton)

        Note:
          If the partial equivalence relation is invalid, that is if two \
states allegedly equivalent do not have the same out-language, then the \
algorithm will not fail but the resulting automaton will not be a quotient \
of <aut>.
          Convenience function written at the python layer.
        """
        _deprecated("quotient_here",since="2.0")
        aut.quotient_here(equiv)


## ========================================================================= ##
def coquotient(Automaton aut, list equiv=None):
        """
        Usage:
          [a] coquotient_here(aut)
          [b] coquotient_here(aut, equiv)

        Description:
          [a] the minimal coquotient of <aut> is computed instead.
          [b] computes and returns the (maximal) coquotient of \
<aut> respecting <equiv> the partial equivalence relation on the \
state-set of <aut>.

        Args:  <equiv> (list of list of int, optional), a partial partition of the state-set of <aut>.

        Returns (Automaton)

        Note:  If the partial equivalence relation is invalid, that is if two states allegedly equivalent do not have the same out-language, then the algorithm will not fail but the resulting automaton will not be a coquotient of <aut>.
        """
        _deprecated("coquotient",since="2.0")
        return aut.coquotient(equiv)


## ========================================================================= ##
def coquotient_here(Automaton aut, list equiv=None):
        """
        Usage:
          [a] coquotient_here(aut)
          [b] coquotient_here(aut, equiv)

        Description:
          [a] computes the minimal coquotient of <aut>
          [b] computes the (maximal) coquotient of \
<aut> respecting <equiv> the partial equivalence relation on the \
state-set of <aut>.

        Args:  <equiv> (list of list of int, optional), a partial partition of the state-set of <aut>.

        Returns (Automaton)

        Note:
          If the partial equivalence relation is invalid, that is if two \
states allegedly equivalent do not have the same out-language, then the \
algorithm will not fail but the resulting automaton will not be a coquotient \
of <aut>.
          Convenience function written at the python layer.
        """
        _deprecated("coquotient_here",since="2.0")
        aut.coquotient_here(equiv)


## ========================================================================= ##
def matrix_repr(Automaton aut):
        """
        Usage:  matrix_repr(aut)

        Description:  returns the matrix representation of <aut> as an \
initial line-vector, a dictionary that maps letters to matrices, and a final \
column-vector.

        Returns (size-1-list of list of str * (dict str -> list of list of str)  * list of size-1-list of str)
        """
        _deprecated("matrix_repr",since="2.0")
        return aut.matrix_repr()


## ========================================================================= ##
def sum(Automaton aut1, Automaton aut2):
        """
        Usage:  sum(aut1, aut2)

        Description:  computes the parallel join of <aut1> and <aut2>

        Args:
            other (Automaton)

        Returns:
            Automaton: the sum of <aut1> and <aut2>
        """
        return aut1.sum(aut2)
