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

#ifndef ONLINE_DOC_BASIC_HH
#define ONLINE_DOC_BASIC_HH

namespace awali { namespace cora { namespace doc { 
	
/*--------------------------------------------------------------------|
|        Long descriptions of basic commands   (longdesc field)       |    
|--------------------------------------------------------------------*/

/*--------------------------------------------------------------------|
| Use the 'verbatim' mode   R"---( .... )---"                         |
|                                                                     |
| Because these descriptions may appear in a variety of situations,   |
| it is convenient to begin immediately after the ---(                |
| and to put a newlinebefore the )---  as in the first desciption.    |
|                                                                     |
|--------------------------------------------------------------------*/

// output of command  'cora help' -- special case as it does not come after
// a 'Usage' line

std::string cora_help { // output of command  'cora help'

R"---(
cora is the line command interface to the Awali platform for
computing automata and transducers.
)---"
	  
"\nUsage:  " + usage_clr + "cora [options] <command> <argument> [<argument2>]" 
             + reset_clr + "\n"

"\nwhere, in most cases," + usage_clr + " <command> " + reset_clr +
"is a function applied to" + usage_clr + " <argument>" + reset_clr + ", usually "

R"---(
an automaton, a transducer, or a rational (regular) expression ('ratexp' in
awali parlance).)---"

" The" + usage_clr + " [options] " + reset_clr + ""

R"---(bring more information to <command>, from the
type of some parameters in <argument> to the input or output format to the
method to be used for the computation.
)---"

R"---(
There are more than 80 commands. To give an overview )---"

"\n" + usage_clr + "   cora list commands " + reset_clr + "\n"

R"---(prints (almost) all commands, organised in a table showing 6 'kinds':
'basic-cmds', 'generic-cmds', 'wfa-cmds', 'nfa-cmds', 'ratexp-cmds', and 
'transducer-cmds', and)---"

"\n" + usage_clr + "   cora list <kind> " + reset_clr + "\n"

R"---(prints the list of the commands in the category <kind> together with a short
description for each command. More classically,)---"

"\n" + usage_clr + "   cora help <cmd> " + reset_clr + "\n"

R"---(prints a more detailed description of the command <cmd>, whereas)---"

"\n" + usage_clr + "   cora help <option> " + reset_clr + "\n"

R"---(gives more information on the option <option>.
)---"

"\nBy default, the result of" + usage_clr + " <command> " + reset_clr + ""

R"---(is output on the 'standard output'. This is
sensible when the result is a number, a word, or a (small) ratexp. Not when
the result is an automaton or a (long) ratexp, in which case the output is to
be redirected to a file, and the command line looks then like:
)---"

"\n" + usage_clr + "   cora <command> <argument>  >  <result-file>" 
             + reset_clr + "\n"

R"---(
Commands can be 'piped' in order to compose the commands:
)---"

"\n" + usage_clr + "   cora <command1> <argument> \\| <command2> - "
"\\| <command3> -  >  <result-file>" 
             + reset_clr + "\n"

R"---(
In this case, the '-' refer to the result of the previous command. Such
composition of functions avoids the transformation, forth and back, between
the inner representation of automata (or ratexps) and their linearisation
in files.
	  
A number of automata and ratexps are stored to serve as examples in command
lines. Accordingly, a number of 'factories' allow to build automata that depend
on one, or two, parameters. Automata and expressions can take their 'weights'
or 'coefficients' in various semirings or 'weightsets'.)---"

"\n" + usage_clr + "   cora list automata " + reset_clr + 
"(or " + usage_clr + "ratexps" + reset_clr +
" or " + usage_clr + "factories" + reset_clr +  
" or " + usage_clr + "weightsets" + reset_clr + ")\n"

R"---(gives the list of such available examples or weightsets. 
	  
Finally, some elements of the Awali platform have been documented in such a way
that the corresponding information can be called from cora with the command
)---"
"" + usage_clr + "cora doc <page>" + reset_clr + " "

R"---(in addition to being readable in the Doxygen pages.
)---"
}; // end of 'cora_help'

// help	
std::string help {

"\n Usage : " + usage_clr + " cora help " + reset_clr + " or " + 
"\n         " + usage_clr + " cora help <cmd>  " + reset_clr + " or "
              + usage_clr + " cora help <option> " + reset_clr + "\n"

"\n\u2022" + usage_clr + "  cora help  " + reset_clr + ""

R"---(prints the first piece of information for the cora interface 
which essentially lists the possibilities offered by the other calls to the 
help and list commands.
)---"

"\n\u2022" + usage_clr + "  cora help <cmd>  " + reset_clr + ""

R"---(prints a more detailed description of the command <cmd>.
)---"

"\n\u2022" + usage_clr + "  cora help <option>  " + reset_clr + ""

R"---(gives more information on the option <option>. 
)---"

"\n\u2022" + "  For the command " + usage_clr + "doc" + reset_clr + ","
+ usage_clr + " cora help doc " + reset_clr + ""

R"---(prints the list of the elements of the
Awali platform that have been documented in such a way that the corresponding
information can be called from cora with the command)---"

"" + usage_clr + " cora doc " + reset_clr + ""

R"---(as well as being
accessible from the Doxygen documentation.
)---"
}; 

// list
std::string list {

"The first usage of the " + usage_clr + "list" + reset_clr + " " 

R"---(command is to give the list of cora commands.
)---"
  
"\n\u2022" + usage_clr + "  cora list commands  " + reset_clr + ""

R"---(prints (almost) all commands, organised in a table,
showing 6 kinds of commands:)---"

"\n    '" + usage_clr + "basic-cmds" + reset_clr + 
"', '" + usage_clr + "generic-cmds" + reset_clr +
"', '" + usage_clr + "wfa-cmds" + reset_clr +
"', '" + usage_clr + "nfa-cmds" + reset_clr +
"', '" + usage_clr + "ratexp-cmds" + reset_clr +
"',\n    or '" + usage_clr + "transducer-cmds" + reset_clr + "'.\n" 


"\n\u2022" + usage_clr + "  cora list <kind>  " + reset_clr + ""

R"---(prints the list of the commands in the <kind> category
together with a short description for each command.
)---"


R"---(
A number of automata and ratexps are stored to serve as examples in command
lines. Accordingly, a number of 'factories' allow to build automata that depend
on one, or two, parameters. Automata and ratexps can take their 'weights'
or 'coefficients' in various semirings or 'weightsets'. 
)---"

"\n\u2022" + usage_clr + "  cora list automata  " + reset_clr + ""

R"---(prints the list of automata that have been stored. 
Note that these automata are called in cora commands by their names 'without'
the .json extension.
)---"

"\n\u2022" + usage_clr + "  cora list ratexps  " + reset_clr + ""

R"---(prints the list of ratexps that have been stored. 
Note that these ratexps are called in cora commands by their names 'without'
the .json extension.
)---"


"\n\u2022" + usage_clr + "  cora list factories  " + reset_clr + ""

R"---(prints the list of available factories. Note that the
parameter(s) that have to be given are not indicated in this list. In order to
know them type:)---"   
"   " + usage_clr + "cora help <factory-name>" + reset_clr + ".\n"


"\n\u2022" + usage_clr + "  cora list weightsets  " + reset_clr + ""

R"---(prints the list of weightsets in which coefficients 
of labels of transitions, or atoms of ratexps can be taken.
)---"
};


// doc 
// Special treatment for the list of pages is printed after 
std::string doc {
R"---(Print documentation on the subject)---"

"" + usage_clr + " <choice>" + reset_clr + "."

R"---(
These documentations are also readable in the Doxygen pages.

The available documentation pages are the following.
)---"
};

// cat
std::string cat {
R"---(Load and output the automaton <aut> or the expression <exp>, with different
behaviours, especially in the interpretation of options.
)---"
	
+ awali::cora::doc::subtitle_aut +

R"---(The argument <aut> can be a pre-defined automaton or an automaton stored by the
user in a file. The list of pre-defined automata is given by the command:
)---"

+ usage_clr + " cora list automata" + reset_clr + ".\n"

R"---(
Available input-output formats are: 
   'json' (default), 'grail' and 'fado' (both for Boolean automata only), 
   'dot' and 'pdf' (both for output only), and 'fsm-json-v0' (input only). 

Input-format option is by-passed for predefined automata. Predefined automata
are stored in 'json' format and they are called without the .json extension.
Input-format option is significant when reading a user-stored automaton only
and should be consistent with the content of the read file.

The interplay between the input-format and the output-format options allows for
format conversion of an automaton. 
The 'fsm-json-v0' option allows to read old automata files that have been
produced by Awali versions prior to 2.0.0 or by Vaucanson-R.

Option -N gives the name <name> to the output automaton (in 'json' format only).
)---"
	
+ awali::cora::doc::subtitle_exp +

R"---(The argument <exp> can be 
   (i)  a pre-defined ratexp, or
  (ii)  an ratexp stored by the user in a file, or
 (iii)  an ratexp in 'text' format that is given (between quotes) directly
        on the standard input.
	  
The list of pre-defined ratexps is given by the command:
)---"

+ usage_clr + " cora list ratexps" + reset_clr + ".\n"

R"---(
Available input-output formats are 'json' and 'text' (default), when the ratexp
is written in a file, and 'exp' (input only) if the ratexp is input on the
standard input. In 'text' format, the expression <exp> is a string; type
)---"

+ usage_clr + " cora doc ratexp " + reset_clr + 

R"---(
for the description of 'text' format of ratexps.
	  
If the ratexp is input in the text format, one has to specify the "context"
with other options.

Option -W chooses the weightset in the weightset list: default weightset is 'B'.
Option -L chooses the type of letters: 'char' (default) or 'int' or 'tdc' for
a ratexp which corresponds to a 2-tape transducer.
	   
In contrast with all other commands which define a new object (automaton 
or ratexp), if the type of letters is 'char' the default alphabet is
not {a,b} but is defined by the letters that appears in <exp>, the ratexp 
that is input.

Otherwise, that is if the alphabet is defined by -A option, or in case of
letters of type 'int' (by -A<n> for the alphabet from 0 to n-1, default {0,1}) 
or of tranducers (by -A<list> and -B<list> for the first and second tape 
respectively, default ab on both tapes) the command will abort if an unknown
letter is input in <exp>.
)---"
};

// display 
std::string display {
"\n"
"Display the automaton " + arg_clr + "<aut>" + reset_clr 
+ " or the ratexp " + arg_clr + "<exp>" + reset_clr
+ " using Graphviz.\n" 

R"---(
The pdf file produced by Graphviz is shown in a window called 'tmp.pdf'.
This window is overwritten by a subsequent call to display.

Option -N<name> gives name <name>.pdf to a fresh window that is not overwritten
by a subsequent call to display.
	  
The usage and syntax of option -I are the same as the ones described for 
the cat command, both for automata and ratexps.
)---"

"Cf." + usage_clr + " cora help cat" + reset_clr + ".\n"

+ awali::cora::doc::subtitle_aut +

"Display the automaton" + arg_clr + " <aut> " + reset_clr + ""

R"---(in horizontal mode.

Option -H shows 'history' of states on the display.  
)---"

+ awali::cora::doc::subtitle_exp +

"Display the expression" + arg_clr + " <exp> " + reset_clr + ""

R"---(as a tree and in vertical mode.

Option -H is not relevant in this case.  
)---"
};

std::string info {

"Prints basic characteristics of the automaton " 
+ arg_clr + "<aut>" + reset_clr + 
" or the expression " 
+ arg_clr + "<exp>" + reset_clr + ".\n"
 
// R"---(
// )---"
};

std::string statistics {

"Gives some statistics on the automaton " 
+ arg_clr + "<aut>" + reset_clr + 
" or the expression " 
+ arg_clr + "<exp>" + reset_clr + ".\n"

"The first argument "  
+ arg_clr + "<choice>" + reset_clr + 
" chooses the elements to be counted. The result\n"
"is an integer in all cases.\n"

+ awali::cora::doc::subtitle_aut +

"If the second argument is an automaton " + arg_clr + "<aut>" + reset_clr + ", "
R"---(available statistics are:

   'states',       )
   'initials',     )   with obvious meaning,
   'finals',       )
   'transitions',  )

   'acc-states',   )   count the number of accessible and
   'coacc-states', )   coaccessible states respectively  

   'sc-components'   counts the number of strongly connected components of
                     the underlying graph of <aut>
)---"

+ awali::cora::doc::subtitle_exp +

"If the second argument is an expression " + arg_clr + "<exp>" + reset_clr + ", "
R"---(available statistics are:

   'size'    counts the number of atoms and operators (number of nodes in
             the syntatctic tree of <exp>);
   'length'  counts the number of letters in <exp>;
   'depth'   gives the depth (or height) the syntatctic tree of <exp>
             (the term 'depth' is prefered to 'height' to avoid confusion
              with the next parameter 'star-height');
   'star-height'   gives the star-height of <exp>.			
)---"
};

std::string edit {
R"---(Enter an interactive mode to edit the automaton )---"
	
"" + arg_clr + "<aut>" + reset_clr + ".\n"
				
R"---(
Blabla
)---"
};

std::string new_ {
R"---(Create an empty automaton or transducer and enter the 
interactive editor to edit it. 
						
Option -O, if set, changes behaviour: editing is bypassed and
the empty automaton is output.
						
Other options are used to define the context of the automaton. 

Option -W chooses weight in the weight semiring list: B (default).
Option -L chooses type of letters:
       char (-Lchar, default) or int (-Lint) or a 2-tape transducer (-Ltdc).
Option -A chooses the alphabet (of the first tape for transducer) -Aab (default).
Option -B chooses the alphabet of the second tape (transducer only) -Bab (default).
)---"
};

std::string is {
R"---()---"
};

std::string is_1 {
R"---(Test whether the automaton <aut> has the property <choice>.
Available properties for test are:
)---"
};

std::string is_2 {
R"---(For every property <choice>, a command  'is-<choice>' also exists.
Type: )---"
" " + usage_clr + " cora help is-<choice> " + reset_clr + 
" for more information on the test."	  

R"---(

If <choice> is also a command that builds from <aut> an automaton that has the 
property <choice>, e.g. 'accessible', then the command appears with an asterisk
in the command lists.
)---"
};

}}} // end of namespace awali::cora::doc, awali::cora and awali


namespace awali { namespace cora { namespace vw_doc { 
	
// help	
std::string help {

"\n Usage : " + usage_clr + " cora help " + reset_clr + " or " + 
"\n         " + usage_clr + " cora help <cmd>  " + reset_clr + " or "
              + usage_clr + " cora help <option> " + reset_clr + "\n"

"\n\u2022" + usage_clr + "  cora help  " + reset_clr + ""

"prints the first piece of information for the cora interface " 
"which essentially lists the possibilities offered by the other calls to the " 
"help and list commands.\n"

"\n\u2022" + usage_clr + "  cora help <cmd>  " + reset_clr + ""

"prints a more detailed description of the command <cmd>.\n"


"\n\u2022" + usage_clr + "  cora help <option>  " + reset_clr + ""

"gives more information on the option <option>." 

"\n\u2022" + "  For the command " + usage_clr + "doc" + reset_clr + ","
+ usage_clr + " cora help doc " + reset_clr + ""

"prints the list of the elements of the"
"Awali platform that have been documented in such a way that the corresponding"
"information can be called from cora with the command"

"" + usage_clr + " cora doc " + reset_clr + ""

"as well as being"
"accessible from the Doxygen documentation."

}; 

// intro_basic_cmds	
std::string intro_basic_cmds {
"\n"
"In this context, 'automaton' means 'automaton or transducer'.\n" 
}; 

}}} // end of namespace awali::cora::vw_doc, awali::cora and awali

#endif // ONLINE_DOC_BASIC_HH
