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

#ifndef AWALI_ALGOS_JS_PARSER_DEPRECATED_HH
# define AWALI_ALGOS_JS_PARSER_DEPRECATED_HH

#include <awali/sttc/misc/raise.hh>
#include <awali/common/json/utils.hh>
#include <awali/common/json/parser.cc>
#include <awali/sttc/misc/add_epsilon_trans.hh>
#include <awali/sttc/core/rat/ratexpset.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/algos/print_exp.hh>
#include <awali/sttc/history/string_history.hh>
# include <stdexcept>
# include <stack>
# include <iostream>
# include <sstream>

namespace awali {
  namespace sttc {
    namespace deprecated {

  namespace detail {

    template<typename RatExpSet>
    struct js_exp_parser {
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using labelset_t = labelset_t_of<context_t>;
      using ratexp_t = typename ratexpset_t::value_t;
      using weightset_t = weightset_t_of<ratexpset_t>;

      js_exp_parser(const ratexpset_t& rs, std::istream& i)
        : rs_(rs), i_(i)
      {}

      ratexp_t parseNode(const std::string& key) {
        ratexp_t e;
        char c;
        if(key == "Sum") {
          bool first=true;
          ::awali::internal::check(i_,'[');
          do {
            ratexp_t f= parseNode();
            if(first) {
              e=f;
              first=false;
            }
            else {
              e=rs_.add(e,f);
            }
            i_ >> c;
          } while (c==',');
          if(c!=']')
            throw std::runtime_error("Json parser: ratexp");
        }
        else if(key == "Prod") {
          bool first=true;
          ::awali::internal::check(i_,'[');
          do {
            ratexp_t f= parseNode();
            if(first) {
              e=f;
              first=false;
            }
            else
              e=rs_.mul(e,f);
            i_ >> c;
          } while (c==',');
          if(c!=']')
            throw std::runtime_error("Json parser: ratexp");
        }
        else if(key == "Star") {
          e= rs_.star(parseNode());
        }
        else if(key == "Label") {
          e= rs_.atom(ls_.value_from_json<1>(i_));
        }
        else if(key == "LWeight") {
          auto w = ws_.value_from_json<1>(i_);
          std::string key2 = ::awali::internal::parsestring(i_);
          ::awali::internal::check(i_,':');
          e=rs_.lmul(w,parseNode(key2));
        }
        else if(key == "RWeight") {
          auto w = ws_.value_from_json<1>(i_);
          std::string key2 = ::awali::internal::parsestring(i_);
          ::awali::internal::check(i_,':');
          e=rs_.rmul(parseNode(key2),w);
        }
        else if(key == "One") {
          char c2 = ::awali::internal::parsecst(i_);
          if(c2 != 'n')
            throw std::runtime_error("Json parser: ratexp");
          e = rs_.one();
        }
        else if(key == "Zero") {
          char c2 = ::awali::internal::parsecst(i_);
          if(c2 != 'n')
            throw std::runtime_error("Json parser: ratexp");
          e = rs_.zero();
        }
        else
          throw std::runtime_error("Json parser: ratexp");
        return e;
      }

      ratexp_t parseNode() {
        std::string key = ::awali::internal::get_first_attr(i_);
        ratexp_t e=parseNode(key);
        ::awali::internal::check(i_,'}');
        return e;
      }

    private:
      const ratexpset_t& rs_;
      std::istream& i_;
      weightset_t ws_ = *rs_.weightset();
      labelset_t ls_ = *rs_.labelset();

    };
  }

  template <typename RatExpSet>
  inline
  typename RatExpSet::ratexp_t
  js_parse_exp_content(const RatExpSet& rs,
                       std::istream& i)
  {
    detail::js_exp_parser<RatExpSet> parser{rs,i};
    return parser.parseNode();
  }

  template <typename Context>
  mutable_automaton<Context>
  js_parse_aut_content(const Context& context, std::istream& i) {
    // using aut_t = mutable_automaton<Context>;
    ::awali::json::node_t* node = nullptr;
    auto ws = context.weightset();
    auto ls = context.labelset();
    mutable_automaton<Context> aut = make_mutable_automaton(context);
    char c;
    std::unordered_map<unsigned,state_t> states;
    unsigned s;
    std::string key = ::awali::internal::get_first_attr(i);
    if(key != "Content")
      throw std::runtime_error("json: Content");
    ::awali::internal::check(i, '[');
    key = ::awali::internal::get_first_attr(i);
    if(key != "States")
      throw std::runtime_error("json: States");
    ::awali::internal::check(i, '[');
    do {
      if(::awali::internal::peek(i)==']') { //no state
        i >> c;
        break;
      }
      key =::awali::internal::get_first_attr(i);
      if(key != "Id")
        throw std::runtime_error("json: State Id");
      s=::awali::internal::parseint(i);
      states[s]=aut->add_state();
      key=::awali::internal::parsestring(i);
      if(key != "Name")
        throw std::runtime_error("json: Name");
      ::awali::internal::check(i, ':');
      key=::awali::internal::parsestring(i);
      aut->set_state_name(states[s],key);
      if(::awali::internal::peek(i)!='}') {
        std::string tmp=::awali::internal::parsestring(i);
        ::awali::internal::check(i, ':');
        if(tmp == "History") {
          key=::awali::internal::parsestring(i);
          auto history=aut->history();
          if(history->get_nature() == history_kind_t::NO_HISTORY) {
            history=std::make_shared<string_history>();
            aut->set_history(history);
          }
          auto& hs = dynamic_cast<sttc::string_history&>(*history);
          hs.add_state(states[s], key);
        }
        else
          ::awali::internal::parseignore(i);
      }
      ::awali::internal::check(i, '}');
      i >> c;
    } while(c==',');
    if(c!=']')
      throw std::runtime_error("json: States ]");
    ::awali::internal::check(i, '}');
    if(::awali::internal::peek(i)==']') {
      ::awali::internal::check(i, ']');
      ::awali::internal::check(i, '}');
      return aut;
    }
    ::awali::internal::check(i, ',');
    key = ::awali::internal::get_first_attr(i);
    if(key == "Initial States") {
      ::awali::internal::check(i, '[');
      do {
        if(::awali::internal::peek(i)!='{') {
          s=::awali::internal::parseint(i);
          aut->set_initial(states[s]);
        }
        else {
          key =::awali::internal::get_first_attr(i);
          if(key != "Id")
            throw std::runtime_error("json: IState Id");
          s=::awali::internal::parseint(i);
          if(::awali::internal::peek(i)=='"') {
            key=::awali::internal::parsestring(i);
            typename Context::labelset_t::value_t l{};
            bool has_label=false;
            if(key == "Label") {
              has_label=true;
              ::awali::internal::check(i, ':');
              node = ::awali::json::parse(i);
              l=ls->template value_from_json<1>(node);
              delete node;
              if(::awali::internal::peek(i)!='"') {
                aut->new_transition(states[s], aut->post(), l);
                ::awali::internal::check(i, '}');
                i >> c;
                continue;
              }
            }
            if(key != "Weight")
              throw std::runtime_error("json: FWeight");
            ::awali::internal::check(i, ':');
            node = ::awali::json::parse(i);
            auto w = ws->template value_from_json<1>(node);
            delete node;
            if(has_label)
              aut->new_transition(states[s], aut->post(), l, w);
            else
              aut->set_initial(states[s], w);
          }
          else
            aut->set_initial(states[s]);
          ::awali::internal::check(i, '}');
        }
        i >> c;
      } while(c==',');
      if(c!=']')
        throw std::runtime_error("json: IStates ]");
      ::awali::internal::check(i, '}');
      if(::awali::internal::peek(i)==']') {
        ::awali::internal::check(i, ']');
        ::awali::internal::check(i, '}');
        ::awali::internal::check(i, ']');
        ::awali::internal::check(i, '}');
        return aut;
      }
      ::awali::internal::check(i,',');
      key=::awali::internal::get_first_attr(i);
    }
    if(key == "Final States") {
      ::awali::internal::check(i, '[');
      do {
        if(::awali::internal::peek(i)!='{') {
          s=::awali::internal::parseint(i);
          aut->set_final(states[s]);
        }
        else {
          key =::awali::internal::get_first_attr(i);
          if(key != "Id")
            throw std::runtime_error("json: FState Id");
          s=::awali::internal::parseint(i);
          if(::awali::internal::peek(i)=='"') {
            key=::awali::internal::parsestring(i);
            typename Context::labelset_t::value_t l{};
            bool has_label=false;
            if(key == "Label") {
              has_label=true;
              ::awali::internal::check(i, ':');
              node = ::awali::json::parse(i);
              l=ls->template value_from_json<1>(node);
              if(::awali::internal::peek(i)!='"') {
                aut->new_transition(states[s], aut->post(), l);
                ::awali::internal::check(i, '}');
                i >> c;
                continue;
              }
            }
            if(key != "Weight")
              throw std::runtime_error("json: FWeight");
            ::awali::internal::check(i, ':');
            node = ::awali::json::parse(i);
            auto w=ws->template value_from_json<1>(node);
            delete node;
            if(has_label)
              aut->new_transition(states[s], aut->post(), l, w);
            else
              aut->set_final(states[s], w);
          }
          else
            aut->set_final(states[s]);
          ::awali::internal::check(i, '}');
        }
        i >> c;
      } while(c==',');
      if(c!=']')
        throw std::runtime_error("json: FStates ]");
      ::awali::internal::check(i, '}');
      if(::awali::internal::peek(i)==']') {
        ::awali::internal::check(i, ']');
        ::awali::internal::check(i, '}');
        ::awali::internal::check(i, ']');
        ::awali::internal::check(i, '}');
        return aut;
      }
      ::awali::internal::check(i,',');
      key=::awali::internal::get_first_attr(i);
    }
    if(key == "Transitions") {
      unsigned t;
      ::awali::internal::check(i, '[');
      do {
        if(::awali::internal::peek(i)==']') { //no transition
          i >> c;
          break;
        }
        key =::awali::internal::get_first_attr(i);
        if(key == "Id") {
          ::awali::internal::parseint(i);
          key=::awali::internal::parsestring(i);
          ::awali::internal::check(i, ':');
        }
        if(key != "Src")
          throw std::runtime_error("json: Src");
        s=::awali::internal::parseint(i);
        key=::awali::internal::parsestring(i);
        ::awali::internal::check(i, ':');
        if(key != "Dst")
          throw std::runtime_error("json: Dst");
        t=::awali::internal::parseint(i);
        if(::awali::internal::peek(i)!='"') {
          new_epsilon_trans(aut, states[s], states[t]);
          ::awali::internal::check(i, '}');
          i >> c;
          continue;
        }
        key=::awali::internal::parsestring(i);
        ::awali::internal::check(i, ':');
        typename Context::labelset_t::value_t l{};
        bool has_label=false;
        if(key == "Label") {
          node = ::awali::json::parse(i);
          l = ls->template value_from_json<1>(node);
          delete node;
          has_label=true;
          if(::awali::internal::peek(i)!='"') {
            aut->new_transition(states[s], states[t],l);
            ::awali::internal::check(i, '}');
            i >> c;
            continue;
          }
          key=::awali::internal::parsestring(i);
          ::awali::internal::check(i, ':');
        }
        if(key != "Weight")
          throw std::runtime_error("json: Weight");
        node = ::awali::json::parse(i);
        auto w  = ws->template value_from_json<1>(node);
        delete node;
        if(has_label)
          aut->new_transition(states[s], states[t],l,w);
        else
          new_epsilon_trans(aut, states[s], states[t], w);
        ::awali::internal::check(i, '}');
        i >> c;
      } while(c==',');
      if(c!=']')
        throw std::runtime_error("json: Trans ]");
      ::awali::internal::check(i, '}');// end object Transition
      ::awali::internal::check(i, ']');// end array Content
      ::awali::internal::check(i, '}');// end objet Content
    }
    return aut;
  }
      
    }
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_JS_PARSER_HH
