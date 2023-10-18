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

#ifndef CORA_SHARED_ONLINE_DOC_HH
#define CORA_SHARED_ONLINE_DOC_HH

// shared strings (ie called from several places)

namespace awali { namespace cora { namespace doc { 
	
// Use the 'verbatim' mode   R"---( .... )---"
	
std::string warning_term_width {

warning_clr +
"\n[Warning] Cora online doc requires terminals to be at least 80 character wide"
" in order to be displayed correctly." + reset_clr
};

std::string dbl_tp_str {
R"---(The commands marked with '**' accept an argument that can be either
an automaton OR an expression, e.g.  cora info <aut>  or  cora info <exp> .
)---"
};

std::string is_chc_str {
R"---(The commands marked with '*' have the corresponding 'test commands'.
E.g.  cora accessible <aut>  computes the accessible part of <aut>;
  cora is-accessible <aut>  tests if <aut> is accessible (see also 'help is').
)---"
};

std::string def_standard {
R"---(An automaton is standard if it has a single initial state with no incoming 
transition and with an initial function equal to the identity of the weightset.
	  
In this definition, 'automaton' is to be understood in the broadest sense and
can be a (weighted) automaton, in the classical sense, as well as a (weighted) 
transducer.
)---"
};

std::string subtitle_aut {
"\n" + title_clr + "1. Case of an automaton." + "\n"
                 + "------------------------" + reset_clr + "\n\n"
};

std::string subtitle_exp {
"\n" + title_clr + "2. Case of a ratexp." + "\n"
                 + "-------------------------" + reset_clr + "\n\n"
};

std::string quiet {
R"---(
Exit with 0 if true.   No output with option -S.
)---"
};


}}} // end of namespace awali::cora::doc, awali::cora and awali

/* vwdoc, a namespace for pieces of documentation intended to be printed 
   with 'adaptative' printing functions                                  */
namespace awali { namespace cora { namespace vw_doc { 
  
std::string warning_term_width {
"[Warning] Cora online doc requires terminals to be at least 80 character wide"
" in order to be displayed correctly."
};

std::string warning_list_width {
"[Warning] Terminal much too narrow. Only names are listed. "
"No description given."
};

std::string dbl_tp_str {
"The commands marked with '**' accept an argument that can be either "
"an automaton OR an expression, e.g.  cora info <aut>  or  cora info <exp> ."
};

std::string is_chc_str {
"The commands marked with '*' have the corresponding 'test commands'."
" E.g.  cora accessible <aut>  computes the accessible part of <aut>;"
" cora is-accessible <aut>  tests if <aut> is accessible (see also 'help is')."
};

}}} // end of namespace awali::cora::vw_doc, awali::cora and awali
  
#endif // CORA_SHARED_ONLINE_DOC_HH


