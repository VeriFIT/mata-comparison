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

#ifndef AWALI_ALGOS_THOMPSON_HH
# define AWALI_ALGOS_THOMPSON_HH

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
    class thompson_visitor
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

      constexpr static const char* me() { return "thompson"; }

      thompson_visitor(const context_t& ctx, bool keep_history)
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
        res_->set_state_name(initial_,"s"+std::to_string(counter));
        res_->set_state_name(final_,"t"+std::to_string(counter++));
        history_->add_state(initial_,"zero");
      }

      AWALI_RAT_VISIT(one,)
      {
        initial_ = res_->add_state();
        final_ = res_->add_state();
        res_->set_state_name(initial_,"s"+std::to_string(counter));
        res_->set_state_name(final_,"t"+std::to_string(counter++));
        res_->new_transition(initial_, final_, epsilon_);
        history_->add_state(initial_,"one");
      }

      AWALI_RAT_VISIT(atom, e)
      {
        initial_ = res_->add_state();
        final_ = res_->add_state();
        res_->set_state_name(initial_,"s"+std::to_string(counter));
        res_->set_state_name(final_,"t"+std::to_string(counter++));
        res_->new_transition(initial_, final_, e.value());
        history_->add_state(initial_,"letter");
      }

      AWALI_RAT_VISIT(sum, e)
      {
        state_t initial = res_->add_state();
        state_t final = res_->add_state();
        for (auto c: e)
          {
            c->accept(*this);
            res_->new_transition(initial, initial_, epsilon_);
            res_->new_transition(final_, final, epsilon_);
          }
        initial_ = initial;
        final_ = final;
        res_->set_state_name(initial_,"s"+std::to_string(counter));
        res_->set_state_name(final_,"t"+std::to_string(counter++));
        history_->add_state(initial_,"sum");
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
            res_->new_transition(final, initial_, epsilon_);
          }
        initial_ = initial;
      }

      AWALI_RAT_VISIT(star, e)
      {
        e.sub()->accept(*this);
        state_t initial = res_->add_state();
        state_t final = res_->add_state();
        res_->new_transition(initial, initial_, epsilon_);
        res_->new_transition(final_,  final,    epsilon_);
        res_->new_transition(final_,  initial_, epsilon_);
        res_->new_transition(initial, final,    epsilon_);
        initial_ = initial;
        final_ = final;
        res_->set_state_name(initial_,"s"+std::to_string(counter));
        res_->set_state_name(final_,"t"+std::to_string(counter++));
        history_->add_state(initial_,"star");
      }

      AWALI_RAT_VISIT(maybe, e)
      {
        e.sub()->accept(*this);
        state_t initial = res_->add_state();
        state_t final = res_->add_state();
        res_->new_transition(initial, initial_, epsilon_);
        res_->new_transition(final_,  final,    epsilon_);
        res_->new_transition(initial, final,    epsilon_);
        initial_ = initial;
        final_ = final;
        res_->set_state_name(initial_,"s"+std::to_string(counter));
        res_->set_state_name(final_,"t"+std::to_string(counter++));
        history_->add_state(initial_,"maybe");
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
        history_->add_state(initial_,"plus");
      }

      AWALI_RAT_VISIT(lweight, e)
      {
        e.sub()->accept(*this);
        state_t initial = res_->add_state();
        state_t final = res_->add_state();
        const weight_t& w = e.weight();
        res_->new_transition(initial, initial_, epsilon_, w);
        res_->new_transition(final_,  final,    epsilon_);
        initial_ = initial;
        final_ = final;
        history_->add_state(initial_,"left wgt");	
      }

      AWALI_RAT_VISIT(rweight, e)
      {
        e.sub()->accept(*this);
        state_t initial = res_->add_state();
        state_t final = res_->add_state();
        const weight_t& w = e.weight();
        res_->new_transition(initial, initial_, epsilon_);
        res_->new_transition(final_,  final,    epsilon_, w);
        initial_ = initial;
        final_ = final;
        history_->add_state(initial_,"right wgt");	
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

   /** @brief builds a Thompson automaton 
    *
    * @tparam Aut      type of the generated automaton
    * @tparam Context  type of the context of the generated automaton
    * @param ctx          the context of the generated automaton
    * @param exp          the rational expression
    * @param keep_history (optional) if `true` (default value), the states are stamped with their origin
    * @return the Thompson automaton
    */
  template <typename Aut,
            typename Context = context_t_of<Aut>>
  Aut
  thompson(const Context& ctx, const typename Context::ratexp_t& exp, bool keep_history=true)
  {
    rat::thompson_visitor<Aut, Context> thompson{ctx,keep_history};
    return thompson(exp);
  }

   /** @brief builds a Thompson automaton 
    *
    * @tparam RatExpSet  type of the context of the rational expression
    * @param rs          the context of the rational expression
    * @param exp          the rational expression
    * @param keep_history (optional) if `true` (default value), the states are stamped with their origin
    * @return the Thompson automaton
    */
    template <typename RatExpSet>
    mutable_automaton<sttc::nullable_of<typename RatExpSet::context_t>>
    thompson(const RatExpSet& rs, const typename RatExpSet::ratexp_t& exp, bool keep_history=true)
    {
      using Aut=mutable_automaton<sttc::nullable_of<typename RatExpSet::context_t>>;
      rat::thompson_visitor<Aut, context_t_of<Aut>> thompson{sttc::get_nullable_context(rs.context()),keep_history};
      return thompson(exp);
    }
    
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_THOMPSON_HH
