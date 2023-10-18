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

"1.  " + usage_clr + "cora eval <aut> <word>" + reset_clr + "\n"

R"---(
Computes the weigth of the word <word> in the series realized by the
automaton <aut>.  Returns a weight.
	  
The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed.
)---"

"\n2.  " + usage_clr + "cora eval <tdc> <aut>" + reset_clr + " or "
         + usage_clr + "cora eval <tdc> <exp>" + reset_clr + "\n"

R"---(
Computes the image by the transducer <tdc> of the language (or series) 
accepted by the automaton <aut>, in the first case, or denoted by the 
ratexp <exp> in the second. Returns an automaton or a ratexp accordingly.
)---"

};

std::string enumerate {
R"---(Enumerate the words accepted by the automaton <aut> in the radix ordering,
together with their weight, and up to length n.
	  
The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed.
)---"
};

std::string shortest {
R"---(Enumerate the shortest n words (in the radix ordering) accepted by the 
automaton <aut>, with their weight.
	  
The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed.
)---"
};

std::string reduce {
R"---(Computes a reduced automaton equivalent to <aut>.
	  
The weightset of <aut> must be Z or a field.

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed.
	  
For a presentation of the reduction algorithm, see  'cora doc reduction'.
)---"
};

std::string left_reduce {
R"---(Computes a controllable automaton equivalent to <aut>.
	  
The weightset of <aut> must be Z or a field.

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed.
	  
For a presentation of the reduction algorithm, see  'cora doc reduction'.
)---"
};

std::string right_reduce {
R"---(Computes an observable automaton equivalent to <aut>.
	  
The weightset of <aut> must be Z or a field.

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed.
	  
For a presentation of the reduction algorithm, see  'cora doc reduction'.
)---"
};

// std::string w_determinize {
std::string determinize {
R"---(Computes the (weighted) determinization of <aut>.
	  
The method is that of (weighted) subset construction, that is, the states of
the determinization are weighted subsets of the state set Q of <aut> or, which
amounts to the same thing, vectors of dimension Q with entries in the weightset
of <aut>.	  

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed.

The weightset must be a (locally) finite semiring to insure finiteness of
the computation. The result is a deterministic automaton and the weight of
a word is given by the final function at the state reached by that word when
inputs in this automaton.

This function is a generalization to weighted automata of the classical
determinization of Boolean automata and behaves exactly as the latter for
Boolean automata.
)---"
};

std::string explore_by_length {
R"---(Computes the part of the weighted determinization that is reached by words of
length less than, or equal to, <n>. (Recall that in general, the weighted 
determinization of <aut> may be infinite.)

As for the determinize function, the method is that of (weighted) subset
construction, that is, the states are weighted subsets of the state set Q of
<aut> or, which amounts to the same thing, vectors of dimension Q with entries
in the weightset of <aut>.	  
	  
The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed.

The result is a deterministic automaton and the weight of a word is given by
the final function at the state reached by that word when inputs in this
automaton. The set of 'accepted' words, that is, the support of the series
realized by the result is a strict subset of the support of the series realized
by <aut>, unless the (weighted) determinization of <aut> is indeed finite and
accessible by words of length less than, or equal to, <n>.

This automaton also contains states that are sinks, with final function equals
to 0, reached by words of length n+1 when different from the states reached by
words of length less than n+1.
)---"
};

std::string explore_with_bound {
R"---(Computes a part of the weighted determinization of <aut>, based on the bound on
 weights given by <k>.  (Recall that in general, the weighted determinization 
of <aut> may be infinite.)

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed, otherwise the command outputs an error message.

In the following, we denote the weightset of <aut> by <weightset>, the
(possibly infinite) weighted determinization of <aut> by <det_aut> and the 
result of this command by <res_aut>; <res_aut> is a deterministic automaton
over <weightset>.	  

Every state of <det_aut> is associated with a vector (x_1,...,x_q), where
the x_i's are weights in <weightset> and q is the number of states in <aut>. 
Aside from sink states, <res_aut> is the subautomaton of <det_aut> containing
the states that satisfy '(x_i)² is less than, or equal, to (<k>)²' for every i
in Q. On the other hand, if the run of a word w in <det_aut> passes through a
state not satisfying the above condition, then the run of w ends in a 
non-accepting sink state in <res_aut>.

The set of words accepted by <res_aut>, that is, the support of the series
realized by the result is a subset of the support of the series realized by
<aut>.

This command requires <weighset> to be N, Z, N-oo or N<int>.
)---"
};

std::string is_deterministic {
R"---(Tests whether the automaton <aut> is deterministic.

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed, otherwise the command outputs an error message.

A (weighted) automaton is deterministic if:
• it has a unique initial state with initial function equals to the 'one' of
  the weightset,
• from every state, there exists at most one transition labelled by each
  letter of the alphabet, and 
• the weight of every transition is the 'one' of the weightset.
    
Exit with 0 if true.
)---"
};

std::string is_sequential {
R"---(Tests whether the automaton <aut> is sequential.

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed, otherwise the command outputs an error message.

A (weighted) automaton is sequential if:
• it has a unique initial state, and
• from every state there exists at most one transition labelled by each
  letter of the alphabet.
    
Exit with 0 if true.
)---"
};

std::string is_ambiguous {
R"---(Tests whether the automaton <aut> is ambiguous.

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed, otherwise the command outputs an error message.

A (weighted) automaton is ambiguous if there exists a word which is the label
of two distinct computations.
    
Exit with 0 if true.
)---"
};

std::string compact_paths {
R"---(Replaces sequences of non branching transitions in <aut> by transitions
labelled by words.

To get back an automaton labeled by letters, use command 'letterize'.
)---"
};

std::string letterize {
R"---(Converts the automaton on words <aut> to an automaton on letters
by replacing every transition labelled by a word by a sequence of 
transitions labelled by letters.
)---"
};

std::string change_alphabet {
R"---(Changes the alphabet of the automaton <aut> to the alphabet specified
by the option -A. 
	  
Transitions with labels which do not belong to the new alphabet are deleted.
Hence the operation amounts to the Hadamard product of the series realised
by <aut> with the characteristic series of the free monoid described by 
the option.
	  
N.B.  (1) If no option is given, the new alphabet is assumed to be the default
one, that is, {a,b}.

(2) The function is not allowed to change the 'type' of letters, and then:

• if the letterType is 'Char', and the option of the form '-An', 
  with 'n' an integer, 'n' will be considered as a character and all other
  characters will be suppressed from the alphabet.

• if the letterType is 'Integer', and the option of the form '-Axyz',
  with 'xyz' characters, the function will abort.  
)---"
};

std::string product {
R"---(Computes the product of the two automata <aut1> et <aut2>.
	  
Both automata must be labelled with letters with no epsilon-transitions
allowed, otherwise the command outputs an error message. Otherwise, the
automata may have different contexts, if 'compatible'. In this case, and if 
not equal, two contexts are compatible if:
	 
 • the weightset of one can be promoted in the weightset of the other;
 
 • the letter kinds must be equal (char or int) but the alphabets may be
   different, in which case the transitions of the result will be labelled
   by letters in the intersection of the two alphabets.
)---"
};

std::string power {
R"---(Computes the n-th power of the automaton <aut>.
	  
That is, if n > 1, builds the product <aut> * <aut> * ... * <aut>  (n times).

If  n = 1, a copy of <aut> is returned.
If  n = 0, the automaton returned is such that its product with <aut> 
gives <aut> again, hence the 1-state automaton, with one transition (loop) for 
each letter of the alphabet of <aut>.

If n < 0 or is not an integer, an error message is output.

The automaton <aut> must be labelled with letters with no epsilon-transitions
allowed, otherwise the command outputs an error message.
)---"
};

std::string shuffle {
R"---(Computes the shuffle product of the two automata <aut1> et <aut2>.

Both automata must be labelled with letters with no epsilon-transitions
allowed, otherwise the command outputs an error message. Otherwise, the
automata may have different contexts, if 'compatible'. In this case, and if 
not equal, two contexts are compatible if:
	 
 • the weightset of one can be promoted in the weightset of the other;
 
 • the letter kinds must be equal (char or int) but the alphabets may be
   different, in which case the transitions of the result will be labelled
)---"
};

std::string infiltration {
R"---(Computes the infiltration product of the two automata <aut1> et <aut2>.
	  
Both automata must be labelled with letters with no epsilon-transitions
allowed, otherwise the command outputs an error message. Otherwise, the
automata may have different contexts, if 'compatible'. In this case, and if 
not equal, two contexts are compatible if:
	 
 • the weightset of one can be promoted in the weightset of the other;
 
 • the letter kinds must be equal (char or int) but the alphabets may be
   different, in which case the transitions of the result will be labelled
)---"
};

std::string are_equivalent {
R"---(Tests whether the two automata  <aut1> et <aut2> are equivalent.

Both automata must be labelled with letters (of the same kind), but possible 
epsilon-transitions are accepted. Both automata must have the same weightset
of the following type: B, N, Z or F, where F is a field.
    
Exit with 0 if true.
)---"
};

std::string partial_identity {
R"---(Builds the transducer which realizes the partial identity on the language
(or the series) defined by the automaton <aut>.
	  
Of course, <aut> must be an automaton and not a transducer.  
)---"
};

}}} // end of namespace awali::cora::doc, awali::cora and awali

namespace awali { namespace cora { namespace vw_doc { 
	
// intro_generic_cmds	
std::string intro_wfa_cmds {
"\n"
"'wfa' means, of course, weighted finite automata -- which includes classical " 
"Boolean automata -- and should be understood here, in the context of the cora " 
"interface, as in contrast with weighted finite transducers. That is, the " 
"commands in this category apply to automata over free monoids and not to "
"automata over direct product of free monoids.\n"
"The commands that apply to both automata and transducers are of the 'generic' "
"kind."
"\n\n"
"Moreover, most of the commands require that the argument be an automaton "
"labelled by letters with no epsilon-transitions allowed."
}; 

}}} // end of namespace awali::cora::vw_doc, awali::cora and awali

#endif // ONLINE_DOC_WFA_HH

// std::string eval {
// R"---(
// )---"
// };

