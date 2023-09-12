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

#include<iostream>
#include<string>
#include <awali/dyn/core/context_description.hh>
#include <awali/dyn/core/abstract_context.hh>
#include <awali/dyn/bridge_sttc/explicit_context.cc>
#include <awali/sttc/labelset/nullableset.hh>
#include <awali/sttc/labelset/tupleset.hh>
#include <awali/sttc/labelset/oneset.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/ctx/lal_int.hh>
#include <awali/sttc/ctx/lal_char.hh>
#include <awali/sttc/weightset/ratexpset.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/dyn/bridge_sttc/explicit_ratexp.cc>
#include <awali/sttc/algos/js_parser.hh>
#include <awali/sttc/algos/js_parser_deprecated.hh>
#include<set-types.hh>

namespace awali {

  template<typename T>
  struct make_labelset{};

  template<>
  struct make_labelset<sttc::oneset>{
    static sttc::oneset get(dyn::context::labelset_description ct) {
      return {};
    }
  };

  template<>
  struct make_labelset<sttc::ctx::lal_char>{
    static sttc::ctx::lal_char get(dyn::context::labelset_description ct) {
      std::set<char> s;
      for(auto i: ct->alphabet)
        s.emplace(i[0]);
      return {sttc::set_alphabet<sttc::char_letters>(s)};
    }
  };

  template<>
  struct make_labelset<sttc::ctx::lal_int>{
    static sttc::ctx::lal_int get(dyn::context::labelset_description ct) {
      std::set<int> s;
      for(auto i: ct->int_alphabet)
        s.emplace(i);
      return {sttc::set_alphabet<sttc::int_letters>(s)};
    }
  };

  template<typename T>
  struct make_labelset<sttc::wordset<T>>{
    static sttc::wordset<T> get(dyn::context::labelset_description ct) {
      std::set<char> s;
      for(auto i: ct->alphabet)
        s.emplace(i[0]);
      return {T(s)};
    }
  };

  template<typename T>
  struct make_labelset<sttc::nullableset<T>>{
    static sttc::nullableset<T> get(dyn::context::labelset_description ct) {
      return {make_labelset<T>::get(ct->children_.front())};
    }
  };

  template<typename... T>
  struct make_labelset<sttc::tupleset<T...>>{
    using tuple_t = sttc::tupleset<T...>;

    template <std::size_t I>
      using valueset_t=typename tuple_t::template valueset_t<I>;

    template <std::size_t... I>
    static tuple_t get(dyn::context::labelset_description ct, awali::internal::index_sequence<I...>) {
      return {std::make_tuple(make_labelset<valueset_t<I>>::get(ct->children_[I])...)};
    }

    static tuple_t get(dyn::context::labelset_description ct) {
      return get(ct, tuple_t::indices);
    }
  };

  template<typename Weightset>
  struct make_weightset{
    static Weightset get(dyn::context::weightset_description ct) {
      return {};
    }
  };

  template<typename Context>
  struct make_context{
    static Context get(dyn::context::context_description ct) {
      return {make_labelset<typename Context::labelset_t>::get(ct->ls_),
          make_weightset<typename Context::weightset_t>::get(ct->ws_)};
    }
  };

  template<typename T>
  struct make_weightset<sttc::ratexpset<T>>{
    static sttc::ratexpset<T> get(dyn::context::weightset_description wd) {
      if(wd->type_!=dyn::context::WTypes::RATEXP && wd->type_!=dyn::context::WTypes::SERIES)
        throw std::runtime_error("Wrong weightset description");
      auto ctx=make_context<T>::get(wd->ct_);
      if(wd->type_==dyn::context::WTypes::RATEXP)
        return sttc::ratexpset<T>(ctx, sttc::rat::identities::trivial);
      return sttc::ratexpset<T>(ctx, sttc::rat::identities::series);
    }
  };

  extern "C" dyn::automaton_t make_automaton_from_context(dyn::context_t ctx) {
    context_t c = dyn::get_stc_context<context_t>(ctx);
    return dyn::make_automaton(sttc::make_mutable_automaton(c));
  }

  extern "C" dyn::automaton_t make_automaton_from_desc(dyn::context::context_description ct) {
    context_t context=make_context<context_t>::get(ct);
    return dyn::make_automaton(sttc::make_mutable_automaton(context));
  }

  extern "C" dyn::context_t make_context_from_desc(dyn::context::context_description ct) {
    context_t context=make_context<context_t>::get(ct);
    return std::make_shared<dyn::explicit_context_t<context_t>>(context);
  }

  extern "C" dyn::automaton_t parse_automaton(json::object_t* p, dyn::context::context_description ct) {
    context_t c=make_context<context_t>::get(ct);
    if(!p->has_child("data"))
      throw std::runtime_error("No data in Automaton description");
    auto aut=sttc::js_parse_aut_content(c, p->at("data")->object());
    if(p->has_child("metadata"))
      js_add_metadata(aut,p->at("metadata")->object());
    return dyn::make_automaton(aut);
  }

  extern "C" dyn::automaton_t parse_automaton_deprecated(std::istream& i, dyn::context::context_description ct) {
    context_t c=make_context<context_t>::get(ct);
    return dyn::make_automaton(sttc::deprecated::js_parse_aut_content(c, i));
  }

  template<typename T>
  struct ratexp_dispatch {
    
    static dyn::ratexp_t 
    make_ratexp_with_context(const char* str, dyn::context_t ctx, 
                             bool fixed_alphabet ) 
    {
      context_t context = dyn::get_stc_context<context_t>(ctx);
      using ratset_t = sttc::ratexpset_of<context_t>;
      auto rcontext=sttc::get_rat_context(context);
      auto ratset = std::make_shared<ratset_t>(rcontext, ratset_t::identities_t::trivial);
      return make_ratexp( sttc::parse_exp(*ratset,str, true, fixed_alphabet), 
                          ratset);

    }
    static dyn::ratexp_t make_ratexp_with_desc(const char* str, dyn::context::context_description ct, bool fixed_alphabet) {
      context_t context=make_context<context_t>::get(ct);
      using ratset_t = sttc::ratexpset_of<context_t>;
      auto rcontext=sttc::get_rat_context(context);
      auto ratset = std::make_shared<ratset_t>(rcontext, ratset_t::identities_t::trivial);
      return make_ratexp(sttc::parse_exp(*ratset,str, true, fixed_alphabet), ratset);
    }

    static dyn::ratexp_t parse_ratexp(dyn::context::context_description ct, json::object_t* j) {
      context_t context=make_context<context_t>::get(ct);
      using ratset_t = sttc::ratexpset_of<context_t>;
      auto ratset = std::make_shared<ratset_t>(get_rat_context(context), ratset_t::identities_t::trivial);
      return make_ratexp(sttc::js_parse_exp_content(*ratset,j), ratset);
    }
  };

  
  extern "C" dyn::ratexp_t 
  make_ratexp_with_context(const char* str, dyn::context_t ctx, 
                           bool fixed_alphabet) 
  {
    return ratexp_dispatch<labelset_t>::make_ratexp_with_context(str, ctx,
                                                              fixed_alphabet); 
  }

  extern "C" dyn::ratexp_t make_ratexp_with_desc(const char* str, dyn::context::context_description ct, bool fixed_alphabet) {
    return ratexp_dispatch<labelset_t>::make_ratexp_with_desc(str, ct, fixed_alphabet);
  }

  extern "C" dyn::ratexp_t parse_ratexp(dyn::context::context_description ct, json::object_t* j) {
    return ratexp_dispatch<labelset_t>::parse_ratexp(ct, j);
  }

  extern "C" std::vector<state_t> states(dyn::abstract_automaton_t const* aut,
                                         bool all)
  {
    return dynamic_cast<dyn::explicit_automaton_t<context_t> const&>(*aut).states(all);
  }

  extern "C" std::vector<state_t> transitions(dyn::abstract_automaton_t const* aut,
                                         bool all)
  {
    return dynamic_cast<dyn::explicit_automaton_t<context_t> const&>(*aut).transitions(all);
  }

  extern "C" std::vector<state_t> outgoing(dyn::abstract_automaton_t const* aut,
                                           dyn::state_t s, bool all)
  {
    return dynamic_cast<dyn::explicit_automaton_t<context_t> const&>(*aut).outgoing(s, all);
  }

  extern "C" std::vector<state_t> incoming(dyn::abstract_automaton_t const* aut,
                                           dyn::state_t s, bool all)
  {
    return dynamic_cast<dyn::explicit_automaton_t<context_t> const&>(*aut).incoming(s, all);
  }


  extern "C" unsigned get_origin_state(
    dyn::automaton_t aut, 
    unsigned s) 
  {
    auto sttc_aut = dyn::get_stc_automaton<context_t> (aut);
    if (sttc_aut->history()->has_history(s))
      return sttc_aut->history()->get_state(s);
    else 
      /* the -2 is for avoid pre and post states */
      return (std::numeric_limits<unsigned>::max()-2);
  }
}

