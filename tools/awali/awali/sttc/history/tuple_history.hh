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

#ifndef AWALI_TUPLE_HISTORY_HH
# define AWALI_TUPLE_HISTORY_HH

# include <deque>
# include <iostream>
# include <map>
# include <utility>

#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/map.hh> // has
#include <awali/common/tuple.hh>
#include <awali/sttc/history/history.hh>
#include <awali/common/tuple.hh>

# include <stdexcept>

namespace awali {
  namespace sttc {

    /*------------------------------.
      | tuple_history<Aut, Auts...>.  |
      `------------------------------*/

    /// An automaton whose states are tuples of states of automata.
    ///
    /// Corresponds to the Cartesian product of states.
    ///
    /// \tparam Aut   the automaton type
    /// \tparam Auts  the origin automaton types
    template <typename Auts>
    class tuple_history : public history_base
    {
      /// The type of the resulting automaton.
    public:
      history_kind_t get_nature() const
      {
        return history_kind_t::TUPLE;
      }

    public:
      /// The type of origin automata.
      using automata_t = Auts;
      /// Tuple of states of input automata.
      using tuple_t = typename std::cst_tuple<state_t, std::tuple_size<Auts>::value>::type;

      /// The type of the Ith origin automaton, unqualified.
      template <size_t I>
      using input_automaton_t
      = internal::base_t<typename std::tuple_element<I, automata_t>::type>;

      tuple_history(const automata_t& auts)
        : auts_(auts)
      {}

      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt) const
      {
        o << '(';
        print_state_name_(s, o, fmt, indices);
        o << ')';
        return o;
      }


      /// A map from result state to tuple of original states.
      using origins_t = std::map<state_t, tuple_t>;

      const origins_t& origins() const
      {
        return origins_;
      }

      // FIXME: protected:
      /// A static list of integers.
      template <std::size_t... I>
      using seq = awali::internal::index_sequence<I...>;

      /// The list of automaton indices as a static list.
      using indices_t = awali::internal::make_index_sequence<std::tuple_size<Auts>::value>;
      static constexpr indices_t indices{};

      template <size_t... I>
      std::ostream&
      print_state_name_(state_t s, std::ostream& o,
                        const std::string& fmt,
                        seq<I...>) const
      {
        auto i = origins().find(s);
        const char* sep = "";
        using swallow = int[];
        (void) swallow
          {
            (o << sep,
             std::get<I>(auts_)->print_state_history(std::get<I>(i->second),
                                                     o, fmt),
             sep = ", ",
             0)...
              };
        return o;
      }

      bool has_history(state_t s) const {
        return (origins_.find(s)!=origins_.end());
      }

      bool remove_history(state_t s) {
        return origins_.erase(s);
      };

      void
      add_state(state_t s,const tuple_t& set)
      {
        origins_[s] = set;
      }

      /// Origin automata, supplied at construction time.
      automata_t auts_;

      mutable origins_t origins_;

      state_t get_state(state_t s) {
        throw std::runtime_error("Origin state not available");
      }

      template <size_t... I>
      std::vector<state_t> get_state_set(state_t s, seq<I...>) {
        return {std::get<I>(origins_[s])...};
      }

      std::vector<state_t> get_state_set(state_t s) {
        return get_state_set(s,indices);
      }
    };

  }
}//end of ns awali::stc

#endif // !AWALI_TUPLE_HISTORY_HH
