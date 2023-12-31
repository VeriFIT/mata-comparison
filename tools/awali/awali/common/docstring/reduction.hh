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
 /!\ Beware, the string below is used as a source for generating a page in 
     Doxygen in a md.
     
     A preprocessing is done that escape any <, >, [, ] or backslash
     that appears outside of a block of code.
     This exclude lines starting with 4 spaces and strings between backquote
     and the character > if it is the very first character of a line.

     This process will fail if there is an odd number of backquotes on any 
     given line !  Also backquotes are doubled for technical reasons, 
     so do not use double backquotes.
  
     Please duplicate this warning if you use this file as a template.
******************************************************************************/

#ifndef AWALI_COMMON_DOCSTRING_TEMPLATEDOCFILE_HH
#define AWALI_COMMON_DOCSTRING_TEMPLATEDOCFILE_HH

#include <awali/common/docstring/entry.hh>

namespace awali { namespace docstring {

static entry_t reduction = {
/* Name:  */ "reduction",
/* Description: */ "Presentation of the reduction algorithm",
/* Title: */ "    Presentation of the reduction algorithm",
/* Content: */ 
R"---doxytoken---(  
A `K`-automaton  **A** over an alphabet `A` of dimension `Q` usually
written under its matrix form as **A**`=<I,E,T>` is more conveniently
described for the purpose of the reduction algorithm under its
'representation' form **A**`=<I,µ,T>`, where `µ` is the morphism from
`A^*` into `K^{QxQ}` defined by
 
    E = \sum_{a\in A} µ(a) a .
    
If `K` is a field, **A** is said to be _controllable_ if the dimension of
the vector-space generated by the set of vectors `{ I.µ(w) | w \in A^* }`
is `Q` and  **A** is said to be _observable_ if the dimension of the
vector-space generated by the set of vectors `{ µ(w).T | w \in A^* }` is
`Q`. Indeed, **A** is observable if the _transpose_ of **A** is
controllable.

An automaton **A** is said to be _reduced_ if it is of minimal dimension
among all equivalent automata equivalent to **A**. The reduction algorithm
is then based on the two following results:

Theorem 1 : _**A** is reduced if it is both controllable and observable._

Theorem 2 : _Given **A**`= <I,µ,T>`, it is possible to compute effectively
an automaton **A'**`= <I',µ',T'>` which is 'conjugate' (hence equivalent)
to **A** and with the two properties:_  
 • _**A'** is controllable;_   
 • _if **A** is observable, so is **A'**._
 
These results, hence the algorithm, may be generalized to _skew fields_, and
to _PID_ (Principal Ideal Domains) such as `Z`.   
 
The algorithm is due to M. P. Schützenberger, in his paper where he
introduced the notion of weighted automata ('On the definition of a family
of automata _Information & Control_ 1961'). More details and
bibliographical information can be found in 'J. Sakarovitch, Rational and
recognisable series, in _Handbook of Weighted Automata_, Springer, 2009'.
)---doxytoken---"
};

}} //End of namespaces awali::docstring and awali


#endif

