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

#ifndef DYN_MODULES_PROPER_HH
#define DYN_MODULES_PROPER_HH

#include <awali/dyn/options/options.hh>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/ratexp.hh>

namespace awali {
  namespace dyn {

    /** Removes epsilon-transitions in \p aut or returns a new automaton
     * equivalent to \p aut that has no epsilon-transition.
     *
     *  NB: the type of automaton returned depends on whether {@link IN_PLACE}
     * is `true` or not.  
     * - If {@link IN_PLACE} is `true`, the epsilon-transitions are removed from
     *  \p aut in place. Hence, \p aut still allows epsilon-transitions, and
     *  functions that require automata that do not allow epsilon-transitions 
     *  will not work.
     * - If {@link IN_PLACE} is `false`, the resulting automaton will not allow
     *  epsilon-transition.
     *
     * @param aut
     * 
     * @param opts 
     * A set of options.  Meaningful options are {@link IN_PLACE},
     * {@link DIRECTION}, {@link PRUNE} and {@link KEEP_HISTORY}.
     *
     *  - If {@link PRUNE} is `true` (default), the algorithm removes the states 
     * that become useless during the process of removing epsilon-transitions.
     * 
     *  - If {@link DIRECTION} is `BACKWARD` (default) the closure of the transitions
     * (or final states) is computed w.r.t. the preceding transitions;
     * if it is `FORWARD` the closure of transitions (or initial states) is
     * computed w.r.t. the following transitions.
     *
     *  - The option {@link KEEP_HISTORY} is meaningful only if {@link IN_PLACE} is `false`. 
     *
     * @return Argument \p aut if {@link IN_PLACE} is `true`, a new automaton
     * otherwise.
     * @pre  \p aut must be {@link is_valid valid} (it is
     * always true for Boolean automata).
     */
    automaton_t proper(automaton_t aut, options_t opts = {});

    /** Tests whether an automaton has epsilon-transitions.
     *
     * This is a dynamical test: the function will check for the existence of
     * a transition labelled by epsilon.  If the automaton does not allow
     * epsilon transitions, this functions return `true` without computation.
     *
     * @param aut
     * @return `true` if \p aut has no transition labelled by epsilon.
     */
    bool is_proper(automaton_t aut);


    /** Tests whether epsilon removal is possible in \p aut.
     *
     *  @param aut
     *  @return `true` if {@link proper} would work on \p aut.
     */
    bool is_valid(automaton_t aut);

    /** Returns a copy of \p aut in which epsilon-transitions are allowed.
     * @param aut
     * @param opts
     */
    automaton_t allow_eps_transition(automaton_t aut, options_t opts = {});

    /** Computes a tree-like automaton representing expression \p exp.
     *  Use with {@link pdfdisplay}
     *  @param exp
     */
    automaton_t draw_exp(ratexp_t exp);


    namespace internal {
      automaton_t weighted_thompson(ratexp_t exp);
      automaton_t thompson(ratexp_t exp);
      automaton_t compact_thompson(ratexp_t exp);
    }

  }
}//end of ns awali::dyn

#endif
