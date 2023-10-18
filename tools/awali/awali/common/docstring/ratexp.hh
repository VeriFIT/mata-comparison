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

#ifndef AWALI_COMMON_DOCSTRING_RATEXP_HH
#define AWALI_COMMON_DOCSTRING_RATEXP_HH

#include <awali/common/docstring/entry.hh>

namespace awali { namespace docstring {

static entry_t ratexp = {
/* Name:  */ "ratexp",
/* Description: */ "Writing rational expressions in text format",
/* Title: */ "Writing of Rational Expressions",
/* Content: */ 
R"---doxytoken---(  
A priori, any character may appear in the alphabet list, and no error or
warning will be sent. However, the use of characters `(`, `)`, `+`, `.`, or `*`
will result into weird output, or abort.

### Symbols for constant

Besides the letters of the alphabet, expressions are written with the symbols
`\z` for the constant zero (denoting the empty language, or the zero series)
and `\e` for the constant one (denoting the empty word). 
Caveat: the empty string is not accepted for denoting the empty word, that is,
for instance, `cora cat ''` results in an error and the correct writing is 
`cora cat '\e'`.

### Trivial identities

All expressions are necessarily reduced modulo the 'trivial identities':

    0+E = E+0 = E    0.E = E.0 = E    1.E = E.1 = E

The elements of the weight semiring are written between < >. All expressions 
are then reduced modulo the 'weighted trivial identities':

    <0>E = E<0> = 0    <1>E = E<1> = E     <h>(<k>E<l>)<m> = <hk>E<lm> 
        
and, if `x` is a letter,   `<h>x<k> = <hk>x`  .

(Note that the writing  `<h><k>E`  is not syntactically correct.)

If the letters are integers, the product has to be marked with a dot 
(in order to distinguish from integers that are written by a sequence of 
digits).

### Parenthezing and operator precedence 

A sequence of operators `+` or `.` is implicitely parenthezised from left
to right and the corresponding parentheses are kept implicit in an output
in text format. And the operator `.` is kept implicit if the letters are
characters (that is, of type `char`). That is, `((a+b)+c)`  is written  `a+b+c`
and  `(a+(b+c))`  `a+(b+c)`, `(a.(b.c))`  is written  `a(bc)`  etc.
 
The operators are ordered by precedence: 

      +  <  .  <  *  <   <k>

where the last operation is the external multiplication by a coefficient k in
the weight semiring. This implies for instance, and contrarily to the usual
reading, that the expression  `ab*` denotes the language 
`{a, ab, abb, abbb,...}` and that `<2>a*` denotes the series 
`\e + <2>a + <4>aa + ...` The parentheses that are made useless by the
precedence ordering are suppressed in the output.
)---doxytoken---"
};


}} //End of namespaces awali::docstring and awali


#endif

// Once Jacques said:
// > Automata are elementary (this text is not a block of code)

// std::string rat_exp_doc {
// R"---(
// Option -W chooses weight in the weight semiring list: B (default).
// Option -L chooses type of letters:
//        char (-Lchar, default) or int (-Lint) or a 2-tape transducer (-Ltdc).
// 	   
// In contrast with all other commands which define a new object (automaton 
// or expression), if the type of letters is 'char' the default alphabet is
// not {a,b} but is defined by the letters that appears in <exp>, the expression 
// that is input.
// 
// Otherwise, that is if the alphabet is defined by -A option, or in case of
// letters of type 'int' (by -A<n> for the alphabet from 0 to n-1, default {0,1}) 
// or of tranducers (by -A<list> and -B<list> for the first and second tape 
// respectively, default ab on both tapes) the command will abort if an unknown
// latter is input in <exp>.
// 
// A priori, any character may appear in the alphabet list, and no error or warning 
// will be sent. However, the use of characters '(', ')', '+', '.', or '*' will
// result into weird output, or abort.
// 
// Besides the letters of the alphabet, expressions are written with the symbols 
// '\z' for the constant zero (denoting the empty language, or the zero series)
// and '\e' for the constant one (denoting the empty word). 
// Caveat: the empty string is not accepted for denoting the empty word, that is,
// "cora catE ''" results in an error and the correct writing is "cora catE '\e'".
// 
// All expressions are necessarily reduced modulo the 'trivial identities':
// 
//         0+E = E+0 = E    0.E = E.0 = E    1.E = E.1 = E
// 		
// The elements of the weight semiring are written between < >. All expressions 
// are then reduced modulo the 'weighted trivial identities':
// 
//     <0>E = E<0> = 0    <1>E = E<1> = E     <h>(<k>E<l>)<m> = <hk>E<lm> 
// 	
// and, if x is a letter,   <h>x<k> = <hk>x  .
// 
// (Note that the writing  '<h><k>E'  is not syntactically correct.)
// 
// If the letters are integers, the product has to be marked with a dot 
// (in order to distinguish from integers that are written b a sequence of digits).
// 
// A sequence of operators '+' or '.' is implicitely parenthezised from left
// to right and the corresponding parentheses are kept implicit in an output
// in text format. And the operator '.' is kept implicit if the letters are
// 'char'. That is, '((a+b)+c)'  is written  'a+b+c'  and  '(a+(b+c))'  'a+(b+c)',
// '(a.(b.c))'  is written  'a(bc)'  etc.
//  
// The operators are ordered by precedence: 
// 
//                  + < . < * < '<k>'  
// 
// where the last operation is the external multiplication by a coefficient k
// in the weight semiring. This implies for instance, and contrarily to the usual
// reading, that the expression  'ab*' denotes the language {a, ab, abb, abbb,...}
// and that '<2>a*' denotes the series '\e + <2>a + <4>aa + ...'. 
// The parentheses that are made useless by the precedence ordering are suppressed
// in the output.
// )---"
// };





