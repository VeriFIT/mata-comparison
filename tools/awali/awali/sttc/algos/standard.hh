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

#ifndef AWALI_ALGOS_STANDARD_HH
# define AWALI_ALGOS_STANDARD_HH

# include <set>

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/memory.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/misc/set.hh>

namespace awali { namespace sttc {


  /*-------------------------.
  | is_standard(automaton).  |
  `-------------------------*/

  template <typename Aut>
  bool
  is_standard(const Aut& a)
  {
    return
      a->num_initials() == 1
      && a->weightset()->is_one(a->weight_of(*(a->initial_transitions().begin())))
      // No arrival on the initial state.
      && a->in(a->dst_of(*(a->initial_transitions().begin()))).empty();
  }

  /*----------------------.
  | standard(automaton).  |
  `----------------------*/

  /// \brief Turn \a aut into a standard automaton.
  ///
  /// \tparam Aut an automaton type, not a pointer type.
  template <typename Aut>
  void
  standard_here(Aut& aut)
  {
    if (is_standard(aut))
      return;
    const auto& ws = *aut->weightset();
    const auto& inits = aut->initial_transitions();
    std::vector<transition_t> initials{begin(inits), end(inits)};

    // See TAF-Kit documentation for the implementation details.
    //
    // (i.a) Add a new state s...
    auto ini = aut->add_state();
    for (auto ti: initials)
      {
        // The initial state.
        auto i = aut->dst_of(ti);
        auto wi = aut->weight_of(ti);
        for (auto t: aut->all_out(i))
          aut->add_transition(ini, aut->dst_of(t), aut->label_of(t),
                              ws.mul(wi, aut->weight_of(t)));
        aut->del_transition(ti);

        // (iv) Remove the former initial states of A that are the
        // destination of no incoming transition.
        if (aut->all_in(i).empty())
          aut->del_state(i);
      }
    // (i.b) Make [state s] initial, with initial multiplicity equal
    // to the unit of the multiplicity semiring.
    aut->set_initial(ini);
  }

  template <typename Aut>
  Aut
  standard(Aut& aut, bool keep_history=true) {
    auto out = copy(aut, keep_history);
        standard_here(out);
        return out;
  }

  /*-------------------.
  | standard(ratexp).  |
  `-------------------*/

  namespace rat
  {
    /// \brief Convert a ratexp to a standard automaton.
    ///
    /// \tparam Aut      relative the generated automaton
    /// \tparam Context  relative to the RatExp.
    template <typename Aut,
              typename Context = context_t_of<Aut>>
    class standard_visitor
      : public Context::const_visitor
    {
    public:
      using automaton_t = Aut;
      using context_t = Context;
      using weightset_t = weightset_t_of<context_t>;
      using weight_t = weight_t_of<context_t>;

      using super_type = typename Context::const_visitor;

      constexpr static const char* me() { return "standard"; }

      standard_visitor(const context_t& ctx)
        : ws_(*ctx.weightset())
        , res_(make_shared_ptr<automaton_t>(ctx))
      {}

      automaton_t
      operator()(const typename context_t::ratexp_t& v)
      {
        v->accept(*this);
        res_->set_initial(initial_);
        return std::move(res_);
      }

      AWALI_RAT_UNSUPPORTED(complement)
      AWALI_RAT_UNSUPPORTED(conjunction)
      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(shuffle)
      AWALI_RAT_UNSUPPORTED(transposition)

      AWALI_RAT_VISIT(zero,)
      {
        initial_ = res_->add_state();
      }

      AWALI_RAT_VISIT(one,)
      {
        auto i = res_->add_state();
        initial_ = i;
        res_->set_final(i);
      }

      AWALI_RAT_VISIT(atom, e)
      {
        auto i = res_->add_state();
        auto f = res_->add_state();
        initial_ = i;
        res_->new_transition(i, f, e.value());
        res_->set_final(f);
      }

      /// The current set of final states.
      using states_t = std::set<state_t>;
      states_t
      finals()
      {
        states_t res;
        for (auto t: res_->final_transitions())
          res.insert(res_->src_of(t));
        return res;
      }

      AWALI_RAT_VISIT(sum, e)
      {
        states_t other_finals = finals();
        e.head()->accept(*this);
        state_t initial = initial_;
        for (auto c: e.tail())
          {
            c->accept(*this);
            for (auto t: res_->all_out(initial_))
              // Not set_transition: for instance 'a*+a*' will make
              // "initial" go twice to post().
              res_->add_transition(initial,
                                  res_->dst_of(t),
                                  res_->label_of(t),
                                  res_->weight_of(t));
            res_->del_state(initial_);
          }
        initial_ = initial;
      }

      AWALI_RAT_VISIT(prod, e)
      {
        // The set of the final states that were introduced in pending
        // parts of the automaton (for instance if we are in the rhs
        // of "a+bc", recording the final state for "a").
        states_t other_finals = finals();

        // Traverse the first part of the product, handle left_weight.
        e.head()->accept(*this);
        state_t initial = initial_;

        // Then the remainder.
        for (auto c: e.tail())
          {
            // The set of the current (left-hand side) final transitions.
            auto ftr_ = res_->final_transitions();
            // Store transitions by copy.
            using transs_t = std::vector<transition_t>;
            transs_t ftr{ begin(ftr_), end(ftr_) };

            // Visit the next member of the product.
            c->accept(*this);

            // Branch all the previously added final transitions to
            // the successors of the new initial state.
            for (auto t1: ftr)
              if (!sttc::internal::has(other_finals, res_->src_of(t1)))
                {
                  // Remove the previous final transition first, as we
                  // might add a final transition for the same state
                  // later.
                  //
                  // For instance on "{2}a+({3}\e+{5}a)", the final
                  // state s1 of {2}a will be made final thanks to
                  // {3}\e.  So if we compute the new transitions from
                  // s1 and then remove t1, we will have removed the
                  // fact that s1 is final thanks to {3}\e.
                  //
                  // Besides, s1 will become final with weight {3}, which
                  // might interfere with {5}a too.
                  auto s1 = res_->src_of(t1);
                  auto w1 = res_->weight_of(t1);
                  res_->del_transition(t1);
                  for (auto t2: res_->all_out(initial_))
                    res_->set_transition(s1,
                                        res_->dst_of(t2),
                                        res_->label_of(t2),
                                        ws_.mul(w1, res_->weight_of(t2)));
                }
            res_->del_state(initial_);
          }
        initial_ = initial;
      }

      // See star_here().
      AWALI_RAT_VISIT(star, e)
      {
        states_t other_finals = finals();
        e.sub()->accept(*this);
        // The "final weight of the initial state", starred.
        weight_t w = ws_.star(res_->get_final_weight(initial_));
        // Branch all the final states (but initial) to the successors
        // of initial.
        for (auto ti: res_->out(initial_))
          {
            res_->lmul_weight(ti, w);
            for (auto tf: res_->final_transitions())
              if (res_->src_of(tf) != initial_
                  && !sttc::internal::has(other_finals, res_->src_of(tf)))
                // Note that the weight of ti has already been
                // multiplied, on the left, by w.
                //
                // Not set_transition, as for instance with "a**", the
                // second star modifies many existing transitions.
                res_->add_transition
                  (res_->src_of(tf),
                   res_->dst_of(ti),
                   res_->label_of(ti),
                   ws_.mul(res_->weight_of(tf), res_->weight_of(ti)));
          }
        for (auto tf: res_->final_transitions())
          res_->rmul_weight(tf, w);
        res_->set_final(initial_, w);
      }

      AWALI_RAT_VISIT(lweight, e)
      {
        e.sub()->accept(*this);
        for (auto t: res_->all_out(initial_))
          res_->lmul_weight(t, e.weight());
      }

      AWALI_RAT_VISIT(rweight, e)
      {
        states_t other_finals = finals();
        e.sub()->accept(*this);
        for (auto t: res_->final_transitions())
          if (! sttc::internal::has(other_finals, res_->src_of(t)))
            res_->rmul_weight(t, e.weight());
      }

    private:
      const weightset_t& ws_;
      automaton_t res_;
      state_t initial_ = automaton_t::element_type::null_state();
    };

  } // rat::

    /* @brief Convert a ratexp to a standard automaton.
     *
     * @tparam Aut     static type of the generated automaton
     * @tparam Context static type of the context of the generated automaton
     * @param ctx      the context of the generated automaton
     * @param exp      the rational expression
     * @return the standard automaton
     */
  template <typename Aut,
            typename Context = context_t_of<Aut>>
  Aut
  standard(const Context& ctx, const typename Context::ratexp_t& e)
  {
    rat::standard_visitor<Aut, Context> standard{ctx};
    return standard(e);
  }

    /* @brief Convert a ratexp to a standard automaton.
     *
     * @tparam RatExpSet type of the context of the rational expression
     * @param rs         the context of the rational expression
     * @param exp        the rational expression
     * @return the standard automaton
     */
  template <typename RatExpSet>
  inline
  mutable_automaton<typename RatExpSet::context_t>
  standard(const RatExpSet& rs, const typename RatExpSet::ratexp_t& e)
  {
    rat::standard_visitor<mutable_automaton<typename RatExpSet::context_t>, typename RatExpSet::context_t> standard{rs.context()};
    return standard(e);
  }


  }}//end of ns awali::stc

#endif // !AWALI_ALGOS_STANDARD_HH
