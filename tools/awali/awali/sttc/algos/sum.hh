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

#ifndef AWALI_ALGOS_SUM_HH
# define AWALI_ALGOS_SUM_HH

# include <map>

#include <awali/sttc/algos/product.hh> // join_automata
#include <awali/sttc/algos/standard.hh> // is_standard
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/raise.hh> // require

namespace awali {
  namespace sttc {
    namespace internal {

      /*,---------------------------.
        | sum of standard automata. |
        `---------------------------'*/

      /// Merge transitions of \a b into those of \a res.
      ///
      /// \pre The context of \a res must include that of \a b.
      /// \pre res and b must be standard.
      template <typename A, typename B>
      A&
      sum_of_standard_here(A& res, const B& b)
      {
        require(is_standard(res), __func__, ": first automaton should be standard");
        require(is_standard(b), __func__, ": second automaton should be standard");

        //Copy b into res (except the initial state)
        // State in b -> state in res.
        std::map<state_t, state_t> m;
        state_t initial = res->dst_of(*(res->initial_transitions().begin()));
        for (auto s: b->states())
          m.emplace(s, b->is_initial(s) ? initial : res->add_state());
        m.emplace(b->pre(), res->pre());
        m.emplace(b->post(), res->post());

        // Add b.
        for (auto t: b->all_transitions())
          // Do not add initial transitions, the unique initial state is
          // already declared as such, and its weight must remain 1.
          if (b->src_of(t) != b->pre())
            {
              if (b->dst_of(t) == b->post())
                res->add_transition(m[b->src_of(t)], m[b->dst_of(t)],
                                    b->label_of(t), b->weight_of(t));
              else
                res->new_transition(m[b->src_of(t)], m[b->dst_of(t)],
                                    b->label_of(t), b->weight_of(t));
            }
        return res;
      }

    }
    
    /*,----------------.
      | (disjoint) sum |
      `----------------'*/
    
    /// Merge transitions of \a b into those of \a res.
    ///
    /// \pre AutIn <: AutOut.
    /** @brief in_place sums two automata
     *
     * @tparam Res the type of the first automaton
     * @tparam Aut the type of the second automaton
     * @param res the first automaton
     * @param aut the second automaton
     * @param sum_standard if `true`, and \p aut1 and \p aut2 are 
     * standard automata, the result is standard
     * @return the automaton \p res
     * @throw runtime_error if \p sum_standard is `true`, 
     *    and \p aut1 and \p aut2 are not standard
     *
     * The type of \p aut must be compatible with the type of \p res
     *
     * If \p sum_standard is `false`, the function copies \p aut into \p res.   
     * If \p sum_standard is `true`, the function checks that 
     * automata \p aut1 and \p aut2 are standard and 
     * copies \p aut into \p res, except the initial state of \p aut
     * which is merged with the initial state of \p res.
     */
    template <typename Res, typename Aut>
    inline
    Res&
    sum_here(Res& res, const Aut& aut, bool sum_standard=false)
    {
      if(sum_standard)
        internal::sum_of_standard_here(res, aut);
      else
        copy_into(aut, res, false);
      return res;
    }
    
    /** @brief sums two automata
     *
     * @tparam Aut1 the type of the first automaton
     * @tparam Aut2 the type of the second automaton
     * @param aut1 the first automaton
     * @param aut2 the second automaton
     * @param sum_standard if `true`, and \p aut1 and \p aut2 are 
     * standard automata, the result is standard
     * @return an automaton 
     * @throw runtime_error if \p sum_standard is `true`, 
     *    and \p aut1 and \p aut2 are not standard
     *
     * The context of the result is the *join* of the contexts of 
     * \p aut1 and \p aut2, if this join exists.
     *
     * If \p sum_standard is `false`, the function computes the union of
     * automata \p aut1 and \p aut2.     
     * If \p sum_standard is `true`, the function checks that 
     * automata \p aut1 and \p aut2 are standard and 
     * builds the standard sum, which is the union 
     * where both initial states are merged.
     */
    template <typename Aut1, typename Aut2>
    inline
    auto
    sum(const Aut1& aut1, const Aut2& aut2, bool sum_standard=false)
      -> decltype(join_automata(aut1, aut2))
    {
      auto res = join_automata(aut1, aut2);
      copy_into(aut1, res, false);
      sum_here(res, aut2, sum_standard);
      return res;
    }
    
    /*,------------------------------.
      | sum(polynomial, polynomial). |
      `------------------------------'*/
    
    /// Sums of values.
    template <typename ValueSet>
    inline
    typename ValueSet::value_t
    sum(const ValueSet& vs,
        const typename ValueSet::value_t& lhs,
        const typename ValueSet::value_t& rhs)
    {
      return vs.add(lhs, rhs);
    }

  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_SUM_HH
