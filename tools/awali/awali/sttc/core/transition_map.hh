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

#ifndef AWALI_CORE_TRANSITION_MAP_HH
# define AWALI_CORE_TRANSITION_MAP_HH

# include <map>
# include <type_traits>

namespace awali { namespace sttc {

  namespace internal
  {

    /// Cache the outgoing transitions of an automaton as efficient
    /// maps label -> vector<(weight, dst)>.  Easy to zip.
    /// \tparam Aut
    ///    The automaton type.
    /// \tparam WeightSet
    ///    The set of weights into which the weights will be converted.
    /// \tparam Deterministic
    ///    Whether the automaton is guaranteed to be deterministic.
    ///    If it is, transition_map[state][label] returns a single
    ///    result, otherwise a vector.
    /// \tparam AllOut
    ///    Whether even the transitions to post() (via the
    ///    special label) are to be included.
    template <typename Aut,
              typename WeightSet = weightset_t_of<Aut>,
              bool Deterministic = false,
              bool AllOut = false>
    struct transition_map
    {
      using weightset_t = WeightSet;
      using weight_t = typename weightset_t::value_t;
      struct transition
      {
        /// The (converted) weight.
        weight_t wgt;
        state_t dst;
      };

      using transitions_t
        = typename std::conditional<Deterministic,
                                    transition,
                                    std::vector<transition>>::type;
      using map_t = std::map<label_t_of<Aut>, transitions_t>;
      using maps_t = std::map<state_t, map_t>;
      maps_t maps_;

      transition_map(const Aut& aut, const weightset_t& ws)
        : aut_(aut)
        , ws_(ws)
      {}

      transition_map(const Aut& aut)
        : transition_map(aut, *aut->weightset())
      {}

      /// Insert l -> t in map.
      template <bool Deterministic_>
      void
      insert_(map_t& map,
              label_t_of<Aut> l, transition t,
              typename std::enable_if<Deterministic_>::type* = nullptr)
      {
        map.emplace(l, t);
      }

      /// Insert l -> t in map.
      template <bool Deterministic_>
      void
      insert_(map_t& map,
              label_t_of<Aut> l, transition t,
              typename std::enable_if<!Deterministic_>::type* = nullptr)
      {
        map[l].emplace_back(t);
      }

      /// Build and return the transition map for state s, store at res.
      /// Insert it in the cache.
      map_t&
      build_map_(typename maps_t::iterator lb, state_t s)
      {
        auto& res = maps_.emplace_hint(lb, s, map_t{})->second;
        for (auto t: aut_->all_out(s))
          if (AllOut || !aut_->labelset()->is_special(aut_->label_of(t)))
            {
              auto w = ws_.conv(*aut_->weightset(), aut_->weight_of(t));
              insert_<Deterministic>(res,
                                     aut_->label_of(t),
                                     transition{w, aut_->dst_of(t)});
            }
        return res;
      }

      map_t& operator[](state_t s)
      {
        auto lb = maps_.lower_bound(s);
        if (lb == maps_.end() || maps_.key_comp()(s, lb->first))
          return build_map_(lb, s);
        else
          return lb->second;
      }

      /// The automaton whose transitions are cached.
      const Aut& aut_;
      /// The result weightset.
      const weightset_t& ws_;
    };
  }

}}//end of ns awali::stc

#endif // !AWALI_CORE_TRANSITION_MAP_HH
