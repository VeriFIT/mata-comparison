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

#ifndef DYN_DETERMINIZE_CC
#define DYN_DETERMINIZE_CC

#include <iostream>
#include <unordered_map>

#include <awali/dyn/loading/handler.hh>

#include <awali/dyn/modules/determinize.hh>
//This module must not be used with zmin or zmax...
//Only for lal automata

namespace awali {
  namespace dyn {

    automaton_t
    determinize (automaton_t aut, options_t opts)
    {
      if(aut->get_context()->weightset_name()=="B")
	return loading::call1<automaton_t>("determinize", "determinize", aut,
                                         opts[KEEP_HISTORY]);
      if(!opts[SAFE] || aut->get_context()->is_locally_finite_weightset())
        return loading::call1<automaton_t>("weighted_determinize", "determinize", aut);
      else
        throw std::runtime_error("Determinization is allowed only for locally finite weightsets");

    }


    automaton_t
    complement (automaton_t aut, options_t opts)
    {
      if (opts[IN_PLACE]) {
        loading::call1<void>("complement_here", "determinize", aut);
        return aut;
      }
      else
        return loading::call1<automaton_t>("complement", "determinize", aut);
    }


    automaton_t
    complete (automaton_t aut, options_t opts)
    {
      if (opts[IN_PLACE]) {
        loading::call1<void>("complete_here", "determinize", aut);
        return aut;
      }
      else
        return loading::call1<automaton_t>("complete", "determinize", aut);
    }


    automaton_t
    reduce (automaton_t aut)
    {
      return loading::call1<automaton_t>("reduce", "determinize", aut);
    }


    automaton_t
    left_reduce (automaton_t aut)
    {
      return loading::call1<automaton_t>("left_reduce", "determinize", aut);
    }


    bool
    is_deterministic (automaton_t aut)
    {
      return loading::call1<bool>("is_deterministic", "determinize", aut);
    }

    bool
    is_sequential (automaton_t aut)
    {
      return loading::call1<bool>("is_sequential", "determinize", aut);
    }

    bool
    is_complete (automaton_t aut)
    {
      return loading::call1<bool>("is_complete", "determinize", aut);
    }

    bool
    is_ambiguous (automaton_t aut)
    {
      return loading::call1<bool>("is_ambiguous", "determinize", aut);
    }

    automaton_t explore_by_length(automaton_t aut, unsigned depth) {
      return loading::call1<automaton_t>("explore_by_length", "determinize", aut, depth);
    }


    
    automaton_t explore_with_bound(automaton_t aut, weight_t bound) {
      std::string ws = aut->get_context()->weightset_name();
      context::weightset_description wd1 = context::weightset(ws);
      /* Although it is stated "N2", below, the test for type will work for any
       * bounded semiring */
      context::weightset_description wd2 = context::weightset("N2"); 
      if (ws == "N" || ws == "Z" || ws == "Noo" || wd1->type_ == wd2->type_)
        return loading::call1<automaton_t>("explore_with_bound", "determinize", aut, bound);
      throw std::runtime_error(
        "[dyn::explore_with_bound] It is required that the automaton is"
        " weighted over N, Z, Noo, or N<int>.");
    }
  }
}//end of ns awali::dyn

#endif
