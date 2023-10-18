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

#ifndef AWALI_ALGOS_ALLOW_WORDS_HH
#define AWALI_ALGOS_ALLOW_WORDS_HH

#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon
#include <awali/sttc/algos/copy.hh>
#include<unordered_map>

/*
  Convert a lal automaton into a law automaton.
*/

namespace awali {
  namespace sttc
  {
    namespace internal {
      template <typename Aut>
      struct allowworder {
        using automaton_t = Aut;
        using labelset_t = labelset_t_of<Aut>;
        using weightset_t = weightset_t_of<Aut>;
        using wordset_t = typename labelset_trait<labelset_t>::wordset_t;
        using ret_context_t = sttc::context<wordset_t, weightset_t>;
        using ret_automaton_t = mutable_automaton<ret_context_t>;
      
        static ret_automaton_t allow_words(const automaton_t& aut, bool keep_history) {
          const auto &labelset=*aut->context().labelset();
          wordset_t wordset = get_wordset(labelset);
          auto ret= make_mutable_automaton(ret_context_t{wordset,*aut->context().weightset()});
          std::unordered_map<state_t,state_t> state_map;
          for(auto s : aut->states()) {
	    ret->add_state(s);
            state_map[s]=s;
	    if(aut->has_name(s))
	      ret->set_state_name(s, aut->get_state_name(s));	     
          }
          state_map[ret->pre()]=aut->pre();
          state_map[ret->post()]=aut->post();
          for(auto tr: aut->all_transitions()) {
            const auto& w=aut->label_of(tr);
            if(labelset.is_special(w))
              ret->new_transition(state_map[aut->src_of(tr)], state_map[aut->dst_of(tr)], wordset.special(), aut->weight_of(tr));
            else if(labelset.is_one(w))
              ret->new_transition(state_map[aut->src_of(tr)], state_map[aut->dst_of(tr)], wordset.one(), aut->weight_of(tr));
            else
              ret->new_transition(state_map[aut->src_of(tr)], state_map[aut->dst_of(tr)], wordset.word(w), aut->weight_of(tr));
          }
          if(keep_history) {
            auto history = std::make_shared<single_history<Aut>>(aut);
            ret->set_history(history);
            for (auto p: aut->all_states()) {
              history->add_state(state_map[p], p);
	    }
          }
          return ret;
        }
      };
    }//end of internal

    /** Turns the automaton into an automaton labeled with words
     *
     * If the input automaton already accepts words as labels,
     * a copy is returned.
     *
     * @tparam Aut the type of the input automaton
     * @param aut the input automaton
     * @param keep_history if true the result is endowed with a {@link single_history} to the input automaton
     * @return an automaton where labels are words
     */
    
    template<typename Aut>
    auto allow_words(const Aut& aut, bool keep_history=true) -> typename internal::allowworder<Aut>::ret_automaton_t {
      return internal::allowworder<Aut>::allow_words(aut, keep_history);
    }

    /** Compacts non branching paths
     *
     * This in-place algorithm replace every non branching path 
     * by a transition labeled by the word formed by the labels of the path.
     * The inner states of the path are deleted.     
     * The labels of the automaton must support multiplication (like words). 
     *
     * @tparam Aut the type of the input automaton
     * @param aut the automaton
     */
    template<typename Aut>
    void compact_here(Aut& aut) {
      auto& ws=*aut->context().weightset();
      for(auto s: aut->states()) {
        if(aut->is_initial(s) || aut->is_final(s))
          continue;
        int ci=0,co=0;
        transition_t tri,tro;
        for(auto tria: aut->all_in(s)) {
          ci++;
          tri=tria;
          if(ci>1)
            break;
          for(auto troa: aut->all_out(s)) {
            co++;
            tro=troa;
          if(co>1)
            break;
          }
        }
        if(ci==1 && co==1) {
          aut->add_transition(aut->src_of(tri),aut->dst_of(tro),aut->label_of(tri)+aut->label_of(tro),ws.mul(aut->weight_of(tri),aut->weight_of(tro)));
          aut->del_state(s);
        }
      }
    }
    
    /** Compacts non branching paths
     *
     * This function applies {@link compact_here} to a copy of \p aut.
     * If the labels of the automaton are letters, the type of the copy is enhanced in order to support words as labels.
     *
     * @tparam Aut the type of the input automaton
     * @param aut the automaton
     * @param keep_history if true the result is endowed with a {@link single_history} to the input automaton
     * @return an automaton with compacted paths.
     */
    template <typename Aut>
    auto
    compact(const Aut& aut, bool keep_history=true) -> typename internal::allowworder<Aut>::ret_automaton_t
    {
      auto ret=allow_words(aut, keep_history);
      compact_here(ret);
      return ret;
    }
  }
}

#endif
