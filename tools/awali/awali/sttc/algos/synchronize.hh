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

#ifndef AWALI_ALGOS_SYNCHRONIZE_HH
#define AWALI_ALGOS_SYNCHRONIZE_HH

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
      struct synchronizer {
        using labelset_t = labelset_t_of<Tdc>;
        using labelset0_t = typename labelset_t::template valueset_t<0>;
        using labelset1_t = typename labelset_t::template valueset_t<1>;
        using wordset0_t = typename labelset_trait<labelset0_t>::wordset_t;
        using wordset1_t = typename labelset_trait<labelset1_t>::wordset_t;
        using wordset0_value_t = typename wordset0_t::value_t;
        using wordset1_value_t = typename wordset1_t::value_t;
        using triple_t = std::tuple<state_t, wordset0_value_t, wordset1_value_t>;
    
        static Tdc algo(const Tdc& tdc, bool keep_history=true) {

          std::unordered_map<triple_t, state_t> states;
          std::stack<triple_t> todo;

          Tdc res = make_mutable_automaton(tdc->context());

          auto get_state = [&](triple_t& triple) -> state_t {
            auto it = states.find(triple);
            if(it == states.end()) {
              state_t r = res->add_state();
              states.emplace(std::make_pair(triple, r));
              todo.emplace(triple);
              return r;
            }
            else
              return it->second;
          };

          //states to explore:
          std::vector<triple_t> finals;

          for(auto i : tdc->initial_transitions()) {
            triple_t tu{tdc->dst_of(i),wordset0_t::one(),wordset1_t::one()};
            auto s = get_state(tu);
            res->set_initial(s, tdc->weight_of(i));
            todo.emplace(tu);
          }
          while(!todo.empty()) {
            auto triple = todo.top();
            todo.pop();
            //retrieve the valuation of state s:
            state_t s = get_state(triple);
            for(auto tr : tdc->all_out(std::get<0>(triple))) {
              state_t t = tdc->dst_of(tr);
              if(t == tdc->post()) {
                if(is_epsilon<wordset0_t>(std::get<1>(triple)) && is_epsilon<wordset1_t>(std::get<2>(triple)))
                  res->set_final(s, tdc->weight_of(tr));
                else
                  finals.emplace_back(triple);
                continue;
              }
              auto l= tdc->label_of(tr);
              //the valuation of t is the valuation of s...
              triple_t t_val{triple};
              //... plus the label of the transition
              std::get<0>(t_val)= t;
              if(!is_epsilon<labelset0_t>(std::get<0>(l))) {
                internal::append(std::get<1>(t_val),std::get<0>(l));
              }
              if(!is_epsilon<labelset1_t>(std::get<1>(l))) {
                internal::append(std::get<2>(t_val),std::get<1>(l));
              }
              auto a = get_epsilon<labelset0_t>();
              auto b = get_epsilon<labelset1_t>();
              if(!is_epsilon<wordset0_t>(std::get<1>(t_val)) && !is_epsilon<wordset1_t>(std::get<2>(t_val))) {
                a = std::get<1>(t_val)[0];
                b = std::get<2>(t_val)[0];
                std::get<1>(t_val) = std::get<1>(t_val).substr(1);
                std::get<2>(t_val) = std::get<2>(t_val).substr(1);
              }
              state_t nt = get_state(t_val);
              res->set_transition(s, nt, std::make_tuple(a,b), tdc->weight_of(tr));
            }
          }

          std::unordered_map<std::tuple<wordset0_value_t, wordset1_value_t>, state_t> statesf;
          if(!finals.empty()) {
            auto get_statef = [&](std::tuple<wordset0_value_t, wordset1_value_t>& p) -> state_t {
              auto it = statesf.find(p);
              if(it == statesf.end()) {
                state_t r = res->add_state();
                statesf.emplace(std::make_pair(p, r));
                return r;
              }
              else
                return it->second;
            };
            state_t s;
            for(auto triple : finals) {
              s = get_state(triple);
              auto p = rem_in_tuple<0>::get(triple);
              bool first=true;
              while(!is_epsilon<wordset0_t>(std::get<0>(p)) || !is_epsilon<wordset1_t>(std::get<1>(p))) {
                auto a = get_epsilon<labelset0_t>();
                auto b = get_epsilon<labelset1_t>();
                if(!is_epsilon<wordset0_t>(std::get<0>(p))) {
                  a=std::get<0>(p)[0];
                  std::get<0>(p) = std::get<0>(p).substr(1);
                }
                if(!is_epsilon<wordset1_t>(std::get<1>(p))) {
                  b=std::get<1>(p)[0];
                  std::get<1>(p) = std::get<1>(p).substr(1);
                }
                state_t t= get_statef(p);
                if(first) {
                  res->set_transition(s, t, std::make_tuple(a, b),tdc->get_final_weight(std::get<0>(triple)));
                  first=false;
                }
                else
                  res->set_transition(s, t, std::make_tuple(a, b));
                s=t;
              }
            }
            res->set_final(s);
          }
          if(keep_history) {
            auto final_history = std::make_shared<single_history<Tdc>>(tdc);
            for(auto pp: states)
              final_history->add_state(pp.second,std::get<0>(pp.first));
          }
          proper_here(res);
          return res;
        }
      };
    
      template<typename Tdc>
      struct is_synchronizable_impl {
        using labelset_t = labelset_t_of<Tdc>;
        using labelset0_t = typename labelset_t::template valueset_t<0>;
        using labelset1_t = typename labelset_t::template valueset_t<1>;
      private :
        Tdc tdc_;
        std::unordered_map<unsigned, unsigned> status;// 1: active, 2: checked
        std::unordered_map<state_t, int> diff;// state-> diff input/output
      public :
        is_synchronizable_impl(Tdc tdc) : tdc_(tdc)  {}

        bool test() {
          for(auto tr : tdc_->initial_transitions()) {
            state_t i=tdc_->dst_of(tr);
            auto it = status.find(i);
            if(it != status.end())
              continue;
            diff[i]=0;
            if(!test_rec(i))
              return false;
          }
          return true;
        }


        bool test_rec(state_t s) {
          status[s]=1;
          for(auto tr : tdc_->out(s)) {
            state_t t=tdc_->dst_of(tr);
            auto it = status.find(t);
            if(it != status.end() && it->second == 2) // every state accessible from t is ok
              continue;
            int d=diff[s];
            if(!is_epsilon<labelset0_t>(std::get<0>(tdc_->label_of(tr))))
              ++d;
            if(!is_epsilon<labelset1_t>(std::get<1>(tdc_->label_of(tr))))
              --d;
            if(it == status.end()) {// t has never been visited
              diff[t]=d;
              if(!test_rec(t))
                return false;
            }
            else { // t is already active; it is an ancester of s
              if(diff[t] != d)
                return false;
            }
          }
          status[s]=2;
          return true;
        }
      };
    }

    template <typename Tdc>
    Tdc synchronize(const Tdc& tdc, bool keep_history=true) {
      return internal::synchronizer<Tdc>::algo(tdc, keep_history);
    }

    template<typename Tdc>
    bool is_synchronizable(Tdc tdc) {
      internal::is_synchronizable_impl<Tdc> algo(tdc);
      return algo.test();
    }

  }
}//end of ns awali::stc

#endif
