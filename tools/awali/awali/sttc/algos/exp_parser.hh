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

#ifndef AWALI_EXP_PARSER_HH
#define AWALI_EXP_PARSER_HH
namespace awali {
  namespace sttc {
    namespace internal {

      template<typename RatExpSet>
      struct exp_parser;
    }
  }
}
#include <awali/sttc/core/rat/ratexpset.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/labelset/tupleset.hh>
#include <awali/sttc/alphabets/pair.hh>
#include <awali/common/parse_exception.cc>
#include<string>
#include<list>

   /*

  E -> P | E+P
  P -> S | PS | P.S
  S -> L | S* | S{exp} | S?
  L -> R | <weight>R
  R -> A | A<weight>
  A -> label | (E) | [labelinterval]

     */

namespace awali { namespace sttc {

  namespace internal {

    template<typename RatExpSet>
    struct exp_parser {
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using labelset_t = labelset_t_of<context_t>;
      using ratexp_t = typename ratexpset_t::value_t;
      using weightset_t = weightset_t_of<ratexpset_t>;

      exp_parser(const ratexpset_t& rs, const std::string& s, bool strict_alphabet)
        : p_(s.length()), rs_(rs), s_(s), strict_(strict_alphabet)
      {}

      exp_parser(const ratexpset_t& rs, const std::string& s, size_t& p, bool strict_alphabet)
        : rs_(rs), s_(s), p_(p), strict_(strict_alphabet)
      {}

      ratexp_t parseE() {
        ignore();
        ratexp_t e2=parseP();
        if(p_ == 0)
          return e2;
        if(s_[p_-1]=='+') {
          --p_;
          ignore();
          if(p_==0)
            throw parse_exception("Parsing expression");
          ratexp_t e1=parseE();
          return ratexp_t(rs_.add(e1,e2));
        }
        return e2;
      }

      ratexp_t parseP() {
        ratexp_t e2=parseS();
        if(p_==0)
          return e2;
        if(s_[p_-1]=='.') {
          --p_;
          ignore();
          if(p_==0)
            throw parse_exception("Parsing product");
          ratexp_t e1=parseP();
          return ratexp_t(rs_.mul(e1,e2));
        }
        if(s_[p_-1]!='(' && s_[p_-1]!='+') {
          ratexp_t e1=parseP();
          return ratexp_t(rs_.mul(e1,e2));
        }
        return e2;
      }

      ratexp_t parseS() {
        if(s_[p_-1]=='*'){
          --p_;
          ignore();
          if(p_==0)
            throw parse_exception("Parsing star");
          ratexp_t e=parseS();
          return ratexp_t(rs_.star(e));
        }
        if(s_[p_-1]=='?'){
          --p_;
          ignore();
          if(p_==0)
            throw parse_exception("Parsing ?");
          ratexp_t e=parseS();
          return ratexp_t(rs_.add(e,rs_.one()));
        }
        if(s_[p_-1]=='}'){
          --p_;
          ignore();
          if(p_==0)
            throw parse_exception("Parsing exponent");
          int a,b,h;
          if(s_[p_-1]==',')
            b=-1;
          else {
            b=0; h=1;
            while(s_[p_-1]>='0' && s_[p_-1]<='9') {
              b+=h*(s_[p_-1]-'0'); h*=10;
              --p_;
              ignore();
              if(p_==0)
                throw parse_exception("Parsing exponent");
            }
          }
          a=b;
          if(s_[p_-1]!='{') {
            if(s_[p_-1]!=',' || p_==1)
              throw parse_exception(p_-1,"Parsing exponent");
            --p_;
            ignore();
            a=0; h=1;
            while(s_[p_-1]>='0' && s_[p_-1]<='9') {
              a+=h*(s_[p_-1]-'0'); h*=10;
              --p_;
              ignore();
              if(p_==0)
                throw parse_exception("Parsing exponent");
            }
            if(s_[p_-1]!='{')
              throw parse_exception(p_-1,"Parsing exponent");
          }
          --p_;
          ignore();
          if(p_==0)
            throw parse_exception("Parsing");
          ratexp_t e=parseS();
          ratexp_t p(rs_.one());
          for(int i=0; i<a; ++i)
            p=ratexp_t(rs_.mul(p,e));
          if(b==-1)
            return ratexp_t(rs_.mul(p,rs_.star(e)));
          ratexp_t pp(rs_.one());
          ratexp_t qq(rs_.one());
          for(int i=a; i<b; ++i) {
            pp=ratexp_t(rs_.mul(pp,e));
            qq=ratexp_t(rs_.add(qq,pp));
          }
          return ratexp_t(rs_.mul(p,qq));
        }
        return parseL();
      }

      ratexp_t parseL() {
        ratexp_t e=parseR();
        if(p_==0)
          return e;
        if(s_[p_-1]=='>') {
          --p_;
          ignore();
          auto w = ws_.parse(s_,p_);
          ignore();
          if(p_==0 || s_[p_-1]!='<')
            throw parse_exception(p_-1,"Parsing weight");
          --p_;
          ignore();
          return ratexp_t(rs_.lmul(w, e));
        }
        return e;
      }

      ratexp_t parseR() {
        if(p_==0)
          throw parse_exception("Parsing right weight");
        if(s_[p_-1]=='>') {
          --p_;
          ignore();
          auto w = ws_.parse(s_,p_);
          ignore();
          if(p_==0 || s_[p_-1]!='<')
            throw parse_exception(p_-1,"Parsing right weight");
          --p_;
          ignore();
          ratexp_t e=parseA();
          return ratexp_t(rs_.rmul(e, w));
        }
        return parseA();
      }

      ratexp_t parseA() {
        if(p_>1 && s_[p_-1]=='e' && s_[p_-2]=='\\') {
          p_-=2;
          ignore();
          return ratexp_t(rs_.one());
        }
        if(p_>1 && s_[p_-1]=='z' && s_[p_-2]=='\\') {
          p_-=2;
          ignore();
          return ratexp_t(rs_.zero());
        }
        if(p_==0)
          throw parse_exception("Parsing atom");
        if(s_[p_-1]==')'){
          --p_;
          ignore();
          ratexp_t res= parseE();
          if(p_==0)
            throw parse_exception("Parsing atom");
          if(s_[p_-1]=='(') {
            --p_;
            ignore();
            return res;
          }
        }
        if(s_[p_-1]==']')
          return parseSqrBrkt(ls_);
        auto l = ls_.parse(s_,p_, strict_);
        ignore();
        return ratexp_t(rs_.atom(l));
      }

      template<typename T>
      ratexp_t parseSqrBrkt(const T&) {
          --p_;
          //ignore(); // to capture space at the end of list
          std::set<typename labelset_t::value_t> letter_list;
          while (p_>0 && s_[p_-1]!='[') {
            if(strict_ && p_>1 && s_[p_-1]=='^' && s_[p_-2]=='['){
              std::set<typename labelset_t::value_t> tmp;
              auto& alpha=ls_.genset();
              auto it=alpha.begin();
              auto ite=alpha.end();
              for( auto l : letter_list) {
                while(*it != l) {
                  tmp.emplace(*it);
                  ++it;
                }
                ++it;
              }
              for(; it!=ite; ++it)
                tmp.emplace(*it);
              letter_list=tmp;
              p_--;
              break;
            }
            auto l = ls_.parse(s_,p_, strict_);
            ignore();
            if(s_[p_-1]=='-') {
              --p_;
              ignore();
              auto k = ls_.parse(s_,p_, strict_);
              ignore();
              for(auto i=l; i>=k; i--) {
                std::ostringstream os;
                ls_.print(i,os);
                const std::string& s=os.str();
                size_t q=s.length();
                ls_.parse(s,q, strict_);
                letter_list.emplace(i);
              }
            }
            else
              letter_list.emplace(l);
          }
          if(p_==0)
            throw parse_exception("Parsing interval");
          --p_;
          ignore();
          bool first=true;
          ratexp_t res;
          for(auto l : letter_list) {
            if(first) {
              res = ratexp_t(rs_.atom(l));
              first=false;
            }
            else
              res = rs_.add(res,ratexp_t(rs_.atom(l)));
          }
          return res;
      }

      template<typename... T>
      ratexp_t parseSqrBrkt(const tupleset<T...>&) {
        auto l = ls_.parse(s_,p_, strict_);
        ignore();
        return ratexp_t(rs_.atom(l));
      }

      template<typename F, typename S>
      ratexp_t parseSqrBrkt(const letterset<set_alphabet<pair_letters<F,S>>>&) {
        auto l = ls_.parse(s_,p_, strict_);
        ignore();
        return ratexp_t(rs_.atom(l));
      }
      
      template<typename T>
      ratexp_t parseSqrBrkt(const wordset<T>&) {
        auto l = ls_.parse(s_,p_, strict_);
        ignore();
        return ratexp_t(rs_.atom(l));
      }

      void ignore() {
        while(p_>0 && s_[p_-1]==' ')
          --p_;
      }

      size_t getPos() {
        return p_;
      }
      
      size_t p_;
    private:
      const ratexpset_t& rs_;
      const std::string& s_;
      const bool strict_;
      weightset_t ws_ = *rs_.weightset();
      labelset_t ls_ = *rs_.labelset();

    };
  }

    /** @brief parser of rational expressions
     * 
     * @tparam RatExpSet the type of the context of rational expressions   
     * @param ratexpset the context of rational expressions   
     * @param s the expression to parse   
     * @param check_complete if `true`, the parsing must encompass the whole string;
     *        otherwise, it may apply only to a suffix        
     * @param strict_alphabet if `true` every letter must belong to the alphabet in \p ratexpset, otherwise, it shall be added
     * @throw parse_exception if the expression is malformed
     * @throw domain_error if \p strict_alphabet is `true` and a letter does not belong to the alphabet
     *
     * The string \p s is parsed to obtain a rational expression;
     * the context is specified by \p ratexpset.
     *
     * In rational expressions, the following characters are special:
     * (,),[,],<,>,{,},?,*,+,.    
     * The spaces are not significant, but they can be used to separate two tokens:    
     * for instance, if letters are integers, "5 10" is the word with two letters, 5 and 10.    
     *
     * The rational expressions are defined by the wollowing grammar:    
     *  E --> P | E+P    
     *  P --> S | PS | P.S     
     *  S --> L | S* | S{exponent} | S?      
     *  L --> R | &lt;weight&gt;R     
     *  R --> A | A&lt;weight&gt;     
     *  A --> label | (E) | [label list]  
     *
     * The priority of operators results from the grammar.
     *
     * Labels and weights are respectively parsed by the labelset 
     * and the weightset given by the \p ratexpset.    
     * The form of the exponent is {p} or {p,q} or {p,} or {,q}, where p and q
     * are nonnegative integers, with p<q.     
     *
     * **Label Lists**
     *
     * Label lists are not available if labels are tuples; 
     * they are concatenation of labels or pairs of labels of the form a-b,
     * where a and b are labels.     
     * Inside label lists, the letter '-' can only
     * appear at the *end* of the concatenation, otherwise, it is interpreted as
     * separation of pairs.     
     * If \p strict_alphabet is `true`, the complementation of label lists is available,
     * indicated by '^' at the beginning of the list.
     *
     * **Special Characters** 
     *
     * The backslash can not be used to escape special characters;
     * it can only be used in front of 'e' and 'z' to represent respectively 
     * the empty word and the zero;    
     * 
     * Every special character excepted [ is allowed 
     * in label lists, hence, it is a way to escape special character: 
     * for instance "[)]" is the letter ')'.      
     * Moreover, the space is significant at the *end* of the list.     
     * Notice also that (,[,{,< can be used as letters ouside character lists 
     * if there is no corresponding parenthesis at right.
     *
     * **Semantics**
     *
     * Every expression generated from E, P, S, L, R or A is interpreted as a 
     * (rational) series inductively defined as follow ( [[E]] is the interpretation of E) :     
     * - a label is interpreted as the charateristic series of this letter;
     * - a label list is interpreted as the union of the labels enumerated in the list,
     *   a pair of labels a-b is the union of every letter in the interval [a;b];     
     * - [[(E)]] = [[E]];   
     * - [[A&lt;weight&gt;]] = [[A]].[[weight]];
     * - [[&lt;weigth&gt;R]] = [[weight]].[[R]];
     * - [[S*]] = [[S]]*;    
     * - [[S?]] = [[S]] + one (the empty word of the monoid);    
     * - [[S{p}]] is the p-th power of [[S]];     
     *   [[S{p,q}]] is the sum of the powers of [[S]] from the p-th to the q-th;    
     *   [[S{,q}]] = [[S{0,q}]];   
     *   [[S{p,}]] = [[S{p}]].[[S*]];    
     * - [[PS]] = [[P.S]] = [[P]].[[S]];    
     * - [[E+P]] = [[E]]+[[P]].
     *
    */
  template <typename RatExpSet>
  inline
  typename RatExpSet::ratexp_t
  parse_exp(const RatExpSet& ratexpset,
            const std::string& s, bool check_complete=true, bool strict_alphabet=true)
  {
    internal::exp_parser<RatExpSet> parser{ratexpset,s,strict_alphabet};
    auto exp=parser.parseE();
    if(check_complete && parser.getPos()>0)
      throw parse_exception(parser.getPos(), "Parsing of a proper suffix");
    return exp;
  }

}}//end of ns awali::stcs


#endif
