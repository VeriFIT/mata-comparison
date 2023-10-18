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

#ifndef AWALI_HISTORY_HH
# define AWALI_HISTORY_HH

# include <iostream>
#include<vector>
#include <awali/common/types.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/ctx/traits.hh>

namespace awali {
  namespace sttc {


    //   enum class history_kind_t {
    //     SINGLE, TUPLE, RATEXP, STRING, NO_HISTORY, PARTITION
    //   };
    /** @brief base type for history of automata
     *
     * Every automaton is connected with two history objects:    
     * - an object that carries the history of the states;
     *   this object can be updated by some algorithms;    
     * - a {@link string_history} that carries the name of states.     
     */
    class history_base {
    public:

      virtual std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt = "text") const = 0;


      virtual history_kind_t get_nature() const =0;

      virtual bool has_history() const {
        return true;
      }

      virtual bool has_history(state_t) const =0;

      virtual bool remove_history(state_t) =0;

      template<typename H>
      H& as()
      {
        return dynamic_cast<H&>(*this);
      }

      template<typename H>
      H& as() const
      {
        return dynamic_cast<const H&>(*this);
      }

      virtual state_t get_state(state_t s) = 0;
      virtual std::vector<state_t> get_state_set(state_t s) = 0;

      virtual ~history_base() {}
    };
  }
}//end of ns awali::stc

#endif // !AWALI_HISTORY_HH
