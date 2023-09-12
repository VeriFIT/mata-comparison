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

#ifndef AWALI_ALGOS_SUB_AUTOMATON_HH
# define AWALI_ALGOS_SUB_AUTOMATON_HH

# include <set>

namespace awali { namespace sttc {


  /*-----------------.
  | sub-automaton).  |
  `-----------------*/

  template <typename Aut, typename Pred>
  inline
  void
  sub_automaton(Aut& aut, Pred keep_state)
  {
    std::vector<state_t> to_erase;
    for(auto s : aut->states())
      if(!keep_state(s))
        to_erase.emplace_back(s);
    for(auto s : to_erase)
      aut->del_state(s);
  }

  template <typename Aut>
  inline
  void
  sub_automaton(Aut& aut, const std::set<state_t>& sts)
  {
    std::vector<state_t> to_erase;
    auto it=sts.begin();
    for(auto s : aut->all_states())
      if(it == sts.end() || *it != s) {
        if(s!= aut->pre() && s!=aut->post())
          to_erase.emplace_back(s);
      }
      else
        ++it;
    for(auto s : to_erase)
      aut->del_state(s);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_SUB_AUTOMATON_HH
