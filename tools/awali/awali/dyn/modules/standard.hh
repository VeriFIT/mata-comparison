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

#ifndef DYN_MODULES_STANDARD_HH
#define DYN_MODULES_STANDARD_HH

#include <iostream>
#include <unordered_map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/ratexp.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
  namespace dyn {

    /**
     * Tests whether \p aut is standard.
     *
     * An automaton is standard if
     * - it has a single initial state with an initial function equal to 1;
     * - the initial state has no incoming transition.
     *
     * @param aut
     * @return `true` if \p aut is a standard automaton.
     */
    bool is_standard(automaton_t aut);

    /**
     * Makes \p aut standard or return a new standard automaton equivalent
     * to \p aut.
     * @param aut
     * @param opts A set of options.  Only {@link IN_PLACE} is meaningful.
     * @return a new automaton, or \p aut if {@link IN_PLACE} was set to `true`.
     * @pre Automata \p aut1 and \p aut2 must have the same weightset and
     * labelset, and both be {@link is_standard standard}.
     */
    automaton_t standard(automaton_t aut, options_t opts = {});
    

    namespace internal {
      /** \ingroup Standard
       *
       * @deprecated Use {@link awali::dyn::exp_to_aut exp_to_aut}`(exp,{EXP_TO_AUT_ALGO=STANDARD})` instead.
       */
      automaton_t standard(ratexp_t exp);
    }




    /**
     * Sums standard automaton \p aut2 into standard automaton \p au1 or
     * computes a new standard automaton that is equivalent to the sum of \p
     * aut1 and \p aut2.
     *
     * @param aut1
     * @param aut2
     * @param opts A set of options. Only {@link IN_PLACE} is meaningful.
     * @return a new automaton, or \p aut1 if {@link IN_PLACE} was set to `true`.
     * @pre Automata \p aut1 and \p aut2 must have the same context, and both
     * be {@link is_standard standard}.
     * @see sum For the general function working on any automaton.
     */
    automaton_t sum_of_standard(automaton_t aut1, automaton_t aut2, options_t opts= {});

    /**
     * Builds a standard automaton that recognizes the star of the language
     * of \p aut.
     *
     * @param aut
     * @param opts A set of options. Only {@link IN_PLACE} is meaningful.
     * @return a new automaton, or \p aut1 if {@link IN_PLACE} was set to `true`.
     * @pre \p aut must be a {@link is_standard standard} automaton.
     */
    automaton_t star(automaton_t aut, options_t opts= {});
  }
}//end of ns awali::dyn

#endif
