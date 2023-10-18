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

#ifndef DYN_MODULES_AREEQUIVALENT_HH
#define DYN_MODULES_AREEQUIVALENT_HH

#include <awali/dyn/core/automaton.hh>

namespace awali {
  namespace dyn {

    /** @brief Tests if \p aut1 and \p au2 are equivalent.
     *
     *  @param aut1
     *  @param aut2
     *  @return `true` if \p aut1 and \p aut2 associates every word with the same weight.
     *  @pre The algorithm only work if either:
     *  - both \p aut1 and \p aut2 are over weightset B;
     *  - automaton \p aut2 is over a weightset that is a field, or over Z, and the context of \p aut1 is compatible with the context of \p aut2.
     */
    bool are_equivalent(automaton_t aut1, automaton_t aut2);
  }
}//end of ns awali::dyn

#endif
