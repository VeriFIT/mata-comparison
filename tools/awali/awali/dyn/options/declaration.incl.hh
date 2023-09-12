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


/* THIS FILE SHOULD NOT BE IMPORTED DIRECTLY. */

namespace awali {
  namespace dyn {

    /* Declaration of options */
#ifdef INSTANTIATE_OPTIONS
#define DECLARE_OPTION(name,type,default_value) \
  internal::option_t<type> name(#name,default_value)
#else
#define DECLARE_OPTION(name,type,default_value) \
  extern internal::option_t<type> name
#endif

    /** @addtogroup Options
     * @{
     */


    /** Option used when a rational expression is computed from an automaton.
     * Defaults to {@link STANDARD_AND_QUOTIENT}.
     */
    DECLARE_OPTION(EXP_TO_AUT_ALGO, exp_to_aut_algo_t, STANDARD_AND_QUOTIENT);

    /** Option used when there are "forward" or "backward" strategies.
     *  Defaults to {@link BACKWARD}.
     */
    DECLARE_OPTION(DIRECTION, direction_t, BACKWARD);

    /** Option used to specify an input or output format.
     * Defaults to {@link JSON}.
     */
    DECLARE_OPTION(IO_FORMAT, io_format_t, JSON);

    /** Option used when an algorithm may be done in place.
     *
     * If set, the automaton is modified (and returned), if unset, the function copies input automaton before executing the algorithm.
     *
     * Defaults to `false`.
     */
    DECLARE_OPTION(IN_PLACE, bool, false);

    /** Option used to specify whether to keep the history of states.
     * - In algorithms, if set to `true`, each state in the resulting automaton is endowed with its history (e.g., with a set of states for determinization).
     * - In display functions, if set to `true`, the history will be shown in states (instead of names).
     * Defaults to true.
     *
     * @see The enum history_kind_t that gives the different kinds of history a state may have.
     */
    DECLARE_OPTION(KEEP_HISTORY, bool, true);


    /** Option used when displaying an automaton with dot.
     *
     * Defaults to {@link HORIZONTAL}.
    **/
    DECLARE_OPTION(LAYOUT, layout_t, HORIZONTAL);

    /** Option used to specify the algorithm to use for computing minimization.
     *
     * Defaults to {@link DETERMINIZE_QUOTIENT}; note that it makes
     * {@link QUOTIENT_ALGO} meaningful.
     */
    DECLARE_OPTION(MINIM_ALGO, minim_algo_t, DETERMINIZE_QUOTIENT);

    /** Option used to specify the algorithm to use for computing quotients.
     * Note that it is also used by default when making a minimization.
     *
     * Defaults to {@link MOORE}.
     */
    DECLARE_OPTION(QUOTIENT_ALGO, quotient_algo_t, MOORE);

// // <<<<<<< HEAD
//     /** Option used when a name may be given.
//      *
//      * Defaults to `""`.
//      */
//     DECLARE_OPTION(NAME, std::string, "");
// 
//     /** Option used when a title may be given.
//      *
//      * Defaults to `"tmp"`.
//      */
//     DECLARE_OPTION(TITLE, std::string, "tmp");
// 
// =======
// >>>>>>> merge-vmsl
    /** Option used when in the process of the algorithm, some elements (e.g.
     * states) may be found to be useless.
     * If set to `true`, these elements are deleted. ( Setting this to `false` is
     * mostly useful for pedagogical purpose.)
     *
     * Defaults to `true`.
     */
    DECLARE_OPTION(PRUNE, bool, true);

    /** Option used when performing some test is costly or impossible.
     * If set to `false` the program will assume the precondition is
     * satisfied.
     * This may result in inconsistent automata or infinite loops.
     *
     * Defaults to `true`.
     */
    DECLARE_OPTION(SAFE, bool, true);

    /** Option used to specify the order in which states should be eliminated
     * (typically in functions such as {@link awali::dyn::exp_to_aut}).
     *
     * Defaults to {@link MIN_INOUT_DEGREE}.
     */
    DECLARE_OPTION(STATE_ELIM_ORDER, state_elim_order_t, MIN_INOUT_DEGREE);


    /** Option used to specify whether to automatically promote an automaton
     * (or automata) if its context does not allow to perform an operation, but
     * a more general context allows it.
     * Used typically in functions such as {@link awali::dyn::product}.
     * Defaults to `false`.
     * @beware 
     * Promotion always requires a full copy of the automaton.
     */
    DECLARE_OPTION(AUTOPROMOTE, bool, false);

    /** Option used to specify whether to consider initial/final status of
     * states as normal transitions.
     * 
     * In this paradigm:
     * - There is one *preinitial* state (see @ref abstract_automaton_t::pre); it has no incoming transition, and is
     *   the source of a transition coming in each initial state.
     * - There is one *postfinal* state (see @ref abstract_automaton_t::post); it has no outgoing transition, and is
     *   the destination of one transition going out of each final state.
     *
     * If set to `true`, some functions will return these additionals states
     * and transitions.  Defaults to `false`.
     *
     * @see 
     * abstract_automaton_t::incoming, 
     * abstract_automaton_t::outgoing, 
     * abstract_automaton_t::states, 
     * abstract_automaton_t::transitions,
     */
    DECLARE_OPTION(PREPOST_PARADIGM, bool, false);

#undef DECLARE_OPTION


    /** @} */ //end of doxygen @addtogroup
  }
}

