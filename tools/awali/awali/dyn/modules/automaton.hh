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

#ifndef DYN_MODULES_AUTOMATON_HH
#define DYN_MODULES_AUTOMATON_HH

#include <unordered_map>

#include <awali/dyn/options/options.hh>

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/ratexp.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/core/context_description.hh>


namespace awali {
  namespace dyn {

    /** Makes a deep copy of \p aut.
     *
     *  @param aut Automaton to copy
     *  @param opts a set of options.
     *  Only option {@link KEEP_HISTORY} is meaningful.
     *
     *  @return a copy of aut
     */
    automaton_t copy(automaton_t aut, options_t opts = {});


    /** Computes the support of \p aut, that is the boolean automaton resulting
     * from replacing any non-zero weight by True.
     *
     * @param aut automaton the support of which is computed
     * @param opts a set of options
     * Only option {@link KEEP_HISTORY} is meaningful.
     * @return automaton over B
     * */
    automaton_t support(automaton_t aut, options_t opts = {});


    /** Computes the characteristic of \p aut over \p weightset.
     *
     * Consists in copying \p aut, and setting the weight of every transition
     * to one (i.e. the multiplicative neutral element of the \p weightset).
     *
     * @param aut
     * @param weightset string representation of a weightset
     * @param opts a set of options
     * Only option {@link KEEP_HISTORY} is meaningful.
     * @return an automaton
     * - whose weightset is the one represented by string \p weightset,
     * - whose labelset is the same as the one of \p aut.
     *
     *  @pre \p aut must be a boolean automaton
     */
    automaton_t characteristic(automaton_t aut, std::string weightset,
                               options_t opts = {});

    /** @brief Change the alphabet of the automaton
     *  
     * This function replaces the alphabet of the automaton by the given alphabet on char.
     * Transitions which are labeled with letters that do not belong to the new
     * alphabet are deleted.
     *
     * @param aut the automaton
     * @param alphabet the new alphabet
     */
    void change_alphabet(automaton_t aut,  const std::string& alphabet);

    /** @brief Change the alphabet of the automaton
     *  
     * This function replaces the alphabet of the automaton by the given alphabet on int.
     * Transitions which are labeled with letters that do not belong to the new
     * alphabet are deleted.
     *
     * @param aut the automaton
     * @param a the first letter of the new alphabet
     * @param b the last letter of the new alphabet
     */
    void change_int_alphabet(automaton_t aut, int a, int b);

    /** Determines if two automata are isomorphic.
     *
     *  @param aut1
     *  @param aut2
     *  @pre \p aut1 and \p aut2 must have the same context
     */
    bool are_isomorphic(automaton_t aut1, automaton_t aut2);


    /** \ingroup Standard
     * Produces an automaton that associates with every word the weight (\p w * x), where x is the weight associated with this word in \p aut.
     *
     * This function has two different behaviours depending on whether \p aut
     * is {@link is_standard standard} or not:
     * - if \p aut is standard : multiplies the weight of each outgoing
     *   transition of the initial state by \p w; (in this case the returned
     *   automaton is standard)
     * - if \p aut is not standard : multiplies every initial weight by \p w on
     *   the left.
     *
     * The following options may be given in \p opts: {@link KEEP_HISTORY},
     * {@link IN_PLACE}.
     * (Other options are ignored)
     *
     * If {@link IN_PLACE} is `true`, this operation operation is done directly
     * on \p aut.  In this case, {@link KEEP_HISTORY} is ignored.
     *
     * If {@link IN_PLACE} is `false, this operation is done on a copy of \p
     * aut.
     *
     * @param aut automaton (unchanged)
     * @param w weight by which the initial weights are multiplied
     * @param opts a set of options
     * @return a copy of `aut` in which the transition weights have been
     *         multiplied by `w`
     */
    automaton_t left_mult(automaton_t aut, weight_t w, options_t opts = {});


    /** \ingroup Standard
     * Produces an automaton that associates with every word the weight (x * \p w), where x is the weight associated with this word in \p aut.
     *
     * Consists in multiplying every final weight by \p w on the right.
     *
     * The following options may be given in \p opts: {@link KEEP_HISTORY},
     * {@link IN_PLACE}.
     * (Other options are ignored)
     *
     * If {@link IN_PLACE} is `true`, this operation operation is done directly
     * on \p aut.  In this case, {@link KEEP_HISTORY} is ignored.
     *
     * If {@link IN_PLACE} is `false`, this operation is done on a copy of \p
     * aut.
     *
     * @param aut automaton (unchanged)
     * @param w weight by which the final are multiplied
     * @param opts a set of options
     * @return a copy of `aut` in which the transition weights have been
     *         multiplied by `w`
     */
    automaton_t right_mult(automaton_t aut, weight_t w, options_t opts = {});

    /** @brief add a subautomaton realizing 
     * the series \p exp between states 
     * \p p and \p q of \p aut.
     *
     * The behaviour of the function depends whether 
     * the automaton \p aut allows epsilon-transitions.
     *
     * -- If it does, a {@link COMPACT_THOMPSON} automaton 
     *   is built from
     *   \p exp and copied between states \p p and \p q.
     *
     * -- If it does not, a {@link STANDARD} automaton is
     *    built from
     *    \p exp and copied between states \p p and \p q.
     *    In this case, the series \p exp must be proper.
     *
     * @param aut the automaton in which the subautomaton is inserted
     * @param p the source state
     * @param q the destination state
     * @param exp the inserted series
     * @throw std::runtime_error if \p aut does not allow 
     * epsilon-transitions and \p exp is not proper
     */
    void add_path(automaton_t aut, state_t p, state_t q, ratexp_t exp);
    
    /** @brief add a subautomaton realizing 
     * the series \p s between states 
     * \p p and \p q of \p aut.
     *
     * The string \p s is parsed to obtain a rational expression;
     * the function {@link add_path} is then called
     *
     * @param aut the automaton in which the subautomaton is inserted
     * @param p the source state
     * @param q the destination state
     * @param s the inserted series
     * @param strict_alphabet if `true` 
     * the letters in the expression must belong to the alphabet 
     * of the automaton; otherwise, they can be added to the alphabet
     * @throw std::runtime_error if \p aut does not allow 
     * epsilon-transitions and \p s is not proper
    */
    void add_path(automaton_t aut, state_t p, state_t q, const std::string& s, bool strict_alphabet=true);

    /** @brief add a subautomaton realizing 
     * the series \p s between states 
     * \p p and \p q of \p aut.
     *
     * The string \p s is parsed to obtain a rational expression;
     * the function {@link add_path} is then called
     *
     * @param aut the automaton in which the subautomaton is inserted
     * @param p the source state
     * @param q the destination state
     * @param s the inserted series
     * @param strict_alphabet if `true` 
     * the letters in the expression must belong to the alphabet 
     * of the automaton; otherwise, they can be added to the alphabet
     * @throw std::runtime_error if \p aut does not allow 
     * epsilon-transitions and \p s is not proper
    */
    

    
  }
}//end of ns awali::dyn

#endif
