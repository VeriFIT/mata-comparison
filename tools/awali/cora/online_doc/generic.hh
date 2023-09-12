// This file is part of Awali.
// Copyright 2016-2021 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
//
// Awali is a free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef ONLINE_DOC_GENERIC_HH
#define ONLINE_DOC_GENERIC_HH

#include <cora/online_doc/shared.hh>

namespace awali { namespace cora { namespace doc { 
	

/*--------------------------------------------------------------------|
|       Long descriptions of generic commands   (longdesc field)      |    
|--------------------------------------------------------------------*/

/*--------------------------------------------------------------------|
| Use the 'verbatim' mode   R"---( .... )---"                         |
|                                                                     |
| Because these descriptions may appear in a variety of situations,   |
| it is convenient to begin immediately after the ---(                |
| and to put a newline before the )---  as in the first desciption.   |
|                                                                     |
|--------------------------------------------------------------------*/

//// Graph traversal functions
std::string accessible {
R"---(Computes the accessible subautomaton of <aut>.
)---"
};

std::string coaccessible {
R"---(Computes the coaccessible subautomaton of <aut>.
)---"
};

std::string trim {
R"---(Computes the trim subautomaton of <aut>.
)---"
};

std::string is_empty {
R"---(Test whether the automaton <aut> is empty.
)---"
};

std::string is_accessible {
R"---(Test whether the automaton <aut> is accessible.
)---"
};

std::string is_coaccessible {
R"---(Test whether the automaton <aut> is coaccessible.
)---"
};

std::string is_trim {
R"---(Test whether the automaton <aut> is trim.
)---"
};

std::string is_useless {
R"---(Test whether the trim part of the automaton <aut> is empty.
)---"
};

//// standard automata
std::string standard {
R"---(Makes the automaton <aut> standard by adding one new initial state and 
the appropriate transitions, labels and weights, if <aut> is not already
standard.
	  
If <aut> is standard, sends a warning on the warning output and returns <aut>. 

)---" //line skipped because of the sequel

+ awali::cora::doc::def_standard
};

std::string is_standard {
R"---(Tests whether the automaton <aut> is standard.

)---" //line skipped because of the sequel

+ awali::cora::doc::def_standard
};
// + awali::cora::doc::quiet

std::string are_isomorphic {
R"---(Test whether the two automata <aut:1> and <aut:2> are isomorphic.
	  
Sends an error on the error output if <aut:1> and <aut:2> have not the same
static context.
)---"
};
// + awali::cora::doc::quiet

//// Miscellaneous
std::string strip {
R"---(Remove every history information in the automaton <aut>.
)---"
};

//// strongly connected components
std::string condensation {
R"---(
Compute the strongly connected components and merge every connected component
into a single state.

(Use -H option with \"display\" to see which states were merged
)---"
};

std::string is_strongly_connected {
R"---(Tests whether the automaton <aut> is strongly connected.
)---"
};

//// Dealing with the weightset
std::string support {
R"---(Build the Boolean automaton with the same transition function as the
input.
 
Sends a warning on the warning output if <aut> is already a Boolean automaton.
	  
Note that the function computes the support of the 'automaton' <aut>, and not
of the series 's' realised by <aut>. The language accepted by the support
automaton may strictly contain the support of 's' if the weightset of <aut> is
not 'positive'.
)---"
};
// + awali::cora::doc::quiet

std::string characteristic {
R"---(Builds the characteristic <weightset>-automaton of the Boolean 
automaton, or transducer, <aut>, that is, the automaton in which every 
transition is a copy of a transition of <aut> but with a weight which is the
identity of <weightset>.
	  
The weightset is to be specified with the option -W. If this option is omitted,
the default semiring taken is 'N' and a warning is sent on the warning output.
	  
Sends an error on the error output if the weightset of <aut> is not 'B'.

Note that the function computes the 'characteristic automaton' of <aut>, and 
that the behaviour of that automaton is not the characteristic series of the
language accepted by <aut>, unless <aut> be unambiguous.
)---"
};

std::string promote {
R"---(Builds a copy of the automaton <aut> where weights (in the transitions and the 
initial and final functions) which belong to the weightset of <aut> are
converted into elements of the weightset <weightset>.

If the weightset of <aut> is B, this command is equivalent to the
characteristic command, ie B can be 'promoted' into any other weightset.
	  
The weightset       can be promoted into
  N                    N-oo, Z, Q, R, C
  Z                    Q, R, C
  Q                    R, C
  R                    C

<weightset> is to be specified with the option -W. If this option is omitted
and <aut> is a Boolean automaton, a warning is output and the result is equal
to <aut>, otherwise an error is output as no weightset can be 'promoted' into B.  
)---"
};

////  Rational operations on automata
std::string sum {
R"---(Form the union of the two automata <aut:1> and <aut:2>.

If the '-Mstandard' option is set, both <aut:1> and <aut:2> must be standard
and the sum of standard automata is performed by merging the two initial states
and adding their final functions.  
)---"
};
// Sends an error on the error output if <aut:1> and <aut:2> have not the same
// static context, or if the '-Mstandard' option is set and at least one of <aut:1>
// and <aut:2> is not standard.

std::string concatenate {
R"---(Build an automaton that accepts the concatenation (or product) of languages
(or series) accepted by the inputs.
    
The result is standard if the first argument is. (An automaton if standard if
it has a single initial state with no incoming transition.)
)---"
};

std::string star {
R"---(Build a standard automaton that accepts the star of the language (or series)
accepted by <aut>.

The algorithm first builds the standard automton of <aut> and then applies the
construction of the star of a standard automaton. (An automaton if standard if
it has a single initial state with no incoming transition.)
)---"
};

std::string left_mult {
R"---(Builds an automaton that accepts the product, on the left, of the series
accepted by <aut> by <weight>.
	  
If <aut> is not standard, every initial weight is multiplied (on the left)
by <weight>. If <aut> is standard, the weight of every transition leaving the
initial state, as well as the initial function of the initial state, is
multiplied (on the left) by <weight>.
)---"
};

std::string right_mult {
R"---(Builds an automaton that accepts the product, on the right, of the series
accepted by <aut> by <weight>.
    
Every final weight is multiplied, on the right, by <weight>.
)---"
};

////  Dealing with spontaneous transitions
std::string allow_eps {
R"---(Returns a copy of the automaton <aut> in which epsilon-transitions are
allowed.
)---"
};

std::string is_proper {
R"---(Tests whether the automaton <aut> has some epsilon-transitions.
)---"
};

std::string is_valid {
R"---(Test if the automaton <aut> is valid, that is, if for every input the 
sum of weights of paths labeled by this input is well-defined.
	  
The simplicity of this definition is misleading as it refers to the summability
of infinite sets, a property which is a priori difficult to decide with an
algorithm.
	  
The precise definition of validity, which is indeed more technical, its 
framework, and the algorithm implemented in Awali are described and justified 
in [*]. A sketchy description goes as follows.

Of course, the problem comes from the existence of 'epsilon-transitions'
in <aut>, which may induce the existence of an infinite number of paths labeled
by a same input. An automaton where the number of accepting paths is finite 
for every input is valid; hence an automaton without epsilon-transitions or
such that the subgraph of epsilon-transitions is acyclic is valid.

If the subgraph of epsilon-transitions has cycles, the algorithm depends on 
the nature of the weight semiring, or weightset, K: 

-- K 'starrable', i.e. the star is defined for every element, then every
   automaton is valid. The starrable semirings implemented in Awali for cora
   are B, N-oo, Fuzzy, and N<int>.
   
-- K 'non starrable': i.e. the star is defined for no element except zero,
   then an automaton is valid iff the subgraph of epsilon-transitions is 
   acyclic. Non starrable semirings implemented in Awali for cora are Z and
   the finite semirings F2 and Z/<int>Z.
   
-- K 'TOPS' (Topologically Ordered Positive Semirings) where the domain of 
   star is downward closed, e.g. Q+, R+, tropical semirings,..., a notion
   introduced in [*]. In this case, the 'proper' algorithm -- described in
   on-line doc of proper command (and in [*]) is performed; it is shown in [*]
   that it fails if and only if the automaton is not valid.
   
-- K 'absval': semirings where absolute value allows to define absolute 
   summations, e.g. Q, R. This brings back to the case of TOPS. 
	  
   
The test whether a ratexp is valid is conceptually simpler. The constant term
c(<exp>) of the ratexp <exp> is computed by a bottom-up traversal of the 
syntactic tree of <exp>. Then, <exp> is valid if and only if the computation
of c(<exp>) succeeds. 

--------------------
[*] S. Lombardy, J. Sakarovitch: The validity of weighted automata, 
Int. J. of Algebra and Computation (2013) 863--913.
)---"
};

std::string proper {
R"---(Remove the epsilon-transitions from the automaton <aut>, without changing
its behaviour.
	  
The option -Mbackward (default) or -Mforward tells the result (see 
specification below).
	  
A warning message is output if <aut> is already proper.	  

	  
Specification:

Write  <aut> = (I, E, T)  where  I  is the vector of initial states 
(of initial values for weighted automata),  T  the vector of final states
(of final values for weighted automata), and  E  the transition matrix.
Let  E = F + G  where  F  is 'proper', that is, its entries do not contain 
the empty word with non zero coefficient, whereas the entries of  G  are, 
when non zero, the empty word with a non zero coefficient. The behaviour
of <aut> is:

  I.E^*.T = I.(F+G)^*.T = I.(G^*.F)^*.G^*.T = I.G^*.(F.G^*)^*.T
  
By definition,

  proper( (I,E,T), backward) = ( I, (G^*.F), (G^*.T) )   and 
  
  proper( (I,E,T), forward)  = ( (I.G^*), (F.G^*), T ) .
  
  
Algorithm:

Proper computes both  G^*.F  and  G^*.T  with a method which is similar
to the state elimination method. All states are visited, one after the other.

At state  s , every epsilon-transition incoming to  s  is removed, giving birth
to one new transition for every transition outgoing from  s . The case of an 
epsilon-transition which is a loop on  s  with weight  k  is special. 
If  k  is starrable, the weight of every transition outgoing from  s  is 
multiplied by  k^*. If  k  is not starrable, then  <aut>  is not valid and
the algorithm fails and stops. Hence the algorithm is also the test for the 
'validity' of <aut> (see 'is-valid' function).

After all epsilon-transition incoming to  s  are removed, if no other incoming
transitions are left, state  s  is also removed.

Of course, being final is treated as an outgoing transition, and being initial 
as an incoming transition.
)---"
};

//// aut-to-exp
std::string aut_to_exp {
R"---(Convert an automaton into a ratexp with the state elimination method.

Use with -M options : 'id-order' or 'min-inout-degree' (default) heuristics.

The 'id-order' heuristic consists in eliminating the states in the order of
their id. An Awali user has no control on the id of a state and this option
plays the role of a random choice. Its purpose is to show by contrast the
interest and efficiency of the 'min-inout-degree' heuristic.
	  
The 'min-inout-degree' heuristic consists in computing for every state  s  
a rank  r(s)  which is a pair  r(s)= (pd,l) where  l  is a boolean which tells
whether  s  bears a loop  or not and  pd = in(s)*out(s)  is the product of the
in-degree of  s  with the out-degree of  s  (and where the (unique) loop on  s ,
if any, does not contribute to the in- and out-degrees). At every step of the
procedure, the rank is computed again and the state chosen to be eliminated
is the one with smallest rank in the lexicographic order (minimize  pd  and for
equal  pd  take rather a state without loop).
)---"
};

}}} // end of namespace awali::cora::doc, awali::cora and awali

#endif // ONLINE_DOC_GENERIC_HH
