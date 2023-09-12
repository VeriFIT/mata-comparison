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

#ifndef AWALI_ALGOS_HAS_TWINS_PROPERTY_HH
# define AWALI_ALGOS_HAS_TWINS_PROPERTY_HH

# include <stack>
# include <vector>
# include <unordered_set>
# include <unordered_map>

#include <awali/sttc/misc/unordered_set.hh>
#include <awali/sttc/misc/unordered_map.hh>
#include <awali/sttc/algos/transpose.hh>
#include <awali/sttc/algos/product.hh>
#include <awali/sttc/algos/accessible.hh>

namespace awali { namespace sttc {

  /*----------.
  |  reverse  |
  `----------*/

  /// Inverse the weight of all edges of \a aut.
  template <typename Aut>
  Aut&
  inverse_here(Aut& aut)
  {
    const auto& ws = *aut->weightset();
    for (auto t : aut->all_transitions())
      aut->set_weight(t, ws.rdiv(ws.one(), aut->weight_of(t)));
    return aut;
  }

  template <typename Aut>
  auto
  inverse(const Aut& aut)
    -> decltype(::sttc::copy(aut))
  {
    auto res = copy(aut);
    return inverse_here(res);
  }


  /*--------------------.
  | reverse_postorder.  |
  `--------------------*/

  namespace internal
  {
    /// Get all vertexs in reverse postorder
    /// by using depth first search.
    template <typename Aut>
    class reverse_postorder_impl
    {
    public:

      reverse_postorder_impl(const Aut& aut)
      {
        for (auto s : aut->all_states())
          if (!has(marked_, s))
            dfs(s, aut);
      }

      std::stack<state_t>& reverse_post()
      {
        return rvp_;
      }

      private:
        void dfs(state_t s, const Aut& aut)
        {
          marked_.emplace(s);
          for (auto t : aut->out(s))
            {
              auto dst = aut->dst_of(t);
              if (!has(marked_, dst))
                dfs(dst, aut);
            }
          rvp_.push(s);
        }
      std::stack<state_t> rvp_;
      std::unordered_set<state_t> marked_;
      std::stack<state_t> todo_;
    };

  }

  /// Get all vertices in reverse postorder.
  template <typename Aut>
  std::stack<state_t>
  reverse_postorder(const Aut& aut)
  {
    internal::reverse_postorder_impl<Aut> dv(aut);
    return dv.reverse_post();
  }

  /*---------------.
  | scc_kosaraju.  |
  `---------------*/

  namespace internal
  {
    /// Use Kosajaju algorithm for finding all of strongly
    /// connected components.
    template <typename Aut>
    class scc_kosaraju
    {
    public:
      using component_t = std::unordered_set<state_t>;
      using components_t = std::vector<component_t>;

      scc_kosaraju(const Aut& aut)
      {
        auto trans = sttc::transpose(aut);
        auto todo = reverse_postorder(trans);
        while (!todo.empty())
          {
            auto s = todo.top();
            todo.pop();
            if (!has(marked_, s))
              {
                dfs(s, aut);
                ++num_;
              }
          }
      }

      const components_t components()
      {
        return components_;
      }

    private:
      void dfs(state_t s, const Aut& aut)
      {
        marked_.emplace(s);
        if (num_ == components_.size())
          components_.emplace_back(component_t{s});
        else
          components_[num_].emplace(s);

        for (auto t : aut->out(s))
          {
            auto dst = aut->dst_of(t);
            if (!has(marked_, dst))
              dfs(dst, aut);
          }
      }

      /// The current component number.
      int num_ = 0;
      components_t components_;
      std::unordered_set<state_t> marked_;
    };

  }

  /// Find all strongly connected components of \a aut.
  template <typename Aut>
  const std::vector<std::unordered_set<state_t>>
  components(const Aut& aut)
  {
    internal::scc_kosaraju<Aut> scc(aut);
    return scc.components();
  }


  /*-----------------.
  | cycle_identity.  |
  `-----------------*/

  namespace internal
  {
    /// Whether the weight of beetween two states on component,
    /// it is always unique.
    template <typename Aut>
    class cycle_identity_impl
    {
    public:
      using weight_t = weight_t_of<Aut>;
      using component_t = std::unordered_set<state_t> ;

      cycle_identity_impl() {}

      // Calcule the weight with depth first search by weight
      // and compare the weight of two state is unique.
      bool check(const component_t& component, const Aut& aut)
      {
        std::unordered_map<state_t, weight_t> wm;
        const auto& ws = *aut->weightset();
        auto s0 = *component.begin();
        wm[s0] = ws.one();

        for (auto t : transitions_by_dfs_(component, aut))
          {
            auto src = aut->src_of(t);
            auto dst = aut->dst_of(t);
            if (!has(wm, dst))
              wm.emplace(dst, ws.mul(wm[src], aut->weight_of(t)));
            if (!ws.equals(wm[dst], ws.mul(wm[src], aut->weight_of(t))))
              return false;
          }
        return true;
      }

    private:
      using transitions_t = std::vector<transition_t>;
      /// Visit all of edges of a component by depth first search.
      transitions_t
      transitions_by_dfs_(const component_t& component,
                          const Aut& aut)
      {
        transitions_t res;
        std::set<transition_t> marked;
        std::stack<transition_t> todo;

        auto s0 = *component.begin();
        for (auto t : aut->out(s0))
          {
            if (has(component, aut->dst_of(t)))
              {
                todo.push(t);
                marked.emplace(t);
              }
          }

        while (!todo.empty())
          {
            auto e = todo.top();
            todo.pop();
            res.emplace_back(e);

            for (auto f : aut->out(aut->dst_of(e)))
              if (has(component, aut->dst_of(f))
                  && !has(marked, f))
                {
                  todo.push(f);
                  marked.emplace(f);
                }
          }
        return res;
      }
    };
  }

  /// Check the weight of two states on this component is unique.
  template <typename Aut>
  bool cycle_identity(const std::unordered_set<state_t>& c,
                      const Aut& aut)
  {
    internal::cycle_identity_impl<Aut> ci;
    return ci.check(c, aut);
  }


  /*---------------------.
  | has_twins_property.  |
  `---------------------*/

  /// Whether \a aut has the twins property.
  template <typename Aut>
  bool has_twins_property(const Aut& aut)
  {
    // TODO: Check cycle-unambiguous.
    auto trim = sttc::trim(aut);
    auto inv = inverse(trim);
    auto a = sttc::product(inv, trim);

    // Find all components of automate a.
    auto cs = components(a);

    // Check unique weight of two states on each component.
    for (auto c : cs)
      if (!cycle_identity(c, a))
        return false;

    return true;
  }

}}//end of ns awali::stc
#endif // !AWALI_ALGOS_HAS_TWINS_PROPERTY_HH
