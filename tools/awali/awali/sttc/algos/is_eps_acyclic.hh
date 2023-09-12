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

#ifndef AWALI_ALGOS_IS_EPS_ACYCLIC_HH
# define AWALI_ALGOS_IS_EPS_ACYCLIC_HH

# include <unordered_map>

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/attributes.hh>

namespace awali { namespace sttc {

  namespace
  {

    template <typename Aut,
              bool has_one = context_t_of<Aut>::has_one()>
    struct epsilon_acyclic;

    /// @class epsilon_acylic
    /// @brief Detect epsilon-circuits.
    ///
    /// In this algorithm, only epsilon-transitions are considered.
    template <typename Aut>
    struct epsilon_acyclic<Aut, true>
    {
      using automaton_t = typename std::remove_cv<Aut>::type;
      using label_t = label_t_of<automaton_t>;
      std::unordered_map<state_t, char> tag;
      /*
         tag gives the status of the state s;
         if s is not in the map, the state has not been reached yet;
         if tag[s]='u', the status is unknown, the graph reachable from s is
           under exploration
         if tag[s]='o', the status is "ok": there is no eps-circuit accessible
           from s
         if tag[s]='c', there is an eps-circuit accessible from s
         */

      const automaton_t& aut_;
      label_t empty_word;

      // Return true if an epsilon-circuit is accessible from s by
      // epsilon-transitions.
      bool has_epsilon_circuit(state_t s)
      {
        auto it = tag.find(s);
        if (it == tag.end())
        {
          tag[s] = 'u';
          for (auto t : aut_->out(s, empty_word))
            if (has_epsilon_circuit(aut_->dst_of(t)))
            {
              tag[s] = 'c';
              return true;
            }
          tag[s] = 'o';
          return false;
        }

        // Switch with respect to tag[s].
        switch (it->second)
        {
          case 'u':
            // s is reached while we are exploring successors of s:
            // there is a circuit.
            tag[s] = 'c';
            return true;
            // Otherwise the graph reachable from s has already been explored.
          case 'o':
            return false;
          default: //'c'
            return true;
        }
      }

      epsilon_acyclic(const automaton_t& aut)
        : aut_(aut)
        , empty_word(aut->labelset()->one())
      {
      }

      bool is_eps_acyclic()
      {
        for (auto s : aut_->states())
          if (has_epsilon_circuit(s))
            return false;
        return true;
      }
    };

    template <typename Aut>
    struct epsilon_acyclic<Aut, false>
    {
      using automaton_t = typename std::remove_cv<Aut>::type;

      constexpr epsilon_acyclic(const automaton_t&)
      {}

      static constexpr bool is_eps_acyclic()
      {
        return true;
      }
    };
  }

  template <typename Aut>
  ATTRIBUTE_CONST
  bool is_eps_acyclic(const Aut& aut)
  {
    epsilon_acyclic<Aut> t{aut};
    return t.is_eps_acyclic();
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_IS_EPS_ACYCLIC_HH
