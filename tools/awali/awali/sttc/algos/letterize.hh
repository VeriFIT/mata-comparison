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

#ifndef AWALI_ALGOS_LETTERIZE_HH
#define AWALI_ALGOS_LETTERIZE_HH

#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon
#include<unordered_map>

/*
  Convert a law automaton into a lan automaton.
  Proper can then be applied to get a lal automaton.
*/

namespace awali {
  namespace sttc {
    namespace internal {

      template <typename Aut, typename Labelset>
      struct letterizer {
        using ret_automaton_t = Aut;
      
        static ret_automaton_t letterize(const Aut& aut, bool keep_history) {
          return copy(aut, keep_history, false, true);
        }
      };
     
      
      template <typename Aut, typename L>
      struct letterizer<Aut,wordset<L>> {
        using automaton_t = Aut;
        using labelset_t = labelset_t_of<Aut>;
        using weightset_t = weightset_t_of<Aut>;
        using letterset_t = typename labelset_trait<labelset_t>::letterset_t;
        using nletterset_t = typename labelset_trait<letterset_t>::nullable_t;
        using ret_context_t = sttc::context<nletterset_t, weightset_t>;
        using ret_automaton_t = mutable_automaton<ret_context_t>;
      
        static ret_automaton_t letterize(const automaton_t& aut, bool keep_history) {
          nletterset_t nletset = get_nullableset(get_letterset(*aut->context().labelset()));
          auto ret= make_mutable_automaton(ret_context_t{nletset,*aut->context().weightset()});
          std::unordered_map<state_t,state_t> state_map;
          for(auto s : aut->states()) {
            state_map[s]= s;
	    ret->add_state(s);
	    if(aut->has_name(s))
	      ret->set_state_name(s, aut->get_state_name(s));	     
          }
          state_map[ret->pre()]=aut->pre();
          state_map[ret->post()]=aut->post();
          for(auto tr: aut->all_transitions()) {
            const auto& w=aut->label_of(tr);
            if(w.length()==0)
              new_epsilon_trans(ret, state_map[aut->src_of(tr)], state_map[aut->dst_of(tr)], aut->weight_of(tr));
            else if (w.length()==1)
              ret->new_transition(state_map[aut->src_of(tr)], state_map[aut->dst_of(tr)], w[0], aut->weight_of(tr));
            else {
              state_t dst=state_map[aut->dst_of(tr)];
              for(unsigned i=w.length()-1; i>0;i--) {
                state_t src=ret->add_state();
                ret->new_transition(src, dst, w[i]);
                dst=src;
              }
              ret->new_transition(state_map[aut->src_of(tr)], dst, w[0], aut->weight_of(tr));
            }
          }
          if(keep_history) {
            auto history = std::make_shared<single_history<automaton_t>>(aut);
            for(auto p : state_map) {
              history->add_state(p.second,p.first);
	      if(aut->has_name(p.first)) {
		ret->set_state_name(p.second, aut->get_state_name(p.first));
	    }
	    }
            ret->set_history(history);
          }
          return ret;
        }
      };
    }//end of internal
    
    template<typename Aut>
    auto letterize(const Aut& aut, bool keep_history=true) -> typename internal::letterizer<Aut,labelset_t_of<Aut>>::ret_automaton_t {
      return internal::letterizer<Aut,labelset_t_of<Aut>>::letterize(aut, keep_history);
    }

                                                     }
}//end of ns awali::stc

#endif
