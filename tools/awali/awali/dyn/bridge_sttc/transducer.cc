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

#include<iostream>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/alphabets/char.hh>
#include <awali/sttc/algos/js_parser.hh>
#include <awali/sttc/algos/projection.hh>
#include <awali/sttc/algos/is_functional.hh>
#include <awali/sttc/algos/lift_tdc.hh>
#include <awali/sttc/algos/synchronize.hh>
#include <awali/sttc/algos/real_time.hh>
#include <awali/sttc/algos/letterize_tape.hh>
#include <awali/sttc/algos/is_of_finite_image.hh>
#include <awali/sttc/labelset/letterset.hh>
#include <awali/sttc/labelset/nullableset.hh>
#include <awali/sttc/labelset/wordset.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/sttc/misc/add_epsilon_trans.hh> //get_epsilon
#include<stdexcept> //get_epsilon

#include<set-types.hh>

namespace awali {

  static const size_t N=std::tuple_size<labelset_t::valuesets_t>::value;

  extern "C" unsigned num_tapes(dyn::automaton_t tdc) {
          return N;
  }

  template<typename LABELSET>
  struct coverstring{};

  template<typename T>
  struct coverstring<sttc::letterset<T>> {
    static typename sttc::letterset<T>::value_t get(const std::string& s) {
      if(s.length()!=1)
        throw std::runtime_error(s+": Wrong letterset value");
      return s[0];
    }

    static std::string tostring(const typename sttc::letterset<T>::value_t& v) {
      std::string s("a");
      s[0]=v;
      return s;
    }
  };

  template<typename T>
  struct coverstring<sttc::wordset<T>> {
    static const typename sttc::wordset<T>::value_t& get(const std::string& s) {
      return s;
    }

    static const std::string& tostring(const typename sttc::wordset<T>::value_t& v) {
      return v;
    }
  };

  template<typename T>
  struct coverstring<sttc::nullableset<T>> {
    static typename sttc::nullableset<T>::value_t get(const std::string& s) {
      if(s.length()>1)
        throw std::runtime_error(s+": Wrong nullableset value");
      if(s.length()==1)
        return s[0];
      return sttc::nullableset<T>::one();
    }

    static std::string tostring(const typename sttc::nullableset<T>::value_t& v) {
      if(sttc::nullableset<T>::is_one(v))
        return "";
      std::string s("a");
      s[0]=v;
      return s;
    }
  };

  template <std::size_t... I>
  labelset_t::value_t get_tuple(const std::vector<std::string>& v, awali::internal::index_sequence<I...>) {
    return labelset_t::value_t{coverstring<labelset_t::valueset_t<I>>::get(v[I])...};
  }

  labelset_t::value_t get_tuple(const std::vector<std::string>& v) {
    return get_tuple(v, labelset_t::indices);
  }

  extern "C" unsigned get_tdc_transition(dyn::automaton_t tdc, unsigned src, unsigned dst, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return td->get_transition(src,dst,get_tuple(label));
  }

  extern "C" bool has_tdc_transition(dyn::automaton_t tdc, unsigned src, unsigned dst, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return td->has_transition(src,dst,get_tuple(label));
  }

  extern "C" unsigned set_tdc_transition(dyn::automaton_t tdc, unsigned src, unsigned dst, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return td->set_transition(src,dst,get_tuple(label));
  }

  extern "C" dyn::weight_t add_tdc_transition(dyn::automaton_t tdc, unsigned src, unsigned dst, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return td->add_transition(src,dst,get_tuple(label));
  }

  extern "C" unsigned set_tdc_wtransition(dyn::automaton_t tdc, unsigned src, unsigned dst, const std::vector<std::string>& label, dyn::weight_t w) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return td->set_transition(src,dst,get_tuple(label),dyn::internal::extract_value(w, *(td->context().weightset())));
  }

  extern "C" dyn::weight_t add_tdc_wtransition(dyn::automaton_t tdc, unsigned src, unsigned dst, const std::vector<std::string>& label, dyn::weight_t w) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return td->add_transition(src,dst,get_tuple(label),dyn::internal::extract_value(w, *(td->context().weightset())));
  }

  extern "C" void del_tdc_transition(dyn::automaton_t tdc, unsigned src, unsigned dst, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    td->del_transition(src,dst,get_tuple(label));
  }

  template <std::size_t... I>
  static std::vector<std::string> get_tdc_label(dyn::automaton_t tdc, unsigned trans, awali::internal::index_sequence<I...>) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    auto l = td->label_of(trans);
    return {coverstring<labelset_t::valueset_t<I>>::tostring(std::get<I>(l))...};
  }

  extern "C" std::vector<std::string> get_tdc_label(dyn::automaton_t tdc, unsigned trans) {
    return get_tdc_label(tdc, trans, labelset_t::indices);
  }

  extern "C" std::vector<unsigned> tdc_successors(dyn::automaton_t tdc, unsigned s, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    std::vector<unsigned> res;
    for(unsigned i : td->out(s,get_tuple(label)))
      res.emplace_back(td->dst_of(i));
    return res;
  }

  extern "C" std::vector<unsigned> tdc_predecessors(dyn::automaton_t tdc, unsigned s, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    std::vector<unsigned> res;
    for(unsigned i : td->in(s,get_tuple(label)))
      res.emplace_back(td->src_of(i));
    return res;
  }

  extern "C" std::vector<unsigned> tdc_in(dyn::automaton_t tdc, unsigned s, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    std::vector<unsigned> res;
    for(unsigned i : td->out(s,get_tuple(label)))
      res.emplace_back(i);
    return res;
  }

  extern "C" std::vector<unsigned> tdc_out(dyn::automaton_t tdc, unsigned s, const std::vector<std::string>& label) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    std::vector<unsigned> res;
    for(unsigned i : td->in(s,get_tuple(label)))
      res.emplace_back(i);
    return res;
  }

  template <std::size_t N>
  static std::vector<char> alphabetN(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    std::vector<char> res;
    for(auto l : td->labelset()->set<N>().genset())
          res.emplace_back(l);
    return res;
  }

  extern "C" std::vector<char> input_alphabet(dyn::automaton_t tdc) {
    return alphabetN<0u>(tdc);
  }

  extern "C" std::vector<char> output_alphabet(dyn::automaton_t tdc) {
    return alphabetN<1u>(tdc);
  }

  template <std::size_t... I>
  static std::vector<std::vector<char>>
  alphabets(dyn::automaton_t tdc, awali::internal::index_sequence<I...>) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return {alphabetN<I>(tdc)...};
  }

  extern "C" std::vector<std::vector<char>> alphabets(dyn::automaton_t tdc) {
    return alphabets(tdc, labelset_t::indices);
  }

  extern "C" dyn::automaton_t domain(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return dyn::make_automaton(sttc::projection<0>(td));
  }

  extern "C" dyn::automaton_t image(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return dyn::make_automaton(sttc::projection<1>(td));
  }

  extern "C" dyn::automaton_t images(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return dyn::make_automaton(sttc::images(td));
  }

  extern "C" dyn::automaton_t inverse(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return dyn::make_automaton(sttc::projections<1,0>(td));
  }

  extern "C" bool is_functional(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return sttc::is_functional(td);
  }

  extern "C" dyn::automaton_t lift_tdc(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return dyn::make_automaton(sttc::lift_tdc(td));
  }

  extern "C" bool is_synchronizable(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return sttc::is_synchronizable(td);
  }

  extern "C" dyn::automaton_t synchronize(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return dyn::make_automaton(sttc::synchronize(td));
  }

  extern "C" dyn::automaton_t realtime(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return dyn::make_automaton(sttc::realtime(td));
  }

  extern "C" bool is_realtime(dyn::automaton_t tdc) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    return sttc::is_realtime(td);
  }

  extern "C" dyn::automaton_t letterize_tape(dyn::automaton_t tdc, unsigned i) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    if(i==0)
      return dyn::make_automaton(sttc::letterize_tape<0>(td));
    else
      return dyn::make_automaton(sttc::letterize_tape<1>(td));
  }

  extern "C" bool is_of_finite_image(dyn::automaton_t tdc, unsigned i) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    if(i==1)
      return sttc::is_of_finite_image<1>(td);
    else
      return sttc::is_of_finite_image<0>(td);
  }

  template <std::size_t... I>
  labelset_t::value_t special_tuple(awali::internal::index_sequence<I...>) {
    return labelset_t::value_t{labelset_t::valueset_t<I>::special()...};
  }

  extern "C" void set_final_output(dyn::automaton_t tdc, unsigned src, const std::string& output) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    auto spec = special_tuple(labelset_t::indices);
    if(!output.empty())
      std::get<1>(spec)=coverstring<labelset_t::valueset_t<1>>::get(output);
    td->set_transition(src, td->post(), spec);
  }

  extern "C" std::string get_final_output(dyn::automaton_t tdc, unsigned src) {
    auto td = dyn::get_stc_automaton<context_t>(tdc);
    auto spec = special_tuple(labelset_t::indices);
    for(auto tr : td->outin(src,td->post()))
      if(td->label_of(tr)==spec)
        return "";
      else
        return coverstring<labelset_t::valueset_t<1>>::tostring(std::get<1>(td->label_of(tr)));
    throw std::runtime_error("get_final_output: not final state");
  }

}

