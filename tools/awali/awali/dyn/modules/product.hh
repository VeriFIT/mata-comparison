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

#ifndef DYN_MODULES_PRODUCT_HH
#define DYN_MODULES_PRODUCT_HH

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
  namespace dyn {

    /** \ingroup Products
     * Computes the classical product of \p aut1 and \p aut2.
     *
     * If \p aut1 and \p aut2 are boolean automata, a word is accepted by the
     * returned automaton if it is accepted by both \p aut1 and \p aut2.
     *
     * If \p aut1 and \p aut2 are weighted automata, this function computes the
     * Hadamard product of \p aut1 and \p aut2.
     * The weight associated with a word w in the returned automaton is the
     * product of the weights associated with w in \p aut1 and \p aut2.
     *
     * @param aut1
     * @param aut2
     * @param opts A set of options. Meaningful options: {@link KEEP_HISTORY}.
     * @return A new automaton
     * @pre \p aut1 and \p aut2 must have compatible weightsets.
     */
    automaton_t product(automaton_t aut1, automaton_t aut2, options_t opts= {});


    /** \ingroup Products
     * Computes the shuffle product of \p aut1 and \p aut2.
     *
     * Notes:  A word u belongs to the shuffle of two languages L, K if u may
     * be factorised as u = v0.w0.v1.w1...vn.wn such that v0.v1...vn belongs
     * to L and w0.w1...wn belongs to K.
     *
     * @param aut1
     * @param aut2
     * @param opts A set of options. Meaningful options: {@link KEEP_HISTORY}.
     * @return A new automaton.
     */
    automaton_t shuffle(automaton_t aut1, automaton_t aut2, options_t opts= {});


    /** \ingroup Products
     * Computes the infiltration product of \p aut1 and \p aut2.
     *
     * Notes:  A word u belongs to the infiltration of two languages L,K if u
     * may be factorised as u = u0.v0.w0.u1.v1.w1...un.vn.wn such that
     * u0.v0.u1.v1...un.vn belongs to L and u0.w0.u1.w1...un.wn belongs to K.
     *
     * @param aut1
     * @param aut2
     * @param opts A set of options. Only {@link KEEP_HISTORY} is meaningful.
     * @return A new automaton.
     */
    automaton_t infiltration(automaton_t aut1, automaton_t aut2,
                             options_t opts= {});


    /** \ingroup Products
     * Computes the parallele union of \p aut1 and \p aut2.
     *
     * If option {@link IN_PLACE} is `true` in \p opts, then \p aut1 is
     * modified and returned; otherwise, a new automaton a returned.
     *
     * @param aut1
     * @param aut2
     * @param opts A set of options. Only {@link IN_PLACE} is meaningful.
     * @return A new automaton or \p aut1
     */
    automaton_t sum(automaton_t aut1, automaton_t aut2, options_t opts= {});
  
    /** \ingroup Products
     * Concatenates \p aut2 to \p aut1 or computes a new standard automaton
     *
     * @param aut1 Left operand of the concatenation.
     * @param aut2 Right operand of the concatenation.
     * @param opts A set of option. Only {@link IN_PLACE} is meaningful.
     * @return a new automaton, or \p aut1 if {@link IN_PLACE} was set to `true`.
     * @pre Automata \p aut1 and \p aut2 must have the same weightset and
     * labelset, and both be {@link is_standard standard}.
     */
    automaton_t concatenate(automaton_t aut1, automaton_t aut2, options_t opts= {});

}}//end of ns awali::dyn

#endif
