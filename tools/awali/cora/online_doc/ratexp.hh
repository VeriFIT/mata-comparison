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

#ifndef ONLINE_DOC_RATEXP_HH
#define ONLINE_DOC_RATEXP_HH

#include <cora/online_doc/shared.hh>

namespace awali { namespace cora { namespace doc { 
	
/*--------------------------------------------------------------------|
|       Long descriptions of ratexp commands   (longdesc field)       |    
|--------------------------------------------------------------------*/

/*--------------------------------------------------------------------|
| Use the 'verbatim' mode   R"---( .... )---"                         |
|                                                                     |
| Because these descriptions may appear in a variety of situations,   |
| it is convenient to begin immediately after the ---(                |
| and to put a newlinebefore the )---  as in the first desciption.    |
|                                                                     |
|--------------------------------------------------------------------*/

std::string constant_term {
R"---(Computes the constant term of the ratexp <exp>, if valid, that is, the weight
of the empty word in the series denoted by the ratexp.
)---"
};

std::string expand {
R"---(Expands the ratexp <exp>. 
	  
Expanding an expression consists in developing all products and grouping
the monomials, and doing this computation recursively under the star operator. 
Expanding a same expression under different weightset may yield quite different
outcome. E.g.
	  
cora expand 'a(a(b+c)+ac)* + a(ab)*' 
a(ab)*+a(ab+ac)*
	  
cora -WF2 expand 'a(a(b+c)+ac)* + a(ab)*'
\z
)---"
};

std::string star_normal_form {
R"---(Computes the star normal form of the (Boolean) ratexp <exp>.
	  
Command valid for Boolean ratexps only. 
)---"
};

std::string exp_to_aut {
R"---(Build an automaton from the rational expression <exp>.
	  
Option -M allows to choose the method of computation, and then the result.	  
The following methods are available in awali and yield the following automata:

1. <glushov>=<standard>   the Glushkov (aka position, standard) automaton
	  
2. <derived-term>         the derived term (aka partial derivatives) automaton
	  
3. <standard-and-quotient>   is the default option; 
             it computes the minimal quotient of the standard automaton;
             it yields a small size automaton, often equal to the derived term 
			 automaton, sometimes smaller, but often more efficiently.

4. <breaking>                the broken derived term automaton
             this is a variant of the derived term automaton
			 
5. <thompson>                the Thompson automaton

6. <compact-thompson>        compact variant of the Thompson automaton

7. <net>=<weighted-thompson> another variant of the Thompson automaton 

All methods work for weighted ratexps, and yield automata that give every word
the same coefficient as the ratexp, but for the cases of methods (5) and (6) 
which may produce, in the case of non positive weight semirings, automata that
are non valid even if <exp> is valid --- e.g. (a* + <-1>(b*))* .
)---"
};

// More details on the computed automata and on the algorithms implemented with
// the command 'cora doc exp-to-aut'.

std::string derivation {
R"---(Compute the derivation of the ratexp <exp> with respect to the word <word>.
The result is a linear combination of terms (ratexps) written as list with 
coefficients
	  
Option -M<method> allows to choose the result:
	  
1. <derivation>  yields the derived terms; default option
2. <breaking>    yields the broken derived terms.
)---"
};

}}} // end of namespace awali::cora::doc, awali::cora and awali

#endif // ONLINE_DOC_RATEXP_HH

// std::string eval {
// R"---(
// )---"
// };

/////.  ????
// std::string exp_to_aut_doc {
// R"---(The command  'cora -M<method> exp-to-aut <exp>' computes the automaton that
// accepts the language, or that realises the series, denoted by <exp> by using the
// the algorithm called <method>.
// 	  
// The available methods in awali are:
// 1. <glushov>, also called <standard> 
// 2. <derived-term>
// 3. <breaking>
// 4. <thompson>
// 5. <compact-thompson>
// 6. <weighted-thompson>
// 7. <standard-and-quotient>
// 
// To be completed
// )---"
// };



