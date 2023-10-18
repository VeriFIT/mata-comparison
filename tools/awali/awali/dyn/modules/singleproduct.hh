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

#ifndef DYN_MODULES_SINGLEPRODUCT_HH
#define DYN_MODULES_SINGLEPRODUCT_HH

#include <awali/dyn/core/automaton.hh>

namespace awali {
  namespace dyn {
    /** \ingroup Products
     * Build the \p n -th power of \p aut.
     *
     * Builds the product \p aut * \p aut * ... * \p aut (\p n times).
     *
     * If \p n is 1, a copy of \p aut is returned.
     * If \p n is 0, the automaton returned is such that its product with \p aut
     * gives aut again.
     *
     * @param aut
     * @param n
     * @return a new automaton
     * @see {@link product}
     */
    automaton_t power(automaton_t aut, unsigned int n);
  }
}//end of ns awali::dyn

#endif
