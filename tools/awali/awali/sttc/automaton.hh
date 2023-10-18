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

#ifndef AWALI_STTC_WAUTOMATON_HH
#define AWALI_STTC_WAUTOMATON_HH

#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/ctx/lal_char.hh>
#include <awali/sttc/ctx/lan_char.hh>
#include <awali/sttc/ctx/lat.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include<vector>
#include <awali/sttc/algos/js_parser.hh>
#include <awali/sttc/algos/js_print.hh>
#include <awali/sttc/algos/exp_parser.hh>
#include<awali/sttc/labelset/traits.hh>// ratexpset_of
#include<awali/sttc/json.hxx>//json_support


namespace awali {
  namespace sttc {

    /**
       Build an awali weighted automaton labeled by letters

       The weightset must admit a constructor without argument.

       @tparam T The type of the weightset : z, c, zz<5>, ... (must be included)
       @param letters An initializer list of letters; for instance {'x','y'}
       @ return : an empty automaton over the weightset and the alphabet
    **/
    template<typename T>
    mutable_automaton<context<ctx::lal_char,T>>
      make_automaton(const std::set<char>& letters)
    {
      return make_shared_ptr<mutable_automaton<context<ctx::lal_char,T>>>
        (context<ctx::lal_char,T>(ctx::lal_char(letters),T()));
    }

    /**
       Specialization of the template function for Boolean automata (NFA)
       @return an empty NFA over the given alphabet
     **/
    mutable_automaton<context<ctx::lal_char,b>>
      make_automaton(const std::set<char>& letters)
    {
      return make_automaton<b>(letters);
    }

    template<typename T>
    context<ctx::lal_char,T>
    make_context(const std::set<char>& letters) {
      return {ctx::lal_char(letters),T()};
    }

    context<ctx::lal_char,b>
    make_context(const std::set<char>& letters) {
      return {ctx::lal_char(letters),b()};
    }

    ctx::lan_char::value_t get_epsilon() {
      return ctx::lan_char::one();
    }

    bool is_epsilon(ctx::lan_char::value_t v) {
      return ctx::lan_char::is_one(v);
    }
    
    /**
       Build an awali weighted automaton labeled by letters with epsilon transitions

       The weightset must admit a constructor without argument.

       @tparam T The type of the weightset : z, c, zz<5>, ... (must be included)
       @param letters An initializer list of letters; for instance {'x','y'}
       @ return : an empty automaton over the weightset and the alphabet
    **/
    template<typename T>
    mutable_automaton<context<ctx::lan_char,T>>
      make_automaton_with_epsilon(const std::set<char>& letters)
    {
      return make_shared_ptr<mutable_automaton<context<ctx::lan_char,T>>>
        (context<ctx::lan_char,T>(ctx::lan_char(letters),T()));
    }

    /**
       Specialization of the template function for Boolean automata (NFA)
       @return an empty NFA over the given alphabet
     **/
    mutable_automaton<context<ctx::lan_char,b>>
      make_automaton_with_epsilon(const std::set<char>& letters)
    {
      return make_automaton_with_epsilon<b>(letters);
    }

      template<typename T>
    mutable_automaton<context<ctx::lat<ctx::lan_char,ctx::lan_char>,T>>
      make_transducer(const std::set<char>& letterA, const std::set<char>& letterB)
    {
      return make_shared_ptr<mutable_automaton<context<ctx::lat<ctx::lan_char,ctx::lan_char>,T>>>
        (context<ctx::lat<ctx::lan_char,ctx::lan_char>,T>(std::make_tuple(ctx::lan_char(letterA), ctx::lan_char(letterB)),T()));
    }

    mutable_automaton<context<ctx::lat<ctx::lan_char,ctx::lan_char>,b>>
      make_transducer(const std::set<char>& letterA, const std::set<char>& letterB)
    {
      return make_transducer<b>(letterA, letterB);
    }
      
    template<typename T>
    mutable_automaton<context<ctx::lal_char,T>>
      load_automaton(std::istream& is) {
      json_ast_t p = json_ast::from(is);
      if(!p->has_child("kind") || p->at("kind")->to_string() != "Automaton")
        throw std::runtime_error("json: Automaton");
      if(!p->has_child("context"))
	throw std::runtime_error("No context in Automaton description");
      awali::json::object_t* jk=p->at({"context"})->object();
      awali::json::object_t* jl=jk->at({"labels"})->object();
      if(jl->at("labelKind")->to_string() != "Letters" 
         || jl->at("letterType")->to_string() != "Char")
        throw std::runtime_error("Not a letter automaton");
      if(jl->has_child("allowEpsilon"))
        throw std::runtime_error("Epsilon transitions not supported by this function");
      auto ctx = context<ctx::lal_char,T>(ctx::lal_char({}),T());
      for(auto jv : *jl->at("alphabet")->array())
        const_cast<ctx::lal_char::genset_t&>(ctx.labelset()->genset()).add_letter(jv->to_string()[0]);
      auto aut = js_parse_aut_content(ctx, p->at("data")->object());
      if(p->has_child("metadata"))
        js_add_metadata(aut, p->at("metadata")->object());
      return aut;
    }
      
    template<typename T>
    mutable_automaton<context<ctx::lan_char,T>>
      load_automaton_with_epsilon(std::istream& is) {
      json_ast_t p = json_ast::from(is);
      if(!p->has_child("kind") || p->at("kind")->to_string() != "Automaton")
        throw std::runtime_error("json: Automaton");
      if(!p->has_child("context"))
	throw std::runtime_error("No context in Automaton description");
      awali::json::object_t* jk=p->at({"context"})->object();
      awali::json::object_t* jl=jk->at({"labels"})->object();
      if(jl->at("labelKind")->to_string() != "Letters" 
         || jl->at("letterType")->to_string() != "Char")
        throw std::runtime_error("Not a letter automaton");
      auto ctx = context<ctx::lan_char,T>(ctx::lan_char(std::set<char>()),T());
      for(auto jv : *jl->at("alphabet")->array())
        const_cast<ctx::lan_char::genset_t&>(ctx.labelset()->genset()).add_letter(jv->to_string()[0]);
      auto aut = js_parse_aut_content(ctx, p->at("data")->object());
      if(p->has_child("metadata"))
        js_add_metadata(aut, p->at("metadata")->object());
      return aut;
    }

    mutable_automaton<context<ctx::lal_char,b>>
    load_automaton(std::istream& is) {
        return load_automaton<b>(is);
    }

    template<typename T>
    ratexpset_of<context<ctx::lal_char, T>>
    make_ratexpset() {
        using context_t = context<ctx::lal_char, T>;
        using ratset_t = ratexpset_of<context_t>;
        context_t ctx(ctx::lal_char{},T());
        return {get_rat_context(ctx), ratset_t::identities_t::trivial};
    }

    ratexpset_of<context<ctx::lal_char, b>>
    make_ratexpset() {
        return make_ratexpset<b>();
    }

    template<typename T>
    ratexpset_of<context<ctx::lat<ctx::lan_char,ctx::lan_char>, T>>
    make_tdc_ratexpset() {
        using context_t = context<ctx::lat<ctx::lan_char,ctx::lan_char>, T>;
        using ratset_t = ratexpset_of<context_t>;
        context_t ctx(ctx::lat<ctx::lan_char,ctx::lan_char>{{ctx::lal_char{}},{ctx::lal_char{}}},T());
        return {get_rat_context(ctx), ratset_t::identities_t::trivial};
    }

    ratexpset_of<context<ctx::lat<ctx::lan_char,ctx::lan_char>, b>>
    make_tdc_ratexpset() {
        return make_tdc_ratexpset<b>();
    }


    template<typename RATEXPSET>
    auto
    make_ratexp(RATEXPSET& ratset, const std::string& exp) -> typename RATEXPSET::ratexp_t
    {
        return parse_exp(ratset, exp, true, false);
    }

    template<typename AUTOMATON>
    auto
    get_ratexpset(AUTOMATON& aut) -> ratexpset_of<context_t_of<AUTOMATON>>
    {
      using ratset_t = ratexpset_of<context_t_of<AUTOMATON>>;
      return ratset_t(get_rat_context(aut->context()),ratset_t::identities_t::trivial);
    }

   template <typename Automaton, unsigned version = version::fsm_json>
   std::ostream& js_print(Automaton aut,
			  std::ostream& o,
			  bool full = false,
			  json_ast_t extra_metadata = json_ast::empty()) {
     return put(aut_to_ast(aut, extra_metadata, full), o);
   }

    template <typename RatExpSet, unsigned version = version::fsm_json>
    std::ostream& js_print(const RatExpSet& rs,
			   const typename RatExpSet::ratexp_t& e,
			   std::ostream& o,
			   json_ast_t extra_metadata = json_ast::empty()) {
      return put(ratexp_to_ast(rs, e, extra_metadata), o);
   }
    
  }
}

#endif
