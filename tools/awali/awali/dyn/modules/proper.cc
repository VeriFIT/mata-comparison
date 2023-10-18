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

#ifndef DYN_MODULES_PROPER_CC
#define DYN_MODULES_PROPER_CC

#include <unordered_map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/abstract_ratexp.hh>
#include <awali/dyn/loading/handler.hh>

#include <awali/dyn/modules/proper.hh>

namespace awali {
  namespace dyn {

    automaton_t proper(automaton_t aut, options_t opts)
    {
      if (opts[IN_PLACE]) {
        loading::call1<void>("proper_here", "proper", aut, opts[DIRECTION],
                             opts[PRUNE]);
        return aut;
      }
      else
        return loading::call1<automaton_t>("proper", "proper", aut,
                                           opts[DIRECTION], opts[PRUNE], opts[KEEP_HISTORY]);
    }

    bool is_proper(automaton_t aut)
    {
      return loading::call1<bool>("is_proper", "proper", aut);
    }

    bool is_valid(automaton_t aut)
    {
      return loading::call1<bool>("is_valid", "proper", aut);
    }

    automaton_t allow_eps_transition(automaton_t aut, options_t opts)
    {
      
      return loading::call1<automaton_t>("allow_eps_transition", "proper", aut, opts[KEEP_HISTORY]);
    }

    automaton_t internal::thompson(ratexp_t exp)
    {
      return loading::call1<automaton_t>("thompson", "proper", exp);
    }

    automaton_t internal::compact_thompson(ratexp_t exp)
    {
      return loading::call1<automaton_t>("compact_thompson", "proper", exp);
    }

    automaton_t internal::weighted_thompson(ratexp_t exp)
    {
      return loading::call1<automaton_t>("weighted_thompson", "proper", exp);
    }

    automaton_t draw_exp(ratexp_t exp)
    {
      return loading::call1<automaton_t>("draw_exp", "proper", exp);
    }
  }
}//end of ns awali::dyn

#endif
