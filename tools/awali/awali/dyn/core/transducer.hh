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

/* @file transducer.hh
 *  This files contains the definition of the transducer type for the dynamic library
 *
 *  It allows direct access to some services which are also accessible through functions
 */
#ifndef DYN_TRANSDUCER_HH
#define DYN_TRANSDUCER_HH

#include<awali/dyn/core/automaton.hh>

namespace awali {
  namespace dyn {

    struct transducer_t { 
    // A transducer wraps an automaton and may be converted to one
    private:
      automaton_t aut_;
    public:
      transducer_t(const automaton_t& a);
      transducer_t(std::istream& json);

      //For compatibility with the behaviour of automata
      automaton_t operator ->();

      operator automaton_t();

      unsigned num_tapes();
      
      transition_t get_transition(state_t src, state_t dst, 
        const std::vector<std::string>& labels);
      
      bool has_transition(state_t src, state_t dst, 
        const std::vector<std::string>& labels);

      transition_t set_transition(state_t src, state_t dst, 
        const std::vector<std::string>& labels);

      transition_t set_transition(state_t src, state_t dst, 
        const std::vector<std::string>& labels, weight_t w);

      weight_t add_transition(state_t src, state_t dst, 
        const std::vector<std::string>& labels);

      weight_t add_transition(state_t src, state_t dst, 
        const std::vector<std::string>& labels, weight_t w);

      void del_transition(state_t src, state_t dst, 
        const std::vector<std::string>& labels);

      std::vector<state_t> successors(state_t s, 
        const std::vector<std::string>& label);

      std::vector<state_t> predecessors(state_t s, 
        const std::vector<std::string>& label);

      std::vector<transition_t> in(state_t s, 
        const std::vector<std::string>& label);

      std::vector<transition_t> out(state_t s, 
        const std::vector<std::string>& label);

      std::vector<std::string> label_of(transition_t tr);

      std::vector<std::vector<char>> alphabets();

      static transducer_t from(std::vector<std::string> alphabets, 
                               std::string weightset = "B");
    };

  }
}//end of ns awali::dyn
#endif
