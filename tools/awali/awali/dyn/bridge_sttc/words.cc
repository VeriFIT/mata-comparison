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
#include <awali/sttc/algos/letterize.hh>
#include <awali/sttc/algos/allow_words.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>

#include <awali/common/enums.hh>

#include<set-types.hh>

namespace awali {
 
  template<typename L, typename P>
  dyn::automaton_t allow_words(dyn::automaton_t aut, priority::ONE<P>) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::allow_words(a));
  }

  template<typename L, typename P>
  dyn::context_t context_words(dyn::automaton_t aut, priority::ONE<P>) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    auto ctx_word = get_wordset_context(a->context());
    return std::make_shared<dyn::explicit_context_t<decltype(ctx_word)>>(ctx_word);
  }

  template<typename L, typename P>
  dyn::automaton_t compact(dyn::automaton_t aut, priority::ONE<P>) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::compact(a));
  }  
  
  template<typename L, typename P>
  auto compact(dyn::automaton_t aut, priority::TWO<P>)
    -> typename std::enable_if<std::is_same<typename L::kind_t, sttc::labels_are_tuples>::value,
    dyn::automaton_t>::type
  {
    throw std::runtime_error("compact not supported for transducers");     
  }
  
  template<typename L, typename P>
  auto allow_words(dyn::automaton_t aut, priority::TWO<P>)
    -> typename std::enable_if<std::is_same<typename L::kind_t, sttc::labels_are_tuples>::value,
    dyn::automaton_t>::type
  {
    throw std::runtime_error("allow-words not supported for transducers");     
  }

  template<typename L, typename P>
  auto context_words(dyn::automaton_t aut, priority::TWO<P>)
    -> typename std::enable_if<std::is_same<typename L::kind_t, sttc::labels_are_tuples>::value,
    dyn::context_t>::type
  {
    throw std::runtime_error("context-words not supported for transducers");     
  }
                    /*
  template<typename L, typename P>
  auto compact(dyn::automaton_t aut, priority::THREE<P>)
    -> typename std::enable_if<std::is_same<typename L::genset_t::letter_t, int>::value,
    dyn::automaton_t>::type
  {
    throw std::runtime_error("compact not supported for int letters");     
  }

  template<typename L, typename P>
  auto allow_words(dyn::automaton_t aut, priority::THREE<P>)
    -> typename std::enable_if<std::is_same<typename L::genset_t::letter_t, int>::value,
    dyn::automaton_t>::type
  {
    throw std::runtime_error("allow-words not supported for int letters");     
  }
                    */

  extern "C" dyn::automaton_t allow_words(dyn::automaton_t aut) {
    return allow_words<labelset_t>(aut, priority::value);
  }

  extern "C" dyn::context_t context_words(dyn::automaton_t aut) {
    return context_words<labelset_t>(aut, priority::value);
  }

  extern "C" dyn::automaton_t compact(dyn::automaton_t aut) {
    return compact<labelset_t>(aut, priority::value);
  }

  extern "C" dyn::automaton_t letterize(dyn::automaton_t aut) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::letterize(a));
  }  
 
}

#include <awali/dyn/core/any.cc>
