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
#include <awali/sttc/algos/factor.hh>

#include<set-types.hh>

namespace awali {

  extern "C" dyn::automaton_t prefix(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::prefix(a));
  }

  extern "C" void prefix_here(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::prefix_here(a);
  }

  extern "C" dyn::automaton_t suffix(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::suffix(a));
  }

  extern "C" void suffix_here(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::suffix_here(a);
  }

  extern "C" dyn::automaton_t factor(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::factor(a));
  }

  extern "C" void factor_here(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::factor_here(a);
  }

}
