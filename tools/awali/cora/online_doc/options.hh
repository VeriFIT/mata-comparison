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

#include <cora/online_doc/shared.hh>

#ifndef ONLINE_DOC_OPTIONS_HH
#define ONLINE_DOC_OPTIONS_HH

/*--------------------------------------------------------------------|
|   Long descriptions of options   (optiondesc field option struct)   |    
|--------------------------------------------------------------------*/
namespace awali { namespace cora { namespace doc { 
	

/*--------------------------------------------------------------------|
| Use the 'verbatim' mode   R"---( .... )---"                         |
|                                                                     |
| The format used allows to fit in the table of options printed by    |
|   cora help options                                                 |
|                                                                     |
|--------------------------------------------------------------------*/

// heading
std::string option_heading {
R"---(
In addition to arguments, Cora commands are given information via options that
control their behaviour, the way they read arguments and output the result, the
methods they use to compute it.

The syntax to give value to an option follows the Unix standard:
)---"

"  " + usage_clr + " -Xvalue " + reset_clr + " or "
       + usage_clr + " --long-option=value " + reset_clr + "\n"
R"---(
For each command <cmd>, the possible options and their effect are described in:
)---"

"  " + usage_clr + " cora help <cmd> " + reset_clr + ".\n"
};
	
// labelset
std::string labelset {
R"---(                                                       
 Possible values: char (default), int, tdc.                 
 For char and int, the alphabet is set with the -A option.  
 For tdc, the labels are pairs of letters (or \e), as the   
 transducers implemented in cora have  2 tapes only. If both
 -L and -A are used, -A should come after on the line.      
)---"
};
//   " for char, the default alphabet is {'a','b'}, for int, it is {0,1};\n"
//    "  it can be modified with option -A;\n"
// 	  "  for tdc, the labels are pairs of letters (or \\e),\n"
// 	  " the default alphabet is {'a','b'}, it can be modified\n"
// 	  " with option -A for the first component,\n"
// 	  " with option -B for the second component;\n"
//    " if both -L and -A are used, -A should be set after -L on
// 	the command line\n",


// alphabet
std::string alphabet {
R"---(
 Comes after -L option if the latter is present.            
 If letters are char, value is a sequence of letters,       
   e.g. -Axy defines the alphabet {'x','y'}. Default: -Aab  
 If letters are int, the letters are the nonnegative        
 integers smaller than the given bound,                     
   e.g. -A3 defines the alphabet {0,1,2}. Default value: -A2
 When creating an expression, the default alphabet is       
 'undefined' and filled with the atoms of the expression.   
)---"
};

// alphabet2
std::string alphabet2 {
  ""
// R"---(
//  Comes after -L option if the latter is present.            
//  If letters are char, value is a sequence of letters,       
//    e.g. -Axy defines the alphabet {'x','y'}. Default: -Aab  
//  If letters are int, the letters are the nonnegative        
//  integers smaller than the given bound,                     
//    e.g. -A3 defines the alphabet {0,1,2}. Default value: -A2
//  When creating an expression, the default alphabet is       
//  'undefined' and filled with the atoms of the expression.   
// )---"
};

// weightset
std::string weightset {
R"---(                                                       
 Choice among: B, N, Z, Q, R, C, F2, N-oo, Z-min-plus,      
 Z-max-plus, R-max-prod, Fuzzy, Z/<int>Z, N[<int>].         
 Default value: B (the Boolean semiring).                   
 List and short description with  cora list weightset.

 Warning: under zsh shell, call N[<int>] between "".
)---"
};
// //     std::string("set the semiring of weights;\n")
//     std::string("                   choice among ")
//     +dyn::context::all_weightset_public_static_names_as_string(),
//     //B, Z, Q, R, C, Z-min-plus, Z-max-plus, R-max-prod, F2, Z/<n>Z."


// input format
std::string input_format {
R"---(                                                       
 For automata, values are: json (default), fado, or grail.  
 For json format, see  cora doc json-format.                
 For expressions, values are: text, json, or exp. json and  
 text formats imply that the expression is given in a file, 
 exp format is text but the expression is given between     
 quotes on the line. For text,  see cora doc ratexp-writing.
 Default is 'between' text and exp: awali tries to guess if 
 the argument is the name of a file or an expression.       
)---"
};

// output format
std::string output_format {
R"---(
                                                                                             
 For automata: json (default), dot, fado, or grail.         
 For automata display: pdf (default), or dot.               
 For expressions: text (default), or json.                  
 See  cora doc json-format  and  cora doc ratexp-writing.   
)---"
};

// shell mode
std::string script_mode {
R"---(                                                         
 Not set by default. When set, do not print to stdout the   
 result of commands if it is an int or a bool, do not print 
 to stderr any warning messages, use the exit code to       
 return int or bool values.                                 
)---"
};
//   "\t\t1) Do not print to stdout of commands if it is an int or a bool.\n"
//   "\t\t2) Do not print to stderr any warning messages.\n"
//   "\t\t3) Use the exitcode to return int or bool values.\n",

// shell mode
std::string verbose {
  "Prints result of test commands (set by default)."
};

// method
std::string method {
R"---(                                                                                                  
 E.g., 'exp-to-aut' implements various algorithms, 'proper' 
 may proceed from left to right or from right to left, etc. 
 See  cora help <cmd>  for details in each case.            
)---"
};

// history
std::string history {
  "Prints state history in dot format or in display."
};

// name
std::string name {
R"---(
 (visible in the metadata and used for naming the display   
 output window) See  cora doc json-format  for the metadata.
)---"
};

// caption
std::string caption {
R"---(
 See  cora doc json-format  for the description of metadata. 
)---"
};

// // title
// std::string title {
// R"---(
// Sets the name of the window open by the command display.
// )---"
// };



}}} // end of namespace awali::cora::doc, awali::cora and awali

namespace awali { namespace cora { namespace vw_doc { 
	
// heading
std::string option_heading {
"\n"
"In addition to arguments, Cora commands are given information via options "
"that control their behaviour, the way they read arguments and output the "
"result, the methods they use to compute it.\n\n"

"The syntax to give value to an option follows the Unix standard:\n"

"  " + usage_clr + " -Xvalue " + reset_clr + " or "
       + usage_clr + " --long-option=value" + reset_clr + "\n\n"

"For each command <cmd>, the possible options and their effect are described in:\n"

"  " + usage_clr + " cora help <cmd> " + reset_clr + ".\n"
};
	
std::string option_heading_part2 {
"For more information on each option, type\n"
"  " + usage_clr + " cora help <long-option> " + reset_clr + ".\n"
};

}}} // end of namespace awali::cora::vw_doc, awali::cora and awali

#endif // ONLINE_DOC_OPTIONS_HH
