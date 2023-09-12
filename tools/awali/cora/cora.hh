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

#ifndef CORA_HH
#define CORA_HH

#include <awali/dyn/loading/locations.hh>
#include <awali/dyn/core/context_description.hh>

#include <cora/online_doc/online_doc.hh>

#include<dirent.h>
#include<fstream>
#include<map>
#include<list>
#include<vector>
#include<stdexcept>
#include<iostream>
#include<iomanip>

#include<awali/dyn.hh>


namespace awali { namespace cora {

using aut_or_exp_t = ::awali::dyn::internal::aut_or_exp_t;
aut_or_exp_t load_aut_or_exp(std::string s);



const char* args[10];
std::string empty_name=" ";

// list of command indexes for to be used in the main switch of cora.cc
enum coms_t {
//  empty command (for layout)
  MPTY_CMD,
////  Basic commands
  HELP,
  LIST, DOC, CAT, DISPLAY, INFO, STATS, EDIT, NEW, IS,
////  Generic commands for automata and transducers
// Graph traversal functions
  ACC, COACC, TRIM, IS_EMPTY, IS_ACC, IS_COACC, IS_TRIM, IS_USELESS,
  STANDARD, IS_STANDARD,
  ARE_ISOMORPHIC,
// Miscellaneous
  STRIP,
// Strongly connected components
  CONDENSATION, IS_SC,
// Dealing with the weightset
  SUPPORT, CHARACTERISTIC, PROMOTE,
// rational operation on automata
  SUM, CONCATENATE, STAR,
  LEFT_MULT, RIGHT_MULT,
// Dealing with spontaneous transitions
  ALLOW_EPS, IS_PROPER, IS_VALID, PROPER,
// aut-to-exp
  AUT_TO_EXP,

////  Commands for (weighted) automata (as opposed to transducers)
// eval
  EVAL, ENUMERATE, SHORTEST,
// quotient
  MINQUOTIENT, MINCOQUOTIENT,
// reduce
  REDUCE, L_REDUCE, R_REDUCE,
// test-determinisation
  IS_DET, IS_SEQ, IS_AMB,
// =======
//   W_DET,
// // is
//   IS_DET, IS_COMPLETE, IS_AMB,
// >>>>>>> merge-vmsl
// transpose
  TRANSPOSE,
// change alphabet
  CHG_ALPHA,
// compact-paths
  COMPACT, LETTERIZE,
// product
  PRODUCT, POWER, SHUFFLE, INFILTRATION,
// equivalence
  ARE_EQVT,
// partial identity
  PARTIAL_ID,

////  Commands for ratexps
  CST_TERM,
//   IS_VALID_EXP,
  EXPAND, SNF, // STAR_HEIGHT,
// derivation
  EXP_TO_AUT, DERIVATION,

////   Commands for Boolean automata
// factors
  PREFIX, SUFFIX, FACTOR,
// determinize
  DETERMINIZE, COMPLEMENT, COMPLETE, IS_COMPLETE,
//  minimize
  MINIMAL,

////  Commands for transducers
  TDC_DOMAIN, TDC_IMAGE,
  EVAL_TW, EVAL_T,
  IS_FUNC, IS_OF_FINITE_IM,
  INVERSE,
  IS_SYNCHRONIZABLE, SYNCHRONIZE,
  COMPOSE,

//  Factories
  LADYBIRD, N_ULTIMATE, DOUBLERING, DIVKBASEB,  CERNY, WITNESS,
// random
  RAND_DFA,

// End of command keys
};


enum arg_kind_t { ALPH, AUT, BOOL, CMD, EXP, FMT, INT, MTD, NONE, LBL,
                  SMR, TDC, WEIGHT, WORD, STR, CHC, AUT_OR_EXP
                };

struct argument_t {
  arg_kind_t kind;
  std::string commentary = "";
  argument_t(arg_kind_t k) {
    kind=k;
  }
  argument_t(arg_kind_t k, std::string str) {
    kind=k;
    commentary=str;
  }
};

std::ostream& operator<<(std::ostream& os, const arg_kind_t& kind)
{
  switch (kind) {
  case ALPH:
    os << "alphabet";
    break;
  case AUT:
    os << "aut";
    break;
  case BOOL:
    os << "bool";
    break;
  case CMD:
    os << "cmd";
    break;
  case EXP:
    os << "exp";
    break;
  case FMT:
    os << "format";
    break;
  case INT:
    os << "int";
    break;
  case MTD:
    os << "method";
    break;
  case NONE:
    os << "none";
    break;
  case SMR:
    os << "semiring";
    break;
  case LBL:
    os << "letters";
    break;
  case TDC:
    os << "tdc";
    break;
  case WEIGHT:
    os << "weight";
    break;
  case WORD:
    os << "word";
    break;
  case STR:
    os << "string";
    break;
  case CHC:
    os << "choice";
    break;
  case AUT_OR_EXP:
    os << "automaton_or_expresionn";
    break;
  };
  return os;
}

std::ostream& operator<<(std::ostream& os, const argument_t& arg)
{
  os << '<' << arg.kind;
  if (arg.commentary.length() != 0)
    os << ':' << arg.commentary;
  os << '>';
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<argument_t>& args)
{
  for (argument_t arg: args)
    os << arg << " ";
  return os;
}




// structure command
//
struct command {               // new format for command
  std::string name;               // name of command
  coms_t cst;                     // id of command
  unsigned argc;                  // nb of arguments of command
  std::vector<argument_t> args;   // list of types of arguments
  std::string desc;               // short description of command
  std::string optiondesc;         // option description
  std::string longdesc;           // long description
};

std::vector<command> commands;
std::vector<command> commands_basic;
std::vector<command> commands_generic;
std::vector<command> commands_aut;
std::vector<command> commands_nfa;
std::vector<command> commands_tdc;
std::vector<command> commands_exp;
std::vector<command> commands_fact;
// std::vector<command> commands_std;
//
std::vector<command> commands_inv;   // commands not shown in the list printing
std::vector<command> commands_mpty;      // empty line for the array printing
command empty_cmd = {empty_name, MPTY_CMD, 0, {}, "  ", "", ""}; //for layout of list printing


std::vector<command>::iterator it;


/* ---------------------------------------|
|  Functions init                          |
|----------------------------------------*/
////// Filling the command lists
void init_cmds()
{
/* ---------------------------------------|
|  Basic commands                         |
|----------------------------------------*/
// help
  commands_basic.emplace_back(
  command{"help", HELP, 1, {{CMD}},
    "Prints first help, various lists of commands, or a command help",
    "",
    awali::cora::doc::help
  });
// list
  commands_basic.emplace_back(
  command{"list", LIST, 1, {{CHC}},
    "Lists various objects: automata, ratexps, weightsets, factories,"
    "that can be used in cora command lines.",
    "",
    awali::cora::doc::list
  });
// docu
  commands_basic.emplace_back(
  command{"doc", DOC, 1, {{CHC}},
    "Prints documentation for some commands or entities in awali",
    "",
    awali::cora::doc::doc
  });

// skipped line in the help list
  commands_basic.emplace_back(empty_cmd);

// cat
  commands_basic.emplace_back(
  command{"cat", CAT, 1, {{AUT_OR_EXP}},
    "Loads and outputs an automaton or a ratexp",
    "[-I<input-fmt>] [-O<output-fmt>] [-N<name>]",
    awali::cora::doc::cat
  });

// display
  commands_basic.emplace_back(
  command{"display", DISPLAY, 1, {{AUT_OR_EXP}},
    "Displays an automaton or a ratexp using Graphviz",
    "[-I<input-fmt>] [-N<name>] [-H]", // [-O<opt>] 201121
    awali::cora::doc::display
  });

// info
  commands_basic.emplace_back(
  command{"info", INFO, 1, {{AUT_OR_EXP}},
    "Prints basic characteristics of an automaton or a ratexp",
    "",
    awali::cora::doc::info
  });

// statistics
  commands_basic.emplace_back(
  command{"statistics", STATS, 2, {{CHC}, {AUT_OR_EXP}},
    "Gives some statistics on an automaton or a ratexp",
    "",
    awali::cora::doc::statistics
  });

// skipped line in the help list
  commands_basic.emplace_back(empty_cmd);

// edit
  commands_basic.emplace_back(
  command{"edit", EDIT, 1, {{AUT}},
    "Enters an interactive mode to edit the automaton <aut>",
    "",
    awali::cora::doc::edit
  });
// new
  commands_basic.emplace_back(
    command{"new", NEW, 0, {},
            "Creates and edits an empty automaton or transducer",
            "[-O -W -L -A -B]",
            awali::cora::doc::new_
    });

// skipped line in the help list
  commands_basic.emplace_back(empty_cmd);

// is    general test command
  commands_basic.emplace_back(
  command{"is", IS, 2, {{CHC}, {AUT_OR_EXP}},
    "Tests some properties of an automaton or a ratexp",
    "[-S]",
    awali::cora::doc::is
  });

// end of Basic commands
//
/* ---------------------------------------|
|  Generic commands                       |
|----------------------------------------*/
//// Graph traversal functions
  // accessible
  commands_generic.emplace_back(
  command{"accessible", ACC, 1, {{AUT}},
    "computes the accessible part of the automaton",
    "",
    awali::cora::doc::accessible
  });
  // coaccessible
  commands_generic.emplace_back(
  command{"coaccessible", COACC, 1, {{AUT}},
    "computes the coaccessible part of the automaton",
    "",
    awali::cora::doc::coaccessible
  });
  // trim
  commands_generic.emplace_back(
  command{"trim", TRIM, 1, {{AUT}},
    "compute the trim part of the automaton",
    "",
    awali::cora::doc::trim
  });
  // is-empty
  commands_inv.emplace_back(
  command{"is-empty", IS_EMPTY, 1, {{AUT}},
    "test whether an automaton is empty",
    "[-S]",
    awali::cora::doc::is_empty
  });
  // is-accessible
  commands_inv.emplace_back(
  command{"is-accessible", IS_ACC, 1, {{AUT}},
    "test whether an automaton is accessible",
    "[-S]",
    awali::cora::doc::is_accessible
  });
  // is-coaccessible
  commands_inv.emplace_back(
  command{"is-coaccessible", IS_COACC, 1, {{AUT}},
    "test whether an automaton is coaccessible",
    "[-S]",
    awali::cora::doc::is_coaccessible
  });
  // is-trim
  commands_inv.emplace_back(
  command{"is-trim", IS_TRIM, 1, {{AUT}},
    "test whether an automaton is trim",
    "[-S]",
    awali::cora::doc::is_trim
  });
  // is-useless
  commands_inv.emplace_back(
  command{"is-useless", IS_USELESS, 1, {{AUT}},
    "test whether an automaton has no useful state",
    "[-S]",
    awali::cora::doc::is_useless
  });
//// standard automata
  // standard
  commands_generic.emplace_back(
  command{"standard", STANDARD, 1, {{AUT}},
    "make an automaton standard",
    "",
    awali::cora::doc::standard
  });
  // is-standard
  commands_inv.emplace_back(
  command{"is-standard", IS_STANDARD, 1, {{AUT}},
    "test whether an automaton is standard",
    "[-S]",
    awali::cora::doc::is_standard
  });

  // are-isomorphic  //// FIX ME   quiet-verbose dilemna
  commands_generic.emplace_back(
  command{"are-isomorphic", ARE_ISOMORPHIC, 2, {{AUT, "1"}, {AUT, "2"}},
    "test whether two automata are isomorphic",
    "[-S]",
    awali::cora::doc::are_isomorphic
  });

//// Miscellaneous
  // strip
  commands_generic.emplace_back(
  command{"strip", STRIP, 1, {{AUT}},
    "strip the history",
    "",
    awali::cora::doc::strip
  });
//// strongly connected components
  // condensation
  commands_generic.emplace_back(
  command{"condensation", CONDENSATION, 1, {{AUT}},
    "reduce each strongly connected component to a single state",
    awali::cora::doc::condensation
  });
  // is-strongly-connected
  commands_inv.emplace_back(
  command{"is-strongly-connected", IS_SC, 1, {{AUT}},
    "test whether an automaton is strongly connected",
    "",
    awali::cora::doc::is_strongly_connected
  });

// skipped line in the command list
  commands_generic.emplace_back(empty_cmd);

//// Dealing with the weightset
  // support
  commands_generic.emplace_back(
  command{"support", SUPPORT, 1, {{AUT}},
    "compute the support of a weighted automaton",
    "",
    awali::cora::doc::support
  });
  // characteristic
  commands_generic.emplace_back(
  command{"characteristic", CHARACTERISTIC, 1, {{AUT}},
    "computes a characteristic automaton of an NFA",
    "-W<weightset>",
    awali::cora::doc::characteristic
  });
  // promote
  commands_generic.emplace_back(
  command{"promote", PROMOTE, 1, {{AUT}},
    "promotes an automaton into an automaton with the given weight",
    "-W<weightset>",
    awali::cora::doc::promote
  });

// skipped line in the command list
  commands_generic.emplace_back(empty_cmd);

////  Rational operations on automata
  // sum  //// FIX ME definition of 'compatible types'
  commands_generic.emplace_back(
  command{"sum", SUM, 2, {{AUT, "1"}, {AUT, "2"}},
    "forms the union of two automata",
    "[-Mstandard]",
    awali::cora::doc::sum
  });
  // concatenation
  commands_generic.emplace_back(
  command{"concatenate", CONCATENATE, 2, {{AUT, "1"}, {AUT, "2"}},
    "builds the concatenation of two automata",
    "",
    awali::cora::doc::concatenate
  });
  // star
  commands_generic.emplace_back(
  command{"star", STAR, 1, {{AUT}},
    "builds an automaton that accepts the star of the language (or series) "
    "accepted by the input automaton",
    "",
    awali::cora::doc::star
  });

  // left-mult
  commands_generic.emplace_back(
  command{"left-mult", LEFT_MULT, 2, {{WEIGHT}, {AUT}},
    "external product of a weight and an automaton",
    "",
    awali::cora::doc::left_mult
  });
  // right-mult
  commands_generic.emplace_back(
  command{"right-mult", RIGHT_MULT, 2, {{AUT}, {WEIGHT}},
    "external product of an automaton and a weight",
    "",
    awali::cora::doc::right_mult
  });

// skipped line in the help list
  commands_generic.emplace_back(empty_cmd);

//  Dealing with spontaneous transitions
  // allow-eps
  commands_generic.emplace_back(
  command{"allow-eps", ALLOW_EPS, 1, {{AUT}},
    "allows epsilon-transition in an automaton",
    "",
    awali::cora::doc::allow_eps
  });
  // is-proper
  commands_inv.emplace_back(
  command{"is-proper", IS_PROPER, 1, {{AUT}},
    "test whether an automaton has some epsilon-transitions",
    "",
    awali::cora::doc::is_proper
  });
  // is-valid
  commands_generic.emplace_back(
  command{"is-valid", IS_VALID, 1, {{AUT_OR_EXP}},
    "test whether a (weighted) automaton or ratexp is valid",
    "",
    awali::cora::doc::is_valid
  });
  // proper
  commands_generic.emplace_back(
  command{"proper", PROPER, 1, {{AUT}},
    "epsilon removal",
    "[-M<option>]",
    awali::cora::doc::proper
  });

// skipped line in the command list
  commands_generic.emplace_back(empty_cmd);

//// aut-to-exp
  commands_generic.emplace_back(
  command{"aut-to-exp", AUT_TO_EXP, 1, {{AUT}},
    "convert an automaton into a ratexp",
    "[-M<option>]",
    awali::cora::doc::aut_to_exp
  });

// end of Generic commands for automata and transducers

/* ---------------------------------------|
|  Wfa commands                           |
|    Commands for (weighted) automata     |
|    (as opposed to transducers)          |
|----------------------------------------*/
//// About coefficients of words
  // eval
  commands_aut.emplace_back(
  command{"eval", EVAL, 2, {{AUT}, {WORD}},
    "compute the weight of a word",
    "",
    awali::cora::doc::eval
  });
  // enumerate
  commands_aut.emplace_back(
  command{"enumerate", ENUMERATE, 2, {{AUT}, {INT, "n"}},
    "enumerate the first words of the language",
    "",
    awali::cora::doc::enumerate
  });
  // shortest   //// FIX ME   should not have 2 arguments
  commands_aut.emplace_back(
  command{"shortest", SHORTEST, 2, {{AUT}, {INT, "n"}},
    "enumerate the shortest words of the language",
    "",
    awali::cora::doc::shortest
  });

// skipped line in the command list
  commands_aut.emplace_back(empty_cmd);

//// About quotient of an automaton
  // min-quotient  //// FIX ME  check the possible options
  commands_aut.emplace_back(
  command{"min-quotient", MINQUOTIENT, 1, {{AUT}},
    "compute the minimal quotient of an automaton",
    "[-M<method>]",
    awali::cora::doc::min_quotient
  });

  // min-coquotient  //// FIX ME  check the possible options
  commands_aut.emplace_back(
  command{"min-coquotient", MINCOQUOTIENT, 1, {{AUT}},
    "compute the minimal coquotient of an automaton",
    "[-M<method>]",
    awali::cora::doc::min_coquotient
  });

  // reduce
  commands_aut.emplace_back(
  command{"reduce", REDUCE, 1, {{AUT}},
    "reduce a WFA",
    "",
    awali::cora::doc::reduce
  });
  // left-reduce
  commands_aut.emplace_back(
  command{"left-reduce", L_REDUCE, 1, {{AUT}},
    "left reduce a WFA",
    "",
    awali::cora::doc::left_reduce
  });
  // right-reduce
  commands_aut.emplace_back(
  command{"right-reduce", R_REDUCE, 1, {{AUT}},
    "reduce a WFA",
    "",
    awali::cora::doc::right_reduce
  });

// skipped line in the command list
  commands_aut.emplace_back(empty_cmd);

//// Determinism
  // weighted-determinize
  // commands_aut.emplace_back(
  // command{"weighted-determinize", W_DET, 1, {{AUT}},
  //   "compute the weighted determinization of a WFA",
  //   "",
  //   awali::cora::doc::w_determinize
  // });
  // determinize
  commands_aut.emplace_back(
  command{"determinize", DETERMINIZE, 1, {{AUT}},
    "compute the weighted determinization of a WFA",
    "",
    awali::cora::doc::determinize
  });

  // is-deterministic  ////  FIX ME  quiet-verbose dilemna
  commands_aut.emplace_back(
  command{"is-deterministic", IS_DET, 1, {{AUT}},
    "test whether an automaton is deterministic",
    "",
    awali::cora::doc::is_deterministic
  });
// is-sequential  ////  FIX ME  quiet-verbose dilemna
  commands_aut.emplace_back(
  command{"is-sequential", IS_SEQ, 1, {{AUT}},
    "test whether an automaton is sequential",
    "",
    awali::cora::doc::is_sequential
  });
// is-ambiguous  ////  FIX ME  quiet-verbose dilemna
  ////     pb with non integral weighht semiring
  commands_aut.emplace_back(
  command{"is-ambiguous", IS_AMB, 1, {{AUT}},
    "test whether an automaton is ambiguous",
    "",
    awali::cora::doc::is_ambiguous
  });

//// Managing transitions
  // transpose
  commands_aut.emplace_back(
  command{"transpose", TRANSPOSE, 1, {{AUT}},
    "transpose an automaton",
    "",
    awali::cora::doc::transpose
  });
  // compact-paths
  commands_aut.emplace_back(
  command{"compact-paths", COMPACT, 1, {{AUT}},
    "compact nonbranching paths",
    "",
    awali::cora::doc::compact_paths
  });
  // letterize
  commands_aut.emplace_back(
  command{"letterize", LETTERIZE, 1, {{AUT}},
    "convert an automaton on words into an automaton on letters",
    "",
    awali::cora::doc::letterize
  });
  // change-alphabet
  commands_aut.emplace_back(
  command{"change-alphabet", CHG_ALPHA, 1, {{AUT}},
    "change the alphabet of the automaton",
    "-Axyz",
    awali::cora::doc::change_alphabet
  });

// skipped line in the command list
  commands_aut.emplace_back(empty_cmd);

//// Various kinds of products
  // product  //// FIX ME definition of 'compatible types'
  commands_aut.emplace_back(
  command{"product", PRODUCT, 2, {{AUT, "1"}, {AUT, "2"}},
    "compute the product of two automata",
    "",
    awali::cora::doc::product
  });
  // power
  commands_aut.emplace_back(
  command{"power", POWER, 2, {{AUT}, {INT, "n"}},
    "compute the n-th power of an automaton",
    "",
    "Compute the n-th power of the automaton <aut>.\n"
  });
  // shuffle  //// FIX ME definition of 'compatible types'
  ////        explain what shuffle is
  commands_aut.emplace_back(
  command{"shuffle", SHUFFLE, 2, {{AUT, "1"}, {AUT, "2"}},
    "compute the shuffle of two automata",
    "",
    awali::cora::doc::shuffle
  });
  // infiltration  //// FIX ME definition of 'compatible types'
  ////        explain what infiltration is
  commands_aut.emplace_back(
  command{"infiltration", INFILTRATION, 2, {{AUT, "1"}, {AUT, "2"}},
    "compute the infiltration of two automata",
    "",
    awali::cora::doc::infiltration
  });

// skipped line in the command list
  commands_aut.emplace_back(empty_cmd);

  // are-equivalent  //// FIX ME   quiet-verbose dilemna
  commands_aut.emplace_back(
  command{"are-equivalent", ARE_EQVT, 2, {{AUT, "1"}, {AUT, "2"}},
    "test whether two automata are equivalent",
    "",
    awali::cora::doc::are_equivalent
  });

// partial-identity
  commands_aut.emplace_back(
  command{"partial-identity", PARTIAL_ID, 1, {{AUT}},
    "partial-identity from automaton",
    "",
    awali::cora::doc::change_alphabet
  });

// end of Commands for (weighted) automata (as opposed to transducers)

/* ---------------------------------------|
| Ratexp commands                         |
|----------------------------------------*/
  // constant-term
  commands_exp.emplace_back(
  command{"constant-term", CST_TERM, 1, {{EXP}},
    "compute the constant term of a ratexp",
    "",
    awali::cora::doc::constant_term
  });

// expand
  commands_exp.emplace_back(
  command{"expand", EXPAND, 1, {{EXP}},
    "expand a ratexp",
    "",
    awali::cora::doc::expand
  });

// star-normal-form
  commands_exp.emplace_back(
  command{"star-normal-form", SNF, 1, {{EXP}},
    "compute the star normal form of a (Boolean) ratexp",
    "",
    awali::cora::doc::star_normal_form
  });

// skipped line in the command list
  commands_exp.emplace_back(empty_cmd);

// derivation
  commands_exp.emplace_back(
  command{"derivation", DERIVATION, 2, {{EXP}, {WORD}},
    "derivation of a ratexp",
    "[-M<method>]",
    awali::cora::doc::derivation
  });

// skipped line in the command list
  commands_exp.emplace_back(empty_cmd);

// exp-to-aut
  commands_exp.emplace_back(
  command{"exp-to-aut", EXP_TO_AUT, 1, {{EXP}},
    "build an automaton from a ratexp",
    "[-M<method>]",
    awali::cora::doc::exp_to_aut
  });
// end of Commands for rational expressions

/* ---------------------------------------|
| Nfa commands                            |
|----------------------------------------*/
////  Factors (so to speak)
  // prefix
  commands_nfa.emplace_back(
  command{"prefix", PREFIX, 1, {{AUT}},
    "build an automaton accepting the language of prefixes",
    "",
    awali::cora::doc::prefix
  });
  // suffix
  commands_nfa.emplace_back(
  command{"suffix", SUFFIX, 1, {{AUT}},
    "build an automaton accepting the language of suffixes",
    "",
    awali::cora::doc::suffix
  });
  // factor
  commands_nfa.emplace_back(
  command{"factor", FACTOR, 1, {{AUT}},
    "automaton accepting the language of factors",
    "",
    awali::cora::doc::factor
  });

// skipped line in the command list
  commands_nfa.emplace_back(empty_cmd);

  // determinize
  commands_nfa.emplace_back(
  command{"determinize", DETERMINIZE, 1, {{AUT}},
    "determinize a Boolean automaton",
    "",
    awali::cora::doc::determinize
  });
  // complement     //// FIX ME   look at the prerequisite
  commands_nfa.emplace_back(
  command{"complement", COMPLEMENT, 1, {{AUT}},
    "complement a complete DFA",
    "",
    awali::cora::doc::complement
  });
// complete
  commands_nfa.emplace_back(
  command{"complete", COMPLETE, 1, {{AUT}},
    "complete an automaton",
    "",
    awali::cora::doc::complete
  });

  // is-complete  ////  FIX ME  quiet-verbose dilemna
  ////          pb distinguish between error and failure
  ////    should it be reserved to Boolean automata
  commands_inv.emplace_back(
  command{"is-complete", IS_COMPLETE, 1, {{AUT}},
    "test whether an automaton is complete",
    "",
    awali::cora::doc::is_complete
  });

// skipped line in the command list
  commands_nfa.emplace_back(empty_cmd);

  // minimal-automaton
  commands_nfa.emplace_back(
  command{"minimal-automaton", MINIMAL, 1, {{AUT}},
    "compute the minimal automaton of a language",
    "[-M<method>]",
    awali::cora::doc::minimal_automaton
  });

// end of Commands for Boolean automata over a free monoid (NFA)

/* ---------------------------------------|
| Transducer commands                     |
|----------------------------------------*/
  // domain
  commands_tdc.emplace_back(
  command{"domain", TDC_DOMAIN, 1, {{TDC}},
    "domain of a transducer",
    "",
    awali::cora::doc::domain
  });
  // image
  commands_tdc.emplace_back(
  command{"image", TDC_IMAGE, 1, {{TDC}},
    "image of a transducer",
    "",
    awali::cora::doc::image
  });
  // inverse
  commands_tdc.emplace_back(
  command{"inverse", INVERSE, 1, {{TDC}},
    "inverse of a transducer",
    "",
    awali::cora::doc::inverse
  });

  // eval-word
  commands_tdc.emplace_back(
  command{"eval-word", EVAL_TW, 2, {{TDC}, {WORD}},
    "image of a word by a transducer",
    "",
    awali::cora::doc::eval_word
    });
  // eval-aut     //// change of name; beware of the order of arguments
  commands_tdc.emplace_back(
  command{"eval-aut", EVAL_T, 2, {{AUT}, {TDC}},
    "image of a language (series) by a transducer",
    "",
    awali::cora::doc::eval_aut
  });

  // is-functional    //// FIX ME   quiet-verbose dilemna
  commands_tdc.emplace_back(
  command{"is-functional", IS_FUNC, 1, {{TDC}},
    "test whether a transducer is functional",
    "",
    awali::cora::doc::is_functional
  });
  // is-of-finite-imagel    //// FIX ME   quiet-verbose dilemna
  commands_tdc.emplace_back(
  command{"is-of-finite-image", IS_OF_FINITE_IM, 1, {{TDC}},
    "test whether a transducer is of finite image",
    "",
    awali::cora::doc::is_of_finite_image
  });

  // is-synchronizable    //// FIX ME   quiet-verbose dilemna
  commands_inv.emplace_back(
  command{"is-synchronizable", IS_SYNCHRONIZABLE, 1, {{TDC}},
    "test whether a transducer is synchronizable",
    "",
    awali::cora::doc::is_synchronizable
  });
// synchronize    ////  FIX ME  quiet-verbose dilemna
  ////          pb distinguish between error and failure
  commands_tdc.emplace_back(
  command{"synchronize", SYNCHRONIZE, 1, {{TDC}},
    "synchronize a (synchronizable) transducer", 
    "",
    awali::cora::doc::synchronize
  });

  // compose    //// FIX ME. What happens if the two transducers are not composable?
  ////.        if the composition is not defined?
  commands_tdc.emplace_back(
  command{"compose", COMPOSE, 2, {{TDC, "1"}, {TDC, "2"}},
    "compose two transducers",
    "",
    awali::cora::doc::compose
  });
// end of Commands for transducers

/* ---------------------------------------|
| Factories.                              |
|----------------------------------------*/
  // ladybird
  commands_fact.emplace_back(
  command{"ladybird", LADYBIRD, 1, {{INT, "n"}},
    "build an automaton whose determinisation is of exponential size",
    "[-A<alphabet>]",
    awali::cora::doc::ladybird
  });
  // n-ultimate   // FIX ME  alphabet modification
  commands_fact.emplace_back(
  command{"n-ultimate", N_ULTIMATE, 1, {{INT, "n"}},
    "build an automaton that recognizes words with an 'a' at the n-th "
    "position from end",
    "[-A<alphabet>]",
    awali::cora::doc::n_ultimate
  });
  // double-ring
  commands_fact.emplace_back(
  command{"double-ring", DOUBLERING, 1, {{INT, "n"}},
    "build an automaton that recognizes words "
    "with the same number of 'a's as 'b's modulo n",
    "[-A<alphabet>]",
    awali::cora::doc::double_ring
  });
  // divkbaseb
  commands_fact.emplace_back(
  command{"divkbaseb", DIVKBASEB, 2, {{INT, "k"}, {INT, "b"}},
    "build an automaton that recognizes multiple of k written in base b",
    "",
    awali::cora::doc::divkbaseb
  });
  // cerny
  commands_fact.emplace_back(
  command{"cerny", CERNY, 1, {{INT, "n"}},
    "build an automaton with long synchronizing word",
    "",
    awali::cora::doc::cerny
  });
  // witness
  commands_fact.emplace_back(
  command{"witness", WITNESS, 1, {{INT, "n"}},
    "build Brzozowski's universal witness",
    "",
    awali::cora::doc::witness
  });
// random-dfa
  commands_fact.emplace_back(
  command{"random-dfa", RAND_DFA, 1, {{INT, "n"}},
    "generate a random DFA",
    "[-A<alphabet>]",
    awali::cora::doc::random_dfa
  });
// end of Factories
//

// vector 'commands' contains all cora commands and will be searh at each call
  it = commands.begin();
  commands.insert(it, commands_basic.begin(), commands_basic.end());
  it = commands.end();
  commands.insert(it, commands_generic.begin(), commands_generic.end());
  it = commands.end();
  commands.insert(it, commands_aut.begin(), commands_aut.end());
  it = commands.end();
  commands.insert(it, commands_nfa.begin(), commands_nfa.end());
  it = commands.end();
  commands.insert(it, commands_tdc.begin(), commands_tdc.end());
  it = commands.end();
  commands.insert(it, commands_exp.begin(), commands_exp.end());
  it = commands.end();
  commands.insert(it, commands_fact.begin(), commands_fact.end());
  it = commands.end();
//   commands.insert(it,commands_std.begin(),commands_std.end()); it = commands.end();
  commands.insert(it, commands_inv.begin(), commands_inv.end());
}

/* ---------------------------------------|
|  Managing  options                      |
|----------------------------------------*/
#include<cora/cora_options.hh>

/* ---------------------------------------|
|  Managing  lists                      |
|----------------------------------------*/
#include<cora/cora_lists.hh>


}}//end of namespace awali::cora


#endif
