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

#include<string>
#include <awali/sttc/history/string_history.hh>
#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/are_isomorphic.hh>
#include <awali/sttc/algos/left_mult.hh>
#include <awali/sttc/algos/add_path.hh>
#include <awali/sttc/algos/sum.hh>
#include <awali/sttc/algos/concatenate.hh>
#include <awali/sttc/algos/restriction.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/dyn/bridge_sttc/explicit_ratexp.cc>
#include <awali/sttc/weightset/b.hh>
#include <awali/common/priority.hh>

#include<set-types.hh>

namespace awali {

  extern "C" dyn::automaton_t copy(dyn::automaton_t aut, bool history) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::copy(a, history));
  }

  extern "C" dyn::automaton_t support(dyn::automaton_t aut, bool history) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    using bcontext_t = sttc::context<labelset_t, sttc::b>;
    auto bcontext = bcontext_t(*a->context().labelset(), sttc::b{});
    auto r = sttc::make_mutable_automaton(bcontext);
    sttc::copy_support(a, r, history);
    return dyn::make_automaton(r);
  }

  extern "C" dyn::automaton_t characteristic(dyn::automaton_t aut, bool history) {
    using bcontext_t = sttc::context<labelset_t, sttc::b>;
    try {
     auto a = dyn::get_stc_automaton<bcontext_t>(aut);
      auto context = context_t(*a->context().labelset(), weightset_t{});
      auto r = sttc::make_mutable_automaton(context);
      sttc::copy_support(a, r, history);
      return dyn::make_automaton(r);
    } 
    catch (std::bad_cast const&) {
      throw std::runtime_error("characteristic is only supported for NFA");
    }
  }

  //extern "C" unsigned add_state(dyn::automaton_t aut, std::string s) {
    //auto a = dyn::get_stc_automaton<context_t>(aut);
    //auto h = a->history();
    //unsigned i = a->add_state();
    //a->set_state_name(i, s);
    //return i;
  //}

  template <typename C, typename T>
  void change_alphabet_(dyn::automaton_t aut, std::set<char>& v, priority::ONE<T>)
  {
    throw std::runtime_error("change_alphabet only supported for WFA with char alphabet");
  }
  
  template <typename C, typename T>
  void change_int_alphabet_(dyn::automaton_t aut, std::set<int>& v, priority::ONE<T>)
  {
    throw std::runtime_error("change_int_alphabet only supported for WFA with int alphabet");
  }

  template <typename C,typename T>
  auto change_alphabet_(dyn::automaton_t aut, std::set<char>& v, priority::TWO<T>) -> typename std::enable_if<std::is_same<typename C::labelset_t::letter_t,char>::value>::type
  {
    auto a = dyn::get_stc_automaton<C>(aut);
    sttc::change_alphabet(a,v);
  }
                                                                             
  template <typename C,typename T>
  auto change_int_alphabet_(dyn::automaton_t aut, std::set<int>& v, priority::TWO<T>) -> typename std::enable_if<std::is_same<typename C::labelset_t::letter_t,int>::value>::type
  {
    auto a = dyn::get_stc_automaton<C>(aut);
    sttc::change_alphabet(a,v);
  }
                                                                             
  extern "C"  void change_alphabet(dyn::automaton_t aut, std::set<char> v) {
    change_alphabet_<context_t>(aut, v, priority::value);
  }

  extern "C"  void change_int_alphabet(dyn::automaton_t aut, std::set<int> v) {
    change_int_alphabet_<context_t>(aut, v, priority::value);
  }

  extern "C" bool are_isomorphic(dyn::automaton_t aut1, dyn::automaton_t aut2) {
    auto a1 = dyn::get_stc_automaton<context_t>(aut1);
    auto a2 = dyn::get_stc_automaton<context_t>(aut2);
    return sttc::are_isomorphic(a1, a2);
  }

  extern "C" dyn::automaton_t left_mult(dyn::automaton_t aut, dyn::any_t weight) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::left_mult(a, dyn::internal::extract_value(weight, *(a->context().weightset()))));
  }

  extern "C" void left_mult_here(dyn::automaton_t aut, dyn::any_t weight) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::left_mult_here(a, 
      dyn::internal::extract_value(weight, *(a->context().weightset()))
      );
  }

  extern "C" dyn::automaton_t right_mult(dyn::automaton_t aut, dyn::any_t weight) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    return dyn::make_automaton(sttc::right_mult(a, dyn::internal::extract_value(weight, *(a->context().weightset()))));
  }

  extern "C" void right_mult_here(dyn::automaton_t aut, dyn::any_t weight) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::right_mult_here(a, dyn::internal::extract_value(weight, *(a->context().weightset())));
  }

  extern "C" void add_path_string(dyn::automaton_t aut, state_t p, state_t q, const std::string& s, bool strict_alphabet) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    sttc::add_path(a, p, q, s, strict_alphabet);
  }

  extern "C" void add_path_exp(dyn::automaton_t aut, state_t p, state_t q, dyn::ratexp_t exp) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    auto e=get_stc_ratexp<context_t>(exp);
    sttc::add_path(a, p, q, e);
  }

  extern "C" void concatenate_here(dyn::automaton_t aut, dyn::automaton_t aut2) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    try {
      auto a2=dyn::get_stc_automaton<context_t>(aut2);
      sttc::concatenate_here(a, a2);
    }
    catch (std::bad_cast const&) {
      throw std::runtime_error("concatenate_here is only supported for automata with the same context");
    }
  }

  extern "C" void sum_here(dyn::automaton_t aut, dyn::automaton_t aut2) {
    auto a=dyn::get_stc_automaton<context_t>(aut);
    try {
    auto a2=dyn::get_stc_automaton<context_t>(aut2);
    sttc::sum_here(a, a2);
    }
    catch (std::bad_cast const&) {
      throw std::runtime_error("concatenate_here is only supported for automata with the same context");
    }
  }
  
}//end of ns awali

