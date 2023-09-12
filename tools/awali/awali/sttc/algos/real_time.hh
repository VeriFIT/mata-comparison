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

#ifndef AWALI_ALGOS_REALTIME_HH
#define AWALI_ALGOS_REALTIME_HH

#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/misc/sub_tuple.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon
#include<unordered_map>
#include<map>
#include<stack>
#include<tuple>
#include<utility>

namespace awali {
  namespace sttc {
    namespace internal {
#ifndef DETAILS_APPEND
#define DETAILS_APPEND
      void append(std::string & s, char c) {
        s.append(1,c);
      }
      void append(std::string & s, const std::string t) {
        s.append(t);
      }
#endif
    
      template <typename Tdc>
      struct realtimer {
        using labelset_t = labelset_t_of<Tdc>;
        using labelset0_t = typename labelset_t::template valueset_t<0>;
        using labelset1_t = typename labelset_t::template valueset_t<1>;
        using wordset1_t = typename labelset_trait<labelset1_t>::wordset_t;
        using weightset_t = weightset_t_of<Tdc>;
        using wordset1_value_t = typename wordset1_t::value_t;
        using res_labelset_t = tupleset<labelset0_t,wordset1_t>;
        using res_context_t = sttc::context<res_labelset_t, weightset_t>;
      
        using notnullset0_t = typename labelset_trait<labelset0_t>::not_nullable_t;
        using ret_labelset_t = tupleset<notnullset0_t,wordset1_t>;
        using ret_context_t = sttc::context<ret_labelset_t, weightset_t>;
        using automaton_t = mutable_automaton<ret_context_t>;
      
      
        using pair_t = std::pair<state_t, wordset1_value_t>;
      
	static bool is_realtime(const Tdc& tdc) {
	  for(auto t : tdc->transitions())
	    if(is_epsilon<labelset0_t>(std::get<0>(tdc->label_of(t))))
	      return false;
	  return true;
	}
	
	static bool is_sequential(const Tdc& tdc) {
	  if(!is_realtime(tdc))
	    return false;
	  if (1 < tdc->num_initials())
	    return false;
	  using label0_t = typename labelset0_t::value_t;
	  for(auto s : tdc->states()) {
	    std::unordered_set<label0_t> seen;
	    for(auto t : tdc->out(s))
	      if (!seen.insert(std::get<0>(tdc->label_of(t))).second)
		return false;
	  }
	  return true;
	}

	static automaton_t realtime(const Tdc& tdc, bool keep_history) {
	  std::unordered_map<pair_t, state_t> states;
	  std::stack<pair_t> todo;
	
	  auto lsets =tdc->context().labelset()->sets();
	  wordset1_t wset1=get_wordset(std::get<1>(lsets));
	  auto res = make_mutable_automaton(res_context_t{
	      res_labelset_t(std::make_tuple(std::get<0>(lsets), wset1)),*tdc->context().weightset()});      
          auto get_state = [&](pair_t& pair) -> state_t {
            auto it = states.find(pair);
            if(it == states.end()) {
              state_t r = res->add_state();
              states.emplace(std::make_pair(pair, r));
              todo.emplace(pair);
              return r;
            }
            else
              return it->second;
          };
      
          //states to explore:
          std::vector<pair_t> finals;
      
          for(auto i : tdc->initial_transitions()) {
            pair_t tu{tdc->dst_of(i),wordset1_t::one()};
            auto s = get_state(tu);
            res->set_initial(s, tdc->weight_of(i));
            todo.emplace(tu);
          }
          while(!todo.empty()) {
            auto pair = todo.top();
            todo.pop();
            //retrieve the valuation of state s:
            state_t s = get_state(pair);
            for(auto tr : tdc->all_out(pair.first)) {
              state_t t = tdc->dst_of(tr);
              if(t == tdc->post()) {
                if(is_epsilon<wordset1_t>(pair.second))
                  res->set_final(s, tdc->weight_of(tr));
                else
                  finals.emplace_back(pair);
                continue;
              }
              auto l= tdc->label_of(tr);
              //the valuation of t is the valuation of s...
              pair_t t_val{pair};
              //... plus the label of the transition
              t_val.first= t;
              if(!is_epsilon<labelset1_t>(std::get<1>(l))) {
                internal::append(t_val.second,std::get<1>(l));
              }
              if(!is_epsilon<labelset0_t>(std::get<0>(l))) {
                auto w=t_val.second;
                t_val.second=wordset1_t::one();
                state_t nt = get_state(t_val);
                res->set_transition(s, nt, std::make_tuple(std::get<0>(l),w), tdc->weight_of(tr));
              }
              else {
                auto w=wordset1_t::one();
                state_t nt = get_state(t_val);
                res->set_transition(s, nt, std::make_tuple(std::get<0>(l),w), tdc->weight_of(tr));
              }
            }
          }

          if(!finals.empty()) {
            //state_t r = res->add_state();
            auto a = labelset0_t::special();
            for(auto pair : finals) {
              state_t s=get_state(pair);
              res->set_transition(s, res->post(), std::make_tuple(a, pair.second),tdc->get_final_weight(pair.first));
            }
            //res->set_final(r);
          }
          proper_here(res);
          notnullset0_t nnlset0 =get_not_nullableset(std::get<0>(lsets));
          auto ret = make_mutable_automaton(ret_context_t{
              ret_labelset_t(std::make_tuple(nnlset0, wset1)),*tdc->context().weightset()});
          copy_into(res,ret,keep_history);
          if(keep_history) {
            auto final_history = std::make_shared<single_history<Tdc>>(tdc);
            auto & ret_history = ret->history()->template as<single_history<decltype(*res)>>();
            std::unordered_map<state_t, state_t> inv_states;
            for(auto pp: states)
              inv_states[pp.second]=pp.first.first;
            for(auto s : ret->states())
              final_history->add_state(s,inv_states[ret_history.get_state(s)]);
            ret->set_history(final_history);
          }
          return ret;
        }
      };
    }
    // 
    template<typename Tdc>
    auto realtime(const Tdc& tdc, bool keep_history=true) -> typename internal::realtimer<Tdc>::automaton_t {
      return internal::realtimer<Tdc>::realtime(tdc, keep_history);
    }
							     
    template<typename Tdc>
    bool is_realtime(const Tdc& tdc) {
      return internal::realtimer<Tdc>::is_realtime(tdc);
    }

    template<typename Tdc>
    bool is_sequential_tdc(const Tdc& tdc) {
      return internal::realtimer<Tdc>::is_sequential(tdc);
    }
									    }
}//end of ns awali::stc

#endif
