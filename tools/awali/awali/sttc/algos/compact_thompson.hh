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

#ifndef AWALI_ALGOS_COMPACT_THOMPSON_HH
# define AWALI_ALGOS_COMPACT_THOMPSON_HH

#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/labelset/traits.hh> //nullable_of

namespace awali { namespace sttc {

  namespace rat
  {
    /// \tparam Aut      relative the generated automaton
    /// \tparam Context  relative to the RatExp.
    template <typename Aut,
              typename Context = context_t_of<Aut>
              >
    class compact_thompson_visitor
      : public Context::const_visitor
    {
    public:
      using automaton_t = Aut;
      using context_t = Context;
      using weightset_t = weightset_t_of<context_t>;
      using weight_t = weight_t_of<context_t>;

      using super_type = typename Context::const_visitor;
      using history_t = std::shared_ptr<string_history>;

      static_assert(context_t::has_one(), "requires nullable labels");

      constexpr static const char* me() { return "compact-thompson"; }

      compact_thompson_visitor(const context_t& ctx, bool keep_history)
        : res_(make_shared_ptr<automaton_t>(ctx)),
          history_(std::make_shared<string_history>()),
          keep_history_(keep_history)
      {}

      automaton_t
      operator()(const typename Context::ratexp_t& v)
      {
        if(keep_history_)
          res_->set_history(history_);
        v->accept(*this);
        res_->set_initial(initial_);
        res_->set_final(final_);
        return std::move(res_);
      }

      AWALI_RAT_VISIT(zero,)
      {
        initial_ = res_->add_state();
        final_ = res_->add_state();
        res_->set_state_name(initial_,"i");
        res_->set_state_name(final_,"s"+std::to_string(counter++));
        history_->add_state(final_,"zero");
      }

      AWALI_RAT_VISIT(one,)
      {
        initial_ = res_->add_state();
        final_ = res_->add_state();
        res_->set_state_name(initial_,"i");
        res_->set_state_name(final_,"s"+std::to_string(counter++));
        res_->new_transition(initial_, final_, epsilon_);
        history_->add_state(final_,"one");
      }

      AWALI_RAT_VISIT(atom, e)
      {
        initial_ = res_->add_state();
        final_ = res_->add_state();
        res_->set_state_name(initial_,"i");
        res_->set_state_name(final_,"s"+std::to_string(counter++));
        res_->new_transition(initial_, final_, e.value());
        history_->add_state(final_,"letter");
      }

      AWALI_RAT_VISIT(sum, e)
      {
        e.head()->accept(*this);
        state_t initial = initial_;
        state_t final = final_;
        for (auto c: e.tail())
          {
            c->accept(*this);
            for (auto t: res_->out(initial_))
              res_->new_transition(initial,
                                   res_->dst_of(t),
                                   res_->label_of(t),
                                   res_->weight_of(t));
            for (auto t: res_->in(final_))
              res_->new_transition(res_->src_of(t),
                                   final,
                                   res_->label_of(t),
                                   res_->weight_of(t));
            res_->del_state(initial_);
            res_->del_state(final_);
          }
        initial_ = initial;
        final_ = final;
        history_->add_state(final_,"sum");
      }
      
      AWALI_RAT_UNSUPPORTED(complement)
      AWALI_RAT_UNSUPPORTED(conjunction)
      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(shuffle)
      AWALI_RAT_UNSUPPORTED(transposition)
      
      AWALI_RAT_VISIT(prod, e)
      {
        e.head()->accept(*this);
        state_t initial = initial_;
        // Then the remainder.
        for (auto c: e.tail())
          {
            state_t final = final_;
            c->accept(*this);
            for (auto t: res_->out(initial_))
              res_->new_transition(final,
                                   res_->dst_of(t),
                                   res_->label_of(t),
                                   res_->weight_of(t));
            res_->del_state(initial_);
          }
        initial_ = initial;
      }

      AWALI_RAT_VISIT(star, e)
      {
        e.sub()->accept(*this);
        state_t initial = res_->add_state();
        state_t final = res_->add_state();
            for (auto t: res_->out(initial_))
              res_->new_transition(final_,
                                   res_->dst_of(t),
                                   res_->label_of(t),
                                   res_->weight_of(t));
            res_->del_state(initial_);
        res_->new_transition(initial, final_, epsilon_);
        res_->new_transition(final_,  final,    epsilon_);
        initial_ = initial;
        final_ = final;
        res_->set_state_name(initial_,"i");
        res_->set_state_name(final_,"s"+std::to_string(counter++));
        history_->add_state(final_,"star");
      }

      AWALI_RAT_VISIT(maybe, e)
      {
        e.sub()->accept(*this);
        res_->add_transition(initial_, final_, epsilon_);
      }

      AWALI_RAT_VISIT(plus, e)
      {
        e.sub()->accept(*this);
        state_t initial = res_->add_state();
        state_t final = res_->add_state();
        res_->new_transition(initial, initial_, epsilon_);
        res_->new_transition(final_,  final,    epsilon_);
        res_->new_transition(final_,  initial_, epsilon_);
        initial_ = initial;
        final_ = final;
        res_->set_state_name(initial_,"s"+std::to_string(counter));
        res_->set_state_name(final_,"t"+std::to_string(counter++));
        history_->add_state(final_,"plus");
      }

      AWALI_RAT_VISIT(lweight, e)
      {
        e.sub()->accept(*this);

        const weight_t& w = e.weight();
        for (auto t: res_->out(initial_))
          res_->set_weight(t, ws_.mul(w, res_->weight_of(t)));
      }

      AWALI_RAT_VISIT(rweight, e)
      {
        e.sub()->accept(*this);

        const weight_t& w = e.weight();
        for (auto t: res_->in(final_))
          res_->set_weight(t, ws_.mul(res_->weight_of(t), w));
      }

    private:
      automaton_t res_;
      const weightset_t& ws_ = *res_->weightset();
      using label_t = label_t_of<automaton_t>;
      const label_t epsilon_ = res_->labelset()->one();
      state_t initial_ = automaton_t::element_type::null_state();
      state_t final_ = automaton_t::element_type::null_state();
      history_t history_;
      bool keep_history_;
      unsigned counter=0;
    };

  } // rat::

   /** @brief builds a compact variant of the Thompson automaton 
     *
     * This automaton has some characteristics similar to
     * the classical Thompson automaton :      
     * - it may have epsilon-transitions;    
     * - it has a unique initial state with initial weight
     *    equal to one; 
     *    this state has no incoming transitions;    
     * - it has a unique final state, 
     *    distinct from the initial state, 
     *    with final weight equal to one; 
     *    this state has no incoming transitions;    
     * - the number of states and transitions is linear
     *    w.r.t. the size of the expression \p exp.    
     * Meanwhile, opposite to the Thompson automaton,
     * neither the in-degree nor the out-degree of each
     * state is bounded.
     *
     * Actually, the following rules are applied:    
     * - to represent the sum, the initial (resp. final) states are merged;      
     * - to represent the product, the final state of the first automaton is merged with the initial state of the second one;     
     * - to represent the star, the initial and the final states are merged; then a fresh initial state and a fresh final states are added.
     *
     * @tparam Aut      type of the generated automaton
     * @tparam Context  type of the context of the generated automaton
     * @param ctx          the context of the generated automaton
     * @param exp          the rational expression
     * @param keep_history (optional) if `true` (default value), the states are stamped with their origin
     * @return the compact Thompson automaton
  */
  template <typename Aut,
            typename Context = context_t_of<Aut>>
  Aut
  compact_thompson(const Context& ctx, const typename Context::ratexp_t& exp, bool keep_history=true)
  {
    rat::compact_thompson_visitor<Aut, Context> cthompson{ctx,keep_history};
    return cthompson(exp);
  }

   /** @brief builds a compact variant of the Thompson automaton 
    *
    * @tparam RatExpSet  type of the context of the rational expression
    * @param rs          the context of the rational expression
    * @param exp          the rational expression
    * @param keep_history (optional) if `true` (default value), the states are stamped with their origin
    * @return the compact Thompson automaton
    */
  template <typename RatExpSet>
  mutable_automaton<sttc::nullable_of<typename RatExpSet::context_t>>
  compact_thompson(const RatExpSet& rs, const typename RatExpSet::ratexp_t& exp, bool keep_history=true)
  {
    using Aut=mutable_automaton<sttc::nullable_of<typename RatExpSet::context_t>>;
    rat::compact_thompson_visitor<Aut, context_t_of<Aut>> cthompson{sttc::get_nullable_context(rs.context()),keep_history};
    return cthompson(exp);
  }
   

    
}}//end of ns awali::stc

#endif // !AWALI_ALGOS_COMPACT_THOMPSON_HH
