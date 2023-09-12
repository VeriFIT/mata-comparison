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

#ifndef DYN_SIMPLE_AUTOMATON_HH
#define DYN_SIMPLE_AUTOMATON_HH

#include <awalipy/bridge-to-dyn/basic_automaton.hh>

namespace awali { namespace py {

  struct simple_automaton_t : basic_automaton_t {

  dyn::any_t label_of(const std::string& label) const {
    return aut_->get_context()->get_label(label==""?"\\e":label);
  }
  public:
    //Initialisation and conversion
    simple_automaton_t() : basic_automaton_t() {}

    simple_automaton_t(const dyn::automaton_t& a) : basic_automaton_t(a) {}

    dyn::transition_t get_transition(int src, int dst, const std::string& label) const {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return aut_->get_transition(src+2, dst+2, label_of(label));
    }

    bool has_transition3(int src, int dst, const std::string&  label) const {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return aut_->has_transition(src+2, dst+2, label_of(label));
    }

    void del_transition3(int src, int dst, const std::string& label) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      aut_->del_transition(src+2, dst+2, label_of(label));
    }

    std::string label_of(dyn::transition_t t) const {
      verify_existence_of_transition(t);
      return aut_->get_context()->label_to_string(aut_->label_of(t));
    }

    dyn::transition_t set_transition4(int src, int dst, const std::string& label, const std::string& weight) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return aut_->set_transition(src+2, dst+2, label_of(label), aut_->get_context()->get_weight(weight));
    }

    dyn::transition_t set_transition3(int src, int dst, const std::string& label) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return aut_->set_transition(src+2, dst+2, label_of(label));
    }

    std::string add_transition4(int src, int dst, const std::string& label, const std::string& weight) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return weight_to_string(
        aut_->add_transition(src+2, dst+2, label_of(label), 
                             aut_->get_context()->get_weight(weight)));
    }

    std::string add_transition3(int src, int dst, const std::string& label) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return weight_to_string(
        aut_->add_transition(src+2, dst+2, label_of(label)));
    }

//     void set_tdc_transition3(state_t src, state_t dst, const std::vector<std::string>& labels) {
//       dyn::set_tdc_transition (aut_, src+2, dst+2, labels);
//     }

    std::vector<int> successors2(int src, const std::string& label) const {
      verify_existence_of_state(src);
      std::vector<int> r;
      for(auto s: aut_->successors(src+2, label_of(label)))
	r.emplace_back(s-2);
      return r;
    }

    std::vector<int> predecessors2(int src, const std::string& label) const {
      verify_existence_of_state(src);
      std::vector<int> r;
      for(auto s: aut_->predecessors(src+2, label_of(label)))
	r.emplace_back(s-2);
      return r;
    }

    std::vector<dyn::transition_t> incoming2(int s, const std::string& label) const {
      verify_existence_of_state(s);
      return aut_->incoming(s+2, label_of(label));
    }

    std::vector<dyn::transition_t> outgoing2(int s, const std::string& label) const {
      verify_existence_of_state(s);
      return aut_->outgoing(s+2, label_of(label));
    }

    std::vector<dyn::transition_t> in2(int s, const std::string& label) const {
      verify_existence_of_state(s);
      return aut_->in(s+2, label_of(label));
    }

    std::vector<dyn::transition_t> out2(int s, const std::string& label) const {
      verify_existence_of_state(s);
      return aut_->out(s+2, label_of(label));
    }

    std::string alphabet() const {
      std::string alph= "";
      for(auto l : aut_->alphabet())
	alph+= this->label_to_string(l);
      return alph;
    }

//     std::vector< std::vector< std::string > > alphabets() const { // ugly
//       std::vector<std::vector<std::string>> v;
//       for(auto a : dyn::alphabets(aut_)) {
//         std::vector<std::string> v2;
//         for(auto l : a)
//           v2.emplace_back(any_to_string(l));
//         v.emplace_back(v2);
//       }
//       return v;
//     }


    bool has_letter(const std::string& l) const {
      return aut_->get_context()->has_letter(l[0]);
    }
  };

  simple_automaton_t make_simple_automaton2(std::string alphabet, std::string semiring) {
    return dyn::automaton_t::from(alphabet, semiring);
  }


  simple_automaton_t make_simple_automaton1(std::string alphabet) {
    return dyn::automaton_t::from(alphabet);
  }

//   simple_automaton_t make_simple_transducer1(std::vector<std::string> alphabets) {
//     return dyn::make_transducer(alphabets);
//   }


}}// end of namespace awali::py

#endif
