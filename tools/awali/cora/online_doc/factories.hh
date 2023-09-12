// This file is part of Awali.
// Copyright 2016-2020 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
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

#ifndef ONLINE_DOC_FACTORIES_HH
#define ONLINE_DOC_FACTORIES_HH

#include <cora/online_doc/shared.hh>

namespace awali { namespace cora { namespace doc { 
	
/*--------------------------------------------------------------------|
|        Long descriptions of factories   (longdesc field)            |    
|--------------------------------------------------------------------*/

/*--------------------------------------------------------------------|
| Use the 'verbatim' mode   R"---( .... )---"                         |
|                                                                     |
| Because these descriptions may appear in a variety of situations,   |
| it is convenient to begin immediately after the ---(                |
| and to put a newlinebefore the )---  as in the first desciption.    |
|                                                                     |
|--------------------------------------------------------------------*/

std::string ladybird {
R"---(Build an automaton with n states whose determinization (complete) 
has 2^n states.
When n=7, the automaton can be drawn like a ladybird, hence the (strange) name.
The 3-letter alphabet can be modified with -A option.
)---"
};

std::string n_ultimate {
R"---(Build the automaton with n+1 states that recognizes words with an 'a' at 
the n-th position from end.
The determinisation of this automaton is the de Bruijn graph with 2^n states.
The default alphabet {a,b} can be modified with the -A option.
)---"
};

std::string double_ring {
R"---(Build an automaton with n states that recognizes words with the same number of
'a's as 'b's modulo n.
The default alphabet {a,b} can be modified with the -A option.
)---"
};

std::string divkbaseb {
R"---(Build the automaton with k states that recognizes words representing multiple
of k written in base b. 
The integers are written with the digit characters from 0 to b-1.
)---"
};

std::string cerny {
R"---(Build an automaton with long synchronizing word.
)---"
};

std::string witness {
R"---(Build Brzozowski's universal witness with n states.
This automaton is a candidate for worst case of many algorithms.
)---"
};

std::string random_dfa {
R"---(Generate a random DFA over the alphabet defined by the -A option.
)---"
};



}}} // end of namespace awali::cora::doc, awali::cora and awali

#endif // ONLINE_DOC_FACTORIES_HH
