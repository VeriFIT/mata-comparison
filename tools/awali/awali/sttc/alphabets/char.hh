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

#ifndef AWALI_ALPHABETS_CHAR_HH
# define AWALI_ALPHABETS_CHAR_HH

# include <cassert>
# include <string>
# include <iostream>

#include <awali/common/json/node.cc>
#include <awali/sttc/misc/escape.hh>
#include <awali/common/version.hh>

namespace awali {
  namespace sttc {
    /// helper for manipulations of char letters
    class char_letters
    {
    public:
      using letter_t = char;
      using word_t = std::string;
      
      virtual ~char_letters() = default;


      static std::string sname()
      {
        return "char";
      }

      static std::ostream& jsname(std::ostream& o)
      {
        o << '"' << "Char" << '"';
        return o;
      }
      
      virtual std::string vname(bool = true) const
      {
        return sname();
      }

      /*
      word_t
      to_word(const letter_t l) const
      {
        return {l};
      }

      const word_t&
      to_word(const word_t& l) const
      {
        return l;
      }

      word_t
      concat(const letter_t l, const letter_t r) const
      {
        return {l, r};
      }

      word_t
      concat(const word_t& l, const letter_t r) const
      {
        return l + r;
      }

      word_t
      concat(const letter_t l, const word_t& r) const
      {
        return l + r;
      }

      word_t
      concat(const word_t& l, const word_t& r) const
      {
        return l + r;
      }

      /// Add the special character first and last.
      word_t delimit(const word_t& w) const
      {
        return concat(concat(special_letter(), w), special_letter());
      }

      /// Remove first and last characters, that must be "special".
      word_t undelimit(const word_t& w) const
      {
        size_t s = w.size();
        assert(2 <= s);
        assert(w[0] == special_letter());
        assert(w[s-1] == special_letter());
        return w.substr(1, s-2);
      }

      static word_t
      empty_word()
      {
        return {};
      }

      static bool
      is_empty_word(const word_t& w)
      {
        return w.empty();
      }

      word_t
      transpose(const word_t& w) const
      {
        // C++11 lacks std::rbegin/rend...
        return {w.rbegin(), w.rend()};
      }
      */
      
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
      /*
      bool
      equals(const word_t& w1, const word_t& w2) const
      {
        return w1 == w2;
      }
      */
      static bool
      is_letter(const letter_t&)
      {
        return true;
      }
      /*
      bool
      is_letter(const word_t& w) const
      {
        return w.size() == 1;
      }
      */
      /// The reserved letter used to forge the "one" label (the unit,
      /// the identity).
      static constexpr letter_t one_letter() { return 0; }

      /// The reserved letter used to forge the labels for initial and
      /// final transitions.
      static constexpr letter_t special_letter() { return 127; }

      static const std::string& separation_mark(){
        static const std::string sep{""};
        return sep;
      }

      static std::ostream&
      print(const letter_t& l, std::ostream& o)
      {
        if (l != one_letter() && l != special_letter())
          o << l;
        return o;
      }
      
      /*
      std::ostream&
      print(const word_t& w, std::ostream& o) const
      {
        return o << format(w);
      }
      */
      
      static letter_t
      parse_one_letter(const std::string& s, size_t& p) {
        --p;
        return s[p];
      }


      static letter_t
      conv_one_letter(std::istream& i)
      {
        return i.get();
      }

      static std::string
      format(const letter_t l)
      {
        if (l == one_letter() || l == special_letter())
          return {};
        else
          return str_escape(l);
      }


      template<unsigned version = version::fsm_json>
      json::string_t* letter_to_json(letter_t const& l)
      const
      {
        switch (version) {
          case 0:
            throw parse_exception("[char] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return new json::string_t(l);
        }
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
            return new json::string_t("Char");

        }
      }


      template<unsigned version = version::fsm_json>
      static letter_t
      letter_from_json(json::node_t const* p, bool allow_empty=false)
      {
        switch (version) {
          case 0:
          case 1:
          default:
            if(p->kind == json::STRING) {
              std::string s = p->to_string();
              if(s.empty() && allow_empty)
                return one_letter();
              if(s.size()==1)
                return s[0];
            }
            throw parse_exception("Json: Bad char format"); 
        }
      }

      /*
      std::string
      format(const word_t& w) const
      {
        size_t s = w.size();

        std::string res;
        if (s == 0
            || (s == 1 && w[0] == one_letter()))
          res = "\\e";

        // If the string starts or ends with the special letter, skip
        // it.  If the resulting string is empty, format it this way.
        // (We DON'T want to format it as "\\e".)
        else if (w[0] == special_letter())
          res = (s == 1) ? "" : str_escape(w.substr(1));
        else if (s > 1 && w[s - 1] == special_letter())
          res = str_escape(w.substr(0, s - 1));
        else
          res = str_escape(w);
        return res;
      }
      */

    };

  }
}//end of ns awali::stc

#endif // !AWALI_ALPHABETS_CHAR_HH
