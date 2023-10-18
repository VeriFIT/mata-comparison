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

reset_clr + "\n   Usage : " + usage_clr + "-L<label-type> " + reset_clr 
  + "  or  " + usage_clr + "--labels=<label-type> " + reset_clr + "\n"

R"---(
Sets the 'type' of labels, when creating an automaton or a ratexp. 
Possible values for <label-type> are :   
 • char (default), or
 • int, or 
 • tdc.  

For char and int, the alphabet is set with the -A option.

For tdc, the labels are pairs of letters (or \e), as the   
transducers implemented in cora have 2 tapes only and with char only. 
	  
If both -L and -A are used, -A should come after on the line.      
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

reset_clr + "\n   Usage : " + usage_clr + "-A<alpha> " + reset_clr 
  + "  or  " + usage_clr + "--alphabet=<alpha> " + reset_clr + "\n"

R"---(
Sets the 'letters' that can be used as labels.

 • If 'label-type' is 'char', <alpha> is a sequence of letters,       
   e.g. -Axy defines the alphabet {'x','y'}. 
   Default: -Aab

 • If 'label-type' is 'int', <alpha> is a positive integer and the letters
   are the nonnegative integers smaller than that given bound, 
   e.g. -A3 defines the alphabet {0,1,2}. 
   Default: -A2  

 • If 'label-type' is 'tdc', it is a shortcut to stating that the automaton
   is a 2-tape transducer, or the atoms of the ratexp are 2-tuples, and that
   the letters (on the tapes or in the tuples) are 'char'.
   
   In this case, <alpha> defines the alphabet on the first tape.
   Default: -Aab 
	  
The case of the definition of the alphabet when creating a ratexp is slightly
different. The default alphabet is 'undefined' and filled with the atoms of
the ratexp given as argument.
                          
The option -A comes after -L option if the latter is present.            
)---"
};

// alphabet2
std::string alphabet2 {

reset_clr + "\n   Usage : " + usage_clr + "-B<alpha> " + reset_clr 
  + "  or  " + usage_clr + "--alphabet2=<alpha> " + reset_clr + "\n"

R"---(
Sets the 'letters' on the second tape in case of a transducer or on the second
component of atoms in ratexp over 2-tuples. 
Default: -Bab

See 'cora help alphabet' for more details.
)---"
};

// weightset
std::string weightset {

reset_clr + "\n   Usage : " + usage_clr + "-W<semiring> " + reset_clr 
  + "  or  " + usage_clr + "--weightset=<semiring> " + reset_clr + "\n"

R"---(
Sets the weightset of the automaton or the ratexp, when created, or when
it is modified (with commands 'characteristic' or 'promote').
	  
Choice among: 
	  
B, N, Z, Q, R, C, 
F2, N-oo, Z-min-plus, Z-max-plus, R-max-prod, Fuzzy, Z/<int>Z, N<int>.         

Default value: B (the Boolean semiring).                   

List and short description with  'cora list weightset'.

Warning: under zsh shell, call N[<int>] between "".
)---"
};

// input format
std::string input_format {

reset_clr + "\n   Usage : " + usage_clr + "-I<format> " + reset_clr 
  + "  or  " + usage_clr + "--input-fmt=<format> " + reset_clr + "\n"

R"---(                                                       
For automata, values for <format> are: 
 • json (default), or
 • fado, or 
 • grail.  
	  
For json format, see  'cora doc json-format'. 
	  
For ratexps, values are: 
 • json (default), or
 • text, or 
 • exp. 
	  
'json' and 'text' formats imply that the expression is given in a file, 
'exp' format is text but the expression is given between quotes on the 
command line. For 'text' and 'exp',  see 'cora doc ratexp'.
	  
Default format is "between" 'json' and 'exp': awali tries to guess if 
the argument is the name of a file or an expression. If it finds a file the 
name of which is given as an argument, it will choose to read it with 'json' 
format, otherwise it will take the argument as a ratexp written in 'text'
format.
)---"
};

// output format
std::string output_format {

reset_clr + "\n   Usage : " + usage_clr + "-O<format> " + reset_clr 
  + "  or  " + usage_clr + "--output-fmt=<format> " + reset_clr + "\n"

R"---(
For automata, values for <format> are: 
 • json (default), or
 • fado, or 
 • grail, or
 • dot.
	  
For json format, see  'cora doc json-format'. 
	  
For automata display, values are: 
 • pdf (default), or
 • dot.
	  
For ratexps, values are: 
 • text (default), or
 • json. 

Note that default format for reading and writing ratexps in a file are 
not the same. For 'text' format, see 'cora doc ratexp'.   
)---"
};

// shell mode
std::string script_mode {

reset_clr + "\n   Usage : " + usage_clr + "-S " + reset_clr 
  + "  or  " + usage_clr + "--script-mode " + reset_clr + "\n"

R"---(
Not set by default. 
	  
When set, 
 • does not print to stdout the result of commands if it is an int or a bool,
 • does not print to stderr any warning messages, 
 • use the exit code to return int or bool values.                                 
)---"
};
//   "\t\t1) Do not print to stdout of commands if it is an int or a bool.\n"
//   "\t\t2) Do not print to stderr any warning messages.\n"
//   "\t\t3) Use the exitcode to return int or bool values.\n",

// shell mode
std::string verbose {

reset_clr + "\n   Usage : " + usage_clr + "-V " + reset_clr 
  + "  or  " + usage_clr + "--verbose " + reset_clr + "\n"

R"---(
Set by default.

When set, prints result of test commands.
)---"
};

// method
std::string method {

reset_clr + "\n   Usage : " + usage_clr + "-M<algo> " + reset_clr 
  + "  or  " + usage_clr + "--method=<algo> " + reset_clr + "\n"

R"---(
Determines the method, or algorithm, used by the command <cmd>.
	  
E.g., 'exp-to-aut' implements various algorithms, 'proper' 
may proceed from left to right or from right to left, etc. 
The possible values hence depend on <cmd>.
 
See  'cora help <cmd>'  for details in each case.            
)---"
};

// history
std::string history {

reset_clr + "\n   Usage : " + usage_clr + "-H " + reset_clr 
  + "  or  " + usage_clr + "--history " + reset_clr + "\n"

R"---(
Not set by default.

When set, prints state history in dot format or in display..
)---"
  "Prints "
};

// name
std::string name {

reset_clr + "\n   Usage : " + usage_clr + "-N<name> " + reset_clr 
  + "  or  " + usage_clr + "--name=<name> " + reset_clr + "\n"

R"---(
Sets the name of the automaton produced by the command.
(Visible in the metadata and used for naming the display output window). 
	  
See  'cora doc json-format'  for the description of metadata.
)---"
};

// caption
std::string caption {

reset_clr + "\n   Usage : " + usage_clr + "-C<caption> " + reset_clr 
  + "  or  " + usage_clr + "--caption=<caption> " + reset_clr + "\n"

R"---(
Sets the caption of the automaton produced by the command.
(Visible in the metadata only). 

See  'cora doc json-format'  for the description of metadata. 
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
