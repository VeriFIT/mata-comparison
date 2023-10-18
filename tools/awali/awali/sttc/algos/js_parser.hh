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

#ifndef AWALI_ALGOS_JS_PARSER_HH
# define AWALI_ALGOS_JS_PARSER_HH

#include <awali/sttc/misc/raise.hh>
#include <awali/common/json/node.cc>
#include <awali/common/json/smart_printer.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh>
//#include <awali/sttc/core/rat/ratexpset.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/algos/print_exp.hh>
#include <awali/sttc/history/string_history.hh>
# include <stdexcept>
# include <stack>
# include <iostream>
# include <sstream>

namespace awali { namespace sttc {


  namespace internal {

    template<typename RatExpSet>
    struct js_exp_parser {
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using labelset_t = labelset_t_of<context_t>;
      using ratexp_t = typename ratexpset_t::value_t;
      using weightset_t = weightset_t_of<ratexpset_t>;

      js_exp_parser(const ratexpset_t& rs)
        : rs_(rs)
      {}

      ratexp_t parseNode(json::node_t const* p) {
        require(p->kind==json::OBJECT,"js parse ratexp:","object expected");
        json::object_t const* jo=dynamic_cast<json::object_t const*>(p);
        ratexp_t e;
        if(jo->has_child("sum")) {
          auto& v=jo->at("sum")->array()->values;
          e=parseNode(v[0]);
          for(unsigned i=1; i<v.size(); ++i)
            e=rs_.add(e,parseNode(v[i]));
        }
        else if(jo->has_child("prod")) {
          auto& v=jo->at("prod")->array()->values;
          e=parseNode(v[0]);
          for(unsigned i=1; i<v.size(); ++i)
            e=rs_.mul(e,parseNode(v[i]));
        }
        else if(jo->has_child("star"))
          e= rs_.star(parseNode(jo->at("star")));
        else if(jo->has_child("maybe"))
          e= rs_.maybe(parseNode(jo->at("maybe")));
        else if(jo->has_child("plus"))
          e= rs_.plus(parseNode(jo->at("plus")));
        else if(jo->has_child("label"))
          e= rs_.atom(ls_.value_from_json(jo->at("label")));
        else if(jo->has_child("one"))
          e = rs_.one();
        else if(jo->has_child("zero"))
          e = rs_.zero();
        else
          raise("Json parser: ratexp:","none expected field found");

        if(jo->has_child("lweight"))
          e=rs_.lmul(ws_.value_from_json(jo->at("lweight")), e);
        if(jo->has_child("rweight"))
          e=rs_.rmul(e, ws_.value_from_json(jo->at("lweight")));
        return e;
      }
      
    private:
      const ratexpset_t& rs_;
      weightset_t ws_ = *rs_.weightset();
      labelset_t ls_ = *rs_.labelset();
      
    };
  }
    
  template <typename RatExpSet>
  inline
  typename RatExpSet::ratexp_t
  js_parse_exp_content(const RatExpSet& rs,
                       json::node_t const* p)
  {
    internal::js_exp_parser<RatExpSet> parser{rs};
    return parser.parseNode(p);
  }

  template <typename Context>
  mutable_automaton<Context>
  js_parse_aut_content(const Context& context, json::object_t const* p) 
  {
    // p is a pointer on the value of the "Content" field 
    // using aut_t = mutable_automaton<Context>;
    auto ws = context.weightset();
    auto ls = context.labelset();
    mutable_automaton<Context> aut = make_mutable_automaton(context);
    std::unordered_map<unsigned,state_t> states;
    unsigned s;
    require(p->has_child("states"),"json automaton:","no field states");
    for(json::node_t * jv : *p->at("states")->array()) {
      json::object_t* jstate = dynamic_cast<json::object_t*>(jv); 
      require(jstate->has_child("id"),"json automaton:","no state id");
      s=jstate->at("id")->to_int();
      states[s]=aut->add_state();
      if(jstate->has_child("name"))
        aut->set_state_name(states[s],jstate->at("name")->to_string());
      if(jstate->has_child("history")) {
        auto history=aut->history();
        if(history->get_nature() == history_kind_t::NO_HISTORY) {
          history=std::make_shared<string_history>();
          aut->set_history(history);
        }
        auto& hs = dynamic_cast<string_history&>(*history);
        hs.add_state(states[s], jstate->at("history")->to_string());
      }
      if(jstate->has_child("initial"))
        aut->set_initial(states[s] , ws->value_from_json(jstate->at("initial")));
      if(jstate->has_child("final"))
        aut->set_final(states[s] , ws->value_from_json(jstate->at("final")));
    }
    if(p->has_child("transitions"))
      for(json::node_t * jv : *p->at("transitions")->array()) {
        json::object_t* jtr = dynamic_cast<json::object_t*>(jv);
        require(jtr->has_child("source"),"js automaton:","no transition source");
        require(jtr->has_child("destination"),"js automaton:","no transition destination");
        s=jtr->at("source")->to_int();
        unsigned t=jtr->at("destination")->to_int();
        if(jtr->has_child("label")) {
          auto l = ls->value_from_json(jtr->at("label"));
          if(jtr->has_child("weight")) {
            auto w  = ws->value_from_json(jtr->at("weight"));
            aut->new_transition(states[s], states[t],l,w);
          }
          else
            aut->new_transition(states[s], states[t],l);
        }
        else
          if(jtr->has_child("weight")) {
            auto w  = ws->value_from_json(jtr->at("weight"));
            new_epsilon_trans(aut, states[s], states[t],w);
          }
          else
            new_epsilon_trans(aut, states[s], states[t]);
      }  
    return aut;
  }

  template <typename Aut>
  void
  js_add_metadata(Aut& aut, json::object_t* p) {
    if(p->has_child("name"))
      aut->set_name(p->at("name")->to_string());
    if(p->has_child("caption"))
      aut->set_desc(p->at("caption")->to_string());
  }    

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_JS_PARSER_HH
