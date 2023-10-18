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

#ifndef DYN_ALGOS_ALIASES
#define DYN_ALGOS_ALIASES

#include <vector>

#include <awali/dyn/core/automaton.hh>
#include <awali/common/enums.hh>
#include <awali/dyn/options/options.hh>
#include <awali/dyn/core/abstract_ratexp.hh>

namespace awali {
  namespace dyn {
    
    /** @ingroup Quotient
     * Computes the minimal coquotient of \p aut.
     * Simply calls {@link transpose}, then {@link min_quotient}, then 
     * {@link transpose}.
     * @param aut
     * @param opts A set of options; {@link QUOTIENT_ALGO} is meaningful.
     * @return A new automaton.
     */
    automaton_t min_coquotient(automaton_t aut, options_t opts = {});

    /**\ingroup Quotient
     * Computes the coquotient of an automaton with respect to a given
     * equivalence.
     *
     * Simply calls {@link transpose} , {@link quotient}, {@link transpose}
     * @param aut The automaton whose states should be merged
     * @param equiv The congruence of mergible states
     * @param opts A set of options; only {@link SAFE} is meaningful.
     * If {@link SAFE} is `true`, a test will be performed to verify that \p
     * equiv is indeed a {@link is_congruence congruence}.
     * @return A new automaton with states merged
     */
    automaton_t
    coquotient (automaton_t aut, std::vector<std::vector<state_t>>& equiv,
      options_t opts = {}); 
  
    /**
     *
     * Simpy calls {@link transpose}, {@link left_reduce}, {@link transpose}
     */
    automaton_t right_reduce(automaton_t aut);
    
    /** Computes an automaton equivalent to \p ratexp.
     * @param ratexp
     * @param opts A set of options; {@link EXP_TO_AUT_ALGO} is meaningful.
     * @return A new automaton whose behaviour is the same as the one of
     * \p ratexp.
     */
    automaton_t exp_to_aut(ratexp_t ratexp, options_t opts={});
  }
}

#endif
