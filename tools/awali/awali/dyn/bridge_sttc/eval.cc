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

#include <awali/sttc/algos/enumerate.hh>
#include <awali/sttc/algos/eval.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/common/priority.hh>
#include<set-types.hh>

namespace awali {

  template <typename C, typename T>
  dyn::weight_t eval_(dyn::automaton_t aut, dyn::any_t word, priority::ONE<T>)
  {
    throw std::runtime_error("eval only supported for free label-sets.");
  }

  template <typename C, typename T>
  auto eval_(dyn::automaton_t aut, dyn::any_t word,  priority::TWO<T>)
    -> typename std::enable_if<C::labelset_t::is_free(), dyn::weight_t>::type
  {
    auto a=dyn::get_stc_automaton<C>(aut);

    return sttc::eval(a, 
      dyn::internal::extract_word(word, *(a->context().labelset())));
  }

  extern "C" dyn::weight_t eval(dyn::automaton_t aut, dyn::any_t word) {
    return eval_<context_t>(aut, word, priority::value);
  }

  //   template <typename C>
  //   auto eval_(dyn::automaton_t aut, dyn::any_t word)
  //   -> typename std::enable_if<!C::labelset_t::is_free(), dyn::weight_t>::type
  //   {
  //     throw std::runtime_error("eval only supported for free label-sets.");
  //   }
  //
  //   template <typename C>
  //   auto eval_(dyn::automaton_t aut, dyn::any_t word)
  //   -> typename std::enable_if<C::labelset_t::is_free(), dyn::weight_t>::type
  //   {
  //     auto a=dyn::get_stc_automaton<context_t>(aut);
  //     return sttc::eval(a, word);
  //   }

  namespace internal {
    template<typename C, typename T>
    std::map<dyn::any_t,dyn::weight_t>
    enumerate(dyn::automaton_t aut, unsigned max, priority::ONE<T>)
    {
      throw std::runtime_error("enumerate only supported for free label-sets.");
    }

    template<typename C, typename T>
    auto enumerate(dyn::automaton_t aut, unsigned max, priority::TWO<T>)
      -> typename std::enable_if< C::labelset_t::is_free(),
                                  std::map<dyn::any_t,dyn::weight_t> >::type
    {
      auto a=dyn::get_stc_automaton<C>(aut);
      auto tmp=sttc::enumerate(a,max);
      std::map<dyn::any_t,dyn::weight_t> res;
      for(auto p : tmp)
        res.emplace(p);
      return res;
    }
  }

  extern "C" std::map<dyn::any_t,dyn::weight_t> enumerate(dyn::automaton_t aut, unsigned max) {
    return internal::enumerate<context_t>(aut, max, priority::value);
  }


  namespace internal {
    template <typename C, typename T>
    auto shortest(dyn::automaton_t aut, unsigned max, priority::ONE<T>)
      -> std::map<dyn::any_t,dyn::weight_t>
    {
      throw std::runtime_error("shortest only supported for free label-sets.");
    }

    template <typename C, typename T>
    auto shortest(dyn::automaton_t aut, unsigned max, priority::TWO<T>)
    -> typename std::enable_if< C::labelset_t::is_free(),
                                std::map<dyn::any_t,dyn::weight_t> >::type
    {
      auto a=dyn::get_stc_automaton<C>(aut);
      auto tmp=sttc::shortest(a,max);
      std::map<dyn::any_t,dyn::weight_t> res;
      for(auto p : tmp)
        res.emplace(p);
      return res;
    }
  }



  extern "C" std::map<dyn::any_t,dyn::weight_t> shortest(dyn::automaton_t aut, unsigned max) {
    return internal::shortest<context_t>(aut,max,priority::value);
  }

}

#include <awali/dyn/core/any.cc>
