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

#ifndef AWALI_ALPHABETS_SETALPHA_HH
# define AWALI_ALPHABETS_SETALPHA_HH

# include <initializer_list>
# include <stdexcept>
# include <cassert>
# include <sstream>


#include <awali/common/version.hh>
#include <awali/common/json/utils.hh>
#include <awali/common/json/node.cc>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/misc/set.hh>
#include <awali/sttc/misc/stream.hh> // eat.

namespace awali {
  namespace sttc {

    /** @brief objets that represent the alphabets of letters as char
     *
     * @tparam L the type of letters    
     *
     * \p L is actually {@link char_letters}
     */ 
    template <typename L>
    class set_alphabet: public L
    {
    public:
      using letter_t = typename L::letter_t;
      //using word_t = typename L::word_t;
      using letters_t = std::set<letter_t>;
      /// The type of our values, when seen as a container.
      using value_type = letter_t;

      static std::string sname()
      {
        return L::sname();
      }

      virtual std::string vname(bool full = true) const override
      {
        std::ostringstream res;
        res << sname();
        if (full)
          {
            res << '(';
            for (letter_t c: alphabet_)
              L::print(c,res) ;
            res << ')';
          }
        return res.str();
      }

      /*
      static set_alphabet make(std::istream& is)
      {
        // name: char(abc)_ratexpset<law_char(xyz)_b>.
        //       ^^^^ ^^^  ^^^^^^^^^^^^^^^^^^^^^^^^^
        //        |    |        weightset
        //        |    +-- gens
        //        +-- letter_type
        std::string letter_type;
        {
          char c;
          while (is >> c)
            {
              if (c == '(')
                {
                  is.unget();
                  break;
                }
              letter_type.append(1, c);
            }
        }
        // The list of generators (letters).
        letters_t gens;
        // Previously read character.
        int prev = -1;
        {
          sttc::eat(is, '(');
          char l;
          while (is >> l && l != ')')
            switch (l)
              {
              case '-':
                if (prev == -1)
                  goto insert;
                else
                  {
                    int l2 = is.get();
                    require(l2 != EOF,
                            "unexpected end in character class");
                    if (l2 == '\\')
                      {
                        l2 = is.get();
                        require(l2 != EOF,
                                "unexpected end after escape in character class");
                      }
                    for (l = prev; l <= l2; ++l)
                      gens.insert(l);
                    prev = -1;
                    continue;
                  }

              case '\\':
                {
                  int l2 = is.get();
                  require(l2 != EOF,
                          "unexpected end after escape");
                  l = l2;
                  goto insert;
                }

              default:
              insert:
                gens.insert(l);
                prev = l;
              }
        }
        return gens;
      }
      */
      
      set_alphabet() = default;
      set_alphabet(const set_alphabet&) = default;
      set_alphabet(const std::initializer_list<letter_t>& l)
        : alphabet_{l}
      {}

      set_alphabet(const letters_t& l)
        : alphabet_{l}
      {}

      /// Whether unknown letters should be added, or rejected.
      /// \param o   whether to accept
      /// \returns   the previous status.
      bool open(bool o) const
      {
        std::swap(o, open_);
        return o;
      }
      
      
      /// Modify \a this by adding \a l, and return *this.
      set_alphabet&
      add_letter(letter_t l)
      {
        assert(l != this->special_letter());
        alphabet_.insert(l);
        return *this;
      }

      set_alphabet&
      remove_letter(letter_t l)
      {
        assert(l != this->special_letter());
        alphabet_.erase(l);
        return *this;
      }
      
      unsigned size() const {
        return alphabet_.size();
      }
      
      set_alphabet&
      clear()
      {
        alphabet_.clear();
        return *this;
      }
      
      /// Whether \a l is a letter.
      bool
      has(letter_t l) const
      {
        if (open_)
          {
            // FIXME: OMG...
            const_cast<set_alphabet&>(*this).add_letter(l);
            return true;
          }
        else
          return alphabet_.find(l)!=alphabet_.end();
      }

      /*
      /// Extract and return the next word from \a i.
      word_t
      conv(std::istream& i) const
      {
        word_t res;
        require(!i.bad(),
                "conv: invalid stream");
        if (i.good() && i.peek() == '\\')
          {
            i.ignore();
            int c = i.peek();
            if (c != 'e')
              throw std::domain_error("invalid label: unexpected \\"
                                      + str_escape(c));
            i.ignore();
          }
        else
          {
            // FIXME: This wrongly assumes that letters are characters.
            // It will break with integer or string alphabets.
            int c;
            while (i.good()
                   && (c = i.peek()) != EOF
                   && c != ','
                   && c != '('
                   && c != ')'
                   && has(c))
              res = this->concat(res, letter_t(i.get()));
          }
        return res;
      }

      word_t
      parse(const std::string& str, size_t& p) const
      {
        word_t w = this->empty_word();
        while (p>0) { //FIXME: this wrongly assumes that there is no delimiter
          // between letters
          letter_t l = this->parse_one_letter(str,p);
          if (this->has(l)) {
            w = this->concat(l,w);
          } 
          else {
            p++;
            break;
          }
        }
        return w;
      }
      */
      
      using iterator = typename letters_t::const_iterator;
      using const_iterator = typename letters_t::const_iterator;

      const_iterator begin() const
      {
        return alphabet_.begin();
      }

      const_iterator end() const
      {
        return alphabet_.end();
      }

      const_iterator cbegin() const
      {
        return alphabet_.begin();
      }

      const_iterator cend() const
      {
        return alphabet_.end();
      }

      const_iterator find(letter_t l) const
      {
        return alphabet_.find(l);
      }

      std::ostream&
      print_set(std::ostream& o, const std::string& format = "text") const
      {
        if (format == "latex")
          {
            const char *sep = "\\{";
            for (auto c: *this)
              {
                o << sep;
                L::print(c, o);
                sep = ", ";
              }
            o << "\\}";
          }
        else if (format == "text")
          o << vname(true);
        else
          raise("invalid format: ", format);
        return o;
      }


      template<unsigned version = version::fsm_json>
      json::object_t* 
      to_json() const {
      version::check_fsmjson<version>();
        switch (version) {
          default:
            json::object_t* obj = new json::object_t();
            obj->push_back("letterType", L::to_json());
            json::array_t* l = new json::array_t();
            for (auto x: *this)
              l->push_back(this->template letter_to_json<version>(x));
            obj->push_back("alphabet", l);
            return obj;
        }
      }

      


//      template<unsigned version = version::fsm_json>
//      json::string_t* 
//      letter_to_json(const letter_t& l) const
//      {
//        switch (version) {
//          case 0:
//            throw parse_exception("[Set Alpha] Unsupported fsm-json version:"
//                                  + std::to_string(version));
//          case 1:
//          default:
//            std::stringstream ss;
//            ss << l;
//            return new json::string_t(ss.str());
//        }
//      }

      /// Compute the intersection with another alphabet.
      template <typename L2>
      friend set_alphabet<L2>
      intersection(const set_alphabet<L2>& lhs, const set_alphabet<L2>& rhs);

      /// Compute the union with another alphabet.
      template <typename L2>
      friend set_alphabet<L2>
      get_union(const set_alphabet<L2>& lhs, const set_alphabet<L2>& rhs);
    private:
      // FIXME: OMG...
      mutable letters_t alphabet_;
      mutable bool open_ = false;
    };

    template <typename L2>
    set_alphabet<L2>
    intersection(const set_alphabet<L2>& lhs, const set_alphabet<L2>& rhs)
    {
      return {internal::intersection(lhs.alphabet_, rhs.alphabet_)};
    }

    template <typename L2>
    set_alphabet<L2>
    get_union(const set_alphabet<L2>& lhs, const set_alphabet<L2>& rhs)
    {
      return {internal::get_union(lhs.alphabet_, rhs.alphabet_)};
    }
  }
}//end of ns awali::stc

#endif // !AWALI_ALPHABETS_SETALPHA_HH
