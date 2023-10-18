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

#ifndef DYN_DETERMINIZE_HH
#define DYN_DETERMINIZE_HH

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/options/options.hh>

//This module must not be used with zmin or zmax...
//Only for lal automata

namespace awali {
  namespace dyn {

    /** Determinizes an automaton.
     *
     * The determinization is computed by the subset construction.
     * In case of weighted automata, every state of the result corresponds to a weighted subset;
     * hence, if the semiring is not locally finite, the termination is not guarantee.
     *
     * Therefore, the function can not be called if the semiring of weights is not locally finite,
     * except if the option {@link SAFE} is set to `false`.
     *
     * The option {@link KEEP_HISTORY} is meaningful only for the Boolean determinization.
     * If `true`, every state of the determinization is linked to a subset of states of \p aut.
     *
     * @param aut Automaton to determinize (possibly weighted)
     * @param opts A set of option.  Only {@link KEEP_HISTORY} and {@link SAFE} are meaningful.
     *
     * @pre \p aut should be over a locally finite weighset, except if {@link SAFE} is `false`.
     * @return The derminization of \p aut
     */
    automaton_t determinize(automaton_t aut, options_t opts = {});

    /** Complements \p aut or returns a complemented copy of \p aut.
     * @param aut automaton to be complemented
     * @param opts A set of option. Only {@link IN_PLACE} is meaningful.
     * @return
     * @pre \p aut should be deterministic and over weightset B
     */
    automaton_t complement(automaton_t aut, options_t opts = {});


    /** Completes \p aut or returns a completed copy of given automaton.
     * @param aut
     * @param opts A set of options. Only {@link IN_PLACE} is meaningfull.
     * @return a complete automaton
     */
    automaton_t complete(automaton_t aut, options_t opts = {});


    //Automata over fields or z only

    /** Reduces an automaton.
     *
     * @pre the weightset of `aut` should be a field or Z.
     */
    automaton_t reduce(automaton_t aut);

    automaton_t left_reduce(automaton_t aut);

    /** Tests whether an automaton is sequential.
     * An automaton is sequential if it has at most one initial state,
     * and, for every state, all the outgoing transitions carry different labels.
     * @param aut the automaton 
     * @pre the labels of \p aut should be letters.
     */
    bool is_sequential(automaton_t aut);

    /** Tests whether an automaton is deterministic.
     * A deterministic automaton is a sequential automatoon ({@link is_sequential})
     * where the weigth of every transition is equal to one,
     * as well as every initial weight.
     * Notice that the final weight can take any value.
     * @param aut the automaton 
     * @pre the labels of \p aut should be letters.
     */
    bool is_deterministic(automaton_t aut);

    /** Tests whether an automaton is complete.
     * @pre the labels of `aut` should be letters.
     */
    bool is_complete(automaton_t aut);

    /** Tests whether an automaton is ambiguous.
     * That is, whether `aut` features two accepting runs for the same word.
     */
    bool is_ambiguous(automaton_t aut);

    /** Computes the exploration of \p aut by length.
     * @param aut the automaton to explore
     * @param depth the depth of the exploration
     * @return a weighted deterministic automaton
     *
     * This function builds every state of the weighted determinization with distance at most \p depth from the initial state.
     * The result is a deterministic automaton which    
     * -- has the same behaviour as \p aut on words of length at most \p depth;       
     * -- does not accept any word which is not accepted by \p aut;      
     * -- either rejects or accepts words of length larger than \p depth that are accepted by \p aut; in the latter case, the weight of the word is equal to the weight of the word in \p aut.
     */
    automaton_t explore_by_length(automaton_t aut, unsigned depth);

    /** Computes the exploration of \p aut with respect to a bound.
     * @param aut the automaton to explore
     * @param bound the bound of the exploration
     * @return a weighted deterministic automaton
     *
     * This function builds every state of the weighted determinization
     * such that the square of the bound is not smaller than the square of every component of the multiset corresponding to the state;
     * The ordering is managed by the weightset.
     *
     * \pre Weightset of \pname{aut} needs to be `N`, `Z`, `Noo` or `N<int>` .
     */
    automaton_t explore_with_bound(automaton_t aut, weight_t bound);

  }
}//end of ns awali::dyn

#endif
