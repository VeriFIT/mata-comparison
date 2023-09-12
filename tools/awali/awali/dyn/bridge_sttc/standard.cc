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

#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/algos/standard.hh>
#include <awali/sttc/algos/star.hh>
#include <awali/sttc/algos/sum.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/dyn/bridge_sttc/explicit_ratexp.cc>

#include<set-types.hh>

namespace awali {

  extern "C" bool is_standard(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return sttc::is_standard(a);
  }

  extern "C" dyn::automaton_t standard(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::standard(a));
  }

  extern "C" void standard_here(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::standard_here(a);
  }

  extern "C" dyn::automaton_t sum_of_standard(dyn::automaton_t aut, dyn::automaton_t aut2) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    auto a2=dyn::get_stc_automaton<context_t>(aut2);
    return dyn::make_automaton(sttc::sum(a,a2, true));
  }
  
  extern "C" void sum_of_standard_here(dyn::automaton_t aut, dyn::automaton_t aut2) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    auto a2=dyn::get_stc_automaton<context_t>(aut2);
    sttc::sum_here(a, a2, true);
  }
  
  extern "C" dyn::automaton_t star(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::star(a));
  }

  extern "C" void star_here(dyn::automaton_t aut) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::star_here(a);
  }

  extern "C" dyn::automaton_t standard_of(dyn::ratexp_t exp) {
    auto e= get_stc_ratexp<context_t>(exp);
    auto rs = get_stc_ratexpset<context_t>(exp);
    using aut_t = sttc::mutable_automaton<context_t>;
    return dyn::make_automaton(sttc::standard<aut_t>(rs.context(), e));
  }

}

#include <awali/dyn/core/any.cc>
