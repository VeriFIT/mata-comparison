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

#ifndef ONLINE_DOC_NFA_HH
#define ONLINE_DOC_NFA_HH

#include <cora/online_doc/shared.hh>

namespace awali { namespace cora { namespace doc { 
	
/*--------------------------------------------------------------------|
|        Long descriptions of nfa commands   (longdesc field)         |    
|--------------------------------------------------------------------*/

/*--------------------------------------------------------------------|
| Use the 'verbatim' mode   R"---( .... )---"                         |
|                                                                     |
| Because these descriptions may appear in a variety of situations,   |
| it is convenient to begin immediately after the ---(                |
| and to put a newlinebefore the )---  as in the first desciption.    |
|                                                                     |
|--------------------------------------------------------------------*/

std::string prefix {
R"---(Builds an automaton which accepts the language of prefixes of the
language accepted by <aut>. For that purpose, makes final every co-accessible
state of <aut> (easy exercise: prove that it answers the question).
	  
Restricted to Boolean automata when called from the cora interface. 
In case of a weighted automaton, the result would be rather unpredictable.
)---"
};

std::string suffix {
R"---(Builds an automaton which accepts the language of suffixes of the
language accepted by <aut>. For that purpose, makes initial every accessible
state of <aut> (easy exercise: prove that it answers the question).
	  
Restricted to Boolean automata when called from the cora interface. 
In case of a weighted automaton, the result would be rather unpredictable.
)---"
};

std::string factor {
R"---(Builds an automaton which accepts the language of factor of the
language accepted by <aut>. For that purpose, makes final every co-accessible
state of <aut> and initial every accessible state of <aut> (easy exercise:
prove that it answers the question).
	  
Restricted to Boolean automata when called from the cora interface. 
In case of a weighted automaton, the result would be rather unpredictable.
)---"
};

// std::string determinize {
// R"---(Compute the determinization of the Boolean automoaton <aut>.
// 	  
// The result may not be complete.
// )---"
// };
// 
std::string complement {
R"---(Complements the automaton <aut> which should be a complete and deterministic
Boolean automaton.
)---"
};

std::string complete {
R"---(Makes the automaton <aut> complete, that is, from every state there is at
least one outgoing transition labelled by every letter, by adding a sink
state and the necessary transitions.
	  
Beware that a sink state is added even if <aut> has already one and that the
operation depends not only on the content of <aut> but on its alphabet also.
	  
If <aut> is already complete, a warning message is output and the result is
equal to <aut>.
)---"
};

std::string is_complete {
R"---(Test whether the (Boolean) automaton <aut> is complete.

Exit with 0 if true.
)---"
};

std::string minimal_automaton {
R"---(Compute the minimal automaton of the language accepted by the Boolean
automaton <aut> or denoted by the (Boolean) ratexp <exp>.  
	  
The result may not be complete.
	  
In the case of a ratexp, the algorithm begins with the command 'exp-to-aut' 
on <exp>, with the default option 'standard-and-quotient' which yields 
an <aut>.

Option -M allows to choose the algorithm applied then to <aut>: 
'moore' (default), 'hopcroft' or 'brzozowski'.
	  
Both 'moore' and 'hopcroft' begin with the determinization of <aut>; then, 
the minimal quotient of the result is computed by the corresponding algorithm.
The 'brzozowski' algorithm is the sequence of transpose, determinization, 
transpose, and determinisation.
)---"
};

}}} // end of namespace awali::cora::doc, awali::cora and awali

#endif // ONLINE_DOC_NFA_HH
