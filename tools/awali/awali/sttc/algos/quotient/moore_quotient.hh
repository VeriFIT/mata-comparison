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

#ifndef AWALI_ALGOS_MOORE_QUOTIENT_HH
#define AWALI_ALGOS_MOORE_QUOTIENT_HH

#include <vector>
#include <queue>
#include <awali/common/types.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/linkedhashmap.hh>

namespace awali {
  namespace sttc {

    /* This is an implementation of Moore algorithm for the quotient of weighted automata.
       This is a partition refining algorithm. At the beginning, there are three parts:
       the pre-initial state(0), the post-final state(1), the other states(2)
       At each round, every non singleton part 'i' is checked:
          for each state 's' of 'i' the 'signature' of 's' is computed;
          this is the parts that are reached from 's' for every letter, with the corresponding weight
          the algorithm guarantees (#) that two states are "Moore equivalent" iff they have the same signature.
          The part 'i' is then split w.r.t. the states that share a signature.
       If no part have been split during a round, the algorithm ends.

       The rounds are separated in the queue through a false part '0' (the true part 0 never appear in the queue).

       (#) The signatures are computed through a "weak sort" implemented with linked hash maps:
       it is a linear algorithm in amortized time.
       First, for every state 's' in 'i', for every transition s-a|k->q,
       the tuple (s,k,part[q]) is put in the list meet[a],
       Then, for each letter 'a' in the domain of 'meet',
       the tuple (s,k,a) is appended to the list meet2[part[q]],
       Finally, for each part part[q] in the domain of 'meet2',
           if there is not another tuple (a,part[q],k')
               the tuple (a,part[q],k) is appended to signature[s],
           else
               the tuple is updated as (a,part[q],k+k')
       This latter loop creates an ordering on letters: in every signature, the ordering over letter is a subsequence of the ordering of the domain of 'meet'
      .
    */

    template <typename Aut>
    unsigned moore_quotient(const Aut& aut, std::vector<std::vector<state_t> >& states_in_part) {

      using automaton_t = Aut;

      using label_t = label_t_of<automaton_t>;
      using weight_t = weight_t_of<automaton_t>;

      //Each part (unsigned) is a list of states
      states_in_part.resize(3);
      //And each state belongs to a part
      std::vector<unsigned> part(aut->max_state()+1);
      //The part is initialised as 2 for every state
      //except for pre() which is 0 and post() which is in 1.
      states_in_part[0].emplace_back(aut->pre());
      part[aut->pre()]=0;
      states_in_part[1].emplace_back(aut->post());
      part[aut->post()]=1;
      for(auto s : aut->states() ) {
        states_in_part[2].emplace_back(s);
        part[s]=2;
      }
      /* A part can potentially be split if it contains at least two states.
         Hence singletons are not put in the queue.
       */
      std::queue<unsigned> queue_part;
      if(states_in_part[2].size()>1)
        queue_part.emplace(2);
      //0 is a marker in the queue for the end of a round
      //if there is no splitting between two occurences of 0
      //the algorithm ends.
      queue_part.emplace(0);
      bool stop=true;
      //// MAIN LOOP
      unsigned iterations=0;
      while(!queue_part.empty()) {
        iterations++;
        //i is the part which is the current splitter
        unsigned i= queue_part.front();
        queue_part.pop();
        if(i==0) {//end of the round
          if(stop)
            break;
          else {
            //the parts for the next round are exactly the parts in the queue
            //we start a new round and put a marker in the queue
            stop=true;
            queue_part.emplace(0);
            continue;
          }
        }       
        std::vector<state_t> states_without_successors;

        //The list of successors for each letter
        internal::linked_hash_map<label_t,std::vector<std::tuple<state_t, weight_t, unsigned> > > meet;
        for(auto s : states_in_part[i]) {
          bool no_successors=true;
          for(auto tr : aut->all_out(s) ) {
            no_successors=false;
            state_t r = aut->dst_of(tr);
            label_t a = aut->label_of(tr);
            weight_t w = aut->weight_of(tr);
            std::tuple<state_t, weight_t, unsigned> tu{s,w,part[r]};
            meet[a].push_back(std::move(tu));
          }
          if(no_successors)
            states_without_successors.emplace_back(s);
        }
        if(meet.domain().empty())
          continue;

        internal::linked_hash_map<unsigned,std::vector<std::tuple<state_t, weight_t, label_t> > > meet2;
        for(auto a : meet.domain())
          for(auto p : meet[a]){
            std::tuple<state_t, weight_t, label_t> tu{std::get<0>(p),std::get<1>(p),a};
            meet2[std::get<2>(p)].push_back(std::move(tu));
          }

        internal::linked_hash_map<state_t,std::vector<std::tuple<label_t,unsigned, weight_t> > > signature;
        for(auto j : meet2.domain())
          for(auto p : meet2[j]) {
            if(signature.count(std::get<0>(p))) {
              auto & t=signature[std::get<0>(p)].back();
              if(std::get<0>(t)==std::get<2>(p) && std::get<1>(t)==j)
                std::get<2>(t) = aut->context().weightset()->add(std::get<2>(t), std::get<1>(p));
              else {
                std::tuple<label_t,unsigned, weight_t> tu{std::get<2>(p),j,std::get<1>(p)};
                signature[std::get<0>(p)].push_back(std::move(tu));
              }
            }
            else {
              std::tuple<label_t,unsigned, weight_t> tu{std::get<2>(p),j,std::get<1>(p)};
              signature[std::get<0>(p)].push_back(std::move(tu));
            }
          }

        // We compute now the new parts.
        std::vector<std::vector<state_t> > new_parts;
        std::queue<std::vector<state_t> > list;
        list.emplace(signature.domain());
        while(!list.empty()) {
          std::vector<state_t>& pp = list.front();
          std::vector<state_t> shorter;
          internal::linked_hash_map<std::tuple<label_t,unsigned, weight_t> ,std::vector<state_t> > tmplist;
          for(state_t& r:pp) {
            std::vector<std::tuple<label_t,unsigned, weight_t>>& sig = signature[r];
            if(sig.empty())
              shorter.push_back(r);
            else {
              std::tuple<label_t,unsigned, weight_t> & tuple= sig.back();
              tmplist[tuple].emplace_back(r);
              sig.pop_back();
            }
          }
          list.pop();
          for(auto pair : tmplist.domain())
            list.push(std::move(tmplist[pair]));
          if(!shorter.empty())
            new_parts.push_back(std::move(shorter));
        }
        //Creation of the new parts

        unsigned lim=new_parts.size();
        //if lim=1 the part has not been split
        if(lim>1)
          stop=false;
        else {
          queue_part.emplace(i);
          continue;
        }
        unsigned np=states_in_part.size();
        for(unsigned k=0; k<lim;++k) {
          unsigned p;
          if(k==0) {
            p=i;
            states_in_part[i].swap(new_parts[0]);
          }
          else {
            p=np++;
            for(auto r : new_parts[k])
              part[r]=p;
            states_in_part.push_back(std::move(new_parts[k]));
          }
          if(states_in_part[p].size()>1) {
            queue_part.emplace(p);
          }
        }
        if(!states_without_successors.empty()) {
          for(auto r : states_without_successors)
            part[r]=np;
          states_in_part.push_back(std::move(states_without_successors));
          //the part of states without successors is not put in the queue
          //since it can not be split
        }
      }
      return iterations;
    }
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_MOORE_QUOTIENT_HH
