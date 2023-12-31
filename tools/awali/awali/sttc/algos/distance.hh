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

#ifndef AWALI_ALGOS_DISTANCE_HH
# define AWALI_ALGOS_DISTANCE_HH

# include <algorithm>
# include <iostream>
# include <queue>
# include <unordered_set>
# include <unordered_map>
# include <vector>

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/misc/pair.hh>

namespace awali { namespace sttc {

    /*
       Reverse breadth first search from state set 'start'
     */
  template<typename Aut>
  std::unordered_map<state_t,
                     std::pair<unsigned,
                               transition_t>>
  paths_ibfs(const Aut& aut, std::vector<state_t> start)
  {
    // using context_t = context_t_of<Aut>;
    // using automaton_t =  mutable_automaton<context_t>;

    std::queue<state_t> todo;
    std::unordered_set<state_t> marked;
    std::unordered_map<state_t, std::pair<state_t, transition_t>> parent;

    for (auto s : start)
      todo.push(s);

    while (!todo.empty())
      {
        state_t p = todo.front();
        todo.pop();
        marked.insert(p);
        for (auto t : aut->in(p))
          {
            auto s = aut->src_of(t);
            if (marked.find(s) == marked.end())
              {
                todo.push(s);
                auto cur_p = parent.find(p);
                unsigned cur_d;
                if (cur_p == parent.end())
                    cur_d = 1;
                else
                    cur_d = cur_p->second.first + 1;
                parent[s] = {cur_d, t};
              }
          }
      }
    return parent;
  }

    /*
      Breadth first search from state set 'start'
     */
  template<typename Aut>
  std::vector<transition_t>
  path_bfs(const Aut& aut, state_t start,
                               state_t end)
  {
    // using context_t = context_t_of<Aut>;
    // using automaton_t =  mutable_automaton<context_t>;

    std::queue<state_t> todo;
    std::unordered_set<state_t> marked;
    std::unordered_map<state_t, std::pair<state_t, transition_t>> parent;

    todo.push(start);
    while (!todo.empty())
      {
        state_t p = todo.front();
        todo.pop();
        marked.insert(p);
        if (p == end)
          {
            std::vector<transition_t> rpath;
            state_t bt_curr = end;
            while (bt_curr != start)
              {
                transition_t t;
                std::tie(bt_curr, t) = parent.find(bt_curr)->second;
                rpath.push_back(t);
              }
            std::reverse(rpath.begin(), rpath.end());
            return rpath;
          }
        else
          for (auto t : aut->out(p))
            {
              auto s = aut->dst_of(t);
              if (marked.find(s) == marked.end())
                {
                  todo.push(s);
                  parent[s] = {p, t};
                }
            }
      }
    return std::vector<transition_t>();
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_DISTANCE_HH
