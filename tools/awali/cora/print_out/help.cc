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

/*---------------------------------------------------------------------------
| functions  help()     200929                                              |
|       and  help(str)  201127    After Victor's  print_name_desc           |
|                                     lists of commands printed by list     |
|                       201211    complete rewriting to comply with Victor's|
|                                 instructions                              |
|                       211029    suppression of help_tokens                |
|                                 special case of command eval              |
---------------------------------------------------------------------------*/

void help()
{
  width_warning();
  
  std::cout << doc::cora_help << std::endl;
  final_output=NONE;
}

// function help(<str>)
void help(std::string s)
{
  std::vector<choice>::iterator it_chc;
  std::map<std::string, option>::iterator it_opt;
  std::vector<command>::iterator it_cmd;
  
  size_t terminal_width = balanced_term_width(); //to allow variable width print


// Look for string s first in 'help_tokens' list, then in 'options' list and
// finally in 'commands' list. If s is not found, it is assumed that s was 
// a mispelled command.
  if ( (!true) ) { // deprecated but not suppressed in case of remorse
//     found_in_list(s, help_tokens, it_chc) ) { // !! returns it_chc !! 
//     // string s has been found in help_tokens
//     width_warning();
//     std::cout << it_chc->longdesc << std::endl;
  } // end processing  s  in help_tokens

  else { // search among options
    if ( found_in_option_list(s, it_opt) ) { // !! returns it_opt !! 
      width_warning(); //option longdesc in awali::cora:doc fixed width 80 chars
      std::cout << it_opt->second.longdesc << std::endl; // begins and ends
                                                         // with a newline 
    } // end processing  s  is an option long-value
     
    else { // search among commands
      if ( !found_in_list( s, commands, it_cmd) ) { //  s has been found nowhere
        std::string error_msg1 = "'" + s +"' unknown Awali command or option.";
        std::string error_msg2 = "Type 'cora list commands' if necessary.";
        error_print(error_msg1, error_msg2);
      }
	  
      else { // !! the test has returned it_cmd !!
		// string s is a command; processing help <cmd>
        // It consists in printing first a line for the usage,
        // and then the 'longdesc' field of the command.
        // The usage line is processed differently, according to <cmd>
        width_warning();  //  because it ends in every case with printing 

	    if ( (s == "help") || (s == "eval") ) { // special cases
          if ( s == "help" ) {
            // Case 1.1  help directly calls printing of longdesc, no usage line		
            std::cout << it_cmd->longdesc << std::endl;
          } 
          else {
            // Case 1.2  eval is a polysemic command, special usage line		
            std::string  str, str3, help_str;
            help_str  = "\nCommand " + usage_clr + it_cmd->name + reset_clr +
              " accepts as first argument an automaton as well as a transducer."
			  " 'eval' is then a common name for two distinct functions.";
            variable_width_print(help_str, terminal_width);
			
            str = usage_clr + "cora " + it_cmd->name ;
            str3="\n   or" + std::string(4,' ') ;
            help_str = " Usage : " + str + " <aut> <word> " + reset_clr + str3
                       + str + " <tdc> <aut> " + reset_clr + 
                       " or  " + str + " <tdc> <exp> " + reset_clr + "\n";  
            std::cout << help_str <<  std::endl;
            std::cout << it_cmd->longdesc << std::endl;
          }
	    } 
	    else {
          std::string  str, str3; 

		  if (found_in_list( s, double_type_cmds)) {
            // Case 2  s is a command that accepts both automata and expressions
            std::string help_str1 = "\nCommand " + usage_clr + it_cmd->name + reset_clr +
                       " accepts as argument an automaton as well as a ratexp.";
            std::cout << help_str1 <<  std::endl;

            str = usage_clr + "cora " + it_cmd->name ;
			if (it_cmd->argc == 1) {
              std::string help_str2 = "\n Usage : " + str + " <aut> " + reset_clr + 
                                    " or " + str + " <exp> " + reset_clr + "\n";
              std::cout << help_str2 <<  std::endl;
            } 
            else {
			  str = str + " " ;    // + it_cmd->args[0];
              std::cout << "\n Usage : " + str << it_cmd->args[0]
                        << " <aut> " + reset_clr + " or " + str << it_cmd->args[0]
                        << " <exp> " + reset_clr + "\n" << std::endl;
            }

            //   The usage line is doubled in presence of options
            if ( it_cmd->optiondesc != "") {
              str = usage_clr + "cora " + it_cmd->optiondesc + " " + it_cmd->name;
			  if (it_cmd->optiondesc.size()>10) 
                str3="\n or" + std::string(17,' ') ; // pad_space(17);
              else 
                str3="  or ";
              std::string help_str3 = " or with option(s): " + str + " <aut>" +
                                      reset_clr + str3 + str + " <exp>" +
                                      reset_clr + "\n";
              std::cout << help_str3 <<  std::endl;
            }
          }

          else {
            // Case 3 s is 'normal' ie accepts either an <aut> OR an <exp>
            str = reset_clr + "\n";
            std::string help_str = "\n Usage : " + usage_clr + "cora " +  
                                   it_cmd->name + " ";
            std::cout << help_str << it_cmd->args << str << std::endl;
            //   The usage line is doubled in presence of options
            if ( it_cmd->optiondesc != "") {
              help_str = " or with option(s) : " + usage_clr + "cora " + 
                         it_cmd->optiondesc + " " + it_cmd->name + " " ;
              std::cout << help_str << it_cmd->args << str << std::endl;
            }
          }
          //   Printing of the 'longdesc' field
          std::cout << it_cmd->longdesc << std::endl; // "\n" <<
        }

        if ( s == "doc" ) // Case  s = doc different from others
          print_docstring_list();

        if ( s == "is" ) {// Case  s = is different from others
          std::cout << doc::is_1 << std::endl;
          print_name_list(is_choices);
          std::cout << doc::is_2 << std::endl;
        }
        
      }
  }// end processing  s is a command
  } // end processing  s is not a help_class

  final_output=NONE;
} // end of fct help(<str>)

// // End of file --- return to print_out.cc
