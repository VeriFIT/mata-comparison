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

#ifndef DYN_MODULES_STANDARD_CC
#define DYN_MODULES_STANDARD_CC

#include <iostream>
#include <unordered_map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/abstract_ratexp.hh>
#include <awali/dyn/loading/handler.hh>

#include <awali/dyn/modules/standard.hh>

namespace awali {
  namespace dyn {



    bool is_standard(automaton_t aut)
    {
      return loading::call1<bool>("is_standard", "standard", aut);
    }

    automaton_t standard(automaton_t aut, options_t opts)
    {
      if (opts[IN_PLACE]) {
        loading::call1<void>("standard_here", "standard", aut);
        return aut;
      }
      else
        return loading::call1<automaton_t>("standard", "standard", aut);
    }



    automaton_t sum_of_standard(automaton_t aut1, automaton_t aut2, options_t opts)
    {
      if (opts[IN_PLACE]) {
        loading::call1<void>("sum_of_standard_here", "standard", aut1, aut2);
        return aut1;
      }
      else
        return loading::call1<automaton_t>("sum_of_standard", "standard", aut1, aut2);
    }

    automaton_t star(automaton_t aut, options_t opts)
    {
      if (opts[IN_PLACE]) {
        loading::call1<void>("star_here", "standard", aut);
        return aut;
      }
      else
        return loading::call1<automaton_t>("star", "standard", aut);
    }


    automaton_t internal::standard(ratexp_t exp) {
      return loading::call1<automaton_t>("standard_of", "standard", exp);
    }

  }
}//end of ns awali::dyn


#endif
