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

// // Functions that print out lists or info

/*---------------------------------------------------------------------------
|     new function list()         201127                                    |
---------------------------------------------------------------------------*/

void list(std::string s)
{
  std::string title, str1;
  std::string output, margin = " ";

  if ( found_in_list(s, list_choices, it_chc) ) { // !! returns it_chc !! 
	// string s has been found in list_choices
    switch(it_chc->choice_id) {

    case COMMANDS_LST : {
      width_warning();
      print_table();
      break;
    }
    
    case OPTIONS_LST : {
      // building a name-desc list from option list
	  std::vector<name_desc_pair_t> v;
      for(auto c : options) {
        std::string name = " -" + c.first + "  --" + c.second.name;
        std::string caption = c.second.desc;
		v.push_back({name,caption});
      }

      title = "Option list";
      adaptative_print(vw_doc::option_heading);
      process_name_desc_list_with_headers(v, title, " -X  --long-option");
      adaptative_print(vw_doc::option_heading_part2);
      break;
    }
    
    case BASIC_LST : {
      title = "Basic commands";
      process_name_desc_list(commands_basic, title);
      break;
    }
    
     case GENERIC_LST : {
      title = "Generic commands for automata and transducers";
      process_name_desc_list(commands_generic, title);
      break;
    }
    
     case WFA_CMDS_LST : {
      title = "Commands for (weighted) automata";
      process_name_desc_list(commands_aut, title);
      break;
    }
    
     case EXP_CMDS_LST : {
      title = "Commands for (weighted) rational expressions";
      process_name_desc_list(commands_exp, title);
      break;
    }
    
     case NFA_CMDS_LST : {
      title = "Commands for Boolean automata";
      process_name_desc_list(commands_nfa, title);
      break;
    }
    
     case TRANSDUCERS_LST : {
      title = "Commands for (weighted) transducers";
      process_name_desc_list(commands_tdc, title);
      break;
    }
    
    case AUTOMATA_LST : {
      std::map<std::string, dyn::loading::file_loc_t> 
	    dcl = dyn::loading::example_automata();
      std::vector<name_desc_pair_t> v = list_directory_content(dcl);

      title = "Automata library";
      process_name_desc_list_with_headers(v, title);
      break;
    }
    case RATEXPS_LST : {
      std::map<std::string, dyn::loading::file_loc_t> 
	    dcl = dyn::loading::example_ratexps();
      std::vector<name_desc_pair_t> v = list_directory_content(dcl);

	  title = "Rational expression library";
      process_name_desc_list_with_headers(v, title);
      break;
    }
    case FACTORIES_LST : {
      title = "Factories library";
      process_name_desc_list(commands_fact, title);
      break;
    }
    case WEIGHTSET_LST : {
      // Building the name-desc list of implemented weightsets
      std::vector<std::string> 
        wsn = dyn::context::all_weightset_public_static_names();
      std::vector<std::string> 
        wsd = dyn::context::all_weightset_public_descriptions();
      std::vector<name_desc_pair_t> v;
      for (size_t i=0; i<wsn.size(); i++) {
        v.push_back({wsn[i],wsd[i]});
      }
	  
      title = "Implemented weightset list";
      process_name_desc_list(v, title);
      break;
    }
    default :
    {} // impossible since switch is guarded by 'exists' flag
    }
  }

  else {  // string s has not been found in list_choices
    std::string error_msg1 = "'" + s + "'  are not objects that can be listed.";
    std::string error_msg2 = "Type 'cora help list' if necessary.";
    error_print(error_msg1, error_msg2);
    return;
  }

  final_output=NONE;
} // end of fct list(<str>)

// // End of file print_list.cc--- return to print_out.cc
