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

#ifndef AWALI_ALGOS_PROJECTION_HH
# define AWALI_ALGOS_PROJECTION_HH

# include <unordered_map>

#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/misc/set.hh>
#include <awali/sttc/labelset/tupleset.hh>
#include <awali/sttc/misc/sub_tuple.hh>
#include <awali/sttc/history/single_history.hh>

namespace awali { namespace sttc {

  /*---------------------------.
  | projection(transducteur).  |
  `---------------------------*/

  namespace internal
  {

    template <typename Labelset, size_t I> struct select;
    template <typename Labelset, size_t... I> struct selects;
    template <typename Labelset, size_t... I> struct select_tail;

    template <typename... T, size_t I>
    struct select<tupleset<T...>,I>{
      using tp_t = tupleset<T...>;
      using labelset_t = typename tp_t::template valueset_t<I>;

      static
      labelset_t get(const tp_t& t) {
        return std::get<I>(t.sets());
      }
    };

    template<typename T> struct tuple_to_tupleset;
    template<typename... T>
    struct tuple_to_tupleset<std::tuple<T...>> {
      using type = tupleset<T...>;
    };

    template <typename... T, size_t... I>
    struct selects<tupleset<T...>,I...>{
      using tp_t = tupleset<T...>;
      using labelset_t = typename tuple_to_tupleset<typename sub_tuple<I...>::template type<std::tuple<T...>>>::type;

      static
      labelset_t get(const tp_t& t) {
        return {sub_tuple<I...>::get(t.sets())};
      }
    };

    template <typename... T>
    struct select_tail<tupleset<T...>>{
      using tp_t = tupleset<T...>;
      using labelset_t = typename tuple_to_tupleset<typename tail_tuple<std::tuple<T...>>::type>::type;

      static
      labelset_t get(const tp_t& t) {
        return {tuple_tail(t.sets())};
      }
    };

    template <typename Aut, size_t I>
    struct projector
    {
      using context_t = context_t_of<Aut>;
      using labelset_t = labelset_t_of<context_t>;
      using weightset_t = weightset_t_of<context_t>;
      using out_labelset_t =  typename select<labelset_t,I>::labelset_t;
      using out_context_t = context<out_labelset_t, weightset_t>;
      using out_automaton_t = mutable_automaton<out_context_t>;

      projector(const Aut& in)
        : in_(in)
      {
        context_t in_context = in->context();
        auto in_labelset = in_context.labelset();
        auto weightset = in_context.weightset();
        auto out_labelset = select<labelset_t,I>::get(*in_labelset);
        out_context_t out_context{out_labelset, *weightset};
        out_ = make_mutable_automaton(out_context);

      }

      out_automaton_t operator()()
      {
        // Copy the states.  We cannot iterate on the transitions
        // only, as we would lose the states without transitions.
        for (auto s: in_->states())
            out_state[s] = out_->add_state();
        out_state[in_->pre()]= out_->pre();
        out_state[in_->post()]= out_->post();

        for (auto t : in_->all_transitions())
          {
            auto src = out_state.find(in_->src_of(t));
            auto dst = out_state.find(in_->dst_of(t));
            if (src != out_state.end() && dst != out_state.end()){
              out_->new_transition(src->second, dst->second,
                                   std::get<I>(in_->label_of(t)), in_->weight_of(t));
            }
          }
        return out_;
      }

      void set_history() {
        auto history = std::make_shared<single_history<Aut>>(in_);
        out_->set_history(history);
        for (auto p: in_->all_states())
            history->add_state(out_state[p], p);
      }

      /// Input automaton.
      const Aut& in_;
      /// Output automaton.
      mutable_automaton<out_context_t> out_;
      /// input state -> output state.
      std::unordered_map<state_t, state_t> out_state;
    };

    template <typename Aut>
    struct imagers
    {
      using context_t = context_t_of<Aut>;
      using labelset_t = labelset_t_of<context_t>;
      using weightset_t = weightset_t_of<context_t>;
      using out_labelset_t =  typename select_tail<labelset_t>::labelset_t;
      using out_context_t = context<out_labelset_t, weightset_t>;
      using out_automaton_t = mutable_automaton<out_context_t>;

      imagers(const Aut& in)
        : in_(in)
      {
        context_t in_context = in->context();
        auto in_labelset = in_context.labelset();
        auto weightset = in_context.weightset();
        auto out_labelset = select_tail<labelset_t>::get(*in_labelset);
        out_context_t out_context{out_labelset, *weightset};
        out_ = make_mutable_automaton(out_context);

      }

      out_automaton_t operator()()
      {
        // Copy the states.  We cannot iterate on the transitions
        // only, as we would lose the states without transitions.
        for (auto s: in_->states())
            out_state[s] = out_->add_state();
        out_state[in_->pre()]= out_->pre();
        out_state[in_->post()]= out_->post();

        for (auto t : in_->all_transitions())
          {
            auto src = out_state.find(in_->src_of(t));
            auto dst = out_state.find(in_->dst_of(t));
            if (src != out_state.end() && dst != out_state.end()){
              out_->new_transition(src->second, dst->second,
                                   tuple_tail(in_->label_of(t)), in_->weight_of(t));
            }
          }
        return out_;
      }

      void set_history() {
        auto history = std::make_shared<single_history<Aut>>(in_);
        out_->set_history(history);
        for (auto p: in_->all_states())
            history->add_state(out_state[p], p);
      }

      /// Input automaton.
      const Aut& in_;
      /// Output automaton.
      mutable_automaton<out_context_t> out_;
      /// input state -> output state.
      std::unordered_map<state_t, state_t> out_state;
    };


    template <typename Aut, size_t... I>
    struct projectors
    {
      using context_t = context_t_of<Aut>;
      using labelset_t = labelset_t_of<context_t>;
      using weightset_t = weightset_t_of<context_t>;
      using out_labelset_t =  typename selects<labelset_t,I...>::labelset_t;
      using out_context_t = context<out_labelset_t, weightset_t>;
      using out_automaton_t = mutable_automaton<out_context_t>;

      projectors(const Aut& in)
        : in_(in)
      {
        context_t in_context = in->context();
        auto in_labelset = in_context.labelset();
        auto weightset = in_context.weightset();
        auto out_labelset = selects<labelset_t,I...>::get(*in_labelset);
        out_context_t out_context{out_labelset, *weightset};
        out_ = make_mutable_automaton(out_context);
      }

      out_automaton_t operator()()
      {
        // Copy the states.  We cannot iterate on the transitions
        // only, as we would lose the states without transitions.
        for (auto s: in_->states())
            out_state[s] = out_->add_state();
        out_state[in_->pre()]= out_->pre();
        out_state[in_->post()]= out_->post();

        for (auto t : in_->all_transitions())
          {
            auto src = out_state.find(in_->src_of(t));
            auto dst = out_state.find(in_->dst_of(t));
            if (src != out_state.end() && dst != out_state.end()){
              out_->new_transition(src->second, dst->second,
                                   sub_tuple<I...>::get(in_->label_of(t)), in_->weight_of(t));
            }
          }
        return out_;
      }

      void set_history() {
        auto history = std::make_shared<single_history<Aut>>(in_);
        out_->set_history(history);
        for (auto p: in_->all_states())
            history->add_state(out_state[p], p);
      }

      /// Input automaton.
      const Aut& in_;
      /// Output automaton.
      mutable_automaton<out_context_t> out_;
      /// input state -> output state.
      std::unordered_map<state_t, state_t> out_state;
    };
  }

    /** Projection of one tape of a transducer
     *
     * @tparam I the index of a tape
     * @tparam Tdc the type of the transducer
     * @param in a transducer
     * @param keep_history if true the result is endowed with a {@link single_history} to the input transducer
     * @return an automaton which is the projection of the tape \p I of \a in
     */
  template <size_t I, typename Tdc>
  inline
  auto
  projection(const Tdc& in, bool keep_history=true) -> typename internal::projector<Tdc, I>::out_automaton_t
  {
    internal::projector<Tdc, I> proj(in);
    auto r= proj();
    if(keep_history)
      proj.set_history();
    return r;
  }

    /** Projection of last tapes of a transducer
     *
     * @tparam Tdc the type of the transducer
     * @param in a transducer
     * @param keep_history if true the result is endowed with a {@link single_history} to the input transducer
     * @return a transducer obtained from \a in by deleting the first tape
     */
  template <typename Tdc>
  inline
  auto
  images(const Tdc& in, bool keep_history=true) -> typename internal::imagers<Tdc>::out_automaton_t
  {
    internal::imagers<Tdc> proj(in);
    auto r= proj();
    if(keep_history)
      proj.set_history();
    return r;
  }

    /** Projection and/or permutation of tapes of a transducer
     *
     * The tapes of the result are given by the list \p I.
     * They appear in the result with the same order as in the list.
     * This function allows to delete or duplicate some tapes.
     *
     * @tparam I the list of the indices of tapes
     * @tparam Tdc the type of the transducer
     * @param in a transducer
     * @param keep_history if true the result is endowed with a {@link single_history} to the input transducer
     * @return a transducer obtained from \a in with the given tapes
     */
  template <size_t... I, typename Tdc>
  inline
  auto
  projections(const Tdc& in, bool keep_history=true) -> typename internal::projectors<Tdc, I...>::out_automaton_t
  {
    internal::projectors<Tdc, I...> proj(in);
    auto r= proj();
    if(keep_history)
      proj.set_history();
    return r;
  }
}}//end of ns awali::stc

#endif // !AWALI_ALGOS_PROJECTION_HH
