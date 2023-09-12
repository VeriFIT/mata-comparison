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

#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/sttc/algos/scc.hh>

#include<set-types.hh>

namespace awali {

  extern "C"
  std::pair< std::unordered_map<dyn::state_t, dyn::state_t>,
             std::vector<std::vector<dyn::state_t>> >
  scc_recursive(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::scc_recursive(a);
  }


  extern "C"
  std::pair< std::unordered_map<dyn::state_t, dyn::state_t>,
             std::vector<std::vector<dyn::state_t>> >
  scc_iterative(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::scc_iterative(a);
  }


  extern "C"
  std::pair< dyn::automaton_t,
             std::pair< std::unordered_map<dyn::state_t, dyn::state_t>,
                        std::vector<std::vector<dyn::state_t>> >  >
  condensation (dyn::automaton_t aut)
  {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    auto res= sttc::condensation(a);
    return { dyn::make_automaton(res.first), std::move(res.second) };
  }

  extern "C"
  std::vector<dyn::state_t>
  scc_of (dyn::automaton_t aut, dyn::state_t s)
  {
    auto sttc_aut= dyn::get_stc_automaton<context_t>(aut);
    return sttc::scc_of(sttc_aut, s);
  }
}
