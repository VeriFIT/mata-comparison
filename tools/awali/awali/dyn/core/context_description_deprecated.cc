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

#include <awali/dyn/core/context_description_deprecated.hh>
#include <awali/common/parse_exception.hh>

namespace awali {
  namespace dyn {
    namespace deprecated {
            
      context_description parse_context(std::istream& i);
      
      int parse_characteristic(std::istream& i) {
        std::string key = awali::internal::parsestring(i);
        if (key == "Characteristic") {
          awali::internal::check(i, ':');
          return std::stoi(awali::internal::parsestring(i));
        } else
          throw parse_exception(i,("json: expected \"Characteristic\", got \""+key+"\"").c_str());
      }
      
      weightset_description parse_weightset(std::istream& i) {
        weightset_description ws=context::make_weightset_description();
        std::string aut = awali::internal::get_first_attr(i);
        if(aut == "Semiring") {
          std::string val=awali::internal::parsestring(i);
          for(auto wgt : context::instances()) {
            if(val==wgt->static_public_name()) {
              ws->type_ = wgt->getIndex();
              break;
            }
          }
          if(ws.use_count()==0)
            throw parse_exception(i,"json: Semiring");
          awali::internal::check(i,'}');
          return ws;
        }
        else if(aut=="Rational Expression") {
          ws->type_ = context::WTypes::RATEXP;
          ws->ct_ = parse_context(i);
          awali::internal::check(i,'}');
          return ws;
        }
        else if(aut == "Tuple") {
          ws->type_ = context::WTypes::TUPLE;
          awali::internal::check(i, '[');
          char c;
          std::vector<weightset_description> vecls;
          do {
            ws->children_.emplace_back(parse_weightset(i));
            i >> c;
          } while(c==',');
          if(c!=']')
            throw parse_exception(i,"json: Elements ]");
          return ws;
        }
        else
          throw parse_exception(i,"json: Weights");
        return ws;
      }
      
      labelset_description parse_labelset(std::istream& i) {
        labelset_description ls= context::make_labelset_description();
        std::string aut = awali::internal::get_first_attr(i);
        if(aut != "Label Type") {
          if(aut == "Tuple") {
            ls->type_ = context::CTypes::TUPLE;
            awali::internal::check(i, '[');
            char c;
            std::vector<labelset_description> vecls;
            do {
              ls->children_.emplace_back(parse_labelset(i));
              i >> c;
            } while(c==',');
            if(c!=']')
              throw parse_exception(i,"json: Elements ]");
            if(awali::internal::peek(i)!='}') {
              std::string al = awali::internal::parsestring(i);
              if(al=="Epsilon transitions") {
                awali::internal::check(i,':');
                if(awali::internal::parsecst(i)=='t') {
                  labelset_description ls2= context::make_labelset_description();
                  ls2->type_ = context::CTypes::NULLABLE;
                  ls2->children_.emplace_back(ls);
                  return ls2;
                }
              }
            }
          }
          return ls;
        }
        std::string ltype = awali::internal::parsestring(i);
        if(ltype == "No Label") {
          ls->type_ = context::CTypes::ONESET;
          awali::internal::check(i, '}');
          return ls;
        }
        if(ltype == "Words") {
          ls->type_ = context::CTypes::WORDSET;
          std::string al = awali::internal::parsestring(i);
          if(al!="Alphabet")
            throw parse_exception(i,"json: Alphabet");
          awali::internal::check(i, ':');
          awali::internal::check(i, '[');
          char c;
          do {
            al = awali::internal::parsestring(i);
            ls->alphabet.emplace_back(al);
            i >> c;
          } while(c==',');
          if(c!=']')
            throw parse_exception(i,"json: Alphabet ]");
          awali::internal::check(i, '}');
          return ls;
        }
        if(ltype == "Letters" || ltype == "Int Letters") {
          bool eps=false;
          if(ltype == "Letters")
            ls->type_ = context::CTypes::LETTERSET;
          else
            ls->type_ = context::CTypes::INTLETTERSET;
          std::string al = awali::internal::parsestring(i);
          if(al=="Epsilon transitions") {
            awali::internal::check(i,':');
            if(awali::internal::parsecst(i)=='t')
              eps=true;
            al = awali::internal::parsestring(i);
          }
          if(al!="Alphabet")
            throw parse_exception(i,"json: Alphabet");
          awali::internal::check(i, ':');
          awali::internal::check(i, '[');
          char c;
          do {
            al = awali::internal::parsestring(i);
            ls->alphabet.emplace_back(al);
            i >> c;
          } while(c==',');
          if(c!=']')
            throw parse_exception(i,"json: Alphabet ]");
          awali::internal::check(i, '}');
          if(eps) {
            labelset_description ls2= context::make_labelset_description();
            ls2->type_ = context::CTypes::NULLABLE;
            ls2->children_.emplace_back(ls);
            return ls2;
          }
          return ls;
        }
        throw parse_exception(i,"json: Labels");
        return ls;
      }
      
      labelset_description parse_labels(std::istream& i) {
        std::string aut = awali::internal::get_first_attr(i);
        if(aut != "Labels")
          throw parse_exception(i,"json: Labels");
        labelset_description ls=parse_labelset(i);
        awali::internal::check(i, '}');
        return ls;
      }
      
      weightset_description parse_weights(std::istream& i) {
        std::string aut = awali::internal::get_first_attr(i);
        if(aut != "Weights")
          throw parse_exception(i,"json: Weights");
        weightset_description ws=parse_weightset(i);
        awali::internal::check(i, '}');
        return ws;
      }
      
      context_description parse_context(std::istream& i) {
        std::string aut = awali::internal::get_first_attr(i);
        if(aut != "Context")
          throw parse_exception(i,"json: Context");
        context_description ct= context::make_context_description();
        awali::internal::check(i, '[');
        ct->ls_=parse_labels(i);
        awali::internal::check(i, ',');
        ct->ws_=parse_weights(i);
        awali::internal::check(i, ']');
        awali::internal::check(i, '}');
        return ct;
      }
      
    }//end of ns awali::dyn::old
  }//end of ns awali::dyn
} //end of ns awali

