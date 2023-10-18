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

#ifndef AWALI_ALGOS_TDC_LAW_TO_LAN_HH
#define AWALI_ALGOS_TDC_LAW_TO_LAN_HH

#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/algos/projection.hh> // tuple_to_tupleset
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon
#include <awali/sttc/misc/sub_tuple.hh>
#include<unordered_map>

/*
  Convert a transducer with words on tape I
  into a transducer with epsilon or letters on this tape.
*/

namespace awali {
  namespace sttc {
    
    namespace internal {
      template<typename T> struct map_nullable;
      template<typename Letterset, typename... T>
      struct map_nullable<tupleset<Letterset, T...>> {
        using tupleT = typename std::cons_tuple<typename labelset_trait<Letterset>::nullable_t,typename map_nullable<tupleset<T...>>::tupleT>::type;
        using type = typename tuple_to_tupleset<tupleT>::type;
        
        static tupleT gettuple(const std::tuple<Letterset, T...>& t) { return std::tuple_cat(std::make_tuple(get_nullableset(std::get<0>(t))), map_nullable<tupleset<T...>>::gettuple (tuple_tail(t))); } 
        
        static type get(const tupleset<Letterset, T...>& ts) { return {gettuple(ts.sets())}; }
        
      };
      
      template<>
      struct map_nullable<tupleset<>> {
        using tupleT = std::tuple<>;
        using type = tupleset<>;
        
        static tupleT gettuple(const std::tuple<>& t) { return t; }
        static type get(const tupleset<>& ts) { return ts; }
      };
      
      template <typename T, size_t I, typename U> struct replace_in_tupleset;
      
      template <typename... T, size_t I, typename U>
      struct replace_in_tupleset<tupleset<T...>, I, U>{
        using tp_t = tupleset<T...>;
        using type = typename tuple_to_tupleset<typename replace_in_tuple<I,U>::template type<std::tuple<T...>>>::type;
        
        static
        type get(const tp_t& t, const U& v) {
          return {replace_in_tuple<I,U>::get(t.sets(), v)};
        }
      };
      
      template <typename Tdc, unsigned I, typename Labelset>
      struct tdc_letterizer {
        using labelset_t = labelset_t_of<Tdc>;
        using weightset_t = weightset_t_of<Tdc>;
        using Ilabelset_t = typename labelset_t::template valueset_t<I>;
        using letterset_t = typename labelset_trait<Ilabelset_t>::letterset_t;
        using nletterset_t = typename labelset_trait<letterset_t>::nullable_t;
        
        using nullable_labelset_t = typename map_nullable<labelset_t>::type;
        using nullable_label_t = typename nullable_labelset_t::value_t;
        
        using ret_labelset_t =  typename replace_in_tupleset<nullable_labelset_t,I,nletterset_t>::type;
        using ret_context_t = sttc::context<ret_labelset_t, weightset_t>;
        using ret_transducer_t = mutable_automaton<ret_context_t>;
        using ret_label_t = typename ret_labelset_t::value_t;
        
        static ret_transducer_t tdc_letterize(const Tdc& tdc, bool keep_history) {
          const labelset_t& in_labelset=*tdc->context().labelset();
          nletterset_t nletset = get_nullableset(get_letterset(in_labelset.template set<I>()));
          nullable_labelset_t null_labelset=map_nullable<labelset_t>::get(in_labelset);
          ret_labelset_t ret_labelset = replace_in_tupleset<nullable_labelset_t,I,nletterset_t>::get(null_labelset,nletset);
          auto ret= make_mutable_automaton(ret_context_t{ret_labelset,*tdc->context().weightset()});
	  copy_into(tdc, ret, keep_history);
          return ret;
        }
      };
      
      template <typename Tdc, unsigned I, typename L>
      struct tdc_letterizer<Tdc, I, wordset<L>> {
        using transducer_t = Tdc;
        using labelset_t = labelset_t_of<Tdc>;
        using weightset_t = weightset_t_of<Tdc>;
        using Ilabelset_t = typename labelset_t::template valueset_t<I>;
        using letterset_t = typename labelset_trait<Ilabelset_t>::letterset_t;
        using nletterset_t = typename labelset_trait<letterset_t>::nullable_t;
        
        using nullable_labelset_t = typename map_nullable<labelset_t>::type;
        using nullable_label_t = typename nullable_labelset_t::value_t;
        
        using ret_labelset_t =  typename replace_in_tupleset<nullable_labelset_t,I,nletterset_t>::type;
        using ret_context_t = sttc::context<ret_labelset_t, weightset_t>;
        using ret_transducer_t = mutable_automaton<ret_context_t>;
        using ret_label_t = typename ret_labelset_t::value_t;
        
        static ret_transducer_t tdc_letterize(const transducer_t& tdc, bool keep_history) {
          const labelset_t& in_labelset=*tdc->context().labelset();
          nletterset_t nletset = get_nullableset(get_letterset(in_labelset.template set<I>()));
          nullable_labelset_t null_labelset=map_nullable<labelset_t>::get(in_labelset);
          ret_labelset_t ret_labelset = replace_in_tupleset<nullable_labelset_t,I,nletterset_t>::get(null_labelset,nletset);
          auto ret= make_mutable_automaton(ret_context_t{ret_labelset,*tdc->context().weightset()});
          std::unordered_map<state_t,state_t> state_map;
          auto epsilon = ret_labelset.one();
          for(auto s : tdc->states()) {
            state_map[s]=ret->add_state();
          }
          state_map[ret->pre()]=tdc->pre();
          state_map[ret->post()]=tdc->post();
          for(auto tr: tdc->all_transitions()) {
            nullable_label_t l=tdc->label_of(tr);
            const auto& w=std::get<I>(l);
            if(w.length()==0)
              ret->new_transition(state_map[tdc->src_of(tr)], state_map[tdc->dst_of(tr)], replace_in_tuple<I,typename nletterset_t::value_t>::get(l,nletset.one()),tdc->weight_of(tr));
            else if (w.length()==1)
              ret->new_transition(state_map[tdc->src_of(tr)], state_map[tdc->dst_of(tr)], replace_in_tuple<I,typename nletterset_t::value_t>::get(l,w[0]), tdc->weight_of(tr));
            else {
              state_t dst=state_map[tdc->dst_of(tr)];
              for(unsigned i=w.length()-1; i>0;i--) {
                state_t src=ret->add_state();
                std::get<I>(epsilon)=w[i];
                ret->new_transition(src, dst, epsilon);
                dst=src;
              }
              ret->new_transition(state_map[tdc->src_of(tr)], dst, replace_in_tuple<I,typename nletterset_t::value_t>::get(l,w[0]), tdc->weight_of(tr));
            }
          }
          if(keep_history) {
            auto sg_history = std::make_shared<single_history<transducer_t>>(tdc);
            for(auto p : state_map) {
              sg_history->add_state(p.second,p.first);
	      if(tdc->has_name(p.first)) {
		ret->set_state_name(p.second, tdc->get_state_name(p.first));
	      }
	    }
            ret->set_history(sg_history);
          }
          return ret;
        }
      };
    }
    
    template<unsigned I, typename Tdc>
    auto letterize_tape(const Tdc& tdc, bool keep_history=true) -> typename internal::tdc_letterizer<Tdc,I,typename labelset_t_of<Tdc>::template valueset_t<I>>::ret_transducer_t {
      return internal::tdc_letterizer<Tdc,I,typename labelset_t_of<Tdc>::template valueset_t<I>>::tdc_letterize(tdc, keep_history);
    }
  }
}//end of ns awali::stc

#endif
