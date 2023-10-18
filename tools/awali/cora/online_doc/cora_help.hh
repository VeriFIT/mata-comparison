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

#ifndef ONLINE_DOC_CORA_HELP_HH
#define ONLINE_DOC_CORA_HELP_HH

namespace awali { namespace cora { namespace doc {
	
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

}}} // end of namespace awali::cora::doc, awali::cora and awali

#endif // ONLINE_DOC_CORA_HELP_HH
