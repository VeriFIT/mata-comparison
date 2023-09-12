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

#include<awali/common/docstring/doc_entries.hh>
#include<cora/cora.hh>
#include <sstream>
#include <sys/types.h>
#include <list>

// #define DEBUG_FLAG    # <-- uncomment for displaying debug on std::cerr
#ifdef DEBUG_FLAG
  #define DEBUG(str) std::cerr << str ;
  #define DEBUG2(str1,str2) std::cerr << str1 << str2;
  #define DEBUG3(str1,str2,str3) std::cerr << str1 << str2 << str3;
  #define DEBUGL(str) DEBUG2(str, std::endl)
  #define DEBUG2L(str1,str2) DEBUG3(str1,str2,std::endl)
#else
  #define DEBUG(str)
  #define DEBUG2(str1,str2)
  #define DEBUG3(str1,str2,str3)
  #define DEBUGL(str)
  #define DEBUG2L(str,str2)
#endif


namespace awali { namespace cora {


std::list<std::string> extensions= { ".json", ".gv", ".grail", ".fado" };

typedef struct {
  std::string real_result;
  std::string addition;
  std::string commentary;
  bool append_space;
} possibility_t;

std::list<possibility_t> possibilities;



void print_if_possible(const std::string &current_arg, const possibility_t &possibility) {
  if (!possibility.real_result.compare(0,current_arg.length(), current_arg)) {
    DEBUG2L(">>>>>>>> Possibility found: ", possibility.real_result)
    possibilities.push_back(possibility);
  } else {
    DEBUG2L("Possibility rejected: ", possibility.real_result)
  }
}

void flush_possibility() {
//   if ( format.compare("zsh") )
  if ( possibilities.size() == 1 ) {
    possibility_t p = possibilities.front();
    std::cout << p.real_result ;
    if (p.append_space) {
      DEBUG2L("Writing an extra space:", p.real_result)
      std::cout << ' ' ;
    }
    std::cout << std::endl;
  } else
    for(const possibility_t& p: possibilities)
    {
      std::cout << p.real_result << p.addition << " ";
//       if ( !p.commentary.compare("") )
//         std::cout << " -- " << p.commentary;
      std::cout << std::endl;
    }
}

void print_local_automata(const std::string &cur_arg) {
  DEBUGL("Considering local files.")
  std::string dir_name = ".";
  for(int i= cur_arg.length(); i-->0;)
    if ( cur_arg[i] == '/' ) {
      dir_name= cur_arg.substr(0, i+1);
      break;
    }
  DEBUG2L("Considered directory: ", dir_name)
  DIR* d = opendir(dir_name.c_str());
  if ( !dir_name.compare(".") )
    dir_name = "";
  struct dirent * entry;
  while((entry=readdir(d))!=nullptr) {
    if ( entry->d_type == DT_REG ) {
      std::string d_name= entry->d_name;
      size_t length= d_name.length();
      DEBUG2L("Found Regular file: ", d_name)
      for (std::string& s: extensions) {
        if (  length> s.length() && ! d_name.compare(length-s.length(), s.length(), s) ) {
          print_if_possible(cur_arg, {dir_name+d_name, "", "", true} );
          break;
        } else {
          DEBUGL("Not an automaton file")
        }
      }
    } else if ( entry->d_type == DT_DIR ) {
      std::string d_name= entry->d_name;
      DEBUG2L("Found directory: ", d_name)
      if ( d_name.compare(".") && d_name.compare("..") )
        print_if_possible(cur_arg, { ""+dir_name+d_name+"/", "", "", false} );
    } else {
      DEBUG2L("Could not determine file type: ", entry->d_name)
    }
  }
}


void print_arg (int argc, std::vector<std::string> argv, bool is_pipe, bool is_option) {
  std::string cmd_name = argv[0];
//   size_t cmd_name_len = cmd_name.size();
  if (argc <= 1) {
    for ( auto pair : commands )
      if (pair.cst != MPTY_CMD)
        print_if_possible(cmd_name, {pair.name, "", "", true} );
  } else {
    command cmd;
    for (auto c : commands) 
      if (c.cst != MPTY_CMD && c.name == cmd_name) {
        cmd= c;
        break;
      }
    std::string cur_arg = argv[argc-1];
    int cur_arg_len = cur_arg.size();
    int tot_arg= cmd.argc;

    argument_t arg= NONE;
    if (tot_arg >= (argc-1)) {
      arg= cmd.args[argc-2];
      // -2: -1 because of "./cora" & -1 because of <command_name>
    }

    switch (arg.kind) {
      case AUT:
      case TDC: 
      case AUT_OR_EXP: 
      {
        print_local_automata(cur_arg);
        if (is_pipe)
          print_if_possible(cur_arg, {"-", "", "", true});
        std::map<std::string,dyn::loading::file_loc_t> ex 
        = dyn::loading::example_automata();

        for (auto p : ex)
          if ( !p.first.compare(0, cur_arg_len, cur_arg) ) {
            print_if_possible(cur_arg, {p.first, "", "", true} );
          }

        if (arg.kind != AUT_OR_EXP)
          break;
      }
      case EXP: {
        print_local_automata(cur_arg);
        if (is_pipe)
          print_if_possible(cur_arg, {"-", "", "", true});
        std::map<std::string,dyn::loading::file_loc_t> ex 
        = dyn::loading::example_ratexps();

        for (auto p : ex)
          if ( !p.first.compare(0, cur_arg_len, cur_arg) ) {
            print_if_possible(cur_arg, {p.first, "", "", true} );
          }
        print_if_possible(cur_arg, {"<Any raw rational expression>", "", "", true});
        break;
      }
      case CHC: {
        std::vector<choice>* choices = nullptr;
        if (cmd.name == "is")
          choices = &is_choices;
        else if (cmd.name == "statistics")
          choices = &stats_choices;
        else if (cmd.name == "list")
          choices = &list_choices;

        if (cmd.name == "doc") {
          for (auto entry : awali::docstring::entries) 
            print_if_possible(cur_arg, {entry.name, "", "", true} );
        }
        else {
          if (choices != nullptr)
            for (auto chc : (*choices))
              print_if_possible(cur_arg, {chc.name, "", "", true} );
        }
        break;
        
      }
      case CMD:
        {
          std::vector<std::string> new_argv = { cur_arg };
          print_arg (1, new_argv, is_pipe, is_option);
          
          if (cmd.name == "help") {
            for (auto chc : help_tokens)
              print_if_possible(cur_arg, {chc.name, "", "", true} );
          }


          break;
        }
      default:
          if (!is_option) {
            print_if_possible(cur_arg, {"!", "", "", true});
//             std::cout << "Argument type:" << std::endl;
            std::stringstream ss;
            std::string s;
            ss << arg;
            ss >> s;
            print_if_possible(cur_arg, {"Expected argument type: "+s, "", "", true});
          }
    }
  }
}

void print_opt(int argc, std::vector<std::string> argv, bool is_pipe)
{
  std::string cur_arg = argv[argc-1];
  for ( auto pair: options ) {
      std::stringstream ss;
      ss << pair.second.args;
      std::string str;
      ss >> str;
      print_if_possible(cur_arg, {"-"+pair.first, str, "", pair.second.argc==0} );

//       ss << "--" << pair.second.desc;
      std::string result="--"+pair.second.name;
      if (pair.second.argc == 1) {
        ss << pair.second.args;
        result+= "=";
      }
      ss >> str;
      print_if_possible(cur_arg, {result, str, "", pair.second.argc==0} );
  }
}

std::vector<std::string> remove_options_and_pipe (int* argc, const char** argv, bool* is_pipe)
{
  std::vector<std::string> part_result;
  for(int i=0;  i< *argc;  i++)
  {
    part_result.push_back( (argv[i]==nullptr) ? "" : argv[i] );
    DEBUG2L("Arg: ", part_result[i]);
  }

  int start=0;
  for(int i=(*argc); i-->0; )
    if ( !part_result[i].compare("\\|") ) {
      start= i+1;
      (*is_pipe)= true;
      (*argc)-= start;
      break;
    } else {
      DEBUG2L("Not a pipe: ", part_result[i])
    }
  DEBUG2L("Index after last Pipe: ",start)
  DEBUG2L("New argc: ",*argc);
  std::vector<std::string> result;
  int end = *argc;
  for (int i=0; i< (end-1); i++)
  {
    int j = start+i;
    if ( (part_result[j].length() > 1) && (part_result[j][0] == '-')  ) {
    // We keep the chain '-' which is a real argument
      DEBUG2L("Discarded option: ",part_result[j]);
      (*argc)--;
    } else {
      DEBUG2L("Kept arg: ",part_result[j]);
      result.push_back(part_result[j]);
    }
  }
  DEBUG2L("Kept last arg/option: ",part_result[start+end-1]);
  result.push_back(part_result[start+end-1]);
  return result;
}

}}//end of ns awali::cora

int main (int argc, const char** argv)
{
  awali::cora::init_cmds();
  awali::cora::init_lists();
  awali::cora::init_options();
  bool is_pipe = false;
  bool is_option = false;
  int new_argc = atoi(argv[1]);
  DEBUGL(new_argc)
  std::vector<std::string> args = awali::cora::remove_options_and_pipe( &new_argc, &argv[3], &is_pipe);
  if (args[new_argc-1][0] == '-') {
    DEBUGL("Last arg may be an option")
    awali::cora::print_opt (new_argc, args, is_pipe);
    is_option = true;
  }
  awali::cora::print_arg (new_argc, args, is_pipe, is_option);
  awali::cora::flush_possibility();
}
