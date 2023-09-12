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

#ifndef AWALI_LABELSET_LETTERSET_HH
# define AWALI_LABELSET_LETTERSET_HH

# include <memory>

#include <awali/sttc/alphabets/setalpha.hh> // intersection
#include <awali/sttc/core/kind.hh>
#include <awali/common/json/node.cc>
#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/misc/escape.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/common/parse_exception.cc>
# include <sstream>
#include <awali/sttc/misc/set.hh> // intersection
//#include <awali/sttc/labelset/genset_labelset.hh>
//#include <awali/sttc/labelset/labelset.hh>
#include <awali/sttc/labelset/wordset.hh>

#include<awali/common/version.hh>

namespace awali {
  namespace sttc {

    /// Implementation of labels are letters.
    template <typename GenSet>
    class letterset
    {
    public:
      using genset_t = GenSet;
      using genset_ptr = std::shared_ptr<const genset_t>;
      using self_type = letterset;

      using letter_t = typename genset_t::letter_t;
      using word_t = typename wordset<genset_t>::word_t;
      using letters_t = std::set<letter_t>;

      using value_t = letter_t;

      using kind_t = labels_are_letters;

    private:
      genset_ptr genset_;
    public:
      letterset(const genset_t& gs = {})
        : genset_(std::make_shared<const genset_t>(gs))
      {}

      letterset(const std::initializer_list<letter_t>& letters)
        : genset_(std::make_shared<const genset_t>(letters))
      {}

      letterset(const letters_t& letters)
        : genset_(std::make_shared<const genset_t>(letters))
      {}

      const genset_t& genset() const {
        return *genset_;
      }
      
      static std::string sname()
      {
        return "lal_" + genset_t::sname();
      }
      
      std::string vname(bool full = true) const
      {
        return "lal_" + genset().vname(full);
      }
      
      
      /// Build from the description in \a is.
      static letterset make(std::istream& is)
      {
        // name: lal_char(abc).
        //       ^^^ ^^^^^^^^^
        //      kind   genset
        kind_t::make(is);
        eat(is, '_');
        auto gs = genset_t::make(is);
        return gs;
      }

      /// Whether unknown letters should be added, or rejected.
      /// \param o   whether to accept
      /// \returns   the previous status.
      bool open(bool o) const
      {
        return this->genset().open(o);
      }

      static constexpr bool is_free()
      {
        return true;
      }

      /// Value constructor.
      template <typename... Args>
      value_t value(Args&&... args) const
      {
        return value_t{std::forward<Args>(args)...};
      }

      /// Convert to a word.
      word_t word(value_t v) const
      {
        return {v};
      }
      
      /// Prepare to iterate over the letters of v.
      static word_t
      letters_of(word_t v)
      {
        return v;
      }
      
      static value_t
      special() ATTRIBUTE_PURE
      {
        return genset_t::special_letter();
      }

      /// Whether \a l == \a r.
      static bool
      equals(const value_t l, const value_t r)
      {
        return l == r;
      }

      /// Whether \a l < \a r.
      static bool less_than(const value_t l, const value_t r)
      {
        return l < r;
      }

      static constexpr bool
      has_one()
      {
        return false;
      }

      static constexpr bool
      is_ratexpset()
      {
        return false;
      }

      static constexpr bool
      is_letterized()
      {
        return true;
      }

      static bool
      is_special(value_t v) ATTRIBUTE_PURE
      {
        return v == special();
      }

      static constexpr bool
      is_one(value_t)
      {
        return false;
      }

      bool
      is_valid(value_t v) const
      {
        return genset().has(v);
      }

      static size_t size(value_t)
      {
        return 1;
      }

      static size_t hash(value_t v)
      {
        return utils::hash_value(v);
      }

      static value_t
      conv(self_type, value_t v)
      {
        return v;
      }
      /*
      template<typename T, typename U>
      static value_t
      conv(T, U)
      {
        std::string message{"Impossible conversion : "};
        message+=sname()+","+T::sname();
        throw std::runtime_error(message);
      }
      */
      /// Read one letter from i, return the corresponding label.
      value_t
      conv(std::istream& i) const
      {
        value_t v=genset().conv_one_letter(i);
        if(genset().has(v))
          return v;
        else 
          throw std::domain_error("invalid label");
        //  : unexpected " +this->genset().to_word (v));
      }

      value_t
      parse(const std::string& s, size_t& p, bool fixed_domain=true) const {
        value_t v = this->genset().parse_one_letter(s,p);
        if (genset().has(v))
          return v;
        if(!fixed_domain) {
          const_cast<genset_t&>(*genset_).add_letter(v);
          return v;
        }
        throw std::domain_error("invalid label");
      }

      std::set<value_t>
      convs(std::istream& i) const
      {
        std::set<value_t> res;
        for (auto r : this->convs_(i))
          res.insert(value_t{r});
        return res;
      }

      std::ostream&
      print(const value_t& l, std::ostream& o,
            const std::string& = "text") const
      {
        if (!is_special(l))
          o << genset().format(l);
        return o;
      }

      std::ostream&
      print_set(std::ostream& o, const std::string& format = "text") const
      {
        if (format == "latex")
          genset().print_set(o, format);
        else if (format == "text")
          o << vname(true);
        else
          raise("invalid format: ", format);
        return o;
      }


      template<unsigned version = version::fsm_json>
      json::node_t* to_json() const {
        switch (version) {
          case 0:
            throw parse_exception("[letterset] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            json::object_t* obj 
              = this->genset().template to_json<version>()->object();
            obj->push_front("labelKind", new json::string_t("Letters"));
            return obj;
          }
      }


      template<unsigned version = version::fsm_json>
      json::node_t* value_to_json(value_t v) const 
      {
        return this->genset().template letter_to_json<version>(v);
      }
      

      template<unsigned version = version::fsm_json>
      value_t
      value_from_json(json::node_t const* p) const {
        return genset().template letter_from_json<version>(p);
        raise("json parser Letterset"); 
      }


      value_t
      transpose(const value_t& v) const {
        return genset().transpose(v);
      }
    };

    /*
    namespace internal
    {
      template <typename GenSet>
      struct law_traits<letterset<GenSet>>
      {
        using type = wordset<GenSet>;
        static type value(const letterset<GenSet>& ls)
        {
          return ls.genset();
        }
      };
    }
    */
    
    /// Compute the meet with another labelset.
    template <typename GenSet>
    letterset<GenSet>
    meet(const letterset<GenSet>& lhs, const letterset<GenSet>& rhs)
    {
      return {intersection(lhs.genset(), rhs.genset())};
    }

    /// Compute the join with another labelset.
    template <typename GenSet>
    letterset<GenSet>
    join(const letterset<GenSet>& lhs, const letterset<GenSet>& rhs)
    {
      return {get_union(lhs.genset(), rhs.genset())};
    }

    template<typename T, typename U>
    T join(const T& l, const U&) {
      std::string message{"Impossible join : "};
      message+=T::sname()+","+U::sname();
      throw std::runtime_error(message);
    }
  }
}//end of ns awali::stc

#endif // !AWALI_LABELSET_LETTERSET_HH
