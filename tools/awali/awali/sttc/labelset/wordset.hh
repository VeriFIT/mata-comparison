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

#ifndef AWALI_LABELSET_WORDSET_HH
# define AWALI_LABELSET_WORDSET_HH

# include <memory>
# include <set>

#include <awali/sttc/core/kind.hh>
//#include <awali/sttc/labelset/genset_labelset.hh>
//#include <awali/sttc/labelset/labelset.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/utils/hash.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc {

  /// Implementation of labels are words.
    template <typename GenSet>
  class wordset
  {
  public:
    using genset_t = GenSet;
    using self_type = wordset;
    using genset_ptr = std::shared_ptr<const genset_t>;

    using letter_t = typename genset_t::letter_t;
    using word_t = typename genset_t::word_t;
    using letters_t = std::set<letter_t>;

    using value_t = word_t;

    using kind_t = labels_are_words;
    
  private:
    genset_ptr genset_;
  public:
    wordset(const genset_t& gs = {})
      : genset_{std::make_shared<const genset_t>(gs)}
    {}

    const genset_t& genset() const {
      return *genset_;
    }

    static std::string sname()
    {
      return "law_" + genset_t::sname();
    }

    std::string vname(bool full = true) const
    {
      return "law_" + genset().vname(full);
    }

    /*
    /// Build from the description in \a is.
    static wordset make(std::istream& is)
    {
      // name: law_char(abc).
      //       ^^^ ^^^^^^^^^
      //      kind   genset
      kind_t::make(is);
      eat(is, '_');
      auto gs = genset_t::make(is);
      return gs;
    }
    */

    /// Whether unknown letters should be added, or rejected.
    /// \param o   whether to accept
    /// \returns   the previous status.
    bool open(bool o) const
    {
      return genset().open(o);
    }

    // If labels are words, the monoid of labels is not free:
    static constexpr bool is_free()
    {
      return false;
    }

    /// Value constructor.
    template <typename... Args>
    value_t value(Args&&... args) const
    {
      return value_t{std::forward<Args>(args)...};
    }

    /// Convert to a word.
    word_t word(const value_t& v) const
    {
      return v;
    }

    /// Convert to a word.
    word_t word(const letter_t& l) const
    {
      return {l};
    }

    word_t
    concat(const letter_t l, const letter_t r) const
    {
      return {l, r};
    }
    
    word_t
    concat(const word_t& l, const letter_t r) const
    {
      word_t w{l};
      w.push_back(r);
      return w;
    }
    
    word_t
    concat(const letter_t l, const word_t& r) const
    {
      word_t w{l};
      w.insert(w.end(),r.begin(),r.end());      
      return w;
    }
    
    word_t
    concat(const word_t& l, const word_t& r) const
    {
      word_t w{l};
      w.insert(w.end(),r.begin(),r.end());      
      return w;
    }
    
    /// Prepare to iterate over the letters of v.
    static word_t
    letters_of(word_t v)
    {
      return v;
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
      if(l.size()<r.size())
	return true;
      if(l.size()>r.size())
	return false;
      return (l < r);
    }

    static value_t
    special()
    {
      return {genset_t::special_letter()};
    }

    static bool
    is_special(const value_t& v)
    {
      return v == special();
    }

    bool
    is_valid(const value_t& v) const
    {
      for (auto l: v)
        if (!genset().has(l))
          return false;
      return true;
    }

    static constexpr bool
    is_ratexpset()
    {
      return false;
    }

    static constexpr bool
    has_one()
    {
      return true;
    }

    static constexpr bool
    is_letterized()
    {
      return false;
    }

    static value_t
    one()
    {
      return {};
    }

    static bool
    is_one(const value_t& l)
    {
      return l.empty();
    }

    static size_t size(const value_t& v)
    {
      return v.size();
    }

    /// Add the special character first and last.
    word_t delimit(const word_t& w) const
    {
      word_t s=special();
      s.insert(s.end(),w.begin(),w.end());
      s.push_back(genset().special_letter());
      return s;
    }
    
    /// Remove first and last characters, that must be "special".
    word_t undelimit(const word_t& w) const
    {
      size_t s = w.size();
      assert(2 <= s);
      assert(w[0] == genset().special_letter());
      assert(w[s-1] == genset().special_letter());
      word_t r(++w.begin(), --w.end());
      return r;
    }
    
    static size_t hash(const value_t& v)
    {
      return utils::hash_value(v);
    }

    static value_t
    conv(self_type, value_t v)
    {
      return v;
    }

    /*
    // FIXME: Why do I need to repeat this?
    // It should be inherited from genset-labelset.
    value_t
    conv(std::istream& i) const
    {
      return this->genset().conv(i);
    }
    */

    value_t
    parse(const std::string& s, size_t& p, bool fixed_alphabet=true) const {
      if (p == 0)
        return one();
      value_t tmp{};
      const std::string& sep=genset_t::separation_mark();
      size_t sep_length=sep.length();
      size_t q=p;
      letter_t l= genset_t::parse_one_letter(s, p);
      if(genset().has(l))
        tmp.push_back(l);
      else {
        p=q;
        return one();
      }
      while(p>0) {
        //Look for a separation mark
        q = p-sep_length;
        bool find_sep=true;
        for(size_t i=0; find_sep && i<sep_length; ++i)
          find_sep=(s[q+i]==sep[i]);
        if(!find_sep)
          break;
        p=q;
        l= genset_t::parse_one_letter(s, p);
        if(genset().has(l)) {
          tmp.push_back(l);
        }
        else if(sep_length>0)
          throw parse_exception(p,"Word parsing");
        else {
          p=q;
          break;
        }
      }
      value_t result;
      for(size_t i=tmp.size(); i!=0; --i)
        result.push_back(tmp[i-1]);
      return result;
    }

    value_t
    parse(const std::string& s) const {
      size_t p = s.size();
      value_t v = parse(s,p);
      if(p>0)
        throw parse_exception(p,"Word parsing");
      return v;
    }
    /*
    std::set<value_t>
    convs(std::istream& i) const
    {
      std::set<value_t> res;
      for (auto r : this->convs_(i))
        res.insert(value_t{r});
      return res;
    }
    */

    std::ostream&
    print(const value_t& l, std::ostream& o,
          const std::string& format = "text") const
    {
      if (is_one(l))
        o << (format == "latex" ? "\\varepsilon" : "\\e");
      else if (!is_special(l)) {
        bool first=true;
        for(auto letter : l) {
          if(first)
            first=false;
          else
            o<< genset_t::separation_mark();
          genset().print(letter, o);
        }
      }
      return o;
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        {
          genset().print_set(o, format);
          o << "^*";
        }
      else if (format == "text")
        o << vname(true);
      else
        raise("invalid format: ", format);
      return o;
    }
    
    template<unsigned version = version::fsm_json>
    json::node_t* to_json() const {
      version::check_fsmjson<version>();
      switch (version) {
        case 1:
        default:
          GenSet const& gs = this->genset();
          json::object_t* obj;
          obj = gs.template to_json<version>()->object();
          obj->push_front("labelKind", new json::string_t("Words"));
          return obj;
      }
    }


    template<unsigned version = version::fsm_json>
    json::node_t* value_to_json(value_t v) const 
    {
      switch (version) {
        case 0:
          throw parse_exception("[wordset] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          json::array_t* arr= new json::array_t();
           for (auto x: v)
             arr->push_back(this->genset().template letter_to_json<version>(x));
           return arr;
      }
    }


    template<unsigned version = version::fsm_json>
    value_t
    value_from_json(json::node_t const* p) const {
      switch (version) {
        case 0:
        case 1:
        default:
          value_t res;
          if(p->kind == json::STRING) {
            std::string const& s = p->string()->value;
            size_t pos=s.length();
            res=parse(s,pos);
            if(pos==0)
              return res;
          }
          else  
            if(p->kind == json::ARRAY) {
              for(auto val : *(p->array())) {
                res.push_back(this->genset().template letter_from_json<version>(val));
              }
              return res;
            }
          throw parse_exception("json parser Word value");
      }
    }

      value_t
      transpose(const value_t& v) const {
        value_t res;
        for(size_t i=v.size(); i!=0; --i)
          res.push_back(genset().transpose(v[i-1]));
        return res;
      }
  };

  /// Compute the meet with another alphabet.
  // FIXME: Factor in genset_labelset?
  template <typename GenSet>
  wordset<GenSet>
  meet(const wordset<GenSet>& lhs, const wordset<GenSet>& rhs)
  {
    return {intersection(lhs.genset(), rhs.genset())};
  }

  /// Compute the union with another alphabet.
  template <typename GenSet>
  wordset<GenSet>
  join(const wordset<GenSet>& lhs, const wordset<GenSet>& rhs)
  {
    return {get_union(lhs.genset(), rhs.genset())};
  }

    

}}//end of ns awali::stc

#endif // !AWALI_LABELSET_WORDSET_HH
