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

#include<awali/common/docstring/doc_entries.hh>
#include<awali/dyn/core/context_description.hh>


#include <sys/ioctl.h>
// #include <stdio.h>
#include <unistd.h>


size_t compute_term_width() {
  size_t term_width = 80;
#ifdef TIOCGSIZE
  struct ttysize ts;
  int ret = ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
  if (ret>=0)
    term_width = ts.ts_cols;
#elif defined(TIOCGWINSZ)
  struct winsize ts;
  int ret = ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
  if (ret>=0)
    term_width = ts.ws_col;
#endif
  return term_width;
}


struct name_desc_pair_t {
  std::string name;
  std::string desc;
};

template<typename T>
void print_name_desc(std::vector<T> list, bool cut_at_word = true) {
  static char first_line_filler = '.';
  static char following_line_filler = ' ';
  /* These four strings must have the same size */
  static std::string first_line_sep = " ";  // separator for 1st line of block
  static std::string last_line_sep = " ";   // sep. for last line of block
  static std::string middle_line_sep = " "; // sep. for other lines
  static std::string one_line_sep = " ";    // sep. for the line if only one
                                            // line
  static size_t sep_width = 1;              // size of above four separators
                                            // (cannot be computed because of 
                                            //  UTF8 chars)
  static size_t min_number_of_filler = 2; // min number of filler char
  static size_t filler_sep = 1; // nb of spaces before filler

  size_t terminal_width = compute_term_width();
  
  /* Computing the max length of names */
  size_t max = 0;                  
  for (T& item : list)       
    if ( item.name.size() > max )
      max = item.name.size();
 
 /* A line is 
     __________col1___________       ____col2___
    /                         \     /           \
    <name><spaces><filler char><sep><description>
          \______/ 
     `filler_sep`  

    col1= max(length of names) + filler_sep + min_number_of_filler
    col2= terminal_width - sep_width - col1
 */
  size_t col1 = max+min_number_of_filler+filler_sep;
  int col2_tmp = terminal_width-col1-sep_width;

  if (col2_tmp < 12)   // If second column is less than 12 width, 
    col2_tmp = 100000; // we give up a nice display and every item 
                       // in one line
  size_t col2 = col2_tmp;

  std::cout << std::left <<  std::setw(col1) << "Name:";
  std::cout << std::string(sep_width, ' ');
  std::cout << std::left <<  std::setw(col2) << "Description:";
  std::cout << std::endl;
  std::cout << std::string(col1,'-') << std::string(sep_width, ' ') 
            << std::string(col2,'-');
  for (T& item : list) {
    size_t size = item.desc.size();
    std::cout << std::left << std::setfill(first_line_filler) 
              << std::setw(col1) << item.name+std::string(filler_sep,' ');
    size_t start = 0;
    std::string fill = "";
    while (start < size) {
      size_t len = col2;
      if (cut_at_word) {
        while ( len != 0 && (start+len) < size 
                && item.desc[start+len] != ' ') 
          len--;
        if (len == 0) /* word is too long for one line */
          len = col2;
      }
      bool is_first_line = (start == 0),
           is_last_line = (start+len >= size);
      std::string sep;
      if (is_first_line && is_last_line)
        sep = one_line_sep;
      else if (is_first_line)
        sep = first_line_sep;
      else if (is_last_line)
        sep = last_line_sep;
      else
        sep = middle_line_sep;
      std::cout << fill << sep << item.desc.substr(start,len) << std::endl;
      if (!fill.compare(""))
        fill = std::string(col1,following_line_filler);
      start+=len;
      while (start<size && item.desc[start] == ' ') 
        start++;
    }
  }
}

//// Printing the array of commands ////
// search in list functions
bool point_in_list(const std::string& s, std::vector<choice>& list,
                   std::vector<choice>::iterator& it)
{
  bool exists=false;
  it = list.begin();
  for (; it != list.end(); ++it)
    if (it->name == s) {
      exists=true;
      break;
    }
  return exists;
}

bool find_in_list(const std::string& s, std::vector<choice>& list)
{
  std::vector<choice>::iterator it;
  bool exists=false;
  it = list.begin();
  for (; it != list.end(); ++it)
    if (it->name == s) {
      exists=true;
      break;
    }
  return exists;
}

std::string pad_space(unsigned n)
{
  std::string output ("");
  for(unsigned i=0; i<n; ++i) output = output + " ";
  return output;
}


std::string padding(std::string margin, std::string s, unsigned n)
{
  std::string output;
  output = margin + s;

  for(unsigned i= output.length(); i<n; ++i) output = output + " ";
  return output;
}

std::string centering( std::string& s, unsigned n)
{
  std::string output;
  unsigned d;

  output = s;
  d = (n-output.length())/2;

  for(unsigned i= 0; i<d; ++i) output = " " + output + " ";

  if (output.length()<n) output = output + " ";

  return output;
}

std::string padd_print( std::vector<command>& v, unsigned j, unsigned n)
{
  std::string output;
  std::string margin = " ";

  if (j < v.size()) {
    output = v[j].name;
    if (find_in_list( output, is_choices)) output = output + "*";
    else {
      if (find_in_list( output, double_type_cmds)) output = output + "**";
    }
    output = margin + output;
  }
  else
    output = "";

  for(unsigned i= output.length(); i<n; ++i) output = output + " ";
  return output;
}

void line_print(std::string s, unsigned n)
{
  std::string u ="";

  for(unsigned i= 0; i<n; ++i) u = u + s;
  std::cout << "|" << u << "|" << u << "|" << u << "|" << std::endl;

  return;

}
//// functions for drawing a table
// horizontal table line for 3 columns of width n1, n2, and n3
void hor_line_print(unsigned n1, unsigned n2, unsigned n3,
                    std::string left, std::string middle, std::string right)
{
  std::string u1 ="", u2 ="", u3 ="";
  std::string s ="\u2500";

  for(unsigned i= 0; i<n1; ++i) u1 = u1 + s;
  for(unsigned i= 0; i<n2; ++i) u2 = u2 + s;
  for(unsigned i= 0; i<n3; ++i) u3 = u3 + s;
  std::cout << left << u1 << middle << u2 << middle
            << u3 << right << std::endl;
  return;
}

// top line for 3 columns of width n1, n2, and n3
void top_line_print(unsigned n1, unsigned n2, unsigned n3)
{
  std::string left ="\u250C", middle ="\u252C", right ="\u2510";
  hor_line_print( n1, n2, n3, left, middle, right) ;
  return ;
}

// top line for 3 columns of width n
void top_line_print( unsigned n)
{
  top_line_print( n, n, n) ;
  return;
}

// mid line for 3 columns of width n1, n2, and n3
void mid_line_print(unsigned n1, unsigned n2, unsigned n3)
{
  std::string left ="\u251C", middle ="\u253C", right ="\u2524";
  hor_line_print( n1, n2, n3, left, middle, right) ;
  return ;
}

// mid line for 3 columns of width n
void mid_line_print( unsigned n)
{
  mid_line_print( n, n, n) ;
  return;
}

// bottom line for 3 columns of width n1, n2, and n3
void bot_line_print(unsigned n1, unsigned n2, unsigned n3)
{
  std::string left ="\u2514", middle ="\u2534", right ="\u2518";
  hor_line_print( n1, n2, n3, left, middle, right) ;
  return ;
}

// bottom line for 3 columns of width n
void bot_line_print( unsigned n)
{
  bot_line_print( n, n, n) ;
  return;
}

void head_block_print(std::string& s1, std::string& s2, std::string& s3,
                      unsigned n)
{
  std::cout << "\u2502"  << centering( s1, n) << "\u2502"
            << centering( s2, n) << "\u2502"
            << centering( s3, n) << "\u2502" << std::endl;
  return;
}


void block_print(std::vector<command>& v1, std::vector<command>& v2,
                 std::vector<command>& v3,
                 unsigned n)
{
  std::string s ="\u2502";
  unsigned max;

  max= std::max(std::max(v1.size(), v2.size()), v3.size());

  for(unsigned j= 0; j<max; ++j) {
    std::cout << s  << padd_print( v1, j, n) << s << padd_print( v2, j, n) << s
              << padd_print( v3, j, n) << s << std::endl;
  }

  return;
}

void print_ttl_lst(std::string s, std::string u)
{
  std::string underline = u + u;

  for(unsigned i= 0; i<s.size(); ++i) underline = underline + u;

  std::cout << "\n" + s + " :" << std::endl;
  std::cout << underline << std::endl;
  return;
}

// void print_cmd_kind(std::vector<command> v, unsigned n) {
//   for(auto c : v) {
//     std::cout << "  " << c.name;
//     for(unsigned i= c.name.length(); i<n; ++i)
//       std::cout << " ";
//     std::cout << c.desc << std::endl;
//   }
//   std::cout << std::endl;
//   return;
// }
//
void print_cmd_kind(std::vector<command> v, unsigned n)
{
  bool dbl_tp=false;
  bool is_chc=false;
  std::string output;
  std::string margin = "  ";

  for(auto c : v) {
    output = c.name;
    if (find_in_list( output, is_choices)) {
      output = output + "*";
      is_chc=true;
    }
    else {
      if (find_in_list( output, double_type_cmds)) {
        output = output + "**";
        dbl_tp=true;
      }
    }

    output = margin + output;
    for(unsigned i= output.length(); i<n; ++i) output = output + " ";

    std::cout << output;
    std::cout << c.desc << std::endl;
    output = "";

  }
  if (is_chc) std::cout << doc::is_chc_str << std::endl;
  if (dbl_tp) std::cout << doc::dbl_tp_str << std::endl;
  return;
}

void print_doc_list( unsigned n)
{
  for (auto entry : awali::docstring::entries) {
    std::cout << " " << entry.name;
    for(unsigned i= entry.name.length(); i<n; ++i)
      std::cout << " ";
    std::cout << entry.desc << std::endl;
  }
  std::cout << std::endl;
  return;
}

void kind_print(std::vector<command> v, std::string s,
                std::string u, unsigned n)
{
  print_ttl_lst( s, u);
  print_cmd_kind( v, n);
  return;
}


void print_dir_cntnt_lst(std::map<std::string, dyn::loading::file_loc_t>& dcl,
                         unsigned n)
{
  for (auto const& p: dcl) {
    std::cout << p.first;
    size_t u = p.first.size();
    for(unsigned i=u; i<n; ++i)
      std::cout << ' ';
    std::cout << "| ";

    std::ifstream fic(p.second.dir+'/'+p.second.name+"."+p.second.ext);
    json_ast_t ast = json_ast::from(fic);
    try {
      std::cout << ast->at("metadata")->at("caption")->to_string();
    } catch (json::exception&) {}

    fic.close();
    std::cout << std::endl;
  }
  std::cout << std::endl;
  return;
}


void print_table()
{
  unsigned col_width=25;
  std::string s1, s2, s3, u;
  std::vector<command> v1, v2, v3;

  top_line_print( col_width);
  s1="Basic commands";
  s2="Generic commands";
  s3="Wfa commands";
  head_block_print( s1, s2, s3, col_width);
//   s1 = "'help basic'"; s2 = "'help generic'"; s3 = "'help wfa'";
//   head_block_print( s1, s2, s3, col_width);
  mid_line_print( col_width);
  v1 = commands_basic;
  v2 = commands_generic;
  v3 = commands_aut;
  block_print( v1, v2, v3, col_width);
  bot_line_print( col_width);

  top_line_print( col_width);
  s1="Rat-exp commands";
  s2="Nfa commands";
  s3="Transducer commands";
  head_block_print( s1, s2, s3, col_width);
//   s1 = "'help ratexp'"; s2 = "'help nfa'";s3 = "'help transducers'";
//   head_block_print( s1, s2, s3, col_width);
  mid_line_print( col_width);
  v1 = commands_exp;
  v2 = commands_nfa;
  v3 = commands_tdc;
  block_print( v1, v2, v3, col_width);
  bot_line_print( col_width);

  std::cout << doc::is_chc_str << std::endl;
  std::cout << doc::dbl_tp_str << std::endl;

  final_output=NONE;
} // End of print_table

void print_options()
{
// col_w1 + col_w2 + col_w3 + 4 = width of page
  unsigned col_w1=3, col_w2=13, col_w3=60;

  std::cout << doc::option_heading << std::endl;
  top_line_print( col_w1, col_w2, col_w3);
  std::cout << "\u2502" << padding(" ", "X", col_w1)
            << "\u2502" << padding(" ", "long-option", col_w2)
            << "\u2502" << padding(" ", "", col_w3) << "\u2502" << std::endl;
  for(auto c : options) {
    mid_line_print( col_w1, col_w2, col_w3);
    std::cout << "\u2502" << padding(" ", c.first, col_w1)
              << "\u2502" << padding(" ", c.second.desc, col_w2)
              << "\u2502" << padding(" ", c.second.longdesc, col_w3)
              << "\u2502" ; // no newline, waiting for the sequel

    if(c.second.optiondesc == "") {
      std::cout << std::endl ; // here is the newline
    }
    else {
      std::cout << c.second.optiondesc ; // begins and ends with a newline
    }
  }
  bot_line_print( col_w1, col_w2, col_w3);
  std::cout << std::endl;

  final_output=NONE;
} // End of print_table
//// Printing lists

/*---------------------------------------------------------------------------
     old function help()

void help() {
  unsigned col_width=21;

  std::string s, u = "-";
  std::vector<command> v;

  v = commands_basic;    s = "Basic commands";
  kind_print( v, s, u, col_width);

  v = commands_generic; s = "Generic commands for automata and transducers";
  kind_print( v, s, u, col_width);

  v = commands_aut;     s = "Commands for (weighted) automata";
  kind_print( v, s, u, col_width);

  v = commands_exp;     s = "Commands for rational expressions";
  kind_print( v, s, u, col_width);

  v = commands_nfa;     s = "Commands for NFA";
  kind_print( v, s, u, col_width);

  v = commands_tdc;     s = "Commands for transducers";
  kind_print( v, s, u, col_width);

  v = commands_fact;    s = "Factories";
  kind_print( v, s, u, col_width);

  std::cout << "\nOptions :  (-Xvalue or --long-option=value)" << std::endl;
  std::cout << "---------" << std::endl;
  for(auto c : options) {
    std::cout << "  -" << c.first;
    for(unsigned i= c.first.length(); i<3; ++i)
      std::cout << ' ';
    std::cout << " --";
    std::cout << c.second.desc;
    for(unsigned i= c.second.desc.length(); i<10; ++i)
      std::cout << ' ';
    std::cout << c.second.longdesc <<  std::endl;
  }
  std::cout << "\nMisc :" << std::endl;
  std::cout << "------" << std::endl;
  std::cout << "* To read the argument on standard input, use -" << std::endl;
  std::cout << "* You can pipe commands with \\| :\n   $> cora cmd1 arg1 \\| cmd2 -" << std::endl;
  std::cout << "  In this case, '-' refers to the result of the last command" << std::endl;
  std::cout << "  When an option is set, its value remains for the next commands on the line" << std::endl;
  std::cout << "* To load stored automata, do not write the suffix .json" << std::endl;
  std::cout << std::endl;
  final_output=NONE;
}
---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
|     new function help()         200929                                    |
---------------------------------------------------------------------------*/
void help()
{
  std::cout << doc::cora_help << std::endl;
  final_output=NONE;
}

// function help(<str>)
void help(std::string s)
{
  bool exists=false, is_list=true;

// variables for print_table() and kind_print()
  unsigned col_width=21;
  std::string title, str1, underline = "-";
  std::vector<command> v;

// search among help_classes
  auto it = help_classes.begin();
  for (; it != help_classes.end(); ++it) {
    if (it->name == s) {
      exists=true;
      break;
    }
  }
  if (exists) {  // string s has been found in help_classes

    if (it->choice_id == TABLE)
      print_table();
    else {
      if (it->choice_id == OPTIONS)
        print_options();
      else {
        switch(it->choice_id) {
        case BASIC : {
          v = commands_basic;
          title = "Basic commands";
          col_width=18;
          break;
        }
        case GENERIC : {
          v = commands_generic;
          title = "Generic commands for automata and transducers";
          break;
        }
        case WFA_CMDS : {
          v = commands_aut;
          title = "Commands for (weighted) automata";
          break;
        }
        case EXP_CMDS : {
          v = commands_exp;
          title = "Commands for rational expressions";
          break;
        }
        case NFA_CMDS : {
          v = commands_nfa;
          title = "Commands for Boolean automata";
          break;
        }
        case TRANSDUCERS : {
          v = commands_tdc;
          title = "Commands for transducers";
          break;
        }
        default : {
          is_list=false;
        }
        }
        if (is_list)
          kind_print( v, title, underline, col_width);
        else   // s is in help_classes for doc
          std::cout << it->longdesc << std::endl;

      }
    }
  } // end processing  s  in help_classes

  else { // search among commands
    auto it = commands.begin();
    for (; it != commands.end(); ++it) {
      if (it->name == s) { // string s has been found in commands
        exists=true;
        break;
      }
    }

    if(!exists) { //  string s has been found nowhere
      *dyn::error_stream  << error_clr << "\n  " << s
                          << " : unknown Awali command\n" << reset_clr
                          << std::endl;
    }
    else { // string s is a command; processing help <cmd>
// It consists in printing first a line for the usage,
// and then the 'longdesc' field of the command.
// The usage line is processed differently, according to <cmd>
      if ( s != "help" ) {
// First case  s = help directly calls printing of longdesc, no usage line
        if (find_in_list( s, double_type_cmds)) {
// Second case  s is a command that accepts both automata and expressions
          if (it->optiondesc.size()>10) str1="\n or" + pad_space(17);
          else str1="  ";
          std::cout << "\nCommand "
                    << usage_clr << it->name << reset_clr
                    << " accepts as argument an automaton as well as an expression."
                    <<  std::endl;
          if (it->argc == 1) {
            std::cout << "\n Usage : "
                      << usage_clr << it->name << " <aut> " << reset_clr << " or "
                      << usage_clr << it->name << " <exp> " << reset_clr
                      << "\n" << std::endl;
          }
          else {
            std::cout << "\n Usage : " << usage_clr << it->name << " "
                      << it->args[0] << " <aut> " << reset_clr << " or "
                      << usage_clr << it->name << " "
                      << it->args[0] << " <exp> " << reset_clr
                      << "\n" << std::endl; //
          }
//   The usage line is doubled in presence of options
          if ( it->optiondesc != "") {
            std::cout << " or with option(s): "
                      << usage_clr << it->optiondesc << " "
                      << it->name << " <aut>" << reset_clr << str1
                      << usage_clr << it->optiondesc << " "
                      << it->name << " <exp>" << reset_clr
                      << "\n" <<  std::endl; //
          }
        }
// Third case  s is 'normal' ie accepts either an <aut> OR an <exp>
        else {
          std::cout << "\nUsage : " << usage_clr << it->name << " "
                    << it->args << reset_clr << "\n" << std::endl;
//   The usage line is doubled in presence of options
          if ( it->optiondesc != "") {
            std::cout << " or with option(s) : " << usage_clr
                      << it->optiondesc << " " << it->name << " "
                      << it->args << reset_clr << "\n" << std::endl;
          }
        }
      }
//   Printing of the 'longdesc' field
      std::cout << it->longdesc << std::endl; // "\n" <<

      if ( s == "doc" ) {// Case  s = doc different from others
        col_width=18;
        print_doc_list( col_width);
      }
    } // end processing  s is a command
  } // end processing  s is not a help_class

  final_output=NONE;
} // end of fct help(<str>)


void list(std::string s)
{
  bool exists=false;
  unsigned col_width=15;
  std::string title, str1, underline = "=";
  std::string output, margin = " ";
  std::vector<std::string> wsn, wsd;


// search among list_choices
  auto it = list_choices.begin();
  for (; it != list_choices.end(); ++it) {
    if (it->name == s) {
      exists=true;
      break;
    }
  }
  if (exists) {  // string s has been found in list_choices
    switch(it->choice_id) {

    case AUTOMATA_LST : {
      title = "Automata library";
      print_ttl_lst( title, underline);

      std::map<std::string, dyn::loading::file_loc_t> dcl =
          dyn::loading::example_automata();
      std::vector<name_desc_pair_t> v;
      for (auto p : dcl) {
        std::string caption = "";
        std::string name = p.second.name;
        std::ifstream fic(p.second.dir+'/'+p.second.name+"."+p.second.ext);
        json_ast_t ast = json_ast::from(fic);
        if(ast->has_path({"metadata","caption"})) {
            caption = ast->at("metadata")->at("caption")->to_string();
        }
        fic.close();
        v.push_back({name,caption});
      }
      print_name_desc(v);

      break;
    }
    case EXPRESSIONS_LST : {
      title = "Rational expression library";
      print_ttl_lst( title, underline);

      std::map<std::string, dyn::loading::file_loc_t> dcl =
          dyn::loading::example_ratexps();

      print_dir_cntnt_lst( dcl, col_width);
      break;
    }
    case FACTORIES_LST : {
      title = "Factories library";
      print_ttl_lst( title, underline);
      
      print_name_desc(commands_fact);

//       print_cmd_kind( commands_fact, col_width);
      break;
    }
    case WEIGHTSET_LST : {
      title = "Implemented weightset list";
      print_ttl_lst( title, underline);

      wsn = dyn::context::all_weightset_public_static_names();
      wsd = dyn::context::all_weightset_public_descriptions();

      std::vector<name_desc_pair_t> v;
      for (size_t i=0; i<wsn.size(); i++) {
        v.push_back({wsn[i],wsd[i]});
      }
      print_name_desc(v);
      break;
    }
    default :
    {} // impossible since switch is guarded by 'exists' flag
    }
  }

  else {
    *dyn::error_stream << error_clr << " Error : '" << s
                       << "'  are not objects that can be listed"
                       << reset_clr << "\n" << std::endl;
    return;
  }

  final_output=NONE;
} // end of fct list(<str>)

//// End of printing lists ////

void docu(std::string s)
{
  for (auto entry : awali::docstring::entries) {
    if (s == entry.name) {
      // We could display the entry title, contained in `entry.title`
      std::cout << "\n" << title_clr << entry.title  << reset_clr << std::endl;
      std::cout << entry.content << std::endl;
      return;
    }
  }
  *dyn::error_stream << error_clr
                     << "No documentation entry available for \"" << s
                     << "\"." << reset_clr << std::endl;
  *dyn::error_stream << "\n  Available entries are listed with the command\n\n"
                     << usage_clr << "     cora list doc" << reset_clr << "\n" << std::endl;
  return;
}



//// Printing info ////
#include <cora/print_info.cc> // file without print_info


// // End of file --- return to cora.cc
