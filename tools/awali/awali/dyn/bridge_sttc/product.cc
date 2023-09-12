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

#include <awali/common/priority.hh>
#include <awali/sttc/algos/product.hh>
#include <awali/sttc/algos/sum.hh>
#include <awali/sttc/algos/concatenate.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>

#include<set-types.hh>

namespace awali {
  struct False{ static const bool value=false;};
  
  template<typename Bool, typename P>
  dyn::automaton_t product(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history, priority::ONE<P>) {
    throw std::runtime_error("product only supported for automata over letters"); 
  }
    
  template<typename Bool, typename P>
  dyn::automaton_t shuffle(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history, priority::ONE<P>) {
    throw std::runtime_error("shuffle only supported for automata over letters"); 
  }

  template<typename Bool, typename P>
  dyn::automaton_t infiltration(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history, priority::ONE<P>) {
    throw std::runtime_error("infiltration only supported for automata over letters"); 
  }

  template<typename Bool, typename P>
  auto
  product(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history, priority::TWO<P>)
    -> typename std::enable_if<Bool::value || labelset1_t::is_free() && labelset2_t::is_free(),
                               dyn::automaton_t>::type
  {
    auto a1 = dyn::get_stc_automaton<context1_t>(aut1);
    auto a2 = dyn::get_stc_automaton<context2_t>(aut2);
    auto res = sttc::product(a1, a2, keep_history);
    return dyn::make_automaton(res);
  }
    
  template<typename Bool, typename P>
  auto
  shuffle(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history, priority::TWO<P>)
    -> typename std::enable_if<Bool::value || labelset1_t::is_free() && labelset2_t::is_free(),
                               dyn::automaton_t>::type
  {
    auto a1 = dyn::get_stc_automaton<context1_t>(aut1);
    auto a2 = dyn::get_stc_automaton<context2_t>(aut2);
    auto res = sttc::shuffle(a1, a2, keep_history);
    return dyn::make_automaton(res);
  }

  template<typename Bool, typename P>
  auto
  infiltration(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history, priority::TWO<P>)
    -> typename std::enable_if<Bool::value || labelset1_t::is_free() && labelset2_t::is_free(),
                               dyn::automaton_t>::type
  {
    auto a1 = dyn::get_stc_automaton<context1_t>(aut1);
    auto a2 = dyn::get_stc_automaton<context2_t>(aut2);
    auto res = sttc::infiltration(a1, a2, keep_history);
    return dyn::make_automaton(res);
  }

  extern "C" dyn::automaton_t product(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history) {
    return product<False>(aut1, aut2, keep_history, priority::value);
  }

  extern "C" dyn::automaton_t shuffle(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history) {
    return shuffle<False>(aut1, aut2, keep_history, priority::value);
  }

  extern "C" dyn::automaton_t infiltration(dyn::automaton_t aut1, dyn::automaton_t aut2, bool keep_history) {
    return infiltration<False>(aut1, aut2, keep_history, priority::value);
  }

  extern "C" dyn::automaton_t concatenate(dyn::automaton_t aut, dyn::automaton_t aut2) {
    auto a=dyn::get_stc_automaton<context1_t>(aut);
    auto a2=dyn::get_stc_automaton<context2_t>(aut2);
    return dyn::make_automaton(sttc::concatenate(a,a2));
  }
  
  extern "C" dyn::automaton_t sum(dyn::automaton_t aut, dyn::automaton_t aut2) {
    auto a=dyn::get_stc_automaton<context1_t>(aut);
    auto a2=dyn::get_stc_automaton<context2_t>(aut2);
    return dyn::make_automaton(sttc::sum(a,a2));
  }

}

#include <awali/dyn/core/any.cc>
