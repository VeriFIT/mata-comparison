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

#ifndef AWALI_CORE_TUPLE_AUTOMATON_HH
# define AWALI_CORE_TUPLE_AUTOMATON_HH

# include <deque>
# include <iostream>
# include <map>
# include <utility>

#include <awali/sttc/core/automaton_decorator.hh>
#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/map.hh> // has
#include <awali/common/tuple.hh>

namespace awali { namespace sttc {

  namespace internal
  {
    /*-------------------------------------.
    | tuple_automaton_impl<Aut, Auts...>.  |
    `-------------------------------------*/

    /// An automaton whose states are tuples of states of automata.
    ///
    /// Corresponds to the Cartesian product of states.
    ///
    /// \tparam Aut   the output automaton type
    /// \tparam Auts  the input automaton types
    template <typename Aut, typename... Auts>
    class tuple_automaton_impl
      : public automaton_decorator<Aut>
    {
      /// The type of the resulting automaton.
      using automaton_t = Aut;
      using super_t = automaton_decorator<automaton_t>;

    public:
      static std::string sname()
      {
        return "tuple_automaton" + sname_();
      }

      std::string vname(bool full = true) const
      {
        return "tuple_automaton" + vname_(full);
      }

      /// The type of context of the result.
      ///
      /// The type is the "join" of the contexts, independently of the
      /// algorithm.  However, its _value_ differs: in the case of the
      /// product, the labelset is the meet of the labelsets, it is
      /// its join for shuffle and infiltration.
      using context_t = context_t_of<Aut>;
      using labelset_t = labelset_t_of<context_t>;
      using weightset_t = weightset_t_of<context_t>;

      using label_t = typename labelset_t::value_t;
      using weight_t = typename weightset_t::value_t;

    public:
      /// The type of input automata.
      using automata_t = std::tuple<Auts...>;

      /// The type of the Ith input automaton, unqualified.
      template <size_t I>
      using input_automaton_t
      = internal::base_t<typename std::tuple_element<I, automata_t>::type>;

      using super_t::aut_;

      tuple_automaton_impl(const automaton_t& aut, const Auts&... auts)
        : super_t(aut)
        , auts_(auts...)
      {
        pmap_[pre_()] = aut_->pre();
        pmap_[post_()] = aut_->post();
      }

      std::ostream&
      print_state_name(typename super_t::state_t s, std::ostream& o,
                       const std::string& fmt = "text") const
      {
        return print_state_name_(s, o, fmt, indices);
      }

      /// Tuple of states of input automata.
      using pair_t = std::tuple<state_t_of<Auts>...>;

      /// A map from result state to tuple of original states.
      using origins_t = std::map<state_t, pair_t>;

      /// A map from result state to tuple of original states.
      const origins_t& origins() const
      {
        if (origins_.empty())
          for (const auto& p: pmap_)
            origins_.emplace(p.second, p.first);
        return origins_;
      }

      // FIXME: protected:
      /// A static list of integers.
      template <std::size_t... I>
      using seq = awali::internal::index_sequence<I...>;

      /// The list of automaton indices as a static list.
      using indices_t = awali::internal::make_index_sequence<sizeof...(Auts)>;
      static constexpr indices_t indices{};

      /// The sname of the sub automata.
      static std::string sname_()
      {
        std::string res = "<" + Aut::element_type::sname() + ", ";
        const char* sep = "";
        using swallow = int[];
        (void) swallow
          {
            (res += sep + Auts::element_type::sname(), sep = ", ", 0)...
          };
        res += ">";
        return res;
      }

      /// The vname of the sub automata.
      std::string vname_(bool full = true) const
      {
        return vname_(full, indices);
      }

      template <size_t... I>
      std::string vname_(bool full, seq<I...>) const
      {
        std::string res = "<" + aut_->vname(full) + ", ";
        const char* sep = "";
        using swallow = int[];
        (void) swallow
          {
            (res += sep + std::get<I>(auts_)->vname(full), sep = ", ", 0)...
          };
        res += ">";
        return res;
      }

      /// The name of the pre of the output automaton.
      pair_t pre_() const
      {
        return pre_(indices);
      }

      template <size_t... I>
      pair_t pre_(seq<I...>) const
      {
        // clang 3.4 on top of libstdc++ wants this ctor to be
        // explicitly called.
        return pair_t{(std::get<I>(auts_)->pre())...};
      }

      /// The name of the post of the output automaton.
      pair_t post_() const
      {
        return post_(indices);
      }

      template <size_t... I>
      pair_t post_(seq<I...>) const
      {
        // clang 3.4 on top of libstdc++ wants this ctor to be
        // explicitly called.
        return pair_t{(std::get<I>(auts_)->post())...};
      }

      /// The state in the product corresponding to a pair of states
      /// of operands.
      ///
      /// Add the given two source-automaton states to the worklist
      /// for the given result automaton if they aren't already there,
      /// updating the map; in any case return.
      state_t state(pair_t state)
      {
        auto lb = pmap_.lower_bound(state);
        if (lb == pmap_.end() || pmap_.key_comp()(state, lb->first))
          {
            lb = pmap_.emplace_hint(lb, state, aut_->add_state());
            todo_.emplace_back(state);
          }
        return lb->second;
      }

      state_t state(state_t_of<Auts>... ss)
      {
        return state(std::make_tuple(ss...));
      }

      template <size_t... I>
      std::ostream&
      print_state_name_(typename super_t::state_t s, std::ostream& o,
                        const std::string& fmt,
                        seq<I...>) const
      {
        auto i = origins().find(s);
        if (i == std::end(origins()))
          this->print_state(s, o);
        else
          {
            const char* sep = "";
            using swallow = int[];
            (void) swallow
              {
                (o << sep,
                 std::get<I>(auts_)->print_state_name(std::get<I>(i->second),
                                                      o, fmt),
                 sep = ", ",
                 0)...
               };
          }
        return o;
      }

      /// Input automata, supplied at construction time.
      automata_t auts_;

      /// Map state-tuple -> result-state.
      using map = std::map<pair_t, state_t>;
      map pmap_;

      /// Worklist of state tuples.
      std::deque<pair_t> todo_;

      mutable origins_t origins_;
    };
  }

  /// A product automaton as a shared pointer.
  template <typename... Auts>
  using tuple_automaton
    = std::shared_ptr<internal::tuple_automaton_impl<Auts...>>;

  template <typename... Auts>
  inline
  auto
  make_tuple_automaton(const Auts&... auts)
    -> tuple_automaton<Auts...>
  {
    using res_t = tuple_automaton<Auts...>;
    return make_shared_ptr<res_t>(auts...);
  }
}}//end of ns awali::stc

#endif // !AWALI_CORE_TUPLE_AUTOMATON_HH
