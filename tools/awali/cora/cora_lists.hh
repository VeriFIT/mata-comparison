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

/* ---------------------------------------------------|
|  Called from cora.hh as fake modularisation         |
|----------------------------------------------------*/

/* ---------------------------------------|
|  Managing  lists of choices             |
|----------------------------------------*/

// list of choice indexes to be used in some cora lists
enum chc_t {
  // double_type
  DBL_CHC,
  // is_choices
  MPTY_CHC, ACC_CHC, COACC_CHC, TRIM_CHC, USELESS_CHC, STANDARD_CHC,
  VALID_CHC, COMPLETE_CHC, PROPER_CHC, SC_CHC, DET_CHC, AMB_CHC,
  // stats_choices
  STATES_STATS, INITIALS_STATS, FINALS_STATS, TRANSITIONS_STATS,
  ACC_STATS, COACC_STATS, SC_COMPONENTS_STATS,
  SIZE_STATS, LENGTH_STATS, DEPTH_STATS, STAR_HEIGHT_STATS,
  // help_tokens for doc
//   LABELSET_TKN, // RAT_EXP_HLP,, AUT_HLP,   deprecated
  // list_choices
  COMMANDS_LST,
  BASIC_LST, GENERIC_LST, WFA_CMDS_LST, 
  EXP_CMDS_LST, NFA_CMDS_LST, TRANSDUCERS_LST,
  OPTIONS_LST, 
  AUTOMATA_LST, RATEXPS_LST, FACTORIES_LST, 
  WEIGHTSET_LST,
};

struct choice {
  std::string name;                    // name of choice
  chc_t choice_id;                     // id of choice
  std::string desc;                    // short description
  std::string longdesc;                // long description

  choice(std::string name,
         chc_t choice_id=DBL_CHC,
         std::string desc ="",
         std::string longdesc ="") :
    name(name), choice_id(choice_id), desc(desc), longdesc(longdesc) {}
};


std::vector<choice> double_type_cmds;
std::vector<choice> is_choices;
std::vector<choice> stats_choices;
// std::vector<choice> help_tokens;  // deprecated
std::vector<choice> list_choices;
std::vector<choice> doc_choices;

std::vector<choice>::iterator it_chc;

//// end of choice type definitions

void init_lists()
{
////// Filling the double_type_cmds list
  double_type_cmds.emplace_back(choice{"cat"});
  double_type_cmds.emplace_back(choice{"display"});
  double_type_cmds.emplace_back(choice{"info"});
  double_type_cmds.emplace_back(choice{"statistics"});
  double_type_cmds.emplace_back(choice{"is"});
  double_type_cmds.emplace_back(choice{"support"});
  double_type_cmds.emplace_back(choice{"characteristic"});
  double_type_cmds.emplace_back(choice{"promote"});
  double_type_cmds.emplace_back(choice{"is-valid"});
  double_type_cmds.emplace_back(choice{"minimal-automaton"});
//
///// Filling the is_choices list
  is_choices.emplace_back(choice{"empty", MPTY_CHC});
  is_choices.emplace_back(choice{"accessible", ACC_CHC});
  is_choices.emplace_back(choice{"coaccessible", COACC_CHC});
  is_choices.emplace_back(choice{"trim", TRIM_CHC});
  is_choices.emplace_back(choice{"useless", USELESS_CHC});
  is_choices.emplace_back(choice{"standard", STANDARD_CHC});
  is_choices.emplace_back(choice{"valid", VALID_CHC});
  is_choices.emplace_back(choice{"complete", COMPLETE_CHC});
  is_choices.emplace_back(choice{"proper", PROPER_CHC});
  is_choices.emplace_back(choice{"strongly-connected", SC_CHC});
  is_choices.emplace_back(choice{"deterministic", DET_CHC});
  is_choices.emplace_back(choice{"ambiguous", AMB_CHC});
//
///// Filling the stats_choices list
  stats_choices.emplace_back(choice{"states", STATES_STATS});
  stats_choices.emplace_back(choice{"initials", INITIALS_STATS});
  stats_choices.emplace_back(choice{"finals", FINALS_STATS});
  stats_choices.emplace_back(choice{"transitions", TRANSITIONS_STATS});
  stats_choices.emplace_back(choice{"acc-states", ACC_STATS});
  stats_choices.emplace_back(choice{"coacc-states", COACC_STATS});
  stats_choices.emplace_back(choice{"sc-components", SC_COMPONENTS_STATS});
  stats_choices.emplace_back(choice{"size", SIZE_STATS});
  stats_choices.emplace_back(choice{"length", LENGTH_STATS});
  stats_choices.emplace_back(choice{"depth", DEPTH_STATS});
  stats_choices.emplace_back(choice{"star-height", STAR_HEIGHT_STATS});
//
////// Filling the help_tokens list   deprecated
//   help_tokens.emplace_back(choice{"labelset", LABELSET_TKN, "blabla", "blibli"});
//
////// Filling the list_choices list
  list_choices.emplace_back(choice{"commands", COMMANDS_LST});
  list_choices.emplace_back(choice{"basic-cmds", BASIC_LST});
  list_choices.emplace_back(choice{"generic-cmds", GENERIC_LST});
  list_choices.emplace_back(choice{"wfa-cmds", WFA_CMDS_LST});
  list_choices.emplace_back(choice{"ratexp-cmds", EXP_CMDS_LST});
  list_choices.emplace_back(choice{"nfa-cmds", NFA_CMDS_LST});
  list_choices.emplace_back(choice{"transducer-cmds", TRANSDUCERS_LST});
  list_choices.emplace_back(choice{"options", OPTIONS_LST});
// 
  list_choices.emplace_back(choice{"automata", AUTOMATA_LST});
  list_choices.emplace_back(choice{"ratexps", RATEXPS_LST});
  list_choices.emplace_back(choice{"factories", FACTORIES_LST});
  list_choices.emplace_back(choice{"weightsets", WEIGHTSET_LST});
}


// // End of file cora_lists.hh --- return to cora.hh

