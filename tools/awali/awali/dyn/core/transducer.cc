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


#include <awali/dyn/core/transducer.hh>
#include <awali/dyn/modules/transducer.hh>
#include <awali/dyn/modules/context.hh>

namespace awali {
namespace dyn {

  transducer_t::transducer_t(const automaton_t& a) : aut_(a) {}

  transducer_t::transducer_t(std::istream& json) {
    aut_=internal::parse_automaton(json);
  }

  automaton_t transducer_t::operator ->() {
        return aut_;
      }

      transducer_t::operator automaton_t() {
        return aut_;
      }

      unsigned transducer_t::num_tapes() {
        return internal::num_tapes(*this);
      }

      transition_t transducer_t::get_transition(state_t src, state_t dst, const std::vector<std::string>& labels) {
        return internal::get_tdc_transition(*this, src, dst, labels);
      }

      bool transducer_t::has_transition(state_t src, state_t dst, const std::vector<std::string>& labels) {
        return internal::has_tdc_transition(*this, src, dst, labels);
      }

      transition_t transducer_t::set_transition(state_t src, state_t dst, const std::vector<std::string>& labels) {
        return internal::set_tdc_transition(*this, src, dst, labels);
      }

      transition_t transducer_t::set_transition(state_t src, state_t dst, const std::vector<std::string>& labels, weight_t w) {
        return internal::set_tdc_transition(*this, src, dst, labels, w);
      }

      weight_t transducer_t::add_transition(state_t src, state_t dst, const std::vector<std::string>& labels) {
        return internal::add_tdc_transition(*this, src, dst, labels, (*this)->get_context()->weight_one());
      }

      weight_t transducer_t::add_transition(state_t src, state_t dst, const std::vector<std::string>& labels, weight_t w) {
        return internal::add_tdc_transition(*this, src, dst, labels, w);
      }

      void transducer_t::del_transition(state_t src, state_t dst, const std::vector<std::string>& labels) {
        return internal::del_tdc_transition(*this, src, dst, labels);
      }

      std::vector<state_t> transducer_t::successors(state_t s, const std::vector<std::string>& label) {
        return internal::tdc_successors(*this, s, label);
      }

      std::vector<state_t> transducer_t::predecessors(state_t s, const std::vector<std::string>& label) {
        return internal::tdc_predecessors(*this, s, label);
      }

      std::vector<transition_t> transducer_t::in(state_t s, const std::vector<std::string>& label) {
        return internal::tdc_in(*this, s, label);
      }

      std::vector<transition_t> transducer_t::out(state_t s, const std::vector<std::string>& label) {
        return internal::tdc_out(*this, s, label);
      }

      std::vector<std::string> transducer_t::label_of(transition_t tr) {
        return internal::get_tdc_label(*this, tr);
      }

      std::vector<std::vector<char>> transducer_t::alphabets() {
        return internal::alphabets(*this);
      }


      transducer_t transducer_t::from(std::vector<std::string> alphabets, 
                                      std::string weightset)
      {
        std::vector<context::labelset_description> v;
        for(auto al : alphabets)
          v.emplace_back(context::nullableset(context::letterset(al)));
        return automaton_t(context::ltupleset(v),
                           context::weightset(weightset));
      }

}}//end of namespaces awali::dyn, and awali
