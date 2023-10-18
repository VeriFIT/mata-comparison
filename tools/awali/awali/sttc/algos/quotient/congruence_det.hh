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

#ifndef AWALI_ALGOS_CONGRUENCE_DET_HH
# define AWALI_ALGOS_CONGRUENCE_DET_HH

#include <vector>
#include <queue>

#include <awali/sttc/weightset/fwd.hh> // b
#include <awali/sttc/misc/linkedhashmap.hh>

/* This files contains two functions which compute the coarsest equivalence on states which is a congruence w.r.t transitions. The first one is an implementation of
the Moore algorithm; the second on is an implementation of the Hopcroft algorithm
*/


namespace awali {
  namespace sttc {

    namespace internal {
      template<typename Aut, typename Weightset>
      struct extended_label {
        using label_t = label_t_of<Aut>;
        using type = std::pair<label_t,typename Weightset::value_t>;
        
        template <typename Tr>
        static type getLabel(const Aut& aut, Tr tr) {
          return {aut->label_of(tr),aut->weight_of(tr)};
        }
      };
      
      template<typename Aut>
      struct extended_label<Aut, b> {
        using label_t = label_t_of<Aut>;
        using type = label_t;
        
        template <typename Tr>
        static type getLabel(const Aut& aut, Tr tr) {
          return aut->label_of(tr);
        }
      };

      template <typename Aut>
      void initialize_partition(const Aut& aut,std::vector<std::vector<state_t> >& states_in_part) {
        using automaton_t = Aut;
        using weightset_t = weightset_t_of<automaton_t>;
        //      using label_t = label_t_of<automaton_t>;
        using extended_label_t = extended_label<automaton_t,weightset_t>;

        //The list of successors for each letter
        linked_hash_map<typename extended_label_t::type ,std::vector<state_t> > meet;
        for(auto s : aut->states()) {
          for(auto tr : aut->all_out(s) ) {
            auto a = extended_label_t::getLabel(aut,tr);
            meet[a].emplace_back(s);
          }
        }

        states_in_part.resize(2);
        states_in_part[0].emplace_back(aut->pre());
        states_in_part[1].emplace_back(aut->post());
        if(meet.domain().empty())
          return;

        //The lists of outgoing labels for each state
        linked_hash_map<state_t,std::vector<typename extended_label_t::type> > labels;
        for(auto a: meet.domain())
          for(auto p:meet[a]) {
            labels[p].emplace_back(a);
          }

        //We first compute initial parts as states with the same output labels.
        //Each part (unsigned) is a list of states
        std::vector<std::vector<state_t> > list(1);
        for(auto s : aut->states() )
          list[0].emplace_back(s);
        linked_hash_map<typename extended_label_t::type,std::vector<state_t> > tmplist;
        for(unsigned k=0;!list.empty();++k) {
          std::vector<std::vector<state_t> > new_list;
          for(auto l : list) {
            tmplist.clear();
            std::vector<state_t> shorter_sig_list;
            for(auto r : l)
              if(labels[r].size()==k)
                shorter_sig_list.emplace_back(r);
              else {
                tmplist[labels[r][k]].emplace_back(r);
              }
            if(!shorter_sig_list.empty())
              states_in_part.push_back(std::move(shorter_sig_list));
            for(auto a : tmplist.domain())
              new_list.push_back(std::move(tmplist[a]));
          }
          list.swap(new_list);
        }
      }
    }
    
    /* This is a partition refining algorithm. At the beginning, there are three parts:     
       - the pre-initial state(0),    
       - the post-final state(1),     
       -  the other states(2)
      
       At each round, every non singleton part 'i' is checked:
          for each state 's' of 'i' the 'signature' of 's' is computed;
          this is the parts that are reached from 's' for every letter
          the algorithm guarantees (#) that two states are "Moore equivalent" iff they have the same signature.
          The part 'i' is then split w.r.t. the states that share a signature.
       If no part have been split during a round, the algorithm ends.

       The rounds are separated in the queue through a false part '0' (the true part 0 never appear in the queue).

       (#) The signatures are computed through a "weak sort" implemented with linked hash maps:
       it is a linear algorithm in amortized time.
       First, for every state 's' in 'i', for every transition s-a->q,
       the pair (s,part[q]) is put in the list meet[a],
       Then, for each letter 'a' in the domain of 'meet',
       the pair (a,part[q]) is appended to the signature of s
       This latter loop creates an ordering on letters: in every signature, the ordering over letter is a subsequence of the ordering of the domain of 'meet'
      .
     */
    /** @brief Moore algorithm for the minimization of deterministic Boolean automata (DFA), not necessarily complete.
     *
     * @tparam Aut the type of the automaton
     * @param aut a Boolean deterministic automaton
     * @param states_in_part a partition of states (which is initialized in the algorithm)
     **/
    template <typename Aut>
    void moore_det(const Aut& aut, std::vector<std::vector<state_t> >& states_in_part) {
      using automaton_t = Aut;
      using label_t = label_t_of<automaton_t>;

      internal::initialize_partition(aut, states_in_part);

      internal::linked_hash_map<state_t,std::vector<state_t> > succs;
      internal::linked_hash_map<label_t,std::vector<std::pair<state_t, state_t> > > meet;
      //The list of successors for each letter
      for(auto s : aut->states())
        for(auto tr : aut->all_out(s) )
          meet[aut->label_of(tr)].emplace_back(std::pair<state_t, state_t>{s,aut->dst_of(tr)});

      for(auto a: meet.domain())
        for(auto p:meet[a])
          succs[p.first].emplace_back(p.second);

      std::queue<unsigned> queue_part;
      //And each state belongs to a part
      std::vector<unsigned> part(aut->max_state()+1);
      for(unsigned k=0; k< states_in_part.size(); ++k) {
        for(auto s:states_in_part[k])
          part[s]=k;
        if(states_in_part[k].size()>1)
          queue_part.emplace(k);
      }

      //0 is a marker in the queue for the end of a round
      //if there is no splitting between two occurences of 0
      //the algorithm ends.
      queue_part.emplace(0);
      bool stop=true;
      //// MAIN LOOP
      while(!queue_part.empty()) {
        //i is the part which is the current splitter
        unsigned i= queue_part.front();
        queue_part.pop();
        if(i==0) {//end of the round
          if(stop)
            return;
          else {
            //the parts for the next round are exactly the parts in the queue
            //we start a new round and put a marker in the queue
            stop=true;
            queue_part.emplace(0);
            continue;
          }
        }

        /* We compute  the new parts
         * At every step k, each vector of list contains states which coincide
         * on the k first components of their signature;
         * each vector in new_parts contains contains states with a signature <k
         */

        std::vector<std::vector<state_t> > list;
        std::vector<std::vector<state_t> >  new_parts;
        list.emplace_back(states_in_part[i]);

        for(unsigned k=0; !list.empty(); ++k) {
          std::vector<std::vector<state_t> > new_list;
          for(auto l : list) {
            internal::linked_hash_map<unsigned,std::vector<state_t> > tmplist;
            std::vector<state_t> shorter_sig_list;
            for(auto r : l)
              if(succs[r].size()==k)
                shorter_sig_list.emplace_back(r);
              else {
                tmplist[part[succs[r][k]]].emplace_back(r);
              }
            if(!shorter_sig_list.empty())
              new_parts.push_back(std::move(shorter_sig_list));
            for(auto a : tmplist.domain())
              new_list.push_back(std::move(tmplist[a]));
          }
          list.swap(new_list);
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
          if(states_in_part[p].size()>1)
            queue_part.emplace(p);
        }
      }
    }

    /*,-------------------.
      |                   |
      `-------------------'*/
    
    namespace internal {
      template<typename Aut, typename Weightset>
      struct backward_initialisation {

        static void init(const Aut& aut,std::vector<std::vector<state_t> >& states_in_part, std::vector<unsigned>& part, std::queue<unsigned>& splitters, std::vector<bool>& is_in_queue) {
          initialize_partition(aut, states_in_part);
          //The part is initialised as 0 for every state
          //except for post() which is in 1.
          part[aut->pre()]=0;
          part[aut->post()]=1;
          // A part is a "splitter" if its predecessors can be considered to split the parts
          is_in_queue.push_back(false); //Part 0 is not in the queue
          unsigned max=1;
          for(unsigned k=2; k< states_in_part.size(); ++k) {
            for(auto s:states_in_part[k])
              part[s]=k;
            if(states_in_part[k].size()>states_in_part[max].size())
              max=k;
          }
          for(unsigned k=1; k< states_in_part.size(); ++k) {
            is_in_queue.push_back(k!=max);
            if(k!=max)
              splitters.emplace(k);
          }
        }
      };

      //In the Boolean case, the initialisation is slightly different
      template<typename Aut>
      struct backward_initialisation<Aut, b> {
        static void init(const Aut& aut,std::vector<std::vector<state_t> >& states_in_part, std::vector<unsigned>& part, std::queue<unsigned>& splitters, std::vector<bool>& is_in_queue) {
          states_in_part.resize(3);
          states_in_part[0].emplace_back(aut->pre());
          states_in_part[1].emplace_back(aut->post());
          part[aut->pre()]=0;
          part[aut->post()]=1;
          for(auto s: aut->states()) {
            states_in_part[2].emplace_back(s);
            part[s]=2;
          }
          is_in_queue.push_back(false);
          is_in_queue.push_back(true);
          is_in_queue.push_back(true);
          splitters.emplace(1);
          splitters.emplace(2);
        }
      };
    }

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

       Implementation idiosyncrasy :
       * The pre-initial state and the post-final state are a part by themself.
       * It is useless to use the pre-initial state as a splitter since it has no predecessor
       * It it useless to identify final states before the main loop since they are exactly predecessors of the post-final state
    */
    /** @brief minimization with Hopcroft for incomplete deterministic automata
     *
     * (cf. Beal & Crochemore, Minimizing incomplete automata, 2008)
     * @tparam Aut the type of the automaton
     * @param aut a Boolean deterministic automaton
     * @param states_in_part a partition of states (which is initialized in the algorithm)
     */
    template <typename Aut>
    void hopcroft_det(const Aut& aut, std::vector<std::vector<state_t> >& states_in_part) {

      using automaton_t = Aut;

      using label_t = label_t_of<automaton_t>;
      using weightset_t = weightset_t_of<automaton_t>;

      //Each part (unsigned) is a list of states
      std::vector<unsigned> part(aut->max_state()+1);
      std::queue<unsigned> splitters;
      std::vector<bool> is_in_queue;

      internal::backward_initialisation<automaton_t, weightset_t>::init(aut, states_in_part, part, splitters, is_in_queue);

      //// MAIN LOOP
      while(!splitters.empty()) {
        //i is the part which is the current splitter
        unsigned i= splitters.front();
        splitters.pop();
        is_in_queue[i]=false;

        //The list of predecessors for each letter
        internal::linked_hash_map<char,std::vector<state_t> > meet;
        for(auto s : states_in_part[i]) {
          for(auto tr : aut->all_in(s) ) {
            state_t r = aut->src_of(tr);
            label_t a = aut->label_of(tr);
            meet[a].emplace_back(r);
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
        internal::linked_hash_map<state_t,std::vector<label_t> > signature;
        for(auto a : meet.domain())
          for(auto r : meet[a])
            signature[r].emplace_back(a);

        /* We compute  the new parts
         * At every step k, each vector of list contains states which coincide
         * on the k first components of their signature;
         * each vector in new_parts contains contains states qith a signature <k
         * First, states that belong to different parts are separated.
         */
        internal::linked_hash_map<unsigned, std::vector<std::vector<state_t> >> lists;
        {
          internal::linked_hash_map<unsigned,std::vector<state_t> > tmplist;
          for(auto r : signature.domain()) {
            tmplist[part[r]].emplace_back(r);
            part[r]=std::numeric_limits<unsigned>::max();
          }
          for(auto j : tmplist.domain())
            lists[j].push_back(std::move(tmplist[j]));
        }
        internal::linked_hash_map<label_t,std::vector<state_t> > tmplist;
          std::vector<std::vector<state_t> > new_list;
          std::vector<state_t> shorter_sig_list;
          for(unsigned j: lists.domain()) {
            std::vector<std::vector<state_t>> list{std::move(lists[j])};
            lists[j].clear();
            for(unsigned k=0;!list.empty();++k) {
              new_list.clear();
              for(auto l : list) {
                tmplist.clear();
                shorter_sig_list.clear();
                for(auto r : l)
                  if(signature[r].size()==k)
                    shorter_sig_list.emplace_back(r);
                  else {
                    tmplist[signature[r][k]].emplace_back(r);
                  }
                if(!shorter_sig_list.empty())
                  lists[j].push_back(std::move(shorter_sig_list));
                for(auto a : tmplist.domain())
                  new_list.push_back(std::move(tmplist[a]));
              }
              list.swap(new_list);
            }
          }
        for(unsigned p: lists.domain()) {
          std::vector<std::vector<state_t> >&  new_parts=lists[p];;
          //unsigned tp=new_parts.size();
          std::vector<state_t> unmet_states;
          for(auto r : states_in_part[p])
            if(part[r]==p) //The met states have an undefined part
              unmet_states.emplace_back(r);
          if(!unmet_states.empty())
            new_parts.push_back(std::move(unmet_states));
          //p is the original part which is being split
          unsigned lim=new_parts.size();
          unsigned np=states_in_part.size();
          if(is_in_queue[p]) {
            states_in_part[p].swap(new_parts[0]);
            for(auto r : states_in_part[p])
              part[r]=p;
            for(unsigned k=1; k<lim; ++k) {
              for(auto r : new_parts[k])
                part[r]=np;
              states_in_part.push_back(std::move(new_parts[k]));
              splitters.emplace(np);
              is_in_queue.push_back(true);
              ++np;
            }
          }
          else {
            //Search of the smallest and largest subparts
            unsigned min_part=0;
            unsigned max_part=0;
            for(unsigned k=1; k<lim; ++k) {
              if(new_parts[k].size() > new_parts[max_part].size())
                max_part=k;
              if(new_parts[k].size() <= new_parts[min_part].size())
                min_part=k;
            }
            for(unsigned k=0; k<lim;++k)
              if(k==min_part) {
                //The index p is recycled for the smallest subpart
                for(auto r : new_parts[k])
                  part[r] = p;
                states_in_part[p].swap(new_parts[k]);
                if(lim>1) {
                  is_in_queue[p]=true;
                  splitters.emplace(p);
                }
              }
              else {
                //For the other subparts, fresh parts are created
                for(auto r : new_parts[k])
                  part[r]=np;
                states_in_part.push_back(std::move(new_parts[k]));
                if(k!=max_part) {
                  splitters.emplace(np);
                  is_in_queue.push_back(true);
                }
                else
                  is_in_queue.push_back(false);
                ++np;
              }
          }
        }
      }
    }
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_MOORE16_HH
