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

#ifndef AWALI_ALGOS_JS_PRINT_EXP_HH
# define AWALI_ALGOS_JS_PRINT_EXP_HH

#include <awali/common/version.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/history/history.hh>
#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/common/json/utils.hh>
#include <awali/common/json_ast.hh>
#include <awali/common/tuple.hh>
# include <stack>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <sstream>

namespace awali { namespace sttc {


  /*---------------------.
  | Json export(ratexp).  |
  `---------------------*/

//   namespace rat
//   {
//     template <typename RatExpSet>
//     class js_print_visitor
//       : public RatExpSet::const_visitor
//     {
//     public:
//       using ratexpset_t = RatExpSet;
//       using context_t = context_t_of<ratexpset_t>;
//       using labelset_t = labelset_t_of<context_t>;
//       using ratexp_t = typename ratexpset_t::value_t;
//       using weightset_t = weightset_t_of<ratexpset_t>;
//       using super_type = typename ratexpset_t::const_visitor;
//       using node_t = typename super_type::node_t;
// 
//      constexpr static const char* me() { return "js-print-exp"; }
// 
//       js_print_visitor(const ratexpset_t& rs, std::ostream& o)
//         : rs_(rs), stream_(o)
//       {}
// 
//       std::ostream& operator()(const ratexp_t& v)
//       {
//         v->accept(*this);
//         return stream_;
//       }
// 
//       AWALI_RAT_UNSUPPORTED(ldiv)
//       AWALI_RAT_UNSUPPORTED(transposition)
// 
//       AWALI_RAT_VISIT(zero,)
//       {
//         stream_ << "\"zero\":null" ;
//       }
// 
//       AWALI_RAT_VISIT(one,)
//       {
//        stream_ << "\"one\":null";
//       }
// 
//       AWALI_RAT_VISIT(atom, e)
//       {
//         stream_ << "\"label\":" ;
//         ls_.js_print(stream_, e.value());
//       }
// 
//       AWALI_RAT_VISIT(sum, e)
//       {
//         stream_ << "\"sum\":[";
//         for (unsigned i = 0, n = e.size(); i < n; ++i) {
//           const auto& v = e[i];
//           stream_ << '{';
//           v->accept(*this);
//           stream_ << ((i<n-1)?"},":"}");
//         }
//         stream_ << ']';
//       }
// 
//       AWALI_RAT_VISIT(prod, e)
//       {
//         stream_ << "\"prod\":[";
//         for (unsigned i = 0, n = e.size(); i < n; ++i) {
//           const auto& v = e[i];
//           stream_ << '{';
//           v->accept(*this);
//           stream_ << ((i<n-1)?"},":"}");
//         }
//         stream_ << ']';
//       }
// 
//       AWALI_RAT_VISIT(conjunction, e)
//       {
//         stream_ << "\"conjunction\":[";
//         for (unsigned i = 0, n = e.size(); i < n; ++i) {
//           const auto& v = e[i];
//           stream_ << '{';
//           v->accept(*this);
//           stream_ << ((i<n-1)?"},":"}");
//         }
//         stream_ << ']';
//      }
// 
//       AWALI_RAT_VISIT(shuffle, e)
//       {
//         stream_ << "\"shuffle\":[";
//         for (unsigned i = 0, n = e.size(); i < n; ++i) {
//           const auto& v = e[i];
//           stream_ << '{';
//           v->accept(*this);
//           stream_ << ((i<n-1)?"},":"}");
//         }
//         stream_ << ']';
//       }
// 
//       AWALI_RAT_VISIT(complement, e)
//       {
//         stream_ << "\"node\":\"Complement\", \"value\":";
//         stream_ << '{';
//        e.sub()->accept(*this);
//         stream_ << '}';
//       }
// 
//       AWALI_RAT_VISIT(star, e)
//       {
//         stream_ << "\"star\":";
//         stream_ << '{';
//        e.sub()->accept(*this);
//         stream_ << '}';
//       }
// 
//       AWALI_RAT_VISIT(lweight, e)
//       {
//         stream_ << "\"lweight\":\"";
//         ws_.print(e.weight(), stream_);
//         stream_ << "\", ";
//         e.sub()->accept(*this);
//       }
// 
//       AWALI_RAT_VISIT(rweight, e)
//       {
//         stream_ << "\"rweight\":\"";
//         ws_.print(e.weight(), stream_);
//         stream_ << "\", ";
//         e.sub()->accept(*this);
//      }
// 
//     private:
//       ratexpset_t rs_;
//       std::ostream& stream_;
//       /// Shorthand to the weightset.
//       weightset_t ws_ = *rs_.weightset();
//       labelset_t ls_ = *rs_.labelset();
//     };
// 
//   } // rat::


  template <unsigned version = version::fsm_json>
  inline
  json::object_t*
  json_timestamp()
  {
  /* timestamp */
    json::object_t* timestamp = new json::object_t();
    time_t tt;
    std::time(&tt);
    auto gmt = std::gmtime(&tt);
    {
    /* date */
      std::stringstream ss;
      ss << std::put_time(gmt,"%F");
      timestamp->push_back("date", new json::string_t(ss.str()));
    }{
    /* time */
      std::stringstream ss;
      ss << std::put_time(gmt, "%TZ");
      timestamp->push_back("time", new json::string_t(ss.str()));
    }
    return timestamp;
  }


  template <unsigned version = version::fsm_json>
  inline
  json::object_t*
  json_creator()
  {
    json::object_t* creator = new json::object_t();
    creator->push_back("programName", new json::string_t("awali"));
    creator->push_back("version", new json::string_t(awali::version::full));
    return creator;
  }
  
  
  template <unsigned version = version::fsm_json>
  inline
  json::object_t*
  json_format()
  {
    json::object_t* format = new json::object_t();
    format->push_back("name", new json::string_t("fsm-json"));
    format->push_back("version", new json::string_t(std::to_string(version)));
    return format;
  }


  template <typename Automaton, unsigned version = version::fsm_json>
  inline
  json_ast_t
  aut_to_ast(Automaton aut,
             json_ast_t extra_metadata = json_ast::empty(),
             bool full = false)
  {
    auto ctx = aut->context();
    auto ws = ctx.weightset();
    auto ls = ctx.labelset();
    json::object_t* root = new json::object_t();

    {
    /* == format == */
      root->push_back("format", json_format<version>());
      root->push_back("kind", new json::string_t("Automaton"));
    }{
    /* == metadata == */
      json::object_t* metadata = new json::object_t();
      /* == name == */
      if(!aut->get_name().empty())
        metadata->push_back("name", new json::string_t(aut->get_name()));
      /* == caption == */
      if(!aut->get_desc().empty())
        metadata->push_back("caption", new json::string_t(aut->get_desc()));
      /* == creator == */
      metadata->push_back("creator", json_creator<version>());
      /* == timestamp == */
      metadata->push_back("timestamp", json_timestamp<version>());
      /* == user-defined metadata == */
      for(auto p : extra_metadata->fields)
        metadata->push_back(p.first, p.second->copy());
      root->push_back("metadata", metadata);
    }{
    /* == context == */
      json::object_t* context = ctx.template to_json<version>()->object();
      root->push_back("context",context);
    }{
    /* == data == */
      json::object_t* data = new json::object_t();
      {
      /* == states == */
        json::array_t* states = new json::array_t();
        for(unsigned i: aut->states()) {
          json::object_t* one_state = new json::object_t();
          one_state->push_back("id", new json::int_t(full?i:i-2));
          if (aut->has_explicit_name(i))
            one_state->push_back("name",
                                 new json::string_t(aut->get_state_name(i)));
          if (aut->is_initial(i))
            one_state->push_back(
              "initial",
               ws->template value_to_json<version>(aut->get_initial_weight(i))
            );
          if (aut->is_final(i))
            one_state->push_back(
                "final",
                ws->template value_to_json<version>(aut->get_final_weight(i)));
          states->push_back(one_state);
        }
        data->push_back("states", states);
      }{
      /* == transitions == */
        json::array_t* transitions = new json::array_t();
        for(unsigned i: aut->transitions()) {
          json::object_t* one_transition = new json::object_t();
          {
          /* == source == */
            unsigned src = aut->src_of(i);
            one_transition->push_back("source",
                                      new json::int_t(full ? src : src-2));
          }{
          /* == destination == */
            unsigned dst = aut->dst_of(i);
            one_transition->push_back("destination",
                                      new json::int_t(full ? dst : dst-2));
          }{
          /* == label == */
            one_transition->push_back(
              "label",
              ls->template value_to_json<version>(aut->label_of(i)));
          }{
          /* == weight == */
            if(!ws->is_one(aut->weight_of(i)) || ws->show_one()) {
              one_transition->push_back(
                "weight",
                ws->template value_to_json<version>(aut->weight_of(i)) );
            }
          }
          transitions->push_back(one_transition);
        }
        data->push_back("transitions", transitions);
      }
      root->push_back("data",data);
    }
    return std::shared_ptr<json::object_t>(root);
  }


  template <typename RatExpSet, unsigned version = version::fsm_json>
  inline
  json_ast_t
  ratexp_to_ast(const RatExpSet& rs,
                const typename RatExpSet::ratexp_t& e,
                json_ast_t extra_metadata = json_ast::empty())
  {
    json::object_t* root = new json::object_t();
    {
     /* == format == */
      root->push_back("format", json_format<version>());
      root->push_back("kind", new json::string_t("Rational Expression"));
    }{
    /* == metadata == */
      json::object_t* metadata = new json::object_t();
      /* == creator == */
      metadata->push_back("creator", json_creator<version>());
      /* == timestamp == */
      metadata->push_back("timestamp", json_timestamp<version>());
      /* == user-defined metadata == */
      for(auto p : extra_metadata->fields)
        metadata->push_back(p.first, p.second->copy());
      root->push_back("metadata", metadata);
    }{
    /* == context == */
      root->push_back("context",rs.context().template to_json<version>());
    }{
    /* == data == */
      root->push_back("data",rs.value_to_json(e));
    }
    return std::shared_ptr<json::object_t>(root);
  }


}}//end of ns awali::stc

#endif // !AWALI_ALGOS_JS_PRINT_EXP_HH
