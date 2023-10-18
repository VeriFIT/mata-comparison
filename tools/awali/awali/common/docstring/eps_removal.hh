// This file is part of Awali.
// Copyright 2016-2023 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
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

/*****************************************************************************
  See template-docfile.hh for documentation and how characters are preprocessed
******************************************************************************/

#ifndef AWALI_COMMON_DOCSTRING_EPSREMOVAL_HH
#define AWALI_COMMON_DOCSTRING_EPSREMOVAL_HH

#include <awali/common/docstring/entry.hh>

namespace awali { namespace docstring {

static entry_t eps_removal = {
/* Name:  */ "eps_removal",
/* Description: */ "Specification and description of the 'epsilon-removal' algorithm",
/* Title: */ "    Specification and description of the epsilon-removal algorithm",
/* Content: */ 
R"---doxytoken---(  
### Specification:

Write  `<aut> = (I, E, T)`  where  `I`  is the vector of initial states 
(of initial values for weighted automata),  `T`  the vector of final states
(of final values for weighted automata), and  `E`  the transition matrix.
Let  `E = F + G`  where  `F`  is 'proper', that is, its entries do not contain 
the empty word with non zero coefficient, whereas the entries of  `G`  are, 
when non zero, the empty word with a non zero coefficient. The behaviour
of `<aut>` is:

    I.E^*.T = I.(F+G)^*.T = I.(G^*.F)^*.G^*.T = I.G^*.(F.G^*)^*.T
  
By definition,

    proper( (I,E,T), backward) = ( I, (G^*.F), (G^*.T) )   and 
  
    proper( (I,E,T), forward)  = ( (I.G^*), (F.G^*), T ) .
  
  
### Algorithm:

Proper computes both  `G^*.F`  and  `G^*.T`  with a method which is similar
to the state elimination method. All states are visited, one after the other.

At state  `s`, every epsilon-transition incoming to  `s`  is removed, giving birth
to one new transition for every transition outgoing from  `s`. The case of an 
epsilon-transition which is a loop on  `s`  with weight  `k`  is special. 
If  `k`  is starrable, the weight of every transition outgoing from  `s`  is 
multiplied by  `k^*`. If  `k`  is not starrable, then  `<aut>`  is not valid and
the algorithm fails and stops. Hence the algorithm is also the test for the 
'validity' of `<aut>` (see 'is-valid' function).

After all epsilon-transition incoming to  `s`  are removed, if no other incoming
transitions are left, state  `s`  is also removed.

Of course, being final is treated as an outgoing transition, and being initial 
as an incoming transition.

More details can be found in: S. Lombardy, J. Sakarovitch: The validity of
weighted automata, *Int. J. of Algebra and Computation* (2013) 863--913.
)---doxytoken---"
};

}} //End of namespaces awali::docstring and awali


#endif

