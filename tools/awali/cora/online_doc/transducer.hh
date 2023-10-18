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

#ifndef ONLINE_DOC_TRANSDUCER_HH
#define ONLINE_DOC_TRANSDUCER_HH

#include <cora/online_doc/shared.hh>

namespace awali { namespace cora { namespace doc { 
	
/*--------------------------------------------------------------------|
|     Long descriptions of transducer commands   (longdesc field)     |    
|--------------------------------------------------------------------*/

/*--------------------------------------------------------------------|
| Use the 'verbatim' mode   R"---( .... )---"                         |
|                                                                     |
| Because these descriptions may appear in a variety of situations,   |
| it is convenient to begin immediately after the ---(                |
| and to put a newlinebefore the )---  as in the first desciption.    |
|                                                                     |
|--------------------------------------------------------------------*/

std::string domain {
R"---(Builds the automaton which is the projection of the transducer <tdc> with respect
to the first tape.
)---"
};

std::string image {
R"---(Builds the automaton which is the projection of the transducer <tdc> with respect
to the second tape.
)---"
};

std::string inverse {
R"---(Swaps the first and the second tape of the transducer <tdc>.
)---"
};

std::string eval_word {
R"---(Computes the image of the word <word> by the transducer <tdc>.
)---"
};

std::string eval_aut {
R"---(Computes the image of the language (series) accepted by the automaton <aut>
by the transducer <tdc>.
	  
Beware of the order of the arguments.
)---"
};

std::string is_functional {
R"---(Tests whether the transducer <tdc> is functional.
	  
Exit with 0 if true.
)---"
};

std::string is_of_finite_image {
R"---(Tests whether the image of every word by the transducer <tdc> is finite.
	  
Exit with 0 if true.
)---"
};

std::string is_synchronizable {
R"---(Tests whether the (subnormalized) transducer <tdc> can be synchronized.

Exit with 0 if true.
)---"
};

std::string synchronize {
R"---(Builds a synchronized transducer from the subnormalized transducer <tdc>.
	  
The synchronization is applied until a contradiction occurs.
)---"
};

std::string subnormalize {
R"---(Builds a subnormalized transducer from transducer <tdc>.
	  
Every tape is labelled by letter or epsilon.
)---"
};

std::string compose {
R"---(Builds the transducer composition of <tdc1> by <tdc2>.
)---"
};



}}} // end of namespace awali::cora::doc, awali::cora and awali

#endif // ONLINE_DOC_TRANSDUCER_HH
