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

#ifndef ONLINE_DOC_WFA_HH
#define ONLINE_DOC_WFA_HH

#include <cora/online_doc/shared.hh>

namespace awali { namespace cora { namespace doc { 
	
/*--------------------------------------------------------------------|
|         Long descriptions of wfa commands   (longdesc field)        |    
|--------------------------------------------------------------------*/

/*--------------------------------------------------------------------|
| Use the 'verbatim' mode   R"---( .... )---"                         |
|                                                                     |
| Because these descriptions may appear in a variety of situations,   |
| it is convenient to begin immediately after the ---(                |
| and to put a newlinebefore the )---  as in the first desciption.    |
|                                                                     |
|--------------------------------------------------------------------*/

std::string eval {
R"---(Compute the weigth of the word <word> in the series realized by the 
automaton <aut>.
)---"
};

std::string enumerate {
R"---(Enumerate the words accepted by the automaton <aut> in the radix ordering,
together with their weight, and up to length n.
)---"
};

std::string shortest {
R"---(Enumerate the shortest n words (in the radix ordering) accepted by the 
automaton <aut>, with their weight.
)---"
};

std::string min_quotient {
R"---(Compute the minimal quotient of the automaton <aut>.

If the automaton <aut> is (Boolean and) deterministic, this computes 'the'
minimal automaton of the language accepted by <aut>, up to the fact that the
result may be not complete.

Option -M allows to choose the algorithm: 'hopcroft' or 'moore' (default).
)---"
};

std::string min_coquotient {
R"---(Compute the minimal coquotient of an automaton.

If the automaton <aut> is (Boolean and) codeterministic, this computes 'the'
cominimal automaton of the language accepted by <aut>, up to the fact that the
result may be not complete.

Option -M allows to choose the algorithm: 'hopcroft' or 'moore' (default).
)---"
};

std::string reduce {
R"---(Compute a reduced automaton equivalent to <aut>.
	  
The weightset must be Z or a field.
)---"
};

std::string left_reduce {
R"---(Compute a controllable automaton equivalent to <aut>.
	  
The weightset must be Z or a field.
)---"
};

std::string right_reduce {
R"---(Compute a observable automaton equivalent to <aut>.
	  
The weightset must be Z or a field.
)---"
};

// std::string w_determinize {
std::string determinize {
R"---(Compute the (weighted) determinization of <aut>.
	  
The method is that of (weighted) subset construction, that is, the states of
the determinization are weighted subsets of the state set Q of <aut> or, which
amounts to the same thing, vectors of dimension Q with entries in the weightset
of <aut>.	  

The weightset must be a (locally) finite semiring to insure the finiteness
of the computation. The result is a deterministic automaton and the weight
of a word is given by the final function at the state reached by that word
when input in this automaton.
)---"
};

std::string is_deterministic {
R"---(Test whether the automaton <aut> is deterministic.

A (weighted) automaton is deterministic if:
. it has a unique initial state,
. from every state there exists at most one transition labelled by each
letter of the alphabet, and 
. the weight of every transition is the 'one' of the weightset.
    
Exit with 0 if true.
)---"
};

std::string is_sequential {
R"---(Test whether the automaton <aut> is sequential.

A (weighted) automaton is sequential if:
. it has a unique initial state, and
. from every state there exists at most one transition labelled by each
letter of the alphabet.
    
Exit with 0 if true.
)---"
};

std::string is_ambiguous {
R"---(Test whether the automaton <aut> is ambiguous.

A (weighted) automaton is ambiguous if there exists a word which is the label
of two distinct computations.
    
Exit with 0 if true.
)---"
};

std::string transpose {
R"---(Build the transpose automaton of <aut>.

)---"
};

std::string compact_paths {
R"---(Replace sequences of non branching transitions in <aut> by transitions
labelled by words.

To retrieve a 'usual' automaton, use command 'letterize'.
)---"
};

std::string letterize {
R"---(Convert the automaton on words <aut> into an automaton on letters
by replacing every transition labelled by a words by a sequence of 
transitions labelled by letters.
)---"
};

std::string change_alphabet {
R"---(Change the alphabet of the automaton <aut> with the alphabet specified
by option -A. 
	  
Transitions with labels which do not belong to the new alphabet are deleted.
Hence the operation amonts to the Hadamard product of the characteristic series
of the free monoid described by the option.
	  
N.B.  1) If no option is given, the new alphabet is assumed to be the default
one, that is {a,b}.

2) The function does not change the 'type' of letters, and then:

. if the letterType is 'Char', and the option of the form '-An', 
with n an integer, n will be considered as a character and all other
characters will be suppressed from the alphabet.

. if the letterType is 'Integer', and the option of the form '-Axyz',
with 'xyz' characters, the function will abort.  
)---"
};

std::string product {
R"---(Compute the product of the two automata <aut1> et <aut2>.
	  
Both automata may have different types, if 'compatible'.
)---"
};

std::string power {
R"---(Compute the n-th power of the automaton <aut>.
	  
That is, if n > 1, builds the product <aut> * <aut> * ... * <aut>  (n times).

If  n = 1, a copy of <aut> is returned.
If  n = 0, the automaton returned is such that its product with <aut> 
gives <aut> again, hence the 1-state automaton, with one transition (loop) for 
each letter of the alphabet of <aut>.

If n < 0 or is not an integer, an error message is output.
)---"
};

std::string shuffle {
R"---(Compute the shuffle product of the two automata <aut1> et <aut2>.

Both automata may have different types, if 'compatible'.
)---"
};

std::string infiltration {
R"---(Compute the infiltration product of the two automata <aut1> et <aut2>.
	  
Both automata may have different types, if 'compatible'.
)---"
};

std::string are_equivalent {
R"---(Test whether the two automata  <aut1> et <aut2> are equivalent.

Both automata must have the same weightset of the following type: B, N, Z or F, 
where F is a field.
    
Exit with 0 if true.
)---"
};

std::string partial_identity {
R"---(Build the transducer which realizes the partial identity on the language
(or the series) defined by the automaton <aut>.

)---"
};


}}} // end of namespace awali::cora::doc, awali::cora and awali

#endif // ONLINE_DOC_WFA_HH

// std::string eval {
// R"---(
// )---"
// };

