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

/* @file accessible.hh
 *
 * This files contains dynamic functions computing accessible states.
 */
#ifndef DYN_MODULES_ACCESSIBLE_HH
#define DYN_MODULES_ACCESSIBLE_HH

#include <set>

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
  namespace dyn {

    /** Computes the list of accessible states in \p aut.
     *
     * @param aut  A dynamic Awali automaton
     *
     * @return the set of accessible states.
     **/
    std::set<state_t> accessible_states(automaton_t aut);


    /** Computes the list of coaccessible states in \p aut.
       *
       * @param aut  A dynamic Awali automaton
       *
       * @return a standard set
       **/
    std::set<state_t> coaccessible_states(automaton_t aut);


    /** Computes the list of useful states in \p aut.
     *
     * @param aut  A dynamic Awali automaton
     *
     * @return a standard set
     **/
    std::set<state_t> useful_states(automaton_t aut);


    /** Computes the number of accessible states in \p aut.
     *
     * Computes the number of accessible states.
     * @param aut  A dynamic Awali automaton
     *
     * @return a size_t number
     **/
    size_t num_accessible_states(automaton_t aut);


    /** Computes the number of coaccessible states in \p aut.
     *
     * @param aut  A dynamic Awali automaton
     *
     * @return a size_t number
     **/
    size_t num_coaccessible_states(automaton_t aut);


    /** Makes \p aut accessible or returns a copy of the accessible part of \p
     * aut.
     *
     * The following options may be given: {@link KEEP_HISTORY}, {@link IN_PLACE}.
     * (Other options are ignored.)
     *
     * @param aut A dynamic Awali automaton
     * @param opt some options
     * @return A dynamic Awali automaton
     *
     * \b Example:
     * \code
     *   automaton_t aut;
     *      // definition of aut
     *   automaton_t b = trim (aut); // b is a copy of the accessible part of aut
     *   accessible (aut, {IN_PLACE=true}); // aut is now accessible
     * \endcode
     */
    automaton_t accessible(automaton_t aut, options_t opt = {});


    /** Makes \p aut coaccessible or returns a copy of the coaccessible part of
     * \p aut.
     *
     * The following options may be given: {@link KEEP_HISTORY}, {@link IN_PLACE}.
     * (Other options are ignored.)
     *
     * @param aut A dynamic Awali automaton
     * @param opt some options
     * @return A dynamic Awali automaton
     *
     * \b Example:
     * \code
     *   automaton_t aut;
     *      // definition of aut
     *   automaton_t b = trim (aut); // b is a copy of the coaccessible part of aut
     *   coaccessible (aut, {IN_PLACE=true}); // aut is now coaccessible
     * \endcode
     */
    automaton_t coaccessible(automaton_t aut, options_t opt = {});


    /** Trims \p aut or returns a trimmed copy of \p aut.
     *
     * The following options may be given: {@link KEEP_HISTORY}, {@link IN_PLACE}.
     * (Other options are ignored.)
     *
     * @param aut A dynamic Awali automaton
     * @param opts some options
     * @return A dynamic Awali automaton
     *
     * \b Example:
     * \code
     *   automaton_t aut;
     *      // definition of aut
     *   automaton_t b = trim (aut); // b is a trimmed copy of aut
     *   trim (aut, {IN_PLACE=true}); // aut is now trim
     * \endcode
     */
    automaton_t trim(automaton_t aut, options_t opts = {});


    /** Tests whether \p aut is trim.
     *
     * @param aut  A dynamic Awali automaton
     *
     * @return `true` if the automaton is trim; false otherwise.
     */
    bool is_trim(automaton_t aut);


    /** Tests whether the automaton has useful states.
     *
     * @param aut  A dynamic Awali automaton
     *
     * @return `true` if the automaton has no useful state; false otherwise.
     */
    bool is_useless(automaton_t aut);


    /** Tests whether every state of the automaton is accessible.
     *
     * @param aut  A dynamic Awali automaton
     *
     * @return `true` if every state is accessible; false otherwise.
     */
    bool is_accessible(automaton_t aut);


    /** Tests whether every state of \p aut is coaccessible.
     *
     * @param aut  A dynamic Awali automaton
     *
     * @return `true` if every state is coaccessible; false otherwise.
     */
    bool is_coaccessible(automaton_t aut);


    /** Tests whether the automaton has no state.
     *
     * @param aut  A dynamic Awali automaton
     *
     * @return `true` if the automaton has no state; false otherwise.
     */
    bool is_empty(automaton_t aut);


  }
}//end of namespaces awali::dyn, and awali.

#endif
