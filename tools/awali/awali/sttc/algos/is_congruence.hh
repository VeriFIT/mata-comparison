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

#ifndef AWALI_ALGOS_ISCONGRUENT_HH
# define AWALI_ALGOS_ISCONGRUENT_HH

#include <vector>
#include <queue>

#include <awali/sttc/weightset/fwd.hh> // b
#include <awali/sttc/misc/linkedhashmap.hh>
#include <awali/sttc/misc/pair.hh>
# include <limits>

namespace awali {
  namespace sttc {

    /*Test whether all states in partition[i] have the same transition function modulo partition.

     */

    template <typename Aut>
    bool is_congruence(const Aut& aut, const std::vector<std::vector<state_t>>& partition) {

      using automaton_t = Aut;

      using label_t = label_t_of<automaton_t>;
      using weight_t = weight_t_of<automaton_t>;

      //part_of[s] is the class of state s
      std::vector<unsigned> part_of(aut->max_state()+1);
      for(unsigned i=0; i<partition.size(); ++i)
        for(auto s : partition[i]){
          part_of[s]=i;
        }

      for(auto part : partition) {
        internal::linked_hash_map<unsigned, internal::linked_hash_map<std::pair<label_t, weight_t>, unsigned>> successors;
        for(auto s : part) {
          //signature[{i,a}] is the sum of weights of transitions
          //with label a from s to states in i
          internal::linked_hash_map<std::pair<unsigned, label_t>, weight_t> signature;
          for(auto tr : aut->all_out(s)) {
            std::pair<unsigned, label_t> key{part_of[aut->dst_of(tr)], aut->label_of(tr)};
            weight_t w=aut->weight_of(tr);

            if(signature.count(key))
              signature[key] = aut->weightset()->add(signature[key], w);
            else
              signature[key] = w;
          }
          for(auto p: signature.domain()) {
            std::pair<label_t, weight_t> plw{p.second, signature[p]};
            if(successors.count(p.first) && successors[p.first].count(plw))
              ++successors[p.first][plw];
            else
              successors[p.first][plw]=1;
          }
        }
        unsigned n=part.size();
        for(auto a:successors.domain())
          for(auto b: successors[a].domain())
            if(successors[a][b]!=n)
              return false;
      }
      return true;
    }
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_ISCONGRUENT_HH
