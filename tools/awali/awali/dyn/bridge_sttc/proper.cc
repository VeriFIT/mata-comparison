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

#include <awali/common/priority.hh>
#include <awali/sttc/algos/lal_lan_conversion.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/algos/thompson.hh>
#include <awali/sttc/algos/draw_exp.hh>
#include <awali/sttc/algos/weighted_thompson.hh>
#include <awali/sttc/algos/compact_thompson.hh>
#include <awali/sttc/algos/copy.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/dyn/bridge_sttc/explicit_ratexp.cc>

#include <awali/common/enums.hh>

#include<set-types.hh>

namespace awali {

  extern "C" void proper_here(dyn::automaton_t aut, direction_t dir, bool prune) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    sttc::proper_here(a, dir, prune);
  }

  extern "C" dyn::automaton_t proper(dyn::automaton_t aut, direction_t dir,
                                     bool prune, bool keep_history) 
  {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::to_lal(a,dir,prune, keep_history));;
  }

  extern "C" dyn::automaton_t allow_eps_transition(dyn::automaton_t aut, 
  bool keep_history) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::to_lan(a,keep_history));;
  }

  extern "C" bool is_proper(dyn::automaton_t aut) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return sttc::is_proper(a);
  }

  extern "C" bool is_valid(dyn::automaton_t aut) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return sttc::is_valid(a);
  }

  extern "C" dyn::automaton_t thompson(dyn::ratexp_t exp) {
    auto e= get_stc_ratexp<context_t>(exp);
    auto rs = get_stc_ratexpset<context_t>(exp);
    using aut_t = sttc::mutable_automaton<sttc::nullable_of<context_t>>;
    return dyn::make_automaton(sttc::thompson<aut_t>(sttc::get_nullable_context(rs.context()), e));
  }

  extern "C" dyn::automaton_t compact_thompson(dyn::ratexp_t exp) {
    auto e= get_stc_ratexp<context_t>(exp);
    auto rs = get_stc_ratexpset<context_t>(exp);
    using aut_t = sttc::mutable_automaton<sttc::nullable_of<context_t>>;
    return dyn::make_automaton(sttc::compact_thompson<aut_t>(sttc::get_nullable_context(rs.context()), e));
  }

  extern "C" dyn::automaton_t weighted_thompson(dyn::ratexp_t exp) {
    auto e= get_stc_ratexp<context_t>(exp);
    auto rs = get_stc_ratexpset<context_t>(exp);
    using aut_t = sttc::mutable_automaton<sttc::nullable_of<context_t>>;
    return dyn::make_automaton(sttc::weighted_thompson<aut_t>(sttc::get_nullable_context(rs.context()), e));
  }

  extern "C" dyn::automaton_t draw_exp(dyn::ratexp_t exp) {
    auto e= get_stc_ratexp<context_t>(exp);
    auto rs = get_stc_ratexpset<context_t>(exp);
    return dyn::make_automaton(sttc::draw_exp(rs, e));
  }
}

#include <awali/dyn/core/any.cc>
