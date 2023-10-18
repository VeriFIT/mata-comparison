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

#include <iostream>
#include <unordered_map>

#include <awali/dyn/modules/derivation.hh>
#include <awali/dyn/loading/handler.hh>

namespace awali {
  namespace dyn {
    /*
     TODO
      std::map<ratexp_t, weight_t> derivation(std::map<ratexp_t, weight_t> e, label_t a, bool breaking = false)
      std::map<ratexp_t, weight_t> derivation(std::map<ratexp_t, weight_t> e, std::string w, bool breaking = false)
      */

    std::map<ratexp_t, weight_t>
    internal::derivation(ratexp_t exp, label_t a, bool breaking)
    {
      return loading::call1<std::map<ratexp_t, weight_t>>("derivation_label",
             "derivation", exp, a, breaking);
    }

    automaton_t internal::breaking_derived_term(ratexp_t exp, options_t opts)
    {
      return loading::call1<automaton_t>("derived_term", "derivation", exp, true,
                                         opts[KEEP_HISTORY]);
    }

    automaton_t internal::derived_term(ratexp_t exp, options_t opts)
    {
      return loading::call1<automaton_t>("derived_term", "derivation", exp, false,
                                         opts[KEEP_HISTORY]);
    }

  }
}//end of ns awali::dyn
