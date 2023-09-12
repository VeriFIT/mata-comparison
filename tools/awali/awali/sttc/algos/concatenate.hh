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

#ifndef AWALI_ALGOS_CONCATENATE_HH
# define AWALI_ALGOS_CONCATENATE_HH

# include <unordered_set>
# include <vector>

#include <awali/common/priority.hh>
#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/labelset/nullableset.hh>
#include <awali/sttc/algos/product.hh> // join_automata
#include <awali/sttc/algos/standard.hh>
#include <awali/sttc/algos/sum.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/misc/raise.hh> // require

namespace awali {
  namespace sttc {

    /*,------------------------------------.
      | concatenate(automaton, automaton). |
      `------------------------------------'*/

  /// Append automaton \a b to \a res.
  ///
    namespace internal {
      /// \pre The context of \a res must include that of \a b.
      template <typename A, typename B, typename P>
      A&
      concatenate_here(A& res, B b, priority::ONE<P>)
      {
        if(!is_standard(b))
          b=standard(b);
        //using automaton_t = A;
        auto& ws = *res->weightset();
        auto& ls = *res->labelset();
        auto& wsb = *b->weightset();
        auto& lsb = *b->labelset();
        
        // The set of the current (left-hand side) final transitions.
        auto ftr_ = res->final_transitions();
        // Store these transitions by copy.
        using transs_t = std::vector<transition_t>;
        transs_t ftr{ begin(ftr_), end(ftr_) };
        
        state_t b_initial = b->dst_of(*(b->initial_transitions().begin()));
        // State in B -> state in Res.
        // The initial state of b is not copied.
        std::unordered_map<state_t, state_t> m;
        m.emplace(b->post(), res->post());
        for (auto s: b->states())
          if (!b->is_initial(s))
            m.emplace(s, res->add_state());
        
        // Import all the B transitions, except the initial ones
        // and those from its (genuine) initial state.
        //
        // FIXME: provide generalized copy() that returns the map of
        // states orig -> copy.
        for (auto t: b->all_transitions())
          if (b->src_of(t) != b->pre() && b->src_of(t) != b_initial)
            res->new_transition_copy(m[b->src_of(t)], m[b->dst_of(t)], b, t);
        
        // Branch all the final transitions of res to the successors of
        // b's initial state.
        for (auto t1: ftr)
          {
            // Remove the previous final transition first, as we might add
            // a final transition for the same state later.
            //
            // For instance on "{2}a+({3}\e+{5}a)", the final state s1 of
            // {2}a will be made final thanks to {3}\e.  So if we compute
            // the new transitions from s1 and then remove t1, we will
            // have removed the fact that s1 is final thanks to {3}\e.
            //
            // Besides, s1 will become final with weight {3}, which might
            // interfere with {5}a too.
            auto s1 = res->src_of(t1);
            auto w1 = res->weight_of(t1);
            res->del_transition(t1);
            for (auto t2: b->all_out(b_initial))
              res->set_transition(s1,
                                  m[b->dst_of(t2)],
                                  ls.conv(lsb,b->label_of(t2)),
                                  ws.mul(w1, ws.conv(wsb,b->weight_of(t2))));
          }
        return res;
      }

      template <typename A, typename B, typename P>
      auto
      concatenate_here(A& res, B b, priority::TWO<P>) -> typename std::enable_if<labelset_t_of<A>::has_one(),A&>::type
      { 
        //using automaton_t = A;
        const auto& ws = *res->weightset();
        const auto& wsb = *b->weightset();
        auto one=res->labelset()->one();
        
        // The set of the final transitions of res;
        auto ftr_ = res->final_transitions();
        // Store these transitions by copy.
        using transs_t = std::vector<transition_t>;
        transs_t ftr{ begin(ftr_), end(ftr_) };
        
        copier<B,A> cp(b,res);
        cp([](state_t) { return true; });
        const auto& iom=cp.in_out_map();
        //The set of initial states of b in res
          std::unordered_set<state_t> ist;
          for(auto itb : b->initial_transitions())
            ist.emplace(iom.at(b->dst_of(itb)));
          // The set of the initial transitions of the copy of b in res;
          transs_t itr;
          for(auto tr : res->initial_transitions())
            if(ist.find(res->dst_of(tr))!=ist.end())
              itr.emplace_back(tr);
          
          for(auto ft: ftr)
            for(auto it: itr)
              res->new_transition(res->src_of(ft),
                                  res->dst_of(it),
                                  one,
                                  ws.mul(res->weight_of(ft), res->weight_of(it)));
          for(auto ft: ftr)
            res->del_transition(ft);
          for(auto it: itr)
            res->del_transition(it);
          return res;
      }
    }//internal

    /** Concatenation of an automaton to another one
     *
     * The function concatenates the automaton \a aut to the automaton \a res,
     * following the description given in {@link concatenate}.     
     * The type of \a aut must be compliant with the type of \a res.
     *
     * @tparam A the type of the modified automaton
     * @tparam B the type of the added automaton
     * @param res the modifed automaton
     * @param aut the added automaton
     * @return the automaton \a res
     */
  template <typename A, typename B>
  A&
  concatenate_here(A& res, const B& aut)
  {
    return internal::concatenate_here(res, aut, priority::value);
  }
    
    /** Concatenates two automata
     * 
     * The result of this function is an automaton
     * which realizes the Cauchy product of the series realized by the
     * input automata.    
     * The type of the result is the *join* of the types of both input automata.
     * If the result supports epsilon transitions, 
     * the concatenation is realized by the addition of epsilon transitions
     * between final states of \a aut1 to initial states of \a aut2, 
     * with a weight equal to the product of the final weight in \a aut1 
     * by the initial weight in \a aut2.     
     * Otherwise, transitions outgoing from initial states of \a aut2 are
     * duplicated as transitions outgoing from final states of \a aut1, with the same labels and destinations; the weight of the transition is multiplied left by the initial weight in \a aut2 and the final weight in \a aut1.    
     * In any case, the final states of \a aut1 are made non final and the
     * initial state of \a aut2 are made non initial.
     *
     * @tparam Aut1 the type of the first automaton
     * @tparam Aut2 the type of the second automaton
     * @param aut1 the first automaton
     * @param aut2 the second automaton
     * @return the concatenation of the automata
     */
  template <typename Aut1, typename Aut2>
  inline
  auto
  concatenate(const Aut1& aut1, const Aut2& aut2)
    -> decltype(join_automata(aut1, aut2))
  {
    auto res = join_automata(aut1, aut2);
    copy_into(aut1, res, false);
    concatenate_here(res, aut2);
    return res;
  }

  /*-----------------------------.
  | chain(automaton, min, max).  |
  `-----------------------------*/

    /** chains automata to compute powers of a series
     * 
     * If *s* is the series realized by \a aut,
     * the series realized by the computed automaton is     
     * - the sum of powers of *s* from \a min to \a max if \a min&le;\a max    
     * - the sum of powers of *s* from \a min to infinity if \a max is negative    
     * - *s* to the power \a min otherwise
     *
     * @tparam Aut the type of the automaton
     * @param aut the automaton
     * @param min a non negative integer
     * @param max an integer
     * @return an automaton
     */
  template <typename Aut>
  Aut
  chain(const Aut& aut, unsigned min, int max)
  {
    Aut res =
      make_shared_ptr<Aut>(aut->context());
    if (max < 0)
      {
        res = star(aut);
        if (min != 0)
          res = concatenate(chain(aut, min, min), res);
      }
    else
      {
        if (min < 0)
          min = 0;
        if (min == 0)
          {
            auto s = res->add_state();
            res->set_initial(s);
            res->set_final(s);
          }
        else
          {
            res = sttc::copy(aut);
            for (unsigned n = 1; n < min; ++n)
              concatenate_here(res, aut);
          }
        if (min < max)
          {
            // Aut sum = automatonset.one();
            Aut sum = make_shared_ptr<Aut>(aut->context());
            {
              auto s = sum->add_state();
              sum->set_initial(s);
              sum->set_final(s);
            }
            for (int n = 1; n <= max - min; ++n)
              sum = sttc::sum(sum, chain(aut, n, n));
            res = sttc::concatenate(res, sum);
          }
      }
    return res;
  }


  /*------------------------------.
  | concatenate(ratexp, ratexp).  |
  `------------------------------*/

    /** wrapper for the multiplication of values
     * 
     * equivalent to \a vs.mul(\a lhs, \a rhs)
     *
     * @tparam ValueSet the type of a value set
     * @param vs a value set (ratexpset, weightset, etc.)
     * @param lhs a value in the value set
     * @param rhs a second value in the value set
     * @return the product of \a lhs and \a rhs in \a vs
     */
  template <typename ValueSet>
  inline
  typename ValueSet::value_t
  concatenate(const ValueSet& vs,
              const typename ValueSet::value_t& lhs,
              const typename ValueSet::value_t& rhs)
  {
    return vs.mul(lhs, rhs);
  }


  /*--------------------------.
  | chain(ratexp, min, max).  |
  `--------------------------*/

    /** computes powers of rational exp
     * 
     * the result is     
     * - the sum of powers of \a r from \a min to \a max if \a min&le;\a max    
     * - the sum of powers of \a r from \a min to infinity if \a max is negative    
     * - \a r to the power \a min otherwise
     *
     * @tparam RatExpSet type of the value set of the expression
     * @param rs the value set of the expression
     * @param r a rational expression
     * @param min a non negative integer
     * @param max an integer
     * @return a rational expression
     */
  template <typename RatExpSet>
  typename RatExpSet::ratexp_t
  chain(const RatExpSet& rs, const typename RatExpSet::ratexp_t& r,
        unsigned min, int max)
  {
    typename RatExpSet::ratexp_t res;
    if (max < 0)
      {
        res = rs.star(r);
        if (min > 0)
          res = rs.mul(chain(rs, r, min, min), res);
      }
    else
      {
        if (min == 0)
          res = rs.one();
        else
          {
            res = r;
            for (unsigned n = 1; n < min; ++n)
              res = rs.mul(res, r);
          }
        if (min < max)
          {
            typename RatExpSet::ratexp_t sum = rs.one();
            for (int n = 1; n <= max - min; ++n)
              sum = rs.add(sum, chain(rs, r, n, n));
            res = rs.mul(res, sum);
          }
      }
    return res;
  }


  /*----------------------.
  | mul(weight, weight).  |
  `----------------------*/


}
}//end of ns awali::stc

#endif // !AWALI_ALGOS_CONCATENATE_HH
