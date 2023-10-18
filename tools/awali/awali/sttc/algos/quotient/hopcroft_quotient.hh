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

#ifndef AWALI_ALGOS_HOPCROFT_QUOTIENT_HH
#define AWALI_ALGOS_HOPCROFT_QUOTIENT_HH

#include <unordered_map>
#include<limits>
#include<vector>
#include<list>
#include<queue>

#include <awali/common/types.hh>
#include <awali/sttc/misc/pair.hh>
#include <awali/sttc/misc/linkedhashmap.hh>
# include <limits>

namespace awali {
  namespace sttc {

  /*
   * minimization with Hopcroft for incomplete deterministic automata
   * (cf. Beal & Crochemore, Minimizing incomplete automata, 2008)
   */
    /* Description of the algorithm

      This algorithm compute the coarsest congruence ~ of the set of states Q such that,
      for each pair of states p and q, if p~q then
      1. p if final <=> q is final
      2. for every letter a, the set of succ(p,a) = succ(q,a) mod ~

      This is a refinement algorithm.
      There is first a partition between final and non final states.
      The principle is that every part p is used as a "splitter":
        for every predecessor r of any s of p,
        the "signature" of r is computed:
           that is the list of letters for which the successor of r exists and is in p.
      Every concerned part is the split w.r.t the signatures of its states.

      To achieve a O(|A||Q|) complexity in each iteration,
         weak sort is used.
         Here, the weak sort is not implemented with sparse lists, but with linked_hash_map;
         the complexity is therefore in amortized O(A||Q|).

         1. For every letter a that appear in a transition incoming to p,
              meet[a] is the list of predecessors of some state of p by a
         2. For every letter in the domain of meet,
              for every state r in meet[a]
                a is appended to the list signature[r]
            -> letters appear in signatures in the same ordering

         Then, the list of states which have a signature must be (weakly) sorted in such a way
         that states with the same signature and that belong to the same part are neighbours
         1. For each state r in the domain of signature
               r is appended to tmplist[part[r]]
            Then, the list of lists (tmplist[p] | p in domain of tmplist) are concatenated.
            For convenience, this concatenation 'list' is represented as a list of lists.
         2. For every integer k,
              r is appended to tmplist[signature[k]]
            Then, the list of lists (tmplist[a] | a in domain of tmplist) are concatenated.
            This is done for k up to the longest signature; if some signature are shorter,
            the corresponding states are set in the "special list" list.

         Finally all the concerned part are splitted.
         The smallest subpart inherits the index of the original part, the other receive fresh indices.
       If there is more than a subpart, every subpart but the largest is pushed into the queue of splitters.
       This leads to a logarithmic number of iterations
    */

    template <typename Aut>
    unsigned hopcroft_quotient(const Aut& aut, std::vector<std::list<state_t> >& states_in_part, bool cancellative = false) {

      using automaton_t = Aut;

      using label_t = label_t_of<automaton_t>;
      using weight_t = weight_t_of<automaton_t>;

      const auto& weightset_ = *aut->weightset();
      std::vector<unsigned > size_of_part;
      //Each part (unsigned) is a list of states
      states_in_part.resize(3);
      size_of_part.resize(3);
      //And each state belongs to a part
      state_t state_capacity=aut->max_state()+1;
      std::vector<unsigned> part(state_capacity);
      std::vector<std::list<state_t>::iterator> address_in_part(state_capacity);
      //The part is initialised as 0 for every state
      //except for post() which is in 1.
      states_in_part[0].emplace_back(aut->pre());
      part[aut->pre()]=0;
      states_in_part[1].emplace_back(aut->post());
      part[aut->post()]=1;
      size_of_part[0]=size_of_part[1]=1;
      for(auto s : aut->states() ) {
        states_in_part[2].emplace_front(s);
        part[s]=2;
        address_in_part[s]=states_in_part[2].begin();
      }
      size_of_part[2]=aut->num_states();
      unsigned n=3; // number of parts

      /* A part is a "splitter" if its predecessors can be considered to split the parts
       */
      std::queue<unsigned> splitters;
      std::vector<bool> is_in_queue;
      splitters.emplace(1);
      splitters.emplace(2);
      is_in_queue.push_back(false);
      is_in_queue.push_back(true);
      is_in_queue.push_back(true);
      //// MAIN LOOP
      unsigned iterations=0;
      while(!splitters.empty()) {
        iterations++;
        //i is the part which is the current splitter
        unsigned i= splitters.front();
        splitters.pop();
        is_in_queue[i]=false;   
        //The list of predecessors for each letter
        internal::linked_hash_map<label_t,std::vector<std::pair<state_t, weight_t> > > meet;
        for(auto s : states_in_part[i]) {
          for(auto tr : aut->all_in(s) ) {
            state_t r = aut->src_of(tr);
            if(size_of_part[part[r]]==1)
              continue;
            label_t a = aut->label_of(tr);
            weight_t w = aut->weight_of(tr);
            meet[a].emplace_back(std::pair<state_t, weight_t>{r,w});
          }
        }
        if(meet.domain().empty())
          continue;
        /* The signature of each met 'r' state is computed
         * signature[r] is the list of letters 'a' such that succ(r,a) is in part 'i'.
         * The ordering of letters in signature[r] is compliant with the ordering
         * of letters in meet.domain().
         * Hence, if two states have successors in 'i' with the same letters, they have the same
         * signature.
         */
        internal::linked_hash_map<state_t,std::vector<std::pair<label_t, weight_t> > > signature;
        for(auto a : meet.domain())
          for(auto p : meet[a]) {
            state_t r=p.first;
            weight_t w=p.second;
            if(signature.count(r)==0 || signature[r].back().first!=a)
              signature[r].emplace_back(std::pair<label_t, weight_t>{a,w});
            else {
              weight_t& w1 = signature[r].back().second;
              w1 = weightset_.add(w1,w);
            }
          }
        /* We build now a list in which states with the same signature
         * that belong to the same part are gathered
         * For convenience, this is actually a list of lists
         */

        internal::linked_hash_map<unsigned,std::vector<std::vector<state_t> > > new_parts_in_this_part;
        std::queue<std::vector<state_t> > list;
        {
          internal::linked_hash_map<unsigned,std::vector<state_t> > tmplist;
          for(auto r : signature.domain())
            tmplist[part[r]].emplace_back(r);
          for(auto j : tmplist.domain())
            list.push(std::move(tmplist[j]));
        }
        while(!list.empty()) {
          std::vector<state_t>& pp = list.front();
          std::vector<state_t> shorter;
          unsigned current_part = part[pp.front()];
          internal::linked_hash_map<std::pair<label_t,weight_t>,std::vector<state_t> > tmplist;
          for(state_t& r:pp) {
            std::vector<std::pair<label_t, weight_t>>& sig = signature[r];
            if(sig.empty())
              shorter.push_back(r);
            else {
              std::pair<label_t,weight_t>& pair= sig.back();
              tmplist[pair].emplace_back(r);
              sig.pop_back();
            }
          }
          list.pop();
          for(auto pair : tmplist.domain())
            list.push(std::move(tmplist[pair]));
          if(!shorter.empty())
            new_parts_in_this_part[current_part].push_back(std::move(shorter));
        }
        //
        //Creation of the new parts
        for(unsigned p : new_parts_in_this_part.domain()) {
          unsigned s=0;
          auto &pp=new_parts_in_this_part[p];
          for(auto& c : pp)
            s=s+c.size();
          bool all_states_met= size_of_part[p]==s;
          unsigned nn=n;
          if(!all_states_met)
            size_of_part[p]-=s;
          if(pp.size()>1 || !all_states_met) { // there is a split
            bool first=true;
            //update parts
            for(auto& c : pp) {
              unsigned k;
              if(first && all_states_met) {
                k=p;
                size_of_part[p]=c.size();
              }
              else {
                k=nn++;
                states_in_part.emplace_back(std::list<state_t>());
                size_of_part.push_back(c.size());
              }
              first=false;
              for(state_t r : c) {
                states_in_part[p].erase(address_in_part[r]);
                states_in_part[k].emplace_front(r);
                address_in_part[r]=states_in_part[k].begin();
                part[r]=k;
              }
            }
            //insert parts in queue
            if(cancellative && !is_in_queue[p]) {
              unsigned max=p, size_max=size_of_part[p];
              for(unsigned k=n; k<nn; ++k)
                if(size_of_part[k]>size_max) {
                  size_max=size_of_part[k];
                  max=k;
                }
              if(max!=p) {
                splitters.emplace(p);
                is_in_queue[p]=true;
              }
              for(unsigned k=n; k<nn; ++k)
                if(max!=k) {
                  splitters.emplace(k);
                  is_in_queue.push_back(true);
                }
                else
                  is_in_queue.push_back(false);
            } else {
              if(!is_in_queue[p]) {
                splitters.emplace(p);
                is_in_queue[p]=true;
              }
              for(unsigned k=n; k<nn; ++k) {
                splitters.emplace(k);
                is_in_queue.push_back(true);
              }
            }
            n=nn;
          }
        }
      }
      return iterations;
    }

  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_HOPCROFT_QUOTIENT_HH
