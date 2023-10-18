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

#ifndef DYN_FACTOR_HH
#define DYN_FACTOR_HH

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
  namespace dyn {

    /** Computes an automaton accepting all prefixes of words accepted by \p aut;
     * function will modify \p aut or not depending on given \p opts.
     * @param aut
     * @param opts A set of options.  Only {@link IN_PLACE} is meaningful.
     * @return A new automaton if {@link IN_PLACE} is set, or \p aut otherwise.
     */
    automaton_t prefix(automaton_t aut, options_t opts = {});

    /** Computes an automaton accepting all suffixes of words accepted by \p aut;
     * function will modify \p aut or not depending on given \p opts.
     * @param aut
     * @param opts A set of options.  Only {@link IN_PLACE} is meaningful.
     * @return A new automaton if {@link IN_PLACE} is set, or \p aut otherwise.
     */
    automaton_t suffix(automaton_t aut, options_t opts = {});

    /** Computes an automaton accepting all factors of words accepted by \p aut;
     * function will modify \p aut or not depending on given \p opts.
     * @param aut
     * @param opts A set of options.  Only {@link IN_PLACE} is meaningful.
     * @return A new automaton if {@link IN_PLACE} is set, or \p aut otherwise.
     */
    automaton_t factor(automaton_t aut, options_t opts= {});

  }
}//end of ns awali::dyn

#endif
