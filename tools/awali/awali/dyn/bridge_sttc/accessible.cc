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

#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/sttc/algos/accessible.hh>

#include<set-types.hh> 

namespace awali {

  extern "C" std::set<unsigned> accessible_states(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::accessible_states(a);
  }

  extern "C" std::set<unsigned> coaccessible_states(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::coaccessible_states(a);
  }

  extern "C" std::set<unsigned> useful_states(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::useful_states(a);
  }

  extern "C" size_t num_accessible_states(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::num_accessible_states(a);
  }

  extern "C" size_t num_coaccessible_states(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::num_coaccessible_states(a);
  }

  extern "C" dyn::automaton_t accessible(dyn::automaton_t aut, bool keep_history) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::accessible(a,keep_history));
  }

  extern "C" dyn::automaton_t coaccessible(dyn::automaton_t aut, bool keep_history) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::coaccessible(a,keep_history));
  }

  extern "C" dyn::automaton_t trim(dyn::automaton_t aut, bool keep_history) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::trim(a,keep_history));
  }

  extern "C" void accessible_here(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::accessible_here(a);
  }

  extern "C" void coaccessible_here(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::coaccessible_here(a);
  }

  extern "C" void trim_here(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::trim_here(a);
  }

  extern "C" bool is_trim(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::is_trim(a);
  }

  extern "C" bool is_useless(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::is_useless(a);
  }

  extern "C" bool is_accessible(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::is_accessible(a);
  }

  extern "C" bool is_coaccessible(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::is_coaccessible(a);
  }

  extern "C" bool is_empty(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::is_empty(a);
  }

}

