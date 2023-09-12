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

#ifndef AWALI_ALGOS_COMPOSE_HH
# define AWALI_ALGOS_COMPOSE_HH

# include <stack>
# include <unordered_map>

#include <awali/sttc/algos/outsplit.hh>
#include <awali/sttc/algos/sort.hh>
#include <awali/sttc/labelset/tupleset.hh>
#include <awali/sttc/misc/sub_tuple.hh> // make_index_sequence
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon
#include <awali/sttc/algos/projection.hh> // tuple_to_tupleset
#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/algos/partial_identity.hh>
#include <awali/sttc/history/tuple_history.hh>


namespace awali { namespace sttc {

  namespace internal
  {
    template<typename T> struct aff{};


    /*---------------------------------.
    | composer<automaton, automaton>.  |
    `---------------------------------*/

    template <typename T, size_t I> struct rem_in_tupleset;
    
    template <typename... T, size_t I>
    struct rem_in_tupleset<tupleset<T...>,I>{
      using tp_t = tupleset<T...>;
      using type = typename tuple_to_tupleset<typename rem_in_tuple<I>::template type<std::tuple<T...>>>::type;

      static
      type get(const tp_t& t) {
        return {rem_in_tuple<I>::get(t.sets())};
      }
    };

    template <size_t I, size_t J, typename Tuple1, typename Tuple2>
    auto
    concat_and_remove(const Tuple1& t1, const Tuple2& t2)
      -> typename std::concat_tuple<
          typename rem_in_tuple<I>::template type<Tuple1>,
          typename rem_in_tuple<J>::template type<Tuple2>>::type
    {
      return std::tuple_cat(rem_in_tuple<I>::get(t1),
                            rem_in_tuple<J>::get(t2));
    }

    /// Build the (accessible part of the) composition.

    template <typename Lhs, typename Rhs, size_t I, size_t J>
    class composer
    {
      static_assert(Lhs::element_type::context_t::is_lat,
                    "requires labels_are_tuples");
      static_assert(Rhs::element_type::context_t::is_lat,
                    "requires labels_are_tuples");
    public:
      using clhs_t = Lhs;
      using crhs_t = Rhs;
      using l_labelset_t = labelset_t_of<Lhs>;
      using r_labelset_t = labelset_t_of<Rhs>;
      using l_weightset_t = weightset_t_of<Lhs>;
      using r_weightset_t = weightset_t_of<Rhs>;
      using I_labelset_t = typename l_labelset_t::template valueset_t<I>;
      using J_labelset_t = typename r_labelset_t::template valueset_t<J>;
      using minusI_labelset_t = typename rem_in_tupleset<l_labelset_t,I>::type;
      using minusJ_labelset_t = typename rem_in_tupleset<r_labelset_t,J>::type;

      /// The type of context of the result.
      using labelset_t = typename concat_tupleset<minusI_labelset_t, minusJ_labelset_t>::type;
      using weightset_t = join_t<weightset_t_of<context_t_of<Lhs>>,
                                 weightset_t_of<context_t_of<Rhs>>>;
      using context_t = sttc::context<labelset_t, weightset_t>;

      /// The type of the resulting automaton.
      using automaton_t = mutable_automaton<context_t>;

      /// Result state type.
      using pair_state_t = std::pair<state_t,state_t>;

      composer(const Lhs& lhs, const Rhs& rhs)
        : lhs_(lhs),
          rhs_(rhs),
          output_(make_mutable_automaton(make_context_(lhs, rhs)))
      {}

      static labelset_t make_labelset_(const l_labelset_t& ll,
                                       const r_labelset_t& rl)
      {
        return {concat_and_remove<I,J>(ll.sets(),rl.sets())};
      }

      static context_t
      make_context_(const Lhs& lhs, const Rhs& rhs)
      {
        return {make_labelset_(*lhs->context().labelset(), *rhs->context().labelset()),
                join(*lhs->weightset(), *rhs->weightset())};
      }

      state_t state(const pair_state_t& ps)
      {
        state_t res;
        auto i = map_.find(ps);
        if (i == std::end(map_))
          {
            res = output_->add_state();
            map_[ps] = res;
            todo_.push(ps);
          }
        else
          res = i->second;
        return res;
      }

      /// The (accessible part of the) product of \a lhs_ and \a rhs_.
      automaton_t compose()
      {
        pair_state_t ps{lhs_->pre(),rhs_->pre()};
        map_[ps] = output_->pre();
        todo_.push(ps);
        map_[std::make_pair(lhs_->post(),rhs_->post())] = output_->post();
        const auto& ws = *output_->context().weightset();
        while (!todo_.empty())
          {
            ps = todo_.top();
            todo_.pop();
            state_t src = state(ps);
            auto it1 = lhs_->all_out(ps.first).begin();
            auto end1 = lhs_->all_out(ps.first).end();
            if(it1 == end1)
              continue;
            if(is_epsilon<I_labelset_t>(std::get<I>(lhs_->label_of(*it1))))
              for(auto tr:lhs_->all_out(ps.first)) {
                state_t q = lhs_->dst_of(tr);
                state_t dst = state(std::make_pair(q, ps.second));
                output_->add_transition(src, dst,
                                        std::tuple_cat(rem_in_tuple<I>::get(lhs_->label_of(tr)), get_epsilon<minusJ_labelset_t>()),
                                        lhs_->weight_of(tr));
              }
            else {
              auto it2 = rhs_->all_out(ps.second).begin();
              auto end2 = rhs_->all_out(ps.second).end();
              while(it2 != end2) {
                const auto& tr2=*it2;
                if(is_epsilon<J_labelset_t>(std::get<J>(rhs_->label_of(tr2)))) {
                  state_t q = rhs_->dst_of(tr2);
                  state_t dst = state(std::make_pair(ps.first, q));
                  output_->add_transition(src, dst,
                                          std::tuple_cat(get_epsilon<minusI_labelset_t>(),rem_in_tuple<J>::get(rhs_->label_of(tr2))),
                                          rhs_->weight_of(tr2));
                  ++it2;
                }
                else if(it1 == end1)
                  ++it2;
                else if(I_labelset_t::less_than(std::get<I>(lhs_->label_of(*it1)),
                                                std::get<J>(rhs_->label_of(tr2))))
                  ++it1;
                else if(I_labelset_t::less_than(std::get<J>(rhs_->label_of(tr2)),
                                                std::get<I>(lhs_->label_of(*it1))))
                  ++it2;
                else {
                  auto begin1 = it1;
                  while(it2 != end2 && I_labelset_t::equals(std::get<I>(lhs_->label_of(*begin1)),
                                                            std::get<J>(rhs_->label_of(*it2)))) {
                    it1=begin1;
                    while(it1 != end1 &&I_labelset_t::equals(std::get<I>(lhs_->label_of(*it1)),
                                                             std::get<J>(rhs_->label_of(*it2)))) {
                      state_t q1 = lhs_->dst_of(*it1);
                      state_t q2 = rhs_->dst_of(*it2);
                      state_t dst = state(std::make_pair(q1, q2));
                      output_->add_transition(src, dst,
                                              std::tuple_cat(rem_in_tuple<I>::get(lhs_->label_of(*it1)),
                                                             rem_in_tuple<J>::get(rhs_->label_of(*it2))),
                                              ws.mul(lhs_->weight_of(*it1),rhs_->weight_of(*it2)));
                      ++it1;
                    }
                    ++it2;
                  }
                }
              }
            }
          }
        return output_;
      }

      void set_history() {
        auto history = std::make_shared<tuple_history<std::tuple<Lhs,Rhs>>>(std::make_tuple(lhs_,rhs_));
        output_->set_history(history);
        for (const auto& p: map_)
          history->add_state(p.second, p.first);
      }

    private:
      const Lhs& lhs_;
      const Rhs& rhs_;
      using label_t = typename labelset_t::value_t;
      using weight_t = typename weightset_t::value_t;

      std::unordered_map<pair_state_t, state_t> map_;
      std::stack<pair_state_t> todo_;

      automaton_t output_;
    };
  }

  /*--------------------------------.
  | compose(automaton, automaton).  |
  `--------------------------------*/
    /** Composition of two transducers on given tapes
     *
     * This function computes the composition of the accessible part of two transducers with respect to statically specified tapes.
     *
     * In order to support weighted compositions,
     * an outsplit is applied to \a tdc1: 
     * for every state *s*, if among labels on tape \p I of transitions outgoing from *s*, there are both epsilon and non epsilon labels,
     * this state is splitted in order to separate these classes of transitions.
     * Then, the composition is performed with the constraint that a state with "epsilon outgoing transitions" of \a tdc1 can not be reached by a transition with label epsilon on tape J of \a tdc2.
     * This forces to deal with epsilons first in \a tdc1; this non commutativity of epsilon transitions guarantees that a pair of matching computations in \a tdc1 and \a tdc2 is only represented once in the result. 
     *
     * In the result, the tapes of \a tdc1 are appened to the tapes of \a tdc2, except the tape \p I of \a tdc1 and the tape \p J of \a tdc2 which are deleted. 
     *
     * @tparam I the index of the composing tape of the first transducer
     * @tparam J the index of the composing tape of the second transducer
     * @tparam TDC1 the type of the firt transducer
     * @tparam TDC2 the type of the second transducer
     * @param tdc1 the first transducer
     * @param tdc2 the second transducer
     * @param keep_history if true, every state of the result is linked to a  pair of states of tdc1 and tdc2
     * @return a transducer which is the result of the composition
     */
  template <size_t I, size_t J, typename TDC1, typename TDC2>
  auto
  composeIJ(TDC1& tdc1, TDC2& tdc2, bool keep_history=true)
    -> typename internal::composer<TDC1, TDC2, I, J>::automaton_t
  {
    auto l = outsplit<I>(tdc1, keep_history);
    sort_tape<I>(l);
    sort_tape<J>(tdc2);
    internal::composer<TDC1, TDC2, I, J> compose(l, tdc2);
    auto r= compose.compose();
    if(keep_history)
      compose.set_history();
    proper_here(r);
    return r;
  }

    /** Composition of two transducers
     *
     * Compose \a tdc1 to \a tdc2 two w.r.t. the second tape of \a tdc1 and the first tape of \a tdc2.    
     * This is equivalent to {@link composeIJ}&lt;1,0&gt;(\a tdc1, \a tdc2)
     *
     * @tparam TDC1 the type of the firt transducer
     * @tparam TDC2 the type of the second transducer
     * @param tdc1 the first transducer
     * @param tdc2 the second transducer
     * @param keep_history if true, every state of the result is linked to a  pair of states of tdc1 and tdc2
     * @return a transducer which is the result of the composition
     */
  template <typename TDC1, typename TDC2>
  auto
  compose(const TDC1& tdc1, const TDC2& tdc2, bool keep_history=true)
    -> typename internal::composer<TDC1, TDC2, 1, 0>::automaton_t
  {
    return composeIJ<1,0>(tdc1, tdc2, keep_history);
  }

    /** Evaluation of an automaton by a transducer
     *
     * The evaluation is made by composition of the automaton with respect to the first tape of the transducer.    
     * The result corresponds then to the second tape of the input transducer.
     *
     * @tparam Aut the type of the automaton
     * @tparam Tdc the type of the transducer
     * @param aut the automaton
     * @param tdc the transducer
     * @param keep_history if true, every state of the result is linked to a  pair of states of aut and tdc
     * @return an automaton 
     */
  template <typename Aut, typename Tdc>
  auto
  eval_tdc(const Aut& aut, const Tdc& tdc, bool keep_history=true)
    -> decltype(projection<1>(tdc))
  {
    auto l = partial_identity<1>(aut,keep_history);
    auto r = composeIJ<0,0>(l, tdc, keep_history);
    return projection<0>(r,keep_history);
  }

}}//end of ns awali::stc


#endif // !AWALI_ALGOS_COMPOSE_HH
