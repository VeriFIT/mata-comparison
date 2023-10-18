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

#include <awali/dyn/core/context_description.hh>
#include<unordered_map>

#include <awali/dyn/core/context_description/weightsets/weightsets.cc>

namespace awali {
  namespace dyn {
    namespace context {
      //To be register in abstract_weighset, each semiring requires
      //the instanciation of a static object


      labelset_description oneset() {
        labelset_description ls = make_labelset_description();
        ls->type_=CTypes::ONESET;
        return ls;
      }

      labelset_description letterset(std::string const& s) {
        labelset_description ls = make_labelset_description();
        ls->type_=CTypes::LETTERSET;
        for(auto c : s)
          ls->alphabet.emplace_back(std::string(1,c));
        return ls;
      }

      labelset_description intletterset(int a, int b) {
        labelset_description ls = make_labelset_description();
        ls->type_=CTypes::INTLETTERSET;
        for(int i=a; i<=b; ++i)
          ls->int_alphabet.emplace_back(i);
        return ls;
      }

      labelset_description intletterset(int n) {
        return intletterset(0, n-1);
      }

      labelset_description wordset(std::string const& s) {
        labelset_description ls = make_labelset_description();
        ls->type_=CTypes::WORDSET;
        for(auto c : s)
          ls->alphabet.emplace_back(std::string(1,c));
        return ls;
      }

      labelset_description nullableset(labelset_description ls1) {
        labelset_description ls = make_labelset_description();
        ls->type_=CTypes::NULLABLE;
        ls->children_.emplace_back(ls1);
        return ls;
      }

      labelset_description ltupleset(std::vector<labelset_description> lss) {
        labelset_description ls = make_labelset_description();
        ls->type_=CTypes::TUPLE;
        for(auto ls1 : lss)
          ls->children_.emplace_back(ls1);
        return ls;
      }

      weightset_description weightset(std::string const&k) {
        static std::unordered_map<std::string,weightset_description> memo;
        auto it = memo.find(k);
        if (it != memo.end())
          return it->second;

        weightset_description ws;
        for(auto wgt : instances()) {
          ws=wgt->fromstring(k);
          if(ws.use_count() >0)
            break;
        }
        if(ws.use_count()==0)
          throw std::invalid_argument("Unknown semiring: "+k);
        memo[k] = ws;
        return ws;
      }

      weightset_description wtupleset(std::vector<weightset_description> wss) {
        weightset_description ws = make_weightset_description();
        ws->type_=WTypes::TUPLE;
        for(auto ws1 : wss)
          ws->children_.emplace_back(ws1);
        return ws;
      }

      weightset_description ratweight(context_description cd) {
        weightset_description ws = make_weightset_description();
        ws->type_ = WTypes::RATEXP;
        ws->ct_ = cd;
        return ws;
      }

      context_description c_desc(labelset_description ls, weightset_description ws) {
        context_description cd = make_context_description();
        cd->ls_=ls;
        cd->ws_ =ws;
        return cd;
      }

      //************************


      //     context_description parse_context(std::istream& i);

      weightset_description parse_weightset(json::object_t const* jp) {
        weightset_description ws=make_weightset_description();
        if(jp->has_child("semiring")) {
          for(auto wgt : instances()) {
            ws=wgt->parse_weightset(jp);
            if(ws.use_count() >0)
              break;
          }
          if(ws.use_count()==0)
            throw std::runtime_error("json: Semiring");
          return ws;
        }
        else if(jp->has_child("rationalExpression")) {
          ws->type_ = WTypes::RATEXP;
          ws->ct_ = parse_context(jp->at("rationalExpression")->object());
          return ws;
        }
        else if(jp->has_child("tuple")) {
          ws->type_ = WTypes::TUPLE;
          for(json::node_t const* jv : *(jp->at("tuple")->array()))
            ws->children_.emplace_back(parse_weightset(jv->object()));
          return ws;
        }
        else
          throw std::runtime_error("json: Weights");
      }

      labelset_description parse_labelset(json::object_t const* jp) {
        labelset_description ls= make_labelset_description();
        if(!jp->has_child("labelKind"))
          throw std::runtime_error("json: labelKind not specified for labels");
        std::string ltype 
          = jp->at("labelKind")->to_string();
        if(ltype == "No Label") {
          ls->type_ = CTypes::ONESET;
          return ls;
        }
        if(ltype == "Multi-tape") {
          ls->type_ = CTypes::TUPLE;
          if(!jp->has_child("tapeLabels"))
            throw std::runtime_error("json: tapeLabel not specified for Multi-tape labels");
          for(json::node_t const* jv : *(jp->at("tapeLabels")->array()))
            ls->children_.emplace_back(parse_labelset(jv->object()));
          if(jp->has_child("allowEpsilon")) {
            bool b  = jp->at("allowEpsilon")->to_bool();
            if (b) {
              labelset_description ls2= make_labelset_description();
              ls2->type_ = CTypes::NULLABLE;
              ls2->children_.emplace_back(ls);
              return ls2;
            }
          }
          return ls;
        }
        if(ltype == "Words") {
          ls->type_ = CTypes::WORDSET;
          if(!jp->has_child("alphabet"))
            throw std::runtime_error("json: Alphabet not specified for Words labels");
          for(json::node_t const* jv : *(jp->at("alphabet")->array()))
            ls->alphabet.emplace_back(jv->to_string());
          return ls;
        }
        if(ltype == "Letters") {
          if(!jp->has_child("letterType"))
            throw std::runtime_error("json: letterType not specified for Letters labels");
          if(jp->at("letterType")->to_string()=="Char")
            ls->type_ = CTypes::LETTERSET;
          else if(jp->at("letterType")->to_string() == "Integer")
            ls->type_ = CTypes::INTLETTERSET;
          else
            throw std::runtime_error("json: wrong letterType");
        }
        else
          throw std::runtime_error("json: labels type");
        if(!jp->has_child("alphabet"))
          throw std::runtime_error("json: Alphabet not specified for labels");
        for(json::node_t const* jv : *(jp->at("alphabet")->array()))
          if( ls->type_ == CTypes::LETTERSET)
            ls->alphabet.emplace_back(jv->to_string());
          else
            ls->int_alphabet.emplace_back(jv->to_int());
        if(jp->has_child("allowEpsilon")) {
          if(jp->at("allowEpsilon")->to_bool()) {
            labelset_description ls2= make_labelset_description();
            ls2->type_ = CTypes::NULLABLE;
            ls2->children_.emplace_back(ls);
            return ls2;
          }
        }
        return ls;
      }
    
      context_description parse_context(json::object_t const* p) {
        context_description ct= make_context_description();
        ct->ls_=parse_labelset(p->at("labels")->object());
        ct->ws_=parse_weightset(p->at("weights")->object());
        return ct;
      }
      //     int parse_characteristic(std::istream& i) {
      //       std::string key = sttc::parsestring(i);
      //       if (key == "Characteristic") {
      //         sttc::check(i, ':');
      //         return std::stoi(sttc::parsestring(i));
      //       } else
      //         throw std::runtime_error("json: expected \"Characteristic\", got \""+key+"\"");
      //     }

      //     weightset_description parse_weightset(std::istream& i) {
      //       weightset_description ws=make_weightset_description();
      //       std::string aut = sttc::get_first_attr(i);
      //       if(aut == "Semiring") {
      //         for(auto wgt : instances()) {
      //           std::streampos position = i.tellg();
      //           ws=wgt->parse_weightset(i);
      //           if(ws.use_count() >0)
      //             break;
      //           i.seekg(position);
      //         }
      //         if(ws.use_count()==0)
      //           throw std::runtime_error("json: Semiring");
      //         sttc::check(i,'}');
      //         return ws;
      //       }
      //       else if(aut=="Rational Expression") {
      //         ws->type_ = WTypes::RATEXP;
      //         ws->ct_ = parse_context(i);
      //         sttc::check(i,'}');
      //         return ws;
      //       }
      //       else if(aut == "Tuple") {
      //         ws->type_ = WTypes::TUPLE;
      //         sttc::check(i, '[');
      //         char c;
      //         std::vector<weightset_description> vecls;
      //         do {
      //           ws->children_.emplace_back(parse_weightset(i));
      //           i >> c;
      //         } while(c==',');
      //         if(c!=']')
      //           throw std::runtime_error("json: Elements ]");
      //         return ws;
      //       }
      //       else
      //         throw std::runtime_error("json: Weights");
      //     }
      // 
      //     labelset_description parse_labelset(std::istream& i) {
      //       labelset_description ls= make_labelset_description();
      //       std::string aut = sttc::get_first_attr(i);
      //       if(aut != "Label Type") {
      //         if(aut == "Tuple") {
      //           ls->type_ = CTypes::TUPLE;
      //           sttc::check(i, '[');
      //           char c;
      //           std::vector<labelset_description> vecls;
      //           do {
      //             ls->children_.emplace_back(parse_labelset(i));
      //             i >> c;
      //           } while(c==',');
      //           if(c!=']')
      //             throw std::runtime_error("json: Elements ]");
      //           if(sttc::peek(i)!='}') {
      //             std::string al = sttc::parsestring(i);
      //             if(al=="Epsilon transitions") {
      //               sttc::check(i,':');
      //               if(sttc::parsecst(i)=='t') {
      //                 labelset_description ls2= make_labelset_description();
      //                 ls2->type_ = CTypes::NULLABLE;
      //                 ls2->children_.emplace_back(ls);
      //                 return ls2;
      //               }
      //             }
      //           }
      //         }
      //         return ls;
      //       }
      //       std::string ltype = sttc::parsestring(i);
      //       if(ltype == "No Label") {
      //         ls->type_ = CTypes::ONESET;
      //         sttc::check(i, '}');
      //         return ls;
      //       }
      //       if(ltype == "Words") {
      //         ls->type_ = CTypes::WORDSET;
      //         std::string al = sttc::parsestring(i);
      //         if(al!="Alphabet")
      //           throw std::runtime_error("json: Alphabet");
      //         sttc::check(i, ':');
      //         sttc::check(i, '[');
      //         char c;
      //         do {
      //           al = sttc::parsestring(i);
      //           ls->alphabet.emplace_back(al);
      //           i >> c;
      //         } while(c==',');
      //         if(c!=']')
      //           throw std::runtime_error("json: Alphabet ]");
      //         sttc::check(i, '}');
      //         return ls;
      //       }
      //       if(ltype == "Letters" || ltype == "Int Letters") {
      //         bool eps=false;
      //         if(ltype == "Letters")
      //           ls->type_ = CTypes::LETTERSET;
      //         else
      //           ls->type_ = CTypes::INTLETTERSET;
      //         std::string al = sttc::parsestring(i);
      //         if(al=="Epsilon transitions") {
      //           sttc::check(i,':');
      //           if(sttc::parsecst(i)=='t')
      //             eps=true;
      //           al = sttc::parsestring(i);
      //         }
      //         if(al!="Alphabet")
      //           throw std::runtime_error("json: Alphabet");
      //         sttc::check(i, ':');
      //         sttc::check(i, '[');
      //         char c;
      //         do {
      //           al = sttc::parsestring(i);
      //           ls->alphabet.emplace_back(al);
      //           i >> c;
      //         } while(c==',');
      //         if(c!=']')
      //           throw std::runtime_error("json: Alphabet ]");
      //         sttc::check(i, '}');
      //         if(eps) {
      //           labelset_description ls2= make_labelset_description();
      //           ls2->type_ = CTypes::NULLABLE;
      //           ls2->children_.emplace_back(ls);
      //           return ls2;
      //         }
      //         return ls;
      //       }
      //       throw std::runtime_error("json: Labels");
      //     }
      // 
      //     labelset_description parse_labels(std::istream& i) {
      //       std::string aut = sttc::get_first_attr(i);
      //       if(aut != "Labels")
      //         throw std::runtime_error("json: Labels");
      //       labelset_description ls=parse_labelset(i);
      //       sttc::check(i, '}');
      //       return ls;
      //     }
      // 
      //     weightset_description parse_weights(std::istream& i) {
      //       std::string aut = sttc::get_first_attr(i);
      //       if(aut != "Weights")
      //         throw std::runtime_error("json: Weights");
      //       weightset_description ws=parse_weightset(i);
      //       sttc::check(i, '}');
      //       return ws;
      //     }
      // 
      //     context_description parse_context(std::istream& i) {
      //       std::string aut = sttc::get_first_attr(i);
      //       if(aut != "Context")
      //         throw std::runtime_error("json: Context");
      //       context_description ct= make_context_description();
      //       sttc::check(i, '[');
      //       ct->ls_=parse_labels(i);
      //       sttc::check(i, ',');
      //       ct->ws_=parse_weights(i);
      //       sttc::check(i, ']');
      //       sttc::check(i, '}');
      //       return ct;
      //     }



      //******************

      std::string tostring(labelset_description ls, bool dynamic){
        std::stringstream res;
        switch(ls->type_){
        case CTypes::ONESET: return "lao";
        case CTypes::LETTERSET:
          res << "lal_char";
          if(dynamic) {
            res << "(";
            for(auto l: ls-> alphabet)
              res << l;
            res << ")";
          }
          return res.str();
        case CTypes::INTLETTERSET:
          res << "lal_int";
          if(dynamic) {
            res << "(";
            for(auto l: ls-> int_alphabet)
              res << l;
            res << ")";
          }
          return res.str();
        case CTypes::WORDSET:
          res << "law_char";
          if(dynamic) {
            res << "(";
            for(auto l: ls-> alphabet)
              res << l;
            res << ")";
          }
          return res.str();
        case CTypes::NULLABLE: return "lan<"+tostring(ls->children_.front(), dynamic)+">";
        case CTypes::TUPLE:
          {
            res << "lat";
            char s='<';
            for(auto l:ls->children_) {
              res << s << tostring(l, dynamic);
              s=',';
            }
            res << '>';
            return res.str();
          }
        default: break;
        }
        return "";
      }

      std::string tostring(context_description ct, bool dynamic);

      std::string tostring(weightset_description ws, bool dynamic){
        std::string r="tuple";
        std::string sep="<";
        if(ws->type_ >= 0)
          return instances()[ws->type_]->tostring(ws, dynamic);
        switch(ws->type_){
        case WTypes::RATEXP: return "ratexpset<"+tostring(ws->ct_,dynamic)+">";
        case WTypes::SERIES: return "series<"+tostring(ws->ct_, dynamic)+">";
        case WTypes::TUPLE:
          for(auto wsc : ws->children_) {
            r+=sep+tostring(wsc, dynamic);
            sep=",";
          }
          return r+">";
        default: break;
        }
        return "";
      }

      std::string tostring(context_description ct, bool dynamic) {
        return tostring(ct->ls_, dynamic)+"_"+tostring(ct->ws_, dynamic);
      }

      std::vector<std::string> all_weightset_public_static_names() 
      {
            std::vector<std::string> result;
        for (auto ws : instances())
          result.push_back(std::string(ws->static_public_name()));
        return result;
      }
 
      std::vector<std::string> all_weightset_public_descriptions() 
      {
            std::vector<std::string> result;
        for (auto ws : instances())
          result.push_back(std::string(ws->static_desc()));
        return result;
      }
 
std::string all_weightset_public_static_names_as_string() 
{
        std::stringstream ss;
        ss << "[";
        bool b = false;
        for (auto ws : instances()) {
          if (b) 
            ss << ", ";
          else
            b= true;
          ss << ws->static_public_name();
        }
        ss << "]";
        return ss.str();
      }

      bool is_promotable(weightset_description src, weightset_description dest){
        if(src->type_<0)
          return false;
        return instances()[src->type_]->is_promotable_to(dest);
      }
      
      std::vector<std::string> const&  
      weightset_promotion_public_static_names(weightset_description ws)
      {
        if (ws->type_<0)
          throw std::runtime_error("No such weightset type:"+std::to_string(ws->type_));
        else
          return instances()[ws->type_]->possible_promotions();
      }
    }
  }
} //end of namespaces awali::dyn::context, awali::dyn, and awali

