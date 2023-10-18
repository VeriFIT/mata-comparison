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

#ifndef AWALI_PRODUCT_HH
# define AWALI_PRODUCT_HH

# include <iostream>
# include <map>
# include <utility>

# include <vector>
# include <memory>
#include <awali/sttc/history/tuple_history.hh>
#include <awali/sttc/core/transition_map.hh>
#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/vector.hh>
#include <awali/sttc/misc/zip_maps.hh>

namespace awali {
  namespace sttc {

    /// Join between automata.
    template <typename... Auts>
    auto
    join_automata(Auts&&... auts)
      -> decltype(make_mutable_automaton(join(auts->context()...)))
    {
      return make_mutable_automaton(join(auts->context()...));
    }

    /// Meet between automata.
    template <typename... Auts>
    auto
    meet_automata(Auts&&... auts)
      -> decltype(make_mutable_automaton(meet(auts->context()...)))
    {
      return make_mutable_automaton(meet(auts->context()...));
    }

    namespace internal {

      template<typename WS, typename W1, typename W2>
      auto variadic_mul(const WS& weightset, const W1& w1, const W2& w2)
        -> typename WS::value_t
      {
        return weightset.mul(w1,w2);
      }
      
      template<typename WS, typename W1, typename... W>
      auto variadic_mul(const WS& weightset, const W1& w1, const W&... w)
        -> typename WS::value_t
      {
        return weightset.mul(w1, variadic_mul(weightset, w...));
      }

      /*----------------------------.
        | product_algo_impl<Aut...>.  |
        `----------------------------*/

      /// Build the (accessible part of the) product.
      template <typename Aut, typename... Auts>
      class product_algo_impl
      {
        static_assert(all_<labelset_t_of<Auts>::is_free()...>(),
                      "requires free labels");

        /// The type of the resulting automaton.
        using automaton_t = Aut;

      public:
        using tuple_t = typename std::cst_tuple<state_t, std::tuple_size<std::tuple<Auts...>>::value>::type;
        template <std::size_t... I>
        using seq = awali::internal::index_sequence<I...>;

        /// The list of automaton indices as a static list.
        using indices_t = awali::internal::make_index_sequence<sizeof...(Auts)>;
        static constexpr indices_t indices{};

        /// The context of the result.
        using context_t = context_t_of<Aut>;
        using labelset_t = labelset_t_of<context_t>;
        using weightset_t = weightset_t_of<context_t>;

        using label_t = typename labelset_t::value_t;
        using weight_t = typename weightset_t::value_t;

        /// The type of our transition maps: convert the weight to weightset_t,
        /// non deterministic, and including transitions to post().
        template <typename A>
        using transition_map_t = transition_map<A, weightset_t, false, true>;

      public:
        /// The type of input automata.
        using automata_t = std::tuple<Auts...>;

        /// The type of the Ith input automaton, unqualified.
        template <size_t I>
        using input_automaton_t
        = base_t<typename std::tuple_element<I, automata_t>::type>;

        product_algo_impl(Aut aut, const Auts&... auts)
          : transition_maps_{{auts, *aut->weightset()}...}
          , aut_(aut)
          , auts_(auts...)
        {
          pmap_[pre_()] = aut_->pre();
          pmap_[post_()] = aut_->post();
        }

        /// The name of the pre of the output automaton.
        tuple_t pre_() const
        {
          return pre_(indices);
        }

        template <size_t... I>
        tuple_t pre_(seq<I...>) const
        {
          // clang 3.4 on top of libstdc++ wants this ctor to be
          // explicitly called.
          return tuple_t{(std::get<I>(auts_)->pre())...};
        }

        /// The name of the post of the output automaton.
        tuple_t post_() const
        {
          return post_(indices);
        }

        template <size_t... I>
        tuple_t post_(seq<I...>) const
        {
          // clang 3.4 on top of libstdc++ wants this ctor to be
          // explicitly called.
          return tuple_t{(std::get<I>(auts_)->post())...};
        }

        /// The state in the product corresponding to a tuple of states
        /// of operands.
        ///
        /// Add the given two source-automaton states to the worklist
        /// for the given result automaton if they aren't already there,
        /// updating the map; in any case return.
        state_t state(tuple_t state)
        {
          auto lb = pmap_.lower_bound(state);
          if (lb == pmap_.end() || pmap_.key_comp()(state, lb->first))
            {
              lb = pmap_.emplace_hint(lb, state, aut_->add_state());
              todo_.emplace_back(state);
            }
          return lb->second;
        }

        /// Compute the (accessible part of the) product.
        void product()
        {
          initialize_product();

          while (!todo_.empty())
            {
              tuple_t psrc = todo_.front();
              todo_.pop_front();
              state_t src = pmap_[psrc];

              add_product_transitions(src, psrc);
            }
        }

        /// Compute the (accessible part of the) shuffle product.
        void shuffle()
        {
          initialize_shuffle();

          while (!todo_.empty())
            {
              tuple_t psrc = todo_.front();
              todo_.pop_front();
              state_t src = pmap_[psrc];

              add_shuffle_transitions(src, psrc);
            }
        }

        /// Compute the (accessible part of the) infiltration product.
        void infiltration()
        {
          // Infiltrate is a mix of product and shuffle operations, and
          // the initial states for shuffle are a superset of the
          // initial states for product:
          initialize_shuffle();

          while (!todo_.empty())
            {
              tuple_t psrc = todo_.front();
              todo_.pop_front();
              state_t src = pmap_[psrc];

              // Infiltrate is a mix of product and shuffle operations.
              //
              // Product transitions must be added before shuffle ones:
              // this way "product" can use "new_transition" only, which
              // is faster than "add_transition".
              add_product_transitions(src, psrc);
              add_shuffle_transitions(src, psrc);
            }
        }

        void set_history() {
          auto history = std::make_shared<tuple_history<automata_t>>(auts_);
          aut_->set_history(history);
          for (const auto& p: pmap_)
            if(p.second != aut_->pre() && p.second != aut_->post())
              history->add_state(p.second, p.first);
        }

      private:
        /// Fill the worklist with the initial source-state pairs, as
        /// needed for the product algorithm.
        void initialize_product()
        {
          todo_.emplace_back(pre_());
        }

        /// Fill the worklist with the initial source-state pairs, as
        /// needed for the shuffle algorithm.
        void initialize_shuffle()
        {
          // Make the result automaton initial states: same as the
          // (synchronized) product of pre: synchronized transitions on $.
          add_product_transitions(aut_->pre(), pre_());
        }

        /// The outgoing tuple of transitions from state tuple \a ss.
        std::tuple<typename transition_map_t<Auts>::map_t&...>
        out_(const tuple_t& ss)
        {
          return out_(ss, indices);
        }

        template <size_t... I>
        std::tuple<typename transition_map_t<Auts>::map_t&...>
        out_(const tuple_t& ss, seq<I...>)
        {
          return std::tie(std::get<I>(transition_maps_)[std::get<I>(ss)]...);
        }

        /// Add transitions to the given result automaton, starting from
        /// the given result input state, which must correspond to the
        /// given pair of input state automata.  Update the worklist with
        /// the needed source-state pairs.
        void add_product_transitions(const state_t src,
                                     const tuple_t& psrc)
        {
          for (auto t: zip_map_tuple(out_(psrc)))
            // These are always new transitions: first because the
            // source state is visited for the first time, and second
            // because the couple (left destination, label) is unique,
            // and so is (right destination, label).
            internal::cross_tuple
              ([&] (const typename transition_map_t<Auts>::transition&... ts)
               {
                 aut_->new_transition(src, state(std::make_tuple(ts.dst...)),
                                      t.first, variadic_mul(*(aut_->weightset()), ts.wgt...));
               },
               t.second);
        }

        /// Add transitions to the given result automaton, starting from
        /// the given result input state, which must correspond to the
        /// given tuple of input state automata.  Update the worklist
        /// with the needed source-state pairs.
        void add_shuffle_transitions(const state_t src,
                                     const tuple_t& psrc)
        {
          weight_t final = add_shuffle_transitions_(src, psrc, indices);
          aut_->set_final(src, final);
        }

        /// Let all automata advance one after the other, and add the
        /// corresponding transitions in the output.
        ///
        /// Return the product of the final states.
        template <size_t... I>
        weight_t add_shuffle_transitions_(const state_t src,
                                          const tuple_t& psrc,
                                          seq<I...>)
        {
          weight_t res = aut_->weightset()->one();
          using swallow = int[];
          (void) swallow
            {
              (res = variadic_mul(*(aut_->weightset()), res,
                                            add_shuffle_transitions_<I>(src, psrc)),
               0)...
                };
          return res;
        }

        /// Let Ith automaton advance, and add the corresponding
        /// transitions in the output.
        ///
        /// If we reach a final state, return the corresponding final
        /// weight (zero otherwise).
        template <size_t I>
        weight_t
        add_shuffle_transitions_(const state_t src,
                                 const tuple_t& psrc)
        {
          // Whether is a final state.
          weight_t res = aut_->weightset()->zero();

          auto& ts = std::get<I>(transition_maps_)[std::get<I>(psrc)];
          for (auto t: ts)
            if (std::get<I>(auts_)->labelset()->is_special(t.first))
              res = t.second.front().wgt;
            else
              for (auto d: t.second) {
                auto pdst = psrc;
                std::get<I>(pdst) = d.dst;
                aut_->add_transition(src, state(pdst), t.first, d.wgt);
              }
          return res;
        }

        /// Transition caches.
        std::tuple<transition_map_t<Auts>...> transition_maps_;
        /// Map (automaton, state) -> has_only_epsilon_out
        using out_tuple_t = std::pair<size_t, state_t>;
        using out_map_t = std::map<out_tuple_t, bool>;
        out_map_t out_map;
        // Output automaton
        automaton_t aut_;
        /// Input automata, supplied at construction time.
        automata_t auts_;

        /// Map state-tuple -> result-state.
        using map = std::map<tuple_t, state_t>;
        map pmap_;

        /// Worklist of state tuples.
        std::deque<tuple_t> todo_;
      };
    }

    /*------------------------.
      | product(automaton...).  |
      `------------------------*/

    /// Build the (accessible part of the) product.
    template <typename... Auts>
    inline
    auto
    k_product_no_history(const Auts&... as)
      -> decltype(join_automata(as...))
    {
      auto res = join_automata(as...);
      internal::product_algo_impl<decltype(res), Auts...> algo(res, as...);
      algo.product();
      return res;
    }

    /// Build the (accessible part of the) product.
    template <typename... Auts>
    inline
    auto
    k_product_with_history(const Auts&... as)
      -> decltype(join_automata(as...))
    {
      auto res = join_automata(as...);
      internal::product_algo_impl<decltype(res), Auts...> algo(res, as...);
      algo.product();
      algo.set_history();
      return res;
    }

    template <typename Lhs, typename Rhs>
    inline
    auto
    product(const Lhs& lhs, const Rhs& rhs, bool keep_history=true)
      -> decltype(join_automata(lhs, rhs)) {
      auto res = join_automata(lhs, rhs);
      internal::product_algo_impl<decltype(res), Lhs, Rhs> algo(res, lhs, rhs);
      algo.product();
      if(keep_history)
        algo.set_history();
      if(lhs->get_name().empty() || rhs->get_name().empty()) {
        res->set_name("product");
        res->set_desc("Automaton obtained by a product");
      }
      else {
        res->set_name("prod-"+lhs->get_name()+"-"+rhs->get_name());
        res->set_desc("Product of "+lhs->get_name()+" and "+rhs->get_name());
      }
      return res;
    }

    /*------------------------.
      | shuffle(automaton...).  |
      `------------------------*/

    /// Build the (accessible part of the) product.
    template <typename... Auts>
    inline
    auto
    k_shuffle_no_history(const Auts&... as)
      -> decltype(join_automata(as...))
    {
      auto res = join_automata(as...);
      internal::product_algo_impl<decltype(res), Auts...> algo(res, as...);
      algo.shuffle();
      return res;
    }

    /// Build the (accessible part of the) product.
    template <typename... Auts>
    inline
    auto
    k_shuffle_with_history(const Auts&... as)
      -> decltype(join_automata(as...))
    {
      auto res = join_automata(as...);
      internal::product_algo_impl<decltype(res), Auts...> algo(res, as...);
      algo.shuffle();
      return res;
    }

    template <typename Lhs, typename Rhs>
    inline
    auto
    shuffle(const Lhs& lhs, const Rhs& rhs, bool keep_history=true)
      -> decltype(join_automata(lhs, rhs)) {
      auto res = join_automata(lhs, rhs);
      internal::product_algo_impl<decltype(res), Lhs, Rhs> algo(res, lhs, rhs);
      algo.shuffle();
      if(keep_history)
        algo.set_history();
      if(lhs->get_name().empty() || rhs->get_name().empty()) {
        res->set_name("shuffle");
        res->set_desc("Automaton obtained by a shuffle product");
      }
      else {
        res->set_name("shuffle-"+lhs->get_name()+"-"+rhs->get_name());
        res->set_desc("Shuffle of "+lhs->get_name()+" and "+rhs->get_name());
      }
      return res;
    }

    /*-------------------------------------.
      | infiltration(automaton, automaton).  |
      `-------------------------------------*/

    /// Build the (accessible part of the) infiltration.
    template <typename Lhs, typename Rhs>
    inline
    auto
    infiltration(const Lhs& lhs, const Rhs& rhs, bool keep_history=true)
      -> decltype(join_automata(lhs, rhs))
    {
      auto res = join_automata(lhs, rhs);
      internal::product_algo_impl<decltype(res), Lhs, Rhs> algo(res, lhs, rhs);
      algo.infiltration();
      if(keep_history)
        algo.set_history();
      if(lhs->get_name().empty() || rhs->get_name().empty()) {
        res->set_name("infiltration");
        res->set_desc("Automaton obtained by an infiltration product");
      }
      else {
        res->set_name("infilt-"+lhs->get_name()+"-"+rhs->get_name());
        res->set_desc("Infiltration of "+lhs->get_name()+" and "+rhs->get_name());
      }
      return res;
    }

    /*----------------------.
      | power(automaton, n).  |
      `----------------------*/

    template <typename Aut>
    auto
    power(const Aut& aut, unsigned n)
      -> typename Aut::element_type::automaton_nocv_t
    {
      auto res = make_mutable_automaton(aut->context());
      {
        // automatonset::one().
        auto s = res->add_state();
        res->set_initial(s);
        res->set_final(s);
        for (auto l: res->context().labelset()->genset())
          res->new_transition(s, s, l);
      }

      if (n)
        {
          // FIXME: for 1, we should return the accessible part only.
          static bool iterative = getenv("AWALI_ITERATIVE");
          if (iterative)
            for (size_t i = 0; i < n; ++i)
              res = product(res, aut, false);
          else
            {
              auto power = aut;
              while (true)
                {
                  if (n % 2)
                    res = product(res, power, false);
                  n /= 2;
                  if (!n)
                    break;
                  power = product(power, power, false);
                }
            }
        }

      return res;
    }


                      }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_PRODUCT_HH
