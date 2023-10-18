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

#ifndef AWALI_ALGOS_LEFT_MULT_HH
# define AWALI_ALGOS_LEFT_MULT_HH

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/standard.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc {


  namespace internal
  {
    template <typename Aut>
    struct standard_operations
    {
      using automaton_t = Aut;
      using context_t = context_t_of<automaton_t>;
      using weight_t = weight_t_of<context_t>;
      using weightset_t = weightset_t_of<context_t>;

      /// Left-multiplication by a weight.
      static automaton_t&
      left_mult_here(automaton_t& res, const weight_t& w, bool standard)
      {
        weightset_t ws(*res->context().weightset());
        if (ws.is_one(w))
          {}
        else if(standard)
          {
            require(is_standard(res), "left_mult : the automaton must be standard");
            state_t initial = res->dst_of(*(res->initial_transitions().begin()));
            if (ws.is_zero(w)) {
              for(auto s : res->states())
                if(s != initial)
                  res->del_state(s);
            }
            else
              for (auto t: res->all_out(initial))
                res->lmul_weight(t, w);
          }
        else if (ws.is_zero(w))
          zero_here(res);
        else 
          for (auto t: res->initial_transitions())
            res->lmul_weight(t, w);
        return res;
      }

      /// Right-multiplication by a weight.
      static automaton_t&
      right_mult_here(automaton_t& res, const weight_t& w)
      {
        weightset_t ws(*res->context().weightset());
        if (ws.is_zero(w))
          zero_here(res);
        else if (ws.is_one(w))
          {}
        else
          for (auto t: res->final_transitions())
            res->rmul_weight(t, w);
        return res;
      }

      /// Transform \a res into the (standard) empty automaton.
      static automaton_t&
      zero_here(automaton_t& res)
      {
        automaton_t a = make_shared_ptr<automaton_t>(res->context());
        a->set_initial(a->add_state());
        res = std::move(a);
        return res;
      }
    };
  }

  /*-----------------------.
  | left-mult(automaton).  |
  `-----------------------*/

    /** @brief multiplies the initial states by a weight
     *
     * @tparam Aut the type of the automaton
     * @param res the automaton
     * @param w the weight
     * @param standard if 'true' and \p res is a standard automaton, 
     *    applies a standard multiplication
     * @return the automaton \p res
     * @throw runtime_error if standard if 'true' and 
     * \p res is nota standard automaton
     *
     * The initial weight of every initial state of \p res is multiplied by \p w.
     *
     * If \p standard is 'true', the initial weight remains equal to one,
     * and the weight of every outgoing transition (and the final weight) of
     * the initial state is multiplied by \p w.
     */
  template <typename Aut>
  inline
  Aut&
  left_mult_here(Aut& res, const weight_t_of<Aut>& w, bool standard=false)
  {
    return internal::standard_operations<Aut>::left_mult_here(res, w, standard);
  }

  template <typename Aut>
  inline
  typename Aut::element_type::automaton_nocv_t
  left_mult(const Aut& aut, const weight_t_of<Aut>& w, bool standard=false)
  {
    auto res = copy(aut);
    left_mult_here(res, w, standard);
    return res;
  }


  /*--------------------.
  | left-mult(ratexp).  |
  `--------------------*/

  template <typename RatExpSet>
  inline
  typename RatExpSet::ratexp_t
  left_mult(const RatExpSet& rs,
            const weight_t_of<RatExpSet>& w,
            const typename RatExpSet::value_t& r)
  {
    return rs.lmul(w, r);
  }

  /*------------------------.
  | right-mult(automaton).  |
  `------------------------*/

  template <typename Aut>
  inline
  Aut&
  right_mult_here(Aut& res, const weight_t_of<Aut>& w)
  {
    return internal::standard_operations<Aut>::right_mult_here(res, w);
  }

  template <typename Aut>
  inline
  typename Aut::element_type::automaton_nocv_t
  right_mult(const Aut& aut, const weight_t_of<Aut>& w)
  {
    auto res = copy(aut);
    right_mult_here(res, w);
    return res;
  }

  /*---------------------.
  | right-mult(ratexp).  |
  `---------------------*/

  template <typename RatExpSet>
  inline
  typename RatExpSet::ratexp_t
  right_mult(const RatExpSet& rs,
             const typename RatExpSet::value_t& r,
             const weight_t_of<RatExpSet>& w)
  {
    return rs.rmul(r, w);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_LEFT_MULT_HH
