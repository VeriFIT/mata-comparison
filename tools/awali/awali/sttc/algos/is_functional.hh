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

#ifndef AWALI_ALGOS_IS_FUNCTIONAL_HH
#define AWALI_ALGOS_IS_FUNCTIONAL_HH

# include <stack>
# include <unordered_map>

#include <awali/sttc/labelset/tupleset.hh>
//#include <awali/sttc/misc/sub-tuple.hh> // make_index_sequence
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon
#include <awali/sttc/algos/projection.hh> // tuple_to_tupleset
#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/algos/partial_identity.hh>
#include <awali/sttc/algos/compose.hh>


namespace awali { namespace sttc {

  template <typename Tdc>
  bool is_functional(Tdc& transducer)
  {
    auto inv = sttc::projections<1,0>(transducer);
    auto tdc = compose(inv, transducer);
    trim_here(tdc);
    std::unordered_map<unsigned, std::pair<std::string,std::string>> valuations;
    std::stack<unsigned> todo;

    std::pair<std::string,std::string> base{"",""};
    using labelset_t = typename labelset_t_of<Tdc>::template valueset_t<1>;

    for(auto ti : tdc->initial_transitions()) {
      //if the initial state has already been met
      auto it = valuations.find(tdc->dst_of(ti));
      if(it != valuations.end()) {
        //it should have a trivial valuation
        if(it->second != base)
          return false;
      }
      else {
        valuations.emplace(std::make_pair(tdc->dst_of(ti),base));
        todo.emplace(tdc->dst_of(ti));
      }
      while(!todo.empty()) {
        unsigned s = todo.top();
        todo.pop();
        //retrieve the valuation of state s:
        auto valuation = valuations.find(s)->second;
        for(auto tr : tdc->all_out(s)) {
          unsigned t = tdc->dst_of(tr);
          if(t == tdc->post()) {
            //if s is final it should have a trivial valuation
            if(valuation != base)
              return false;
          }
          else {
            auto l= tdc->label_of(tr);
            //the valuation of t is the valuation of s...
            std::pair<std::string,std::string> t_val{valuation};
            //... plus the label of the transition
            if(!is_epsilon<labelset_t>(std::get<0>(l))) {
              auto c = std::get<0>(l);
              t_val.first = t_val.first+c;
            }
            if(!is_epsilon<labelset_t>(std::get<1>(l))) {
              auto c = std::get<1>(l);
              t_val.second = t_val.second+c;
            }
            //compute the length p of the greater common prefix
            unsigned p;
            for(p=0; p< t_val.first.length() && p<t_val.second.length() && t_val.first[p]==t_val.second[p]; ++p)
              ;
            //... minus the greatest common prefix
            t_val.first = t_val.first.substr(p);
            t_val.second = t_val.second.substr(p);
            //at least one component of the valuation should be empty
            if(t_val.first.length() >0 && t_val.second.length()>0)
              return false;
            //if t has already been met, it should have the same valuation
            auto it = valuations.find(t);
            if(it != valuations.end()) {
              if(it->second != t_val)
                return false;
            }
            else {
              valuations.emplace(std::make_pair(t,t_val));
              todo.emplace(t);
            }
        }
        }
      }
    }
    return true;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_IS_FUNCTIONAL_HH
