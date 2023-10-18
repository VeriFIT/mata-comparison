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

#ifndef AWALI_ALPHABETS_INT_HH
# define AWALI_ALPHABETS_INT_HH

# include <cassert>
# include <string>
# include <iostream>
#include <awali/common/json/node.cc>
# include <awali/sttc/weightset/z.hh>

#include <awali/sttc/misc/escape.hh>

namespace awali {
  namespace sttc {
    /// helper for manipulations of int letters
    class int_letters
    {
    public:
      using letter_t = int;
      using word_t = std::basic_string<int>;

      virtual ~int_letters() = default;

      static std::string sname()
      {
        return "int";
      }

      static std::ostream& jsname(std::ostream& o)
      {
        o << '"' << "Integer" << '"';
        return o;
      }
      
      virtual std::string vname(bool = true) const
      {
        return sname();
      }
     
      static letter_t
      transpose(letter_t l)
      {
        return l;
      }

      static bool
      equals(const letter_t& l1, const letter_t& l2)
      {
        return l1 == l2;
      }

      static bool
      is_letter(const letter_t&)
      {
        return true;
      }

      /// The reserved letter used to forge the "one" label (the unit,
      /// the identity).
      static constexpr letter_t one_letter() { return std::numeric_limits<letter_t>::min(); }

      /// The reserved letter used to forge the labels for initial and
      /// final transitions.
      static constexpr letter_t special_letter() { return std::numeric_limits<letter_t>::max(); }

      static const std::string& separation_mark(){
        static const std::string sep{":"};
        return sep;
      }

      static std::ostream&
      print(const letter_t& l, std::ostream& o)
      {
        if (l != one_letter() && l != special_letter())
          o << l;
        return o;
      }
            
      static letter_t
      parse_one_letter(const std::string& s, size_t& p) {
        letter_t r=z::parse(s,p);
        /* Warning
         * In order to avoid ambiguity in the parsing of ratexp,
         * the '+' is excluded from the int
         */
        if(s[p]=='+')
          ++p;
        return r;
      }


      static letter_t
      conv_one_letter(std::istream& i)
      {
        return z::conv(i);
      }

      static std::string
      format(const letter_t l)
      {
        if (l == one_letter() || l == special_letter())
          return {};
        else
          return std::to_string(l);
      }


      template<unsigned version = version::fsm_json>
      json::node_t* to_json()
      const
      {
        switch (version) {
          case 0:
            throw parse_exception("[char] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return new json::string_t("Integer");

        }
      }

      
      template<unsigned version = version::fsm_json>
      letter_t
      letter_from_json(json::node_t const* p)
      const
      {
        switch (version) {
          case 0:
            throw parse_exception("[int] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return p->to_int();
        }
      }


      template<unsigned version = version::fsm_json>
      json::int_t* letter_to_json(letter_t const& l)
      const
      {
        switch (version) {
          case 0:
            throw parse_exception("[int] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return new json::int_t(l);
        }
      }

    };

  }
}//end of ns awali::stc

#endif // !AWALI_ALPHABETS_INT_HH
