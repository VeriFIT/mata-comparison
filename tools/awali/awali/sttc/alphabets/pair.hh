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

#ifndef AWALI_ALPHABETS_PAIR_HH
# define AWALI_ALPHABETS_PAIR_HH

# include <cassert>
# include <string>
# include <vector>
# include <iostream>
# include <utility>
# include <functional>
#include <awali/common/json/node.cc>
# include <awali/sttc/weightset/z.hh>

#include <awali/sttc/misc/escape.hh>

namespace awali {
  namespace sttc {
    /// helper for manipulations of pair letters
    template<typename F, typename S>
    class pair_letters {
    public:
      using first_letter_t = typename F::letter_t;
      using second_letter_t = typename S::letter_t;
      using letter_t = std::pair<first_letter_t, second_letter_t>;
      using word_t = std::vector<letter_t>;

      static std::string sname()
      {
         return "lap<"+F::sname()+","+S::sname()+">";
      }

      static std::ostream& jsname(std::ostream& o)
      {
        o << "\"Pair\", \"entryTypes\":[";
        F::jsname(o) << ',';
        S::jsname(o) << ']';
return o;
      }
      
      virtual std::string vname(bool = true) const
      {
        return sname();
      }
     
      static letter_t
      transpose(letter_t l)
      {
        l.first=F::transpose(l.first);
        l.second=S::transpose(l.second);
        return l;
      }

      static bool
      equals(const letter_t& l1, const letter_t& l2)
      {
        return F::equals(l1.first,l2.first) && S::equals(l1.second,l2.second);
      }

      static bool
      is_letter(const letter_t&)
      {
        return true;
      }

      /// The reserved letter used to forge the "one" label (the unit,
      /// the identity).
      static letter_t one_letter() { return {F::one_letter(),S::one_letter()};}

      /// The reserved letter used to forge the labels for initial and
      /// final transitions.
      static letter_t special_letter() { return {F::special_letter(),S::special_letter()};}

      static const std::string& separation_mark(){
        static const std::string c;
        return c;
      }

      static std::ostream&
      print(const letter_t& l, std::ostream& o)
      {
        if (l != one_letter() && l != special_letter()) {
          o << '[';
          F::print(l.first,o) << ',';
          S::print(l.second,o) << ']';
        }
        return o;
      }
            
      static letter_t
      parse_one_letter(const std::string& s, size_t& p) {
        std::string message="Tuple letter expected ] got .";
        if(s[--p]!=']') {
          message[28]=s[p];
          throw parse_exception(p,message);
        }
        typename S::letter_t y=S::parse_one_letter(s,p);
        if(s[--p]!=',') {
          message[22]=',';
          message[28]=s[p];
          throw parse_exception(p,message);
        }
        typename F::letter_t x=F::parse_one_letter(s,p);
        if(s[--p]!='[') {
          message[22]='[';
          message[28]=s[p];
          throw parse_exception(p,message);
        }
        return {x,y};
      }


      static letter_t
      conv_one_letter(std::istream& i) {
        if(i.peek()!='[')
          throw parse_exception(i,"incorrect tuple");
        eat(i,'[');
        typename F::letter_t x=F::conv_one_letter(i);
        if(i.peek()!=',')
          throw parse_exception(i,"incorrect tuple");
        eat(i,',');
        typename S::letter_t y=S::conv_one_letter(i);
        if(i.peek()!=']')
          throw parse_exception(i,"incorrect tuple");
        eat(i,',');
        return {x,y};
      }

      static std::string
      format(const letter_t l)
      {
        if (l == one_letter() || l == special_letter())
          return {};
        else
          return "["+F::format(l.first)+","+S::format(l.second)+"]";
      }

    };

  }
}//end of ns awali::stc

#endif // !AWALI_ALPHABETS_PAIR_HH
