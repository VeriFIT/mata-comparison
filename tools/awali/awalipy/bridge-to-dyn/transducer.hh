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

#ifndef DYN_SIMPLE_TRANSDUCER_HH
#define DYN_SIMPLE_TRANSDUCER_HH

#include <awalipy/bridge-to-dyn/basic_automaton.hh>
#include <awali/dyn/core/transducer.hh>

namespace awali { namespace py {

  struct simple_transducer_t : basic_automaton_t {
  
  private:
    dyn::transducer_t tdc() const { return (dyn::transducer_t) aut_; }
  
  public:
    //Initialisation and conversion
    simple_transducer_t() : basic_automaton_t() {}

    simple_transducer_t(const dyn::automaton_t& a) : basic_automaton_t(a) {}
    simple_transducer_t(const dyn::transducer_t& a) : basic_automaton_t((const dyn::automaton_t&) a) {}

    unsigned num_tapes() {
      return tdc().num_tapes();
    }

    operator dyn::transducer_t() const {
      return tdc();
    }

    dyn::transition_t get_transition(int src, int dst, const std::vector<std::string>& label) const {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return tdc().get_transition(src+2, dst+2, label);
    }

    bool has_transition3(int src, int dst, const std::vector<std::string>&  label) const {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return tdc().has_transition(src+2, dst+2, label);
    }

    void del_transition3(int src, int dst, const std::vector<std::string>& label) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      tdc().del_transition(src+2, dst+2, label);
    }

    std::vector<std::string> label_of(dyn::transition_t t) const {
      verify_existence_of_transition(t);
      return tdc().label_of(t);
    }

    dyn::transition_t set_transition4(int src, int dst, const std::vector<std::string>& label, const std::string& weight) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return tdc().set_transition(src+2, dst+2, label, aut_->get_context()->get_weight(weight));
    }

    dyn::transition_t set_transition3(int src, int dst, const std::vector<std::string>& label) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return tdc().set_transition(src+2, dst+2, label);
    }

    std::string add_transition4(int src, int dst, const std::vector<std::string>& label, const std::string& weight) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return weight_to_string( 
        tdc().add_transition(src+2, dst+2, label, 
                             aut_->get_context()->get_weight(weight))
      );
    }

    std::string add_transition3(int src, int dst, const std::vector<std::string>& label) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return weight_to_string(tdc().add_transition(src+2, dst+2, label));
    }

    std::vector<int> successors2(int src, const std::vector<std::string>& label) const {
      verify_existence_of_state(src);
      std::vector<int> r;
      for(auto s: tdc().successors(src+2, label))
	r.emplace_back(s-2);
      return r;
    }

    std::vector<int> predecessors2(int src, const std::vector<std::string>& label) const {
      verify_existence_of_state(src);
      std::vector<int> r;
      for(auto s: tdc().predecessors(src+2, label))
	r.emplace_back(s-2);
      return r;
    }

    std::vector<dyn::transition_t> incoming2(int s, const std::vector<std::string>& label) const {
      verify_existence_of_state(s);
      return tdc().in(s+2, label);
    }

    std::vector<dyn::transition_t> outgoing2(int s, const std::vector<std::string>& label) const {
      verify_existence_of_state(s);
      return tdc().out(s+2, label);
    }

    std::vector<dyn::transition_t> in2(int s, const std::vector<std::string>& label) const {
      verify_existence_of_state(s);
      return tdc().in(s+2, label);
    }

    std::vector<dyn::transition_t> out2(int s, const std::vector<std::string>& label) const {
      verify_existence_of_state(s);
      return tdc().out(s+2, label);
    }

    std::vector<std::string> alphabets() const {
      std::vector<std::string> alphs;
      for (std::vector<char> one_alphabet: tdc().alphabets()) {
        std::string alph= "";
        for (char letter: one_alphabet)
            alph+=letter;
        alphs.emplace_back(alph);
      }
      return alphs;
    }

    bool has_letter(const std::string& l, unsigned tape) const {
      if (l.size() != 1)
        return false;
      std::vector<std::vector<char>> alphabets = tdc().alphabets();
      if (tape >= alphabets.size())
        throw std::invalid_argument("Second argument not in valid range. This transducer only has "+std::to_string(alphabets.size())+" tapes.");
      for (char c : alphabets.at(tape))
        if (c == l[0])
          return true;
      return false;
    }
  };

  simple_transducer_t make_simple_transducer2(std::vector<std::string> alphabets, std::string semiring) {
    return dyn::transducer_t::from(alphabets, semiring);
  }


  simple_transducer_t make_simple_transducer1(std::vector<std::string> alphabets) {
    return dyn::transducer_t::from(alphabets);
  }

}}// end of namespace awali::py

#endif
