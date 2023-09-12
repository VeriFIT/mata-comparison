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

/* ---------------------------------------------------|
|  Called from cora.hh as fake modularisation         |
|----------------------------------------------------*/

/* ---------------------------------------|
|  Managing  options                      |
|----------------------------------------*/

// list of option indexes to be used in some of cora commands
enum opts_t {
  LABELS, ALPHABET, ALPHABET_B,  WEIGHTS,
  INPUT_FMT, OUTPUT_FMT,
  SHELL,  VERBOSE, 
  METHOD, 
  HISTORY, NAME, CAPTION, // TITLE
};

// Options definitions and default values
std::string letter_type="char";
bool allow_eps_transition = false;
std::string alphabet="ab";
bool alphabet_was_set = false;
std::string alphabetb="ab";
std::string semiring="B";
std::string output_format="default";
std::string input_format="default";
bool shell=false;
bool verbose=true;
std::string algo="default";
std::string dflt_name="tmp";
std::string dflt_caption="";
std::string name=dflt_name;
std::string caption=dflt_caption;

// Change struct option by js 20210121 
struct option {              //  format for option
  std::string name;               // long-option name
  opts_t cst;                     // id of option
  unsigned argc;                  // nb of arguments of option
  std::vector<argument_t> args;   // list of types of arguments
  std::string desc;               // short description of option
  std::string longdesc;           // long description
};

std::map<std::string, option> options;

void init_options()
{
  options.emplace(std::make_pair("L", 
    option{"labels",
           LABELS, 1, {{LBL}},
           "set the type of labels for a new automaton or ratexp",
           awali::cora::doc::labelset}));
  
  options.emplace(std::make_pair("A", 
    option{"alphabet",
           ALPHABET, 1, {{ALPH}},
           "set the alphabet for a new automaton or ratexp",
           awali::cora::doc::alphabet}));
 
  options.emplace(std::make_pair("B", 
    option{"alphabet2",
           ALPHABET_B, 1, {{ALPH}},
           "for transducers, set the output alphabet. See alphabet",
           awali::cora::doc::alphabet2}));

  options.emplace(std::make_pair("W", 
    option{"weightset",
           WEIGHTS, 1, {{SMR}},
           "set the weightset for a new automaton or ratexp",
           awali::cora::doc::weightset}));  

  options.emplace(std::make_pair("I",
    option{"input-fmt",
           INPUT_FMT, 1, {{FMT}},
           "set input format for automata and ratexps",
           awali::cora::doc::input_format}));

  options.emplace(std::make_pair("O",
    option{"output-fmt",
           OUTPUT_FMT, 1, {{FMT}},
           "set output format for the automaton or ratexp produced by the command.",
           awali::cora::doc::output_format}));

  options.emplace(std::make_pair("S", 
    option{"script-mode",
           SHELL, 0, {},
           "Compatibility mode for an easy use in a shell script",
           awali::cora::doc::script_mode}));

  options.emplace(std::make_pair("V",
    option{"verbose",
           VERBOSE, 0, {},
           "print result of test commands (set by default)",
           awali::cora::doc::verbose}));

  options.emplace(std::make_pair("M",
    option{"method",
           METHOD, 1,{{MTD}},
           "set the 'method' used by some commands for the computation of their result.",
           awali::cora::doc::method}));

  options.emplace(std::make_pair("H", 
    option{"history",
           HISTORY, 0, {},
           "print state history in dot format or in display",
            awali::cora::doc::history}));

  options.emplace(std::make_pair("N",
    option{"name",
           NAME, 1,{{STR}},
           "sets the name of the automaton produced by the command",
           awali::cora::doc::name}));

  options.emplace(std::make_pair("C",
    option{"caption",
           CAPTION, 1,{{STR}},
           "sets caption of the automaton (visible in the metadata only)",
           awali::cora::doc::caption}));

//   options.emplace(std::make_pair("T",
//     option{"title",
//            TITLE, 1,{{STR}},
//            "sets the name of the window opened by the command display",
//            awali::cora::doc::title}));

}

// // End of file cora_options.hh --- return to cora.hh

// Description of struct by js 20201110
// struct option {              //  format for option
//   std::string name;               // name?  always empty string
//   std::string desc;               // long-option name
//   std::string longdesc;           // long description
//   std::string optiondesc;         // option description
//   std::vector<argument_t> args;   // list of types of arguments
//   unsigned argc;                  // nb of arguments of command
//   coms_t cst;                     // id of option
// };


