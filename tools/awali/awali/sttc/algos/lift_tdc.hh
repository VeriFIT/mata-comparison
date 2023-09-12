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

#ifndef AWALI_ALGOS_LIFT_TDC_HH
# define AWALI_ALGOS_LIFT_TDC_HH

# include <map>

#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/labelset/oneset.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/core/rat/ratexpset.hh>
#include <awali/sttc/core/rat/ratexp.hh>

namespace awali { namespace sttc {


  namespace internal
  {

    template <typename Tdc>
    struct tdc_lifter {
      using i_labelset_t = labelset_t_of<Tdc>;
      using i_weightset_t = weightset_t_of<Tdc>;
      using o_labelset_t = typename i_labelset_t::template valueset_t<0>;
      using exp_labelset_t = typename i_labelset_t::template valueset_t<1>;
      using exp_context_t = context<typename labelset_trait<exp_labelset_t>::ratlabelset_t,
                                    i_weightset_t>;
      using o_weightset_t = ratexpset_of<exp_context_t>;
      using o_context_t = context<o_labelset_t, o_weightset_t>;

      using automaton_t = mutable_automaton<o_context_t>;

      tdc_lifter (const Tdc& tdc) : tdc_(tdc) {}

      /*------------------.
        | lift(automaton).  |
        `------------------*/

      inline
      mutable_automaton<o_context_t>
      lift()
      {
        const o_labelset_t& o_labelset{std::get<0>(tdc_->context().labelset()->sets())};
        const exp_labelset_t exp_labelset{std::get<1>(tdc_->context().labelset()->sets())};
        exp_context_t exp_context{std::get<1>(tdc_->context().labelset()->sets()), *(tdc_->context().weightset())};
        o_weightset_t o_weightset{exp_context, rat::identities::series};
        o_context_t o_context{o_labelset, o_weightset};
        res = make_mutable_automaton(o_context);
        in_out_map[tdc_->pre()] = res->pre();
        in_out_map[tdc_->post()] = res->post();
        for (auto s: tdc_->states())
          in_out_map[s] = res->add_state();

        for (auto t: tdc_->all_transitions())
          if (tdc_->src_of(t) == tdc_->pre())
            res->add_initial(in_out_map[tdc_->dst_of(t)],
                             o_weightset.lmul(tdc_->weight_of(t), o_weightset.one()));
          else if (tdc_->dst_of(t) == tdc_->post())
            res->add_final(in_out_map[tdc_->src_of(t)],
                           o_weightset.lmul(tdc_->weight_of(t), o_weightset.one()));
          else if (exp_labelset.is_one(std::get<1>(tdc_->label_of(t))))
            {
              res->add_transition
                (in_out_map[tdc_->src_of(t)], in_out_map[tdc_->dst_of(t)],
                 std::get<0>(tdc_->label_of(t)),
                 o_weightset.lmul(tdc_->weight_of(t), o_weightset.one()));
            }
          else
            res->add_transition
              (in_out_map[tdc_->src_of(t)], in_out_map[tdc_->dst_of(t)],
               std::get<0>(tdc_->label_of(t)),
               o_weightset.lmul(tdc_->weight_of(t), o_weightset.atom(std::get<1>(tdc_->label_of(t)))));
        return res;
      }

      void set_history() {
        auto history = std::make_shared<single_history<Tdc>>(tdc_);
        res->set_history(history);
        for (auto p: in_out_map)
            history->add_state(p.second, p.first);
      }


      const Tdc& tdc_;
      automaton_t res;
      std::map<state_t, state_t> in_out_map;
    };
  }

  template <typename Tdc>
  inline
  auto
  lift_tdc(const Tdc& tdc, bool keep_history=true) -> typename internal::tdc_lifter<Tdc>::automaton_t
  {
    auto tl = internal::tdc_lifter<Tdc>(tdc);
    tl.lift();
    if(keep_history)
      tl.set_history();
    return tl.res;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_LIFT_TDC_HH
