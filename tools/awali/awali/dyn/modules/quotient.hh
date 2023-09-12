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

#ifndef DYN_MODULES_QUOTIENT_HH
#define DYN_MODULES_QUOTIENT_HH
/// @file quotient.hh

#include <vector>

#include <awali/dyn/options/options.hh>
#include <awali/dyn/core/ratexp.hh>
#include <awali/dyn/core/automaton.hh>

// NB: symmetric functions are in dyn/algos/aliases.hh

namespace awali {
  namespace dyn {

    /** Computes the minimal quotient of \p aut.
     *
     *
     * @param aut
     * @param opts A set of option; only {@link QUOTIENT_ALGO} and
     * {@link KEEP_HISTORY} are meaninglful.
     *
     * Note that algorithm {@link BRZOZOWSKI} is not valid.
     * @pre The function works for all automata but gives the classical minimal automaton
     * only if \p aut is a deterministic boolean.
     */
    automaton_t min_quotient(automaton_t aut, options_t opts = {});

    namespace internal {
      bool is_quotient(automaton_t aut1, automaton_t aut2); //DO NOT WORK
    }

//     automaton_t merge(automaton_t aut, std::vector<std::vector<state_t>>& equiv);



    /** \ingroup Quotient
     * @brief Computes the quotient of an automaton with respect to a given
     * equivalence.
     *
     * This function merges the states w.r.t. the partition given
     * as second argument.
     *
     * Argument \p equiv must be a partition: the vectors of states must be
     * disjoint and their union must span over all the states.
     * Only the outgoing transitions of the first state of each part are
     * considered to build the quotient.
     * As a consequence, the result of this function is meaningful only if all
     * equivalent states have the same successors (modulo equiv).
     * A check of this is done unless \p opts sets {@link SAFE} to `false`.
     *
     * Param \p equiv is described as the list of its parts, each part being a list of states;
     * equiv can be partially described: if a state does not appear in any part, it is considered as a singleton part.
     * @param aut The automaton whose states should be merged
     * @param equiv The congruence of mergible states
     * @param opts A set of options; only {@link SAFE} is meaningful.
     * If {@link SAFE} is `true`, a test will be performed to verify that \p
     * equiv is indeed a {@link is_congruence congruence}.
     * @return A new automaton with states merged
    **/
    automaton_t quotient(automaton_t aut, std::vector<std::vector<state_t>>& equiv,
                         options_t opts= {});

    /**
        @brief Check whether an equivalence is a congruence

     This function tests whether the equivalence is a congruence with respect to the transitions of the automaton;
      that is, if p~q then p and q have the same final weight and, for every letter a and every part P of the equivalence,
      Sum(k, p--a|k -> s with s in p) = Sum(k, q--a|k -> s with s in P).
      equiv is described as the list of its parts, each part being a list of states;
      equiv can be partially described: if a state does not appear in any part, it is considered as a singleton part.
       @arg aut The automaton onwhich the equivalence is tested
       @arg equiv The equivalence to test
       @return true if equiv is a congruence, false otherwise
    **/
    bool is_congruence(automaton_t aut, std::vector<std::vector<state_t>>& equiv);

    /** Computes the minimal complete deterministic automaton of the language
     * accepted by \p aut.  This may result in a call to {@link proper}, to
     * {@link quotient}, and to {@link determinize} if needed.
     * @param aut
     * @param opts A set of options; {@link MINIM_ALGO} is meaningful,
     * and so is {@link QUOTIENT_ALGO} in the case where {@link MINIM_ALGO} is
     * set to {@link DETERMINIZE_QUOTIENT}.
     * @return a new automaton.
     * @pre Automaton \p aut needs to be over weightset B (and not a transducer).
     */
    automaton_t minimal_automaton(automaton_t aut, options_t opts = {});

    /** Computes the minimal complete deterministic automaton of the language
     * of \p exp.
     * Simply calls {@link exp_to_aut} and then
     * {@link minimal_automaton(automaton_t,options_t)}.
     * @param exp
     * @param opts A set of options; {@link MINIM_ALGO},
     * {@link EXP_TO_AUT_ALGO}, and {@link KEEP_HISTORY} are meaningful; if
     * {@link MINIM_ALGO} is set to {@link DETERMINIZE_QUOTIENT} (default),
     * {@link QUOTIENT_ALGO} is meaningful.
     *
     * @return a new automaton.
     * @pre Automaton \p aut needs to be over weightset B (and not a transducer).
     */
    automaton_t minimal_automaton(ratexp_t exp, options_t opts = {});


  }
}//end of ns awali::dyn

#endif
