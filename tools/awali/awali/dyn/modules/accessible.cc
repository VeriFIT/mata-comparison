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

#ifndef DYN_ACCESSIBLE_CC
#define DYN_ACCESSIBLE_CC

#include <iostream>
#include <unordered_map>
#include <set>

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/loading/handler.hh>

#include <awali/dyn/modules/accessible.hh>

namespace awali {
  namespace dyn {


    std::set<state_t>
    accessible_states (automaton_t aut)
    {
      return loading::call1<std::set<state_t>>("accessible_states",
                                               "accessible", aut);
    }

    std::set<state_t>
    coaccessible_states (automaton_t aut)
    {
      return loading::call1<std::set<state_t>>("coaccessible_states",
                                               "accessible", aut);
    }

    std::set<state_t>
    useful_states(automaton_t aut)
    {
      return loading::call1<std::set<state_t>>("useful_states", "accessible",
                                               aut);
    }

    size_t num_accessible_states(automaton_t aut)
    {
      return loading::call1<size_t>("num_accessible_states", "accessible", aut);
    }

    size_t num_coaccessible_states(automaton_t aut)
    {
      return loading::call1<size_t>("num_coaccessible_states", "accessible",
                                    aut);
    }

    automaton_t accessible(automaton_t aut, options_t opts)
    {
      bool keep_history = opts[KEEP_HISTORY];
      if(opts[IN_PLACE]) {
        loading::call1<void>("accessible_here", "accessible", aut,
                             keep_history);
        return aut;
      }
      return loading::call1<automaton_t>("accessible", "accessible", aut, opts);
    }

    automaton_t coaccessible(automaton_t aut, options_t opts)
    {
      bool keep_history = opts[KEEP_HISTORY];
      if (opts[IN_PLACE]) {
        loading::call1<void>("coaccessible_here", "accessible", aut,
                             keep_history);
        return aut;
      }
      return loading::call1<automaton_t>("coaccessible", "accessible", aut,
                                         keep_history);
    }

    automaton_t trim(automaton_t aut, options_t opts)
    {
      bool keep_history = opts[KEEP_HISTORY];
      if (opts[IN_PLACE]) {
        loading::call1<void>("trim_here", "accessible", aut, keep_history);
        return aut;
      }
      return loading::call1<automaton_t>("trim", "accessible", aut,
                                         keep_history);

    }

    bool is_trim(automaton_t aut)
    {
      return loading::call1<bool>("is_trim", "accessible", aut);
    }

    bool is_useless(automaton_t aut)
    {
      return loading::call1<bool>("is_useless", "accessible", aut);
    }

    bool is_accessible(automaton_t aut)
    {
      return loading::call1<bool>("is_accessible", "accessible", aut);
    }

    bool is_coaccessible(automaton_t aut)
    {
      return loading::call1<bool>("is_coaccessible", "accessible", aut);
    }

    bool is_empty(automaton_t aut)
    {
      return loading::call1<bool>("is_empty", "accessible", aut);
    }
  }
}//end of ns awali::dyn

#endif
