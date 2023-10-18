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

#ifndef DYN_MODULES_CONTEXT_CC
#define DYN_MODULES_CONTEXT_CC

#include <set>

#include <awali/dyn/algos/sys.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/modules/automaton.hh>
#include <awali/dyn/modules/context.hh>

namespace awali {
namespace dyn {


void 
internal::rename_from_origin_inplace (automaton_t dst, automaton_t orig) 
{
  for (state_t s : dst->states()) {
    state_t orig_state = dst->origin_of(s);
    if (orig->has_state(orig_state)) {
      dst->set_state_name(s,orig->get_state_name(orig_state));
    }
  }
}


automaton_t 
internal::make_automaton_from_context(context_t ctx)
{
  return loading::call0<automaton_t>("make_automaton_from_context", 
                                     "context", ctx->sname(), ctx);
}



automaton_t 
internal::make_automaton_from_context(context::context_description cd)
{
  std::string stat_ctx = context::tostring(cd, false);
  return loading::call0<automaton_t>("make_automaton_from_desc", "context",
                                     stat_ctx, cd);
}


automaton_t 
internal::make_automaton_from_context(context::labelset_description ls,
                                      context::weightset_description ws)
{
  return make_automaton_from_context(c_desc(ls, ws));
}


context_t internal::make_context(context::labelset_description ls,
                                 context::weightset_description ws)
{
  return make_context(c_desc(ls,ws));
}



context_t 
internal::make_context(context::context_description cd)
{
  return loading::call0<context_t>("make_context_from_desc", "context",
                                   tostring(cd, false), cd);
}

//bool 
//internal::is_promotion(std::string const& src_semiring, 
//                       std::string const& semiring) {
//  return (
//          src_semiring == "B"
//          || (src_semiring == "Z" && (semiring == "Q" ||
//                                      semiring == "R" ||
//                                      semiring == "C"))
//          || (src_semiring == "Q" && (semiring == "R" ||
//                                      semiring == "C"))
//          || (src_semiring == "R" && semiring == "C")
//          );
//}
//
//
//context_t 
//internal::make_context_with_another_semiring(context_t ctx, 
//                                             std::string conststd::string const& semiring) 
//{
//  std::string src_semiring = ctx->weightset_name();
//  std::string stat_ctx1 = ctx->sname();
//  std::string stat_ctx2 
//    = ctx->labelset_name() + "_"
//      + context::tostring(context::weightset(semiring),false);
//  typedef context_t (*bridge_t)(context_t);
//  bridge_t bridge 
//    = (bridge_t) loading::get_handler("promote_ctx", "promotion", 
//                                      stat_ctx1, stat_ctx2);
//  return bridge(ctx);
//}
//
//
//automaton_t 
//internal::promote_automaton(automaton_t aut, std::string const& semiring, 
//                            options_t opts) 
//{
//  bool keep_history = opts[KEEP_HISTORY];
//  context_t ctx=aut->get_context();
//  std::string src_semiring = ctx->weightset_name();
//  if(src_semiring=="B")
//    return characteristic(aut, semiring, opts);
//  if(src_semiring==semiring)
//    return copy(aut);
//  if(!is_promotion(src_semiring, semiring))
//    throw std::runtime_error("no promotion from "+src_semiring+" to "+semiring);
//  std::string stat_ctx1 = ctx->sname();
//  std::string stat_ctx2 
//    = ctx->labelset_name() + "_" 
//      + context::tostring(context::weightset(semiring),false);
//  typedef automaton_t (*bridge_t)(automaton_t,bool);
//  bridge_t bridge 
//    = (bridge_t) loading::get_handler("promote_aut", "promotion", 
//                                      stat_ctx1, stat_ctx2);
//  return bridge(aut, keep_history);
//}


automaton_t 
internal::parse_automaton(json_ast_t p)
{
  if(!p->has_child("kind") || p->at("kind")->to_string() != "Automaton")
    throw std::runtime_error("json: Automaton");
  if(!p->has_child("context"))
    throw std::runtime_error("No context in Automaton description");
  dyn::context::context_description ct
    = dyn::context::parse_context(p->at("context")->object());
  std::string stat_ctx = tostring(ct, false);
  typedef automaton_t (*bridge_t)(json::object_t*, 
                                  dyn::context::context_description);
  static std::unordered_map<std::string, void*> bridges;
  auto it = bridges.find(stat_ctx);
  automaton_t res;
  if(it == bridges.end()) {
    auto bridge = (bridge_t) loading::get_handler("parse_automaton", "context",
                                                  stat_ctx);
    bridges.emplace(stat_ctx, (void*) bridge);
    res= bridge(&(*p), ct);
  }
  else
    res = ((bridge_t) it->second)(&(*p), ct);
  return res;
}


automaton_t 
internal::parse_automaton(std::istream& i)
{
  return internal::parse_automaton(json_ast::from(i));
}


automaton_t 
internal::deprecated_parse_automaton(std::istream& i)
{
  std::string aut = awali::internal::get_first_attr(i);
  while(aut != "Automaton") {
    awali::internal::parsestring(i);
    aut=awali::internal::parsestring(i);
    awali::internal::check(i, ':');
  }
  awali::internal::check(i, '[');
  context::context_description ct= dyn::deprecated::parse_context(i);
  awali::internal::check(i, ',');
  std::string stat_ctx = tostring(ct,false);
  typedef automaton_t (*bridge_t)(std::istream&, context::context_description);
  static std::unordered_map<std::string, bridge_t> bridges;
  auto it = bridges.find(stat_ctx);
  automaton_t res;
  if(it == bridges.end()) {
    auto bridge = (bridge_t) loading::get_handler("parse_automaton_deprecated",
                                                  "context", stat_ctx);
    bridges.emplace(stat_ctx, bridge);
    res= bridge(i, ct);
  }
  else
    res = it->second(i, ct);
  awali::internal::check(i,']');
  awali::internal::check(i,'}');
  return res;
}


ratexp_t 
internal::make_ratexp_with_context(std::string const& exp, context_t ctx, 
                                   bool fixed_alphabet)
{
 return loading::call0<ratexp_t, const char*, context_t, bool>(
    "make_ratexp_with_context", 
    "context",
    ctx->sname(),
    exp.c_str(), ctx, fixed_alphabet);
}



ratexp_t 
internal::make_ratexp_with_context(std::string const& exp,
                                   context::context_description cd, 
                                   bool fixed_alphabet)
{
  std::string stat_ctx = tostring(cd, false);
  typedef ratexp_t (*bridge_t)(const char*, context::context_description, bool);
  static std::unordered_map<std::string, void*> bridges;
  auto it = bridges.find(stat_ctx);
  if(it == bridges.end()) {
    auto bridge = (bridge_t) loading::get_handler("make_ratexp_with_desc",
                  "context", stat_ctx);
    bridges.emplace(stat_ctx, (void*) bridge);
    return bridge(exp.c_str(), cd, fixed_alphabet);
  }
  else
    return ((bridge_t) it->second)(exp.c_str(), cd, fixed_alphabet);
}


ratexp_t 
internal::make_ratexp_with_context(std::string const& exp,
                                   context::labelset_description ls, 
                                   context::weightset_description ws,
                                   bool fixed_alphabet)
{
  return make_ratexp_with_context(exp, c_desc(ls, ws), fixed_alphabet);
}



ratexp_t 
internal::make_ratexp_with_alphabet(std::string const& exp,
                                    std::string const& alph, 
                                    std::string const& sr, 
                                    bool fixed_alphabet)
{
  return make_ratexp_with_context(exp, context::letterset(alph),
                                  context::weightset(sr), fixed_alphabet);
}


ratexp_t 
internal::make_ratexp(std::string const& exp, std::string const& sr)
{
  return make_ratexp_with_alphabet(exp, "", sr, false);
}
// 
//     ratexp_t make_int_ratexp_with_range(std::string const& exp, int a, int b,
//       return make_ratexp_with_context(exp, c_desc(ls, ws), fixed_alphabet);
//     }

//     ratexp_t make_ratexp_with_alphabet(std::string const& exp,
//                                        std::string const& alph, std::string const& sr, bool fixed_alphabet)
//     {
//       return make_ratexp_with_context(exp, context::letterset(alph),
//                                       context::weightset(sr), fixed_alphabet);
//     }
// 
//     ratexp_t make_ratexp(std::string const& exp, std::string const& sr)
//     {
//       return make_ratexp_with_alphabet(exp, "", sr, false);
//     }
// 
//     ratexp_t make_int_ratexp_with_range(std::string const& exp, int a, int b,
//                                         std::string const& sr, bool fixed_alphabet)
//     {
//       return make_ratexp_with_context(exp, context::intletterset(a, b),
//                                       context::weightset(sr), fixed_alphabet);
//     }
// 
//     ratexp_t make_int_ratexp(std::string const& exp, std::string const& sr)
//     {
//       return make_int_ratexp_with_range(exp, 0, -1, sr, false);
//     }
// 
//     ratexp_t make_ratexp_with_alphabets(std::string const& exp,
//                                         std::vector<std::string> alphabets, std::string semiring, bool fixed_alphabet)
//     {
//       std::vector<context::labelset_description> v;
//       for(auto al : alphabets)
//         v.emplace_back(context::nullableset(context::letterset(al)));
//       return make_ratexp_with_context(exp, context::ltupleset(v),
//                                       context::weightset(semiring), fixed_alphabet);
//     }
// 
//     ratexp_t make_tuple_ratexp(std::string const& exp, unsigned n,
//                                std::string semiring)
//     {
//       std::vector<context::labelset_description> v(n,
//           context::nullableset(context::letterset("")));
//       return make_ratexp_with_context(exp, context::ltupleset(v),
//                                       context::weightset(semiring), false);
//     }


std::vector<state_t>
internal::states(abstract_automaton_t const* aut, 
                 bool all) 
{
  return 
    loading::call1<std::vector<state_t>, abstract_automaton_t const*,bool>
      ("states","context",aut, all);
}


std::vector<transition_t> 
internal::transitions(abstract_automaton_t const* aut, bool all)
{
  return 
    loading::call1<std::vector<state_t>,
      abstract_automaton_t const*,bool>("transitions","context",aut, all);
}
 

std::vector<transition_t> 
internal::incoming(abstract_automaton_t const* aut, state_t s, bool all)
{
  return 
    loading::call1<std::vector<state_t>,
      abstract_automaton_t const*,state_t, bool>("incoming","context",aut, s, all);
}
  

std::vector<transition_t> 
internal::outgoing(abstract_automaton_t const* aut, state_t s, bool all)
{
  return loading::call1< std::vector<state_t> ,
                         abstract_automaton_t const*,
                         state_t, 
                         bool                        >("outgoing",
                                                       "context",
                                                       aut, 
                                                       s, 
                                                       all);
}
 
  

ratexp_t 
internal::parse_ratexp(json_ast_t p) 
{
  if(!p->has_child("kind"))
    throw std::runtime_error("json: File has no \"kind\" field.");
  if (p->at("kind")->to_string() != "Rational Expression")
    throw std::runtime_error("json: File has wrong \"kind\", expected "
                             "\"Rational Expression.\"");
  dyn::context::context_description ct= dyn::context::parse_context(
      p->at("context")->object());
  std::string stat_ctx = tostring(ct, false);
  typedef ratexp_t (*bridge_t)(context::context_description, 
                               json::object_t*);
  static std::unordered_map<std::string, void*> bridges;
  auto it = bridges.find(stat_ctx);
  ratexp_t res;
  if(it == bridges.end()) {
    auto bridge = (bridge_t) loading::get_handler("parse_ratexp", "context",
                  stat_ctx);
    bridges.emplace(stat_ctx, (void*) bridge);
    res= bridge(ct, p->at("data")->object());
  }
  else
    res = ((bridge_t)it->second)(ct, p->at("data")->object());
  return res;
}


ratexp_t 
parse_ratexp(std::istream& i)
{
  return internal::parse_ratexp(json_ast::from(i));
}

}// end of namespace awali::dyn
}// end of namespace awali

#endif
