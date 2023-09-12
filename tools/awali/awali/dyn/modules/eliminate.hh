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

#ifndef DYN_ELIMINATE_HH
#define DYN_ELIMINATE_HH

#include <awali/dyn/core/automaton.hh>

//Only for lao

namespace awali {
  namespace dyn {

    namespace lift {
      /** Deletes a state from a lifted automaton.
       *
       * @param aut Lifted automaton in which the state will be deleted
       * @param s state to delete
       * @pre @pname{aut} needs to accept rational expressions as labels; hence
       * must result from function {@link lift}.
       */
      void eliminate_state(automaton_t aut, state_t s);

      /** Deletes a state from a lifted automaton.
       *
       * @param aut Lifted automaton in which the state will be deleted
       * @param state_name Name of the state to delete
       * @pre @pname{aut} needs to accept rational expressions as labels; hence
       * must result from function {@link lift}.
       * 
       */
      void eliminate_state_by_name(automaton_t aut, std::string state_name);
    }
  }
}//end of ns awali::dyn

#endif
