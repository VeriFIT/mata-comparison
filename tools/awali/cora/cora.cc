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

#include <cora/cora.hh>
#include <cora/edit/editter.hh>
#include <cora/print_out/color.hh>
#include <awali/common/ato.hh>

#include<dirent.h>
#include<fstream>
#include<sstream>
#include<map>
#include<stdexcept>

namespace awali {
namespace cora {

class null_buffer_t : public std::streambuf {
 public:
  int overflow(int c) override {
    return c;
  }
};
class null_stream_t : public std::ostream {
 public:
  null_stream_t() : std::ostream(&m_sb) {}
 private:
  null_buffer_t m_sb;
};

null_stream_t null_stream;

dyn::automaton_t arg1;
dyn::automaton_t arg2;
dyn::automaton_t res;
dyn::ratexp_t exp1;
dyn::ratexp_t exp_res;
dyn::weight_t weight("");
int count;
bool boolean;
bool history;
dyn::context_t ctx;
coms_t current_command;


bool no_user_file;
bool first_cmd = true; //to know whether - is the real standard input
                       //used in load functions and in the 'main' loop

arg_kind_t final_output;

/* ---------------------------------------|
|  Search in list functions               |
|----------------------------------------*/

/* tells whether <s> is a field <name> of an element of <list> */
template<typename T>
bool found_in_list(const std::string& s, std::vector<T>& list) 
{
  bool exists=false;
  for (T& item : list) {
    if (item.name == s) { 
      exists=true;
      break;
    }
  }
  return exists;
}
  
/* tells whether <s> is a field <name> of an element of <list> 
   AND returns the iterator that points to that element.       */
template<typename T>
bool found_in_list(const std::string& s, std::vector<T>& list,
                   typename std::vector<T>::iterator& it )
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

/* tells whether <s> is a field <name> of an element of <list>
   AND returns the iterator that points to that element.       
   Special case of option list, made look like the others.  
   Refers to the 'options' list.                              */
// template<typename T>
bool found_in_option_list(const std::string& s, //std::vector<T>& list,
                          std::map<std::string, option>::iterator& it)
{
  bool exists=false;
  it = options.begin();
  for (; it != options.end(); ++it) {
    if (it->second.name == s) {
      exists=true;
      break;
    }
  }
  return exists;
}

/* ---------------------------------------|
|  Print functions                        |
|  print out lists, helps, docs or info   |
|----------------------------------------*/
#include <cora/print_out/print_out.cc> 

// function output takes care of the result of the function called by cora
void output()
{
  switch(final_output) {
  case AUT:
  case TDC: {
    dyn::options_t opts = { dyn::KEEP_HISTORY = history,
                            dyn::IO_FORMAT = output_format
                          };
    if (name != dflt_name) { // bloc for debugging
      res->set_name(name);
    }
    if (caption != dflt_caption) { // bloc for debugging
      res->set_desc(caption);
    }
	std::string nm = res->get_name();
    dyn::put(res, std::cout, opts) << std::endl;
    return;
  }
  case EXP:
    if(output_format == "json")
      dyn::operator<<(std::cout,::awali::JSON)<< exp_res << std::endl;
    else
      if (output_format == "text"
          || output_format == "default")
        exp_res->print(std::cout) << std::endl;
      else
        std::cerr << output_format
                  << " : unknown output format for ratexp."
                  << std::endl;
    return;
  case BOOL:
    if(verbose)
      std::cout << std::boolalpha << boolean << std::endl;
    if (shell)
      exit(!boolean);
    return;
  case INT:
    if (verbose)
      std::cout << count << std::endl;
    if (shell)
      exit(count);
    return;
  case WEIGHT:
    try {
      boolean = (bool) weight;
      final_output= BOOL;
      return output();
    }
    catch (dyn::any_cast_exception const&) {}
    try {
      count = (int) weight;
      final_output= INT;
      return output();
    }
    catch (dyn::any_cast_exception const&) {}
    try {
      count = (unsigned int) weight;
      final_output= INT;
      return output();
    }
    catch (dyn::any_cast_exception const&) {}
    std::cout << weight << std::endl;
    return;
  case WORD: //No such command right now
    return;
  case CMD: //No such command right now
    return;
  case SMR: //No such command right now
    return;
  case LBL: //No such command right now
    return;
  case ALPH: //No such command right now
    return;
  case FMT: //No such command right now
    return;
  case MTD: //No such command right now
    return;
  case STR: //No such command right now
    return;
  case CHC:
    return;
  case AUT_OR_EXP: // Fixme: NOT USED BUT SHOULD BE
    return;
  case NONE:
    return;
  }

  return;
} // end of output


/* ---------------------------------------|
|  Load functions.                        |
|----------------------------------------*/

// load_exp    load expression
dyn::ratexp_t load_exp(std::string s, bool warn_if_word = true)
{
  static const std::vector<std::string> formats
    = {"json", "default", "text", "exp"};
  {
    bool b = false;
    for (std::string const& f : formats)
      if (f == input_format) {
        b = true;
        break;
      }
    if (!b)
      throw std::runtime_error(std::string("Format ")+input_format
                               +" is not acceptable for expressions.");
  }
  std::string ifmt = input_format;
  if(s=="-") {
    if(first_cmd) {
      if(ifmt=="json")
        return dyn::parse_ratexp(std::cin);
      else {
        ifmt = "exp";
        std::cin >> s;
      }
    }
    else
      if(final_output!=EXP)
        throw std::runtime_error(
            "Rational expression expected on pseudo standard input");
      else
        return exp_res;
  }

  if (ifmt == "text") {
    std::ifstream i(s);
    if (i.good()) {
      i >> s;
      ifmt = "exp";
    }
    else
      throw no_such_file_exception(error_clr+"Could not find file: "+s+reset_clr);
  }

  if ((ifmt == "default") || (ifmt == "json")) {
    try {
      return dyn::load_exp(s);
    }
    catch (no_such_file_exception& e) {
      if (ifmt == "json")
        throw e;
    }
  }

  dyn::ratexp_t res;
  if (ifmt == "exp" || ifmt == "default") {
    //If proper_exp is false, we will check later on that the exp
    //is not too simple, and issue a warning if it is.
    bool proper_exp = !(input_format == "default");
    if(letter_type=="tdc") {
      proper_exp = true; //
      if (alphabet_was_set)
        res = dyn::ratexp_t::
              with_tuple_labels::from(s, {alphabet, alphabetb},
                                      semiring);
      else
        res = dyn::ratexp_t::with_tuple_labels::from(s, 2, semiring);
    }
    else {
      if(letter_type=="char") {
        if (alphabet_was_set)
          res = dyn::ratexp_t::from(s, semiring, alphabet);
        else
          res = dyn::ratexp_t::from(s, semiring, "auto");
      }
      else {
        if (alphabet_was_set) {
          int n=stoi(alphabet);
          res = dyn::ratexp_t::with_int_labels::from_size(s, n, semiring);
        }
        else
          res = dyn::ratexp_t::with_int_labels::from(s, semiring);
      }
    }


    if (warn_if_word) {
      // We look for sums, stars or weights in s to possibly display
      // a warning if the expression is too simple.
      std::list<dyn::ratexp_t> list;
      list.push_back(res);
      while (!proper_exp && !list.empty()) {
        dyn::ratexp_t e = list.back();
        list.pop_back();
        if ( e->get_kind() == dyn::ExpKind::ATOM
             || e->get_kind() == dyn::ExpKind::PROD ) {
          std::vector<dyn::ratexp_t> const& children = e->children();
          for (const dyn::ratexp_t& c: children)
            list.push_back(c);
        }
        else
          proper_exp = true;
      }
      if (!proper_exp) {
        std::string warning_msg = "No file '" + s + "' was found; hence '" + s
                                + "' was interpreted as a ratexp but "
                                + "the produced expression seems a bit simple.\n"
                                + "Use option --input/-I to disambiguate." ;
        warning_vw_print(warning_msg);
      }
//       *dyn::warning_stream << warning_clr
//                            << "[Warning] No file \"" << s << "\" was found; hence \""
//                            << s << "\" was interpreted as a rational \n"
//                            << "expression but the produced expression seems a bit simple.\n"
//                            << "Use option --input/-I to disambiguate. "
//                            << reset_clr << std::endl;
    }
  }
  return res;

} // end load_exp


dyn::automaton_t normalize_automaton_context(dyn::automaton_t a) 
{
  static bool law_warning = true;     // These two variables are `static` to
  static bool lat_law_warning = true; // display the warning only once.

  auto ls_desc = a->get_context()->description()->labelset();

  if (ls_desc->is_lat()) { // vvv lat vvv
    /* Exit with error if more than one tape. */
    if (ls_desc->tape_number() != 2) {
      std::stringstream ss;
      ss << "Cora only supports transducers with two tapes.  "
         << "This automaton has " << ls_desc->tape_number() << ".";
      throw std::runtime_error(ss.str());
    }
    /* Display warning if at least one tape is law. */
    for (auto child_ls_desc : ls_desc->children_) {
      if (child_ls_desc->is_law()) {
        if (    lat_law_warning
             && current_command != CAT
             && current_command != SUBNORMALIZE
             && current_command != DISPLAY)
        {
          warning_vw_print(
            "You are manipulating a transducer in which one of the tape uses"
            " words as labels.  "
            "In Cora, the support for such transducers is currently limited,"
            " and might change in the future.  "
            "Use command subnormalize to convert your transducer into a better"
            " supported type of transducers."
          ); 
          lat_law_warning = false;
        }
        return a;
      }
    }
    /* Otherwise, normalize the transducer to lat<lan,lan> */
    for (auto child_ls_desc : ls_desc->children_) {
      if (child_ls_desc->is_lal()) // If at least one children is lal
                                   // we change the context
        return dyn::internal::make_nullable_under_lat(a);
    }
  }                             // ^^^ lat ^^^
  else if (ls_desc->is_law()) { // vvv law vvv
    if (    law_warning
         && current_command != DISPLAY 
         && current_command != LETTERIZE  
         && current_command != CAT )
    {
      warning_vw_print(
        "You are manipulating an automaton that uses words as labels.  "
        "In Cora, the support for such automata is currently limited,"
        " and might change in the future.  "
        "Use command letterize to convert your automaton into a better"
        " supported type of automata."
      ); 
      law_warning = false;
    }
  }                             // ^^^ law ^^^
  else if (ls_desc->is_lan()) { // vvv lan vvv
    bool found_eps_transition = false;
    for(auto t : a->transitions()) {
      if (a->label_of(t) == a->get_context()->epsilon()) {
        found_eps_transition = true;
        break;
      }
    }
    if (!found_eps_transition)
      return dyn::proper(a, {dyn::IN_PLACE=false});
  } // ^^^ lan ^^^
  return a;
}

dyn::internal::aut_or_exp_t normalize_aoe_context(
      dyn::internal::aut_or_exp_t aoe) 
{
  if (aoe.is_aut) {
    return normalize_automaton_context(aoe.aut);
  }
  else {
    return aoe;
  }
}

// load    load automaton
dyn::automaton_t load(std::string s)
{
  if(s=="-") {
    no_user_file = true;
    if(first_cmd)
      return normalize_automaton_context(dyn::parse_automaton(std::cin,{dyn::IO_FORMAT = input_format}));
    if(final_output!=AUT)
      throw std::runtime_error("Automaton expected on pseudo standard input");
    return normalize_automaton_context(res);
  }
  return normalize_automaton_context(dyn::internal::load(s,
                             no_user_file,
                             dyn::IO_FORMAT.of_string(input_format)));
} // end load  (automaton)

// load    load automaton OR expression
aut_or_exp_t load_aut_or_exp(std::string s, bool warn_if_word)
{
  if (s == "-" && !first_cmd) {
    if (final_output == EXP)
      return load_exp(s);
    else
      if (final_output == AUT)
        return load(s);
      else
        throw std::runtime_error("Expecting an automaton or an expression on the standard input.");
  }

  if (input_format == "exp" || input_format == "text")
    return load_exp(s);
  if (input_format != "json" && input_format != "default")
    return load(s);

  //We don't know
  try {
    // Calling the dyn function
    if (s == "-") { // first_cmd is necessarily true
      dyn::internal::aut_or_exp_t result 
        = dyn::internal::parse_aut_or_exp(json_ast::from(std::cin));
      return normalize_aoe_context(result);
    }
    else
      return normalize_aoe_context(dyn::internal::load_aut_or_exp(s, true));
  }
  catch (no_such_file_exception const& e) {
    if (input_format == "default")
      return load_exp(s, warn_if_word);
    throw e;
  }
} // end load  automaton OR expression

// process_options
void process_option(const char* opt)
{
  opts_t optcst;
  const char* arg=nullptr;
  if(opt[1]!='-') {
    auto it = options.find(std::string(1, opt[1]));
    if(it == options.end())
      throw std::runtime_error(std::string("Unknown option : -")+(opt+1));
    optcst=it->second.cst;
    arg=opt+2;
    if(it->second.argc==0 && *arg!='\0')
      throw std::runtime_error(std::string("Unexpected argument with option : ")+
                               (opt[1]));
    if(it->second.argc==1 && *arg=='\0')
      throw std::runtime_error(std::string("Missing argument with option : ")+
                               (opt[1]));
  }
  else {
    std::string sopt(opt);
    for(auto s : options) {
      std::string d=s.second.name;
      unsigned l = d.length();
      if(sopt.compare(2, l, d)==0) {
        if(opt[l+2]!='\0' && opt[l+2]!='=')
          throw std::runtime_error(std::string("Unknown option : --")+(opt+2));
        if(s.second.argc==0 && opt[l+2]=='=')
          throw std::runtime_error(std::string("Unexpected argument with option : ")+
                                   (opt+2));
        if(s.second.argc==1 && opt[l+2]=='\0')
          throw std::runtime_error(std::string("Missing argument with option : ")+
                                   (opt+2));
        optcst=s.second.cst;
        arg=opt+3+l;
        break;
      }
    }
    if(arg == nullptr)
      throw std::runtime_error(std::string("Unknown option : --")+(opt+2));
  }
  switch(optcst) {
  case INPUT_FMT:
    input_format=arg;
    break;
  case OUTPUT_FMT:
    output_format=arg;
    break;
  case ALPHABET:
    alphabet_was_set=true;
    alphabet=arg;
    break;
  case ALPHABET_B:
    alphabetb=arg;
    break;
  case METHOD:
    algo=arg;
    break;
  case LABELS:
    letter_type=arg;
    if(letter_type=="int")
      alphabet="2";
    break;
  case WEIGHTS:
    semiring=arg;
    break;
  case SHELL: {
    verbose= false;
    shell= true;
    dyn::warning_stream = &null_stream;
    break;
  }
  case VERBOSE:
    verbose=true;
    break;
  case HISTORY:
    history=true;
    break;
  case CAPTION:
    caption=arg;
    break;
  case NAME:
    name=arg;
    break;
//   case TITLE:
//     title=arg;
//     break;
  default:
    break;
  }
}

//// main  inside namespace awali::cora
int main(int argc, const char** argv)
{

// initialisation of command lists and other things
  init_cmds(); // in cora.hh
  init_lists(); // in cora_lists.hh
  init_options(); // in cora_options.hh
// reading the arguments
// call of cora without argument: error message
  if (argc==1) {
    std::string error_msg1 = "Please enter an Awali command.";
    std::string error_msg2 = "Type 'cora help' if necessary.";
    error_print(error_msg1, error_msg2);
    return 1;
  }

// call of cora with arguments: reading the arguments
  int i=1;
  do {  // large do loop  that  covers almost all the main function
    // every run of the loop processes a chunk of the command line
    // between two \|
    // reinitialisation of option values at the beginning of a chunk
    // but for the option I, O, S, H
    letter_type="char";
    alphabet="ab";
    alphabet_was_set = false;
    alphabetb="ab";
    semiring="B";
    algo="default";
    name=dflt_name;
    caption=dflt_caption;
    allow_eps_transition = false;
    // end of reinitialisation of options

    unsigned j=0;
    // reading until the next  |
    for(; i<argc && argv[i][0]!='|' ; ++i) {
      if(argv[i][0]=='-' && argv[i][1]!='\0'
          && (argv[i][1]<'0' || argv[i][1]>'9'))
        process_option(argv[i]);
      else
        args[j++]=argv[i];
    }
    if(i<argc)
      ++i; // preparation for the next run inside the do loop, if any.
    if (j == 0) { // no command argument given to cora; should not happen
                  // as this case has been caught earlier
      std::string error_msg1 = "Please enter an Awali command.";
      std::string error_msg2 = "Type 'cora help' if necessary.";
      error_print(error_msg1, error_msg2);
      return 1;
    }

    // here  args[0] should contain the name of the command
    bool exists = found_in_list(args[0], commands, it); // !! returns it !!
	
    if (!exists) { // +
      std::string error_msg1 = args[0];
      error_msg1 = "'" + error_msg1 +"' unknown Awali command.";
      std::string error_msg2 = "Type 'cora list commands' if necessary.";
      error_print(error_msg1, error_msg2);
      return 1;
    }

// case of HELP command which may have 0 or 1 argument
    if (it->cst == HELP) {
      if (j==1)
        help();
      else {
        if (j!=2) {
          std::string error_msg = "Usage : help [cmd]";
          error_print(error_msg);
          return 1;
        }
        help(args[1]);
      }
      return 0;
    } // end of  help

// Check arg card  +  Hack by VM to allow forgetting the "-"
//                    seems to introduce a bug for argc=0 (case of list and hilp)
    if(j!=it->argc+1) {
      bool print_arg_error = false;
      if (it->argc > 0) {
        arg_kind_t arg_kind = it->args.back().kind;
        if ((!first_cmd) && (it->argc == 1) && (j == it->argc)
            && (   arg_kind == AUT_OR_EXP || arg_kind == EXP || arg_kind == AUT 
                || arg_kind == TDC))
          args[j++]="-";
        else 
          print_arg_error = true;
      }
      else  
        print_arg_error = true;

      if (print_arg_error) {
        // printing it->args requires detour by stringstream
        std::stringstream ss_msg;
        ss_msg << "Usage : " << it->name << ' ' << it->args;
        std::string error_msg1 = ss_msg.str();
        std::string error_msg2 = "Type 'cora help " + it->name 
                               + "' for more information.";
        error_print(error_msg1, error_msg2);
        return 1;
      }
    }

    try { // try-catch to recover from an error in the main switch 
          // with a meaningful message

///////////// Main switch according to the command given to cora ////////////

    current_command = it->cst;
      switch(it->cst) {
		
/* -----------------------------------------|
|  Calls to command modules                 |
|------------------------------------------*/

//// Basic commands          ////
/* ---------------------------------------|
|  Basic commands                         |
|----------------------------------------*/
// LIST
      case LIST :
        list(args[1]);
        break;
// DOC
      case DOC : {
        doc_(args[1]);
        break;
      }
      
// CAT
      case CAT : {
       aut_or_exp_t aoe = load_aut_or_exp(args[1]);
        if (aoe.split_case(arg1, exp1)) {
          res = arg1;
          final_output = AUT;
        }
        else {
          exp_res = exp1;
          final_output = EXP;
        }
        break;
      } // function cat is indeed processed by function 'put' (for automata)
        // or by methods '.json' or '.print' (for ratexps) called by output.

// DISPLAY
      case DISPLAY : {
        aut_or_exp_t aoe = load_aut_or_exp(args[1]);
        if (aoe.split_case(arg1, exp1)) {
          if (name != dflt_name)
            arg1->set_name(name);
          dyn::pdfdisplay(arg1, {dyn::KEEP_HISTORY=history});
        }
        else {
          dyn::automaton_t tmp = dyn::draw_exp(exp1);
          if (name != dflt_name)
            tmp->set_name(name);
          dyn::pdfdisplay(tmp, { dyn::KEEP_HISTORY=true,
                                 dyn::LAYOUT=VERTICAL    });
        }
        final_output=NONE;
        break;
      }

// INFO
      case INFO : {
        aut_or_exp_t aoe = load_aut_or_exp(args[1]);
        if (aoe.split_case(arg1, exp1))
          info(arg1);
        else
          info(exp1);
        final_output=NONE;
        break;
      }

// STATS
      case STATS : {
        std::string chc=args[1]; 

        if ( !found_in_list(chc, stats_choices, it_chc) ) {// !!returns it_chc!!
          std::string error_msg1 = "'" + chc 
                                 + "'  is not a parameter that can be counted.";
          std::string error_msg2 = "Type 'cora help statistics' if necessary.";
          error_print(error_msg1, error_msg2);
          return 1;
        }
        else {
          aut_or_exp_t aoe = load_aut_or_exp(args[2]);

          if (aoe.split_case(arg1, exp1)) {
            switch(it_chc->choice_id) {
            case STATES_STATS :
              count = arg1->num_states();
              break;
            case INITIALS_STATS :
              count = arg1->num_initials();
              break;
            case FINALS_STATS :
              count = arg1->num_finals();
              break;
            case TRANSITIONS_STATS :
              count = arg1->num_transitions();
              break;
            case ACC_STATS :
              count = dyn::num_accessible_states(arg1);;
              break;
            case COACC_STATS :
              count = dyn::num_coaccessible_states(arg1);
              break;
            case SC_COMPONENTS_STATS :
              count =
                  dyn::strongly_connected_components(arg1).partition.size();
              break;
            default: {
              std::string error_msg1 = "'" + chc + "'  is not a parameter that "
                                     + "can be counted for automata.";
              std::string error_msg2 = "Type 'cora help statistics' if necessary.";
              error_print(error_msg1, error_msg2);
              return 1;
            }
            } // end of switch for automata
          }

          else {
            switch(it_chc->choice_id) {
            case SIZE_STATS :
              count = exp1->size();
              break;
            case LENGTH_STATS :
              count = exp1->length();
              break;
            case DEPTH_STATS :
              count = exp1->height();
              break;
            case STAR_HEIGHT_STATS :
              count = exp1->star_height();
              break;
            default: {
              std::string error_msg1 = "'" + chc + "'  is not a parameter that "
                                     + "can be counted for ratexps.";
              std::string error_msg2 = "Type 'cora help statistics' if necessary.";
              error_print(error_msg1, error_msg2);
              return 1;
            }
            } // end of switch for expressions
          }

          final_output=INT;
          break;
        }
      }

 // EDIT
      case EDIT :{
        arg1 =load(args[1]);
        if (no_user_file) {
          editter_t editter(arg1, dyn::IO_FORMAT.of_string(output_format));
          editter.run();
        }
        else { 
          editter_t editter(arg1, dyn::IO_FORMAT.of_string(output_format),  
                            args[1]);
          editter.run();
        }
        final_output=NONE;
        break;
      }
// NEW
      case NEW :{
        if(letter_type=="char")
          res=dyn::automaton_t::from(alphabet, semiring);
        else {
          if(letter_type=="int") {
            int n= std::stoi(alphabet);
            res=dyn::automaton_t::with_int_labels::from_size(n, semiring);
          }
          else {
            if(letter_type=="tdc") {
              res=dyn::transducer_t::from({alphabet, alphabetb}, semiring);
            }
            else
              throw std::runtime_error("Unknown letter_type: "+letter_type);
          }
        }
        if(output_format != "json") {
          if (name != dflt_name)
            res->set_name(name);
          if (caption != dflt_caption)
            res->set_desc(caption);
          editter_t editter(res, dyn::IO_FORMAT.of_string(output_format));
          editter.run();
          final_output=NONE;
        } 
        else
          final_output=AUT;
        break;
      }

// is   General test command
      case IS : {
        std::string chc=args[1];
        bool find = found_in_list( chc, is_choices, it_chc);

        if (!find) {
          std::string error_msg1 = "'" + chc + "'  is not a property that "
                                 + "can be tested.";
          std::string error_msg2 = "Type 'cora help is' if necessary.";
          error_print(error_msg1, error_msg2);
          return 1;
        }

        else {
          if ( chc == "valid" ) {// valid is the only test for both automata 
                                 // and ratexps
            aut_or_exp_t aoe = load_aut_or_exp(args[2]);
            if (aoe.is_aut) {
//               if (aoe.aut->get_context()->weightset_name() == "B") {
//                 std::string warning_msg = "a Boolean automaton is always valid.";
//                 warning_vw_print(warning_msg);
//                 boolean = true;
//               }
//               else
              boolean = dyn::is_valid(aoe.aut);          
            } 
		    else {
//               if (aoe.exp->get_context()->weightset_name() == "B") {
//                 std::string warning_msg = "a Boolean ratexp is always valid.";
//                 warning_vw_print(warning_msg);
//                 boolean = true;
//               }
//               else
		        boolean = dyn::is_valid(aoe.exp);
		    }
          } 
          else {// other tests, for automata only		  
            arg1=load(args[2]);
            switch(it_chc->choice_id) {
            case MPTY_CHC :
              boolean = dyn::is_empty(arg1);
              break;
            case ACC_CHC :
              boolean = dyn::is_accessible(arg1);
              break;
            case COACC_CHC :
              boolean = dyn::is_coaccessible(arg1);
              break;
            case TRIM_CHC :
              boolean = dyn::is_trim(arg1);
              break;
            case USELESS_CHC :
              boolean = dyn::is_useless(arg1);
              break;
            case STANDARD_CHC :
              boolean = dyn::is_standard(arg1);
              break;
            case COMPLETE_CHC :
              boolean = dyn::is_complete(arg1);
              break;
            case PROPER_CHC :
              if (!arg1->is_eps_allowed()) {
                std::string warning_msg = args[2];
                warning_msg = "'" + warning_msg 
                    + "' is already an automaton where epsilon-transitions " 
                    + "are not allowed, hence is proper.";
                warning_vw_print(warning_msg);
                boolean = true;
              }
              else
                boolean = dyn::is_proper(arg1);
              break;
            case SC_CHC :
              boolean = 
                (1 == dyn::strongly_connected_components(arg1).partition.size());
              break;
            case DET_CHC :
              boolean = dyn::is_deterministic(arg1);
              break;
            case AMB_CHC :
              boolean = dyn::is_ambiguous(arg1);
              break;
            default:
              {}
            }
          }
        }

        final_output=BOOL;
        break;
      }


//// generic commands        ////
/* ---------------------------------------|
|  Generic commands                       |
|----------------------------------------*/
//// Graph traversal functions
      case ACC :
        arg1=load(args[1]);
        dyn::accessible(arg1, {dyn::IN_PLACE=true});
        res=arg1;
        final_output=AUT;
        break;
      case COACC :
        arg1=load(args[1]);
        dyn::coaccessible(arg1, {dyn::IN_PLACE=true});
        res=arg1;
        final_output=AUT;
        break;
      case TRIM :
        arg1=load(args[1]);
        dyn::trim(arg1, {dyn::IN_PLACE=true});
        res=arg1;
        final_output=AUT;
        break;
      case IS_EMPTY :
        arg1=load(args[1]);
        boolean = dyn::is_empty(arg1);
        final_output=BOOL;
        break;
      case IS_ACC :
        arg1=load(args[1]);
        boolean = dyn::is_accessible(arg1);
        final_output=BOOL;
        break;
      case IS_COACC :
        arg1=load(args[1]);
        boolean = dyn::is_coaccessible(arg1);
        final_output=BOOL;
        break;
      case IS_TRIM :
        arg1=load(args[1]);
        boolean = dyn::is_trim(arg1);
        final_output=BOOL;
        break;
      case IS_USELESS :
        arg1=load(args[1]);
        boolean = dyn::is_useless(arg1);
        final_output=BOOL;
        break;

      // standard
      case STANDARD:
        arg1 =load(args[1]);

        if (dyn::is_standard(arg1)) {
          std::string arg_msg = args[1];
          std::string warning_msg = "'" + arg_msg 
                      + "'  is already standard. Result is '" 
                      + arg_msg + "' unchanged.";
          warning_vw_print(warning_msg);
        }
        else
          dyn::standard(arg1, {dyn::IN_PLACE=true});

        res=arg1;
        final_output=AUT;
        break;
      case IS_STANDARD:
        arg1 =load(args[1]);
        boolean = dyn::is_standard(arg1);
        final_output=BOOL;
        break;

     // are-isomorphic
      case ARE_ISOMORPHIC:
        arg1 =load(args[1]);
        arg2 =load(args[2]);

        if (arg1->get_context()->weightset_name() !=
            arg2->get_context()->weightset_name() ||
            arg1->get_context()->labelset_name() != 
              arg2->get_context()->labelset_name()) 
        {
          std::string error_msg1 = args[1];
          std::string error_msg2 = args[2];
          error_msg1 = "'" + error_msg1 + "'  and  '" +  error_msg2 
                     + "'  have not the same static context.";
          error_msg2 = "Type 'cora help are-isomorphic' if necessary.";
          error_print(error_msg1, error_msg2);
          return 1;
        }

        else {
          boolean = dyn::are_isomorphic(arg1, arg2);
          final_output=BOOL;
          break;
        }

//// Miscellaneous
      // strip
      case STRIP :
        arg1=load(args[1]);
        arg1->strip_history();
        res=arg1;
        final_output=AUT;
        break;

//// Transformations of the automaton
      // transpose
      case TRANSPOSE:
        arg1 =load(args[1]);
        res= dyn::transpose(arg1);
        final_output=AUT;
        break;
      //// Quotient
      // min-quotient
      case MINQUOTIENT:
        arg1 =load(args[1]);
        res = dyn::min_quotient(arg1, {dyn::QUOTIENT_ALGO=algo});
        final_output=AUT;
        break;
      case MINCOQUOTIENT:
        arg1 =load(args[1]);
        res = dyn::min_coquotient(arg1, {dyn::QUOTIENT_ALGO=algo});
        final_output=AUT;
        break;			
      //// Strongly connected components
      // condensation
      case CONDENSATION :
        arg1=load(args[1]);
        res = dyn::condensation(arg1);
        final_output= AUT;
        break;
      // is-strongly-connected
      case IS_SC :
        arg1=load(args[1]);
        boolean = 
          (1 == dyn::strongly_connected_components(arg1).partition.size());
        final_output= BOOL;
        break;

//// Dealing with the weightset
      // support
      case SUPPORT : {
        aut_or_exp_t aoe = load_aut_or_exp(args[1]);
        if (aoe.split_case(arg1, exp1)) {// // case of automata
          if (arg1->get_context()->weightset_name() == "B") {
            std::string arg_msg = args[1];
            std::string warning_msg = "'" + arg_msg + "' is already a Boolean "
                         + "automaton. Result is '" + arg_msg + "' unchanged.";
            warning_vw_print(warning_msg);
            res = arg1;
          } 
          else {
            res = dyn::support(arg1);
          }
        final_output=AUT;
        }
        else {// case of ratexps
          if (exp1->get_context()->weightset_name() == "B") {
            std::string arg_msg = args[1];
            std::string warning_msg = "'" + arg_msg + "'  is already a Boolean "
                             + "ratexp. Result is '" + arg_msg + "' unchanged.";
            warning_vw_print(warning_msg);
            exp_res = exp1;
          } 
          else {
            exp_res = dyn::ratexp_support(exp1);
          }
          final_output = EXP;
        }
      }
        break;
      // characteristic
      case CHARACTERISTIC : {
        aut_or_exp_t aoe = load_aut_or_exp(args[1]);
        if (aoe.split_case(arg1, exp1)) {// case of automata
          if (semiring == "B" && arg1->get_context()->weightset_name() == "B") {
            std::string warning_msg = "No weightset specified. "
                                      "Weightset of argument is B. "
                                      "Result is equal to the input.";
            warning_vw_print(warning_msg);
            res = arg1;
          }
          else {
            res = dyn::characteristic(arg1, semiring);
          }
          final_output=AUT;
        }
        else {// case of ratexps
          if (semiring == "B" && exp1->get_context()->weightset_name() == "B") {
            std::string warning_msg = "No weightset specified. "
                                      "Weightset of argument is B. "
                                      "Result is equal to the input.";
            warning_vw_print(warning_msg);
            exp_res = exp1;
          }
          else {
            exp_res = dyn::ratexp_characteristic(exp1, semiring);
          }
          final_output=EXP;
        }
      }
      break;	  
      // promote
      case PROMOTE: {
        aut_or_exp_t aoe = load_aut_or_exp(args[1]);
        if (aoe.split_case(arg1, exp1)) {// case of automata
          if (semiring == "B" && arg1->get_context()->weightset_name() == "B") {
            std::string warning_msg = "No weightset specified. Weightset of "
    	                         "argument is B. Result is equal to the input.";
            warning_vw_print(warning_msg);
            res = arg1;
          }
          else {
            res = dyn::promote_automaton(arg1, semiring);
          }
          final_output=AUT;
        }
        else {// case of ratexps
          if (semiring == "B" && exp1->get_context()->weightset_name() == "B") {
            std::string warning_msg = "No weightset specified. Weightset of "
    	                         "argument is B. Result is equal to the input.";
            warning_vw_print(warning_msg);
            exp_res = exp1;
          }
          else {
            exp_res = dyn::promote_ratexp(exp1, semiring);
          }
          final_output=EXP;
        }
      }
      break;

////  Rational operations on automata
      // sum and concatenate
	  
      case SUM:
      case CONCATENATE: {
        std::string error_msg1, error_msg2;
        arg1 =load(args[1]);
        arg2 =load(args[2]);
        bool arg1_tdc = arg1->is_transducer();
        bool arg2_tdc = arg2->is_transducer();
        bool arg1_law = arg1->are_words_allowed();
        bool arg2_law = arg2->are_words_allowed();
		
        if ( ((arg1_tdc || arg2_tdc) && !(arg1_tdc && arg2_tdc)) ) {
          error_msg1 = "The two arguments should be both transducers " 
                       "or both automata.";
          error_msg2 = "Type 'cora help sum' or 'concatenate' if necessary.";
          error_print(error_msg1, error_msg2);
          return 1;
        } 
        else {
          if ( ((arg1_law || arg2_law) && !(arg1_law && arg2_law)) ) {
            error_msg1 = "The two arguments should be automata that both allow " 
                         "words or both not.";
            error_msg2 = "Type 'cora help sum' or 'concatenate' if necessary.";
            error_print(error_msg1, error_msg2);
            return 1;
          } 
          else {
            switch(it->cst) {
              case SUM: {
                if (algo == "standard") {
                  res = dyn::sum_of_standard(arg1, arg2);
                } 
                else {
                  res = dyn::sum(arg1, arg2);
                }
                break;
              }
              case CONCATENATE: {
                res = dyn::concatenate(arg1, arg2);
                break;
              }
              default: 
                {}
            }
          }
        }
        final_output=AUT;
        break;
      }

// 		case SUM: {
//         arg1 =load(args[1]);
//         arg2 =load(args[2]);
//         final_output=AUT;
//         break;
//       }
//       // concatenation
//       case CONCATENATE:
//         arg1 =load(args[1]);
//         arg2 =load(args[2]);
//         final_output=AUT;
//         break;
      // star
      case STAR:
        arg1 =load(args[1]);
        dyn::star(arg1, {dyn::IN_PLACE=true});
        res = arg1;
        final_output=AUT;
        break;
      // left-mult, right-mult
      case LEFT_MULT:
        arg1 =load(args[2]);
        if (algo == "standard") {
          dyn::left_mult_standard(arg1, arg1->get_context()->get_weight(args[1]),
                       {dyn::IN_PLACE=true});
        } 
        else {
          dyn::left_mult(arg1, arg1->get_context()->get_weight(args[1]),
                       {dyn::IN_PLACE=true});
        }
        res = arg1;
        final_output=AUT;
        break;
      case RIGHT_MULT:
        arg1 =load(args[1]);
        dyn::right_mult(arg1, arg1->get_context()->get_weight(args[2]),
                        {dyn::IN_PLACE=true});
        res = arg1;
        final_output=AUT;
        break;

// reminiscence of tests made common for sum and concatenation
//           switch(it->cst) {
//           case SUM:
//           case CONCATENATE:
//             res = dyn::concatenate(arg1, arg2);
//             break;
//           default:
//           {}
//           }
//         }
//         if (arg1->get_context()->weightset_name() !=
//             arg2->get_context()->weightset_name() ||
//             arg1->get_context()->labelset_name() != arg2->get_context()->labelset_name()) 
//         {
//           std::string error_msg1 = args[1];
//           std::string error_msg2 = args[2];
//           error_msg1 = "'" + error_msg1 + "'  and  '" +  error_msg2 
//                      + "'  have not the same static context.";
//           error_msg2 = "Type 'cora help sum' or 'concatenate' if necessary.";
//           error_print(error_msg1, error_msg2);
//           return 1;
//         }
//         else {
//               if (!dyn::is_standard(arg1) || ! dyn::is_standard(arg2)) {
//                 std::string error_msg1 = args[1];
//                 std::string error_msg2 = args[2];
//                 error_msg1 = "'" + error_msg1 + "'  or  '" +  error_msg2 
//                            + "'  is not a standard automaton.";
//                 error_msg2 = "Type 'cora help sum' if necessary.";
//                 error_print(error_msg1, error_msg2);
//                 return 1;
//               }
//               else

//// Dealing with spontaneous transitions
      // allow-eps
      case ALLOW_EPS :
        arg1=load(args[1]);

        if (arg1->is_eps_allowed()) {
          std::string warning_msg = args[1];
          warning_msg = "'" + warning_msg 
                      + "' is already an automaton where epsilon-transitions are allowed. " 
                      + "Result equal to input.";
          warning_vw_print(warning_msg);

          res = arg1;
        }

        else
          res = dyn::allow_eps_transition(arg1);

        final_output=AUT;
        break;

      // is-proper
      case IS_PROPER:
        arg1 =load(args[1]);
        if (!arg1->is_eps_allowed()) {
          std::string warning_msg = args[1];
          warning_msg = "'" + warning_msg 
                      + "' is an automaton where epsilon-transitions are " 
                      + "not allowed, hence is proper.";
          warning_vw_print(warning_msg);
          boolean = true;
        }

        else
          boolean = dyn::is_proper(arg1);

        final_output=BOOL;
        break;

// is-valid
      case IS_VALID: {
        aut_or_exp_t aoe = load_aut_or_exp(args[1]);
        if (aoe.is_aut) {
//           if (aoe.aut->get_context()->weightset_name() == "B") {
//             std::string warning_msg = "a Boolean automaton is always valid.";
//             warning_vw_print(warning_msg);
//             boolean = true;
//           }
//           else
            boolean = dyn::is_valid(aoe.aut);          
        } 
		else {
//           if (aoe.exp->get_context()->weightset_name() == "B") {
//             std::string warning_msg = "a Boolean ratexp is always valid.";
//             warning_vw_print(warning_msg);
//             boolean = true;
//           }
//           else
		    boolean = dyn::is_valid(aoe.exp);
		}

        final_output=BOOL;
        break;
      }

      // proper
      case PROPER: {
        arg1 =load(args[1]);
		std::string lbl_name = arg1->get_context()->labelset_name();
		
		if (arg1->is_transducer() || arg1->are_words_allowed()) {
          if (dyn::is_proper(arg1)) {
            std::string warning_msg = args[1];
            warning_msg = "'" + warning_msg + "' is an automaton with no "
                          + "epsilon-transitions, hence is proper. "
	                      + "Result is equal to input.";
            warning_vw_print(warning_msg);
          } 
          else {
            dyn::proper(arg1, {dyn::IN_PLACE=true, dyn::DIRECTION=algo});
          }
		}

        else {
          if ( !(arg1->is_eps_allowed()) ) {
            std::string warning_msg = args[1];
            warning_msg = "'" + warning_msg + "' is an automaton where "
	                    + "'epsilon-transitions are not allowed, hence is proper." 
                        + " Result is equal to input.";
            warning_vw_print(warning_msg);
          }

		  else {
            if (dyn::is_proper(arg1)) {
              std::string warning_msg = args[1];
              warning_msg = "'" + warning_msg + "' is an automaton with no "
                            + "epsilon-transitions, hence is proper. "
	                        + "Context of the input only is changed.";
              warning_vw_print(warning_msg);
            }

            arg1 = dyn::proper(arg1, {dyn::IN_PLACE=false, dyn::DIRECTION=algo});
          }
        } 
      
		res = arg1;
        final_output=AUT;
        break;
      }
																						 																																								 
      // AUT_TO_EXP   implements the state elimination method
      case AUT_TO_EXP :
        arg1=load(args[1]);
        exp_res = dyn::aut_to_exp(arg1, {dyn::STATE_ELIM_ORDER = algo});
        final_output=EXP;
        break;


//// wfa commands            ////
/* ---------------------------------------|
|  Wfa commands                           |
|    Commands for (weighted) automata     |
|    (as opposed to transducers)          |
|----------------------------------------*/
//// About coefficients of words
      // eval
      case EVAL : {
        arg1=load(args[1]);
        if (arg1->is_transducer()) {
          aut_or_exp_t aoe = load_aut_or_exp(args[2], false);
          if (aoe.split_case(arg2,exp1)) {
            res = dyn::eval_tdc(arg2, arg1);
            final_output = AUT;
          }
          else {
            exp_res = dyn::eval_exp(exp1, arg1);
            final_output=EXP;
          }
        } 
        else {
          ctx = arg1->get_context(); 
          weight = dyn::eval(arg1, ctx->get_word(std::string(args[2])));
          final_output=WEIGHT;
        }
      }
      break;
      // enumerate, shortest
      case ENUMERATE :
      case SHORTEST : {
        arg1 =load(args[1]);
        unsigned n = strict_atou(args[2]);
        std::map<dyn::any_t, dyn::weight_t> map;
        switch(it->cst) {
        case ENUMERATE :
          map= dyn::enumerate(arg1, n);
          break;
        case SHORTEST :
          map= dyn::shortest(arg1, n);
          break;
        default :
        {}
        }
        for(auto p: map) {
          if (arg1->get_context()->weightset_name() != "B")
            std::cout << '<' 
                      << arg1->get_context()->weight_to_string(p.second) 
                      << "> ";
          std::cout << p.first << std::endl;
        }
        final_output=NONE;
      }
      break;
	  
//// About quotient of an automaton
      // reduce
      case REDUCE :
        arg1=load(args[1]);
        res = dyn::reduce(arg1);
        final_output=AUT;
        break;
      case L_REDUCE :
        arg1=load(args[1]);
        res = dyn::left_reduce(arg1);
        final_output=AUT;
        break;
      case R_REDUCE :
        arg1=load(args[1]);
        res = dyn::right_reduce(arg1);
        final_output=AUT;
        break;
		
//// Determinism
      // weighted-determinize
      // case W_DET :
      //   arg1=load(args[1]);
      //   res = dyn::weighted_determinize(arg1);
      //   final_output=AUT;
      //   break;
      case DETERMINIZE :
        arg1=load(args[1]);
        res = dyn::determinize(arg1);
        final_output=AUT;
        break;
      case EXPLORE_LENGTH :
        arg1=load(args[1]);
	    {
         unsigned n = strict_atou(args[2]);
         res = dyn::explore_by_length(arg1,n);
        }
        final_output=AUT;
        break;
      case EXPLORE_BOUND :
        arg1=load(args[1]);
        ctx = arg1->get_context();
        res = dyn::explore_with_bound(arg1,ctx->get_weight(args[2]));
        final_output=AUT;
        break;
      // is
     case IS_DET :
        arg1=load(args[1]);
        boolean = dyn::is_deterministic(arg1);
        final_output=BOOL;
        break;
      case IS_SEQ :
        arg1=load(args[1]);
        boolean = dyn::is_sequential(arg1);
        final_output=BOOL;
        break;
      case IS_AMB :
        arg1=load(args[1]);
        boolean = dyn::is_ambiguous(arg1);
        final_output=BOOL;
        break;

//// Managing transitions
      // compact-paths
      case COMPACT:
        arg1 =load(args[1]);
        res=dyn::compact(dyn::allow_words(arg1));
        final_output=AUT;
        break;
      case LETTERIZE:
        arg1 =load(args[1]);
        res=dyn::letterize(arg1);
        final_output=AUT;
        break;
      // change-alphabet
      case CHG_ALPHA:
        arg1 =load(args[1]);
        if(arg1->is_int_automaton()) {
          int n= std::stoi(alphabet);
          dyn::change_int_alphabet(arg1, 0, n-1);
        }
        else
          dyn::change_alphabet(arg1, alphabet);
        res=arg1;
        final_output=AUT;
        break;

//// Various kinds of products
      // product
      case PRODUCT:
        arg1 =load(args[1]);
        arg2 =load(args[2]);
        res = dyn::product(arg1, arg2);
        final_output=AUT;
        break;
      case POWER: {
        arg1 =load(args[1]);
        unsigned n =strict_atou(args[2]);
        res = dyn::power(arg1, n);
        final_output=AUT;
        break;
      }
      case SHUFFLE:
        arg1 =load(args[1]);
        arg2 =load(args[2]);
        res = dyn::shuffle(arg1, arg2);
        final_output=AUT;
        break;
      case INFILTRATION:
        arg1 =load(args[1]);
        arg2 =load(args[2]);
        res = dyn::infiltration(arg1, arg2);
        final_output=AUT;
        break;

      // are-equivalent
      case ARE_EQVT :
        arg1=load(args[1]);
        arg2=load(args[2]);
        boolean = dyn::are_equivalent(arg1, arg2);
        final_output=BOOL;
        break;

      // partial-identity
      case PARTIAL_ID:
        arg1 =load(args[1]);
        if ( arg1->is_transducer() ) {
        std::string msg = " In command " + it->name 
                          + " , transducers are not allowed.";
        error_print(msg);
        return 1;
        }

        else
          res = dyn::partial_identity(arg1);

        final_output=AUT;
        break;


//// ratexp commands         ////
/* ---------------------------------------|
| Ratexp commands                         |
|----------------------------------------*/
      // constant-term
      case CST_TERM : 
        exp1=load_exp(args[1]);
        weight = dyn::constant_term(exp1);
        final_output=WEIGHT;
        break;
      // expand
      case EXPAND :
        exp1=load_exp(args[1]);
        exp_res = dyn::expand(exp1);
        final_output=EXP;
        break;
      // star-normal-form
      case SNF : {
        exp1=load_exp(args[1]);
        if (exp1->get_context()->weightset_name() != "B") {
          std::string str = args[0];
          std::string error_msg1 = "'" + str 
                                 + "' is a command for Boolean ratexp only.";
          std::string error_msg2 = "Type 'cora help" + str + "' if necessary.";
          error_print(error_msg1, error_msg2);
          return 1;
        }
        else
          exp_res = dyn::star_normal_form(exp1);

        final_output=EXP;
        break;
      }
      
      // exp-to-aut
      case EXP_TO_AUT :
        exp1=load_exp(args[1]);
        res = dyn::exp_to_aut(exp1, {dyn::EXP_TO_AUT_ALGO=algo});
        final_output=AUT;
        break;
      // derivation
      case DERIVATION :
        exp1=load_exp(args[1]);
        ctx = exp1->get_context(); 
        {
          std::map<dyn::ratexp_t, dyn::any_t> map;
          if(algo=="breaking")
            map = dyn::internal::derivation(exp1, ctx->get_label(args[2]), true);
          else
            map = dyn::internal::derivation(exp1, ctx->get_label(args[2]));
          for(auto p: map) {
            if(ctx->show_one() || p.second != ctx->weight_one())
              std::cout << "<" << p.second << "> ";
            else
              std::cout << "    ";
            p.first->print(std::cout) << std::endl;
          }
        }
        final_output=NONE;
        break;


//// nfa commands            ////
/* ---------------------------------------|
| Nfa commands                            |
|----------------------------------------*/
      // Commands which call 'en-place' functions
      case PREFIX :
      case SUFFIX :
      case FACTOR :
      case COMPLEMENT :
        arg1=load(args[1]);
        if (arg1->get_context()->weightset_name() != "B" &&
            !arg1->is_transducer()) {
          std::string str = args[0];
          std::string error_msg1 = "'" + str 
                                 + "' is a command for Boolean automata only.";
          std::string error_msg2 = "Type 'cora help " + str + "' if necessary.";
          error_print(error_msg1, error_msg2);
          return 1;
        }
        else {
          switch(it->cst) {
          case PREFIX :
            dyn::prefix(arg1, {dyn::IN_PLACE=true});
            break;
          case SUFFIX :
            dyn::suffix(arg1, {dyn::IN_PLACE=true});
            break;
          case FACTOR :
            dyn::factor(arg1, {dyn::IN_PLACE=true});
            break;
          case COMPLEMENT :
            if (!dyn::is_deterministic(arg1)) {
              // The message sent by dyn/bridge/determinize.cc refers to 
              // complement_here, which is meaningless for cora users
              std::string error_msg1 = args[0];
              error_msg1 = "'" + error_msg1 
                 + "' is a command for deterministic Boolean automata only.";
              std::string error_msg2 = "Type 'cora help complement' if necessary.";
              error_print(error_msg1, error_msg2);
              return 2;
            }
            else {
              dyn::complement(arg1, {dyn::IN_PLACE=true});
              break;
            }
          default :
            break;
          }
          res=arg1;
          final_output=AUT;
        }
        break;

      // complete
      case COMPLETE:
        arg1=load(args[1]);
        if (dyn::is_complete(arg1)) {
          std::string warning_msg = args[1];
          warning_msg = "'" + warning_msg 
                      + "' is already a complete automaton. "
                      + "Result equal to input.";
          warning_vw_print(warning_msg);
        }
        dyn::complete(arg1, {dyn::IN_PLACE=true});
        res=arg1;
        final_output=AUT;
        break;

      case IS_COMPLETE :
        arg1=load(args[1]);
        boolean = dyn::is_complete(arg1);
        final_output=BOOL;
        break;


      // Commands which produce a 'new' automaton
      case MINIMAL: {
        dyn::options_t opts = {};
        try {
          opts += (dyn::MINIM_ALGO = algo);
        }
        catch (const std::domain_error&) {
          try {
            opts += (dyn::QUOTIENT_ALGO = algo);
          }
          catch (const std::domain_error&) {
            std::stringstream ss;
            ss << "Given algorithm, " << algo
               << ", is not valid for min-automaton. "
               << std::endl;
            throw std::domain_error(ss.str());
          }
        }
        aut_or_exp_t aoe = load_aut_or_exp(args[1]);
        if (aoe.split_case(arg1, exp1))
          res = dyn::minimal_automaton(arg1, opts);
        else
          res = dyn::minimal_automaton(exp1, opts);
        final_output = AUT;
        break;
      }

//// to be found in wfa_cmds.cc the function may appear at two places in
//// cora.hh and then in the table, but not in the switch
//       case DETERMINIZE :
//         arg1=load(args[1]);
//         res = dyn::determinize(arg1);
//         final_output=AUT;
//         break;

//// transducer commands     ////
/* ---------------------------------------|
| Transducer commands                     |
|----------------------------------------*/

///////// First check that the argument is a transducer (to be removed)
      case TDC_DOMAIN:
      case TDC_IMAGE:
      case INVERSE:
      case IS_FUNC:
      case IS_OF_FINITE_IM:
      case IS_SYNCHRONIZABLE:
      case SYNCHRONIZE:
      case SUBNORMALIZE:
        arg1=load(args[1]);
        if ( !arg1->is_transducer() ) {
          std::string str = args[0];
          std::string error_msg1 = "'" + str 
                                 + "' is a command for transducers only.";
          std::string error_msg2 = "Type 'cora help " + str + "' if necessary.";
          error_print(error_msg1, error_msg2);
          return 1;
        }
        else {
          switch(it->cst) { // then deals with each command
            // domain, image
            case TDC_DOMAIN:
              res = dyn::domain(arg1);
              final_output=AUT;
              break;
            case TDC_IMAGE:
              res = dyn::image(arg1);
              final_output=AUT;
              break;
            // inverse
            case INVERSE:
              res = dyn::inverse(arg1);
              final_output=AUT;
              break;

            // is
            case IS_FUNC:
              boolean = dyn::is_functional(arg1);
              final_output=BOOL;
              break;
            case IS_OF_FINITE_IM:
              boolean = dyn::is_of_finite_image(arg1);
              final_output=BOOL;
              break;
			  
            // synchronize
            case IS_SYNCHRONIZABLE:
              boolean = dyn::is_synchronizable(arg1);
              final_output=BOOL;
              break;
            case SYNCHRONIZE:
              res = dyn::synchronize(arg1);
              final_output=AUT;
              break;
            case SUBNORMALIZE:
              res = dyn::subnormalize(arg1);
              final_output=AUT;
              break;
 
            default :
              {}
              break;
          }
        }
        break;

		// compose
      case COMPOSE:
        arg1 =load(args[1]);
        arg2 =load(args[2]);
        res = dyn::compose(arg1, arg2);
        final_output=AUT;
        break;

// // // deprecated commands  eval command is set up in wfa_cmds
      // cases processed for retro compatibility
      // eval
      case EVAL_TW:
        deprecated_command("eval-word", "2.1", "command eval");
        arg1 =load(args[1]);
        exp_res = dyn::eval_word(arg1, args[2]);
        final_output=EXP;
        break;
      case EVAL_T:
        deprecated_command("eval-aut", "2.1", "command eval");
        arg1 =load(args[1]);
        arg2 =load(args[2]);
        res = dyn::eval_tdc(arg1, arg2);
        final_output=AUT;
        break;


//// factories commands      ////
/* ---------------------------------------|
| Factories.                              |
|----------------------------------------*/
      case N_ULTIMATE:
      case DOUBLERING:
      case LADYBIRD:
      case CERNY:
      case WITNESS: {
        if(letter_type=="char") {
          unsigned n=strict_atou(args[1]);
          switch(it->cst) {
          case N_ULTIMATE:
            res = dyn::factory::n_ultimate(n, alphabet, semiring);
            break;
          case DOUBLERING:
            res = dyn::factory::double_ring(n, {0}, alphabet, semiring);
            break;
          case LADYBIRD:
            if (alphabet_was_set)
              res = dyn::factory::ladybird(n, alphabet, semiring);
            else
              res = dyn::factory::ladybird(n, "abc", semiring);
            break;
          case CERNY:
            res = dyn::factory::cerny(n, alphabet, semiring);
            break;
          case WITNESS:
            if (alphabet_was_set)
              res = dyn::factory::witness(n, alphabet, semiring);
            else
              res = dyn::factory::witness(n, "abc", semiring);
            break;
          default :
            ;
          }
        }
        else
          if(letter_type=="int") {
            int n= std::stoi(alphabet);
            switch(it->cst) {
            case N_ULTIMATE:
              res = dyn::factory::n_ultimate(n, 0, n-1, semiring);
              break;
            case DOUBLERING:
              res = dyn::factory::double_ring(n, {0}, 0, semiring);
              break;
            case LADYBIRD:
              res = dyn::factory::ladybird(n, 0, semiring);
              break;
            case CERNY:
              res = dyn::factory::cerny(n, 0, n-1, semiring);
              break;
            case WITNESS:
              res = dyn::factory::witness(n, 0, semiring);
              break;
            default :
              ;
            }
          }
          else
            throw std::runtime_error("unsupported letter type: "+letter_type);

        final_output=AUT;
      }
      break;

//  divkbaseb
      case DIVKBASEB: {
        unsigned k=strict_atou(args[1]);
        unsigned b=strict_atou(args[2]);
        if(letter_type=="char")
          if (alphabet_was_set)
            res = dyn::factory::divkbaseb(k, b, alphabet, semiring);
          else
            res = dyn::factory::divkbaseb(k, b, "auto", semiring);
        else
          if(letter_type=="int")
            res = dyn::factory::int_divkbaseb(k, b, semiring);
          else
            throw std::runtime_error("unsupported letter type: "+letter_type);
        final_output=AUT;
      }
      break;

// random
      case RAND_DFA : {
        unsigned n=strict_atou(args[1]);
        res=dyn::factory::randomDFA(n, alphabet);
        final_output=AUT;
      }
      break;

//////////
      default : // Does not happen ! Just here to avoid warnings
        break;
      } // end of the switch on all cora commands

// Ready for the reading of the next chunk of the input line
      first_cmd=false;
    } // end of try

    catch(const std::runtime_error& e) {// call centralised error message print
//  std::cerr << "command " << it->name <<": error: " << e.what() << std::endl;
      std::string msg = " In command " + it->name + " : " + e.what();
      error_print(msg);
      std::exit(EXIT_FAILURE);
    }
    catch(const std::domain_error& e) {// call centralised error message print
      std::string msg = " In command " + it->name + " : " + e.what();
      std::string err_typ = "Domain error";
      x_error_print(msg, err_typ);
      std::exit(EXIT_FAILURE);
    }
    catch(const std::invalid_argument& e) {// call centralised error message print
      std::string msg = " In command " + it->name + " : " + e.what();
      std::string err_typ = "Invalid argument error";
      x_error_print(msg, err_typ);
      std::exit(EXIT_FAILURE);
    }
//     catch(const parse_exception& e) {// call centralised error message print
//       std::string msg = " In command " + it->name + " : " + e.what();
//       std::string err_typ = "Parse exception";
//       x_error_print(msg, err_typ);
//       std::exit(EXIT_FAILURE);
//     }  seems to be useless as taken care of by runtime_error
      
    if ( (final_output == AUT || final_output == TDC) && (name != "tmp") )
      res->set_name(name);

  }
  while(i!=argc);    // end of the do loop
  //
  output();
  return EXIT_SUCCESS;
} // end of  main  in namespace awali::cora

}
}//end of namespace awali::cora

//// main  visible from outside
int main(int argc, const char** argv)
{
  return awali::cora::main(argc, argv);
}

//////   end of cora.cc  //////
