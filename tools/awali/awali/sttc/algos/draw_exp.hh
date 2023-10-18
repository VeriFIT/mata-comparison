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

#ifndef AWALI_ALGOS_DRAW_EXP_HH
# define AWALI_ALGOS_DRAW_EXP_HH

#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/ctx/law_char.hh>
#include <awali/sttc/weightset/b.hh>

namespace awali { namespace sttc {

    namespace rat
    {
      /// \tparam Aut      relative the generated automaton
    /// \tparam Context  relative to the RatExp.
      template <typename RatExpSet>
      class drawexp_visitor
        : public RatExpSet::context_t::const_visitor
      {
      public:
        using aut_labelset_t = ctx::law_char;
        using aut_context_t = context<aut_labelset_t, b>;
        using automaton_t = sttc::mutable_automaton<aut_context_t>;
        using context_t = typename RatExpSet::context_t;
        using weightset_t = weightset_t_of<context_t>;
        using labelset_t = labelset_t_of<context_t>;
        using weight_t = weight_t_of<context_t>;
        using super_type = typename context_t::const_visitor;
        using label_t = label_t_of<automaton_t>;

        constexpr static const char* me() { return "draw_exp"; }

        drawexp_visitor(const RatExpSet& rs)
          : ctx_(rs.context())
        { }

        automaton_t
        operator()(const typename RatExpSet::ratexp_t& v)
        {
          v->accept(*this);
          res_->set_initial(initial_);
          return std::move(res_);
        }

        AWALI_RAT_VISIT(zero,)
        {
          initial_ = res_->add_state();
          res_->set_state_name(initial_,"[z]");
        }

        AWALI_RAT_VISIT(one,)
        {
          initial_ = res_->add_state();
          res_->set_state_name(initial_,"[e]");
        }

        AWALI_RAT_VISIT(atom, e)
        {
          std::ostringstream oss;
          ls_.print(e.value(), oss);
          initial_ = res_->add_state();
          res_->set_state_name(initial_,oss.str());
        }

        AWALI_RAT_VISIT(sum, e)
        {
          state_t initial = res_->add_state();
          std::string l("");
          for (auto c: e)
            {
              c->accept(*this);
              l+=".";
              res_->new_transition(initial, initial_, l);
            }
          initial_ = initial;
          res_->set_state_name(initial_,"[+]");
        }

        AWALI_RAT_UNSUPPORTED(complement)
        AWALI_RAT_UNSUPPORTED(conjunction)
        AWALI_RAT_UNSUPPORTED(ldiv)
        AWALI_RAT_UNSUPPORTED(shuffle)
        AWALI_RAT_UNSUPPORTED(transposition)

        AWALI_RAT_VISIT(prod, e)
        {
          state_t initial = res_->add_state();
          std::string l("");
          for (auto c: e)
            {
              c->accept(*this);
              l+=".";
              res_->new_transition(initial, initial_, l);
            }
          initial_ = initial;
          res_->set_state_name(initial_,"[.]");
        }

        AWALI_RAT_VISIT(star, e)
        {
          e.sub()->accept(*this);
          state_t initial = res_->add_state();
          res_->new_transition(initial, initial_, " ");
          initial_ = initial;
          res_->set_state_name(initial_,"[*]");
        }

        AWALI_RAT_VISIT(maybe, e)
        {
          e.sub()->accept(*this);
          state_t initial = res_->add_state();
          res_->new_transition(initial, initial_, " ");
          initial_ = initial;
          res_->set_state_name(initial_,"[?]");
        }

        AWALI_RAT_VISIT(plus, e)
        {
          e.sub()->accept(*this);
          state_t initial = res_->add_state();
          res_->new_transition(initial, initial_, " ");
          initial_ = initial;
          res_->set_state_name(initial_,"[^+]");
        }

        AWALI_RAT_VISIT(lweight, e)
        {
          e.sub()->accept(*this);
          std::ostringstream oss;
          oss<<"<";
          ws_.print(e.weight(), oss)<< ">";
          res_->print_state(initial_, oss);
          res_->set_state_name(initial_,oss.str());
        }

        AWALI_RAT_VISIT(rweight, e)
        {
          e.sub()->accept(*this);
          std::ostringstream oss;
          res_->print_state(initial_, oss) << "<";
          ws_.print(e.weight(), oss)<< ">";
          res_->set_state_name(initial_,oss.str());
        }

      private:
        automaton_t make_automaton () {
          typename aut_labelset_t::genset_t alph{'.',' '};
          aut_labelset_t ls(alph);
          aut_context_t ct(ls, b());
          return make_shared_ptr<automaton_t>(ct);
        }

        automaton_t res_ = make_automaton() ;
        const context_t& ctx_;
        const weightset_t& ws_ = *ctx_.weightset();
        const labelset_t& ls_ = *ctx_.labelset();
        const label_t epsilon_ = res_->labelset()->special();
        state_t initial_ = automaton_t::element_type::null_state();
     };

    } // rat::
    
    /* @brief build the graph of the expression
     *
     * This function builds an automaton over the alphabet {.}
     * where labels are words.
     * Every node of the expression becomes a state of the automaton
     * the root is the initial state; there is no final state.
     * This automaton is actually a tree.
     * The label of every state depends on the nature of the corresponding node.
     *
     * @tparam RatExpSet the type of the ratexpset of the expression
     * @param rs the ratexpset of the expression
     * @param exp the rational expression
     * @return an automaton representing the tree of the rational expression
     */
    template <typename RatExpSet>
    mutable_automaton<context<ctx::law_char, b>>
      draw_exp(const RatExpSet& rs, const typename RatExpSet::ratexp_t& exp)
    {
      rat::drawexp_visitor<RatExpSet> drawexp{rs};
      return drawexp(exp);
    }
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_DRAW_EXP_HH
