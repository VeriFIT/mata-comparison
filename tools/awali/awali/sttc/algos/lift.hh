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

#ifndef AWALI_ALGOS_LIFT_HH
# define AWALI_ALGOS_LIFT_HH

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
    /*--------------.
    | lift(types).  |
    `--------------*/

    template <typename ValueSet>
    struct context_of
    {
      using type = typename ValueSet::context_t;
    };

    template <typename Context>
    struct context_of<sttc::mutable_automaton<Context>>
    {
      using type = Context;
    };

    template <typename ValueSet>
    using context_of_t = typename context_of<ValueSet>::type;

    template <typename Context>
    using lifted_context_t =
      context<oneset, ratexpset_of<Context>>;

    template <typename Aut>
    using lifted_automaton_t =
      mutable_automaton<lifted_context_t<context_t_of<Aut>>>;

    template <typename RatExpSet>
    using lifted_ratexpset_t =
      ratexpset<lifted_context_t<context_t_of<RatExpSet>>>;

    // lift(ctx) -> ctx
    template <typename LabelSet, typename WeightSet>
    lifted_context_t<context<LabelSet, WeightSet>>
    lift_context(const context<LabelSet, WeightSet>& ctx)
    {
      auto rs_in
        = ratexpset_of<context<LabelSet, WeightSet>>(get_rat_context(ctx),
                                                  rat::identities::trivial);
      return {oneset{}, rs_in};
    }

    // lift(ratexpset) -> ratexpset
    template <typename Context>
    lifted_ratexpset_t<ratexpset<Context>>
    lift_ratexpset(const ratexpset<Context>& rs)
    {
      return {lift_context(rs.context()), rs.identities()};
    }

  }

  /*------------------.
  | lift(automaton).  |
  `------------------*/

  template <typename Aut>
  inline
  internal::lifted_automaton_t<Aut>
  lift(const Aut& a, bool keep_history=true)
  {
    using auto_in_t = typename Aut::element_type;
    using ctx_in_t = context_t_of<auto_in_t>;

    // Produce RatExps of the same context as the original automaton.
    using rs_in_t = ratexpset_of<ctx_in_t>;
    rs_in_t rs_in{get_rat_context(a->context()), rs_in_t::identities_t::trivial};

    auto ctx_out = internal::lift_context(a->context());
    using auto_out_t = internal::lifted_automaton_t<auto_in_t>;
    auto_out_t res = make_shared_ptr<auto_out_t>(ctx_out);
    std::map<state_t, state_t> map;
    map[a->pre()] = res->pre();
    map[a->post()] = res->post();
    for (auto s: a->states())
      map[s] = res->add_state();

    for (auto t: a->all_transitions())
      if (a->src_of(t) == a->pre())
        res->add_initial(map[a->dst_of(t)],
                        rs_in.lmul(a->weight_of(t), rs_in.one()));
      else if (a->dst_of(t) == a->post())
        res->add_final(map[a->src_of(t)],
                      rs_in.lmul(a->weight_of(t), rs_in.one()));
      else if (a->context().labelset()->is_one(a->label_of(t)))
        {
       res->add_transition
          (map[a->src_of(t)], map[a->dst_of(t)],
           {},
           rs_in.lmul(a->weight_of(t), rs_in.one()));
        }
      else
        res->add_transition
          (map[a->src_of(t)], map[a->dst_of(t)],
           {},
           rs_in.lmul(a->weight_of(t), rs_in.atom(a->label_of(t))));
    if(keep_history) {
      auto history = std::make_shared<single_history<Aut>>(a);
      for(auto p : map)
        history->add_state(p.second,p.first);
      res->set_history(history);
    }
    return res;
  }

  /*---------------.
  | lift(ratexp).  |
  `---------------*/

  namespace internal
  {
    template <typename Exp>
    using lifted_ratexp_t =
      typename lifted_context_t<context_t_of<Exp>>::ratexp_t;
  }

  template <typename RatExpSet>
  inline
  typename internal::lifted_ratexpset_t<RatExpSet>::ratexp_t
  lift(const RatExpSet& rs, const typename RatExpSet::ratexp_t& e)
  {
    auto lrs = internal::lift_ratexpset(rs);
    return lrs.lmul(e, lrs.one());
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_LIFT_HH
