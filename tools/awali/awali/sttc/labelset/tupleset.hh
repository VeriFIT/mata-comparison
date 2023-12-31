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

#ifndef AWALI_LABELSET_TUPLESET_HH
# define AWALI_LABELSET_TUPLESET_HH

# include <iosfwd>
# include <istream>
# include <set>
# include <tuple>

#include <awali/common/version.hh>

#include <awali/sttc/config.hh> // AWALI_HAVE_CORRECT_LIST_INITIALIZER_ORDER
#include <awali/sttc/labelset/fwd.hh>
//#include <awali/sttc/labelset/labelset.hh>
#include <awali/sttc/misc/escape.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/core/kind.hh>

#include <awali/common/tuple.hh>
#include <awali/sttc/misc/cross.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/misc/zip.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/weightset/b.hh>
#include<awali/sttc/weightset/is_finite.hh>

namespace awali {
  namespace sttc {

    namespace internal
    {    
      /// A traits so that tupleset may define types that may exist.
      ///
      /// The types genset_t, letter_t, and word_t, exists only for
      /// tuples of labelsets, characterized as featuring a word_t type.
      template <typename Enable = void, typename... ValueSets>
      struct labelset_types
      {
        using genset_t = void;
        using letter_t = void;
        using word_t = void;
      };

      /// Specialization for tuples of labelsets.
      template <typename... ValueSets>
      struct labelset_types<decltype(pass{typename ValueSets::word_t()...}, void()),
                            ValueSets...>
      {
        using genset_t = internal::cross_sequences<decltype(std::declval<ValueSets>().genset())...>;
        using letter_t = std::tuple<typename ValueSets::letter_t...>;
        using word_t = std::tuple<typename ValueSets::word_t...>;
      };
    }

    template <typename... ValueSets>
    using labelset_types = internal::labelset_types<void, ValueSets...>;

    /// A ValueSet which is a Cartesian product of ValueSets.
    ///
    /// Exposes a LabelSet interface for products of LabelSets, and similarly
    /// for WeightSets.
    template <typename... ValueSets>
    class tupleset
    {
    public:
      using valuesets_t = std::tuple<ValueSets...>;
      using indices_t = awali::internal::make_index_sequence<sizeof...(ValueSets)>;
      static constexpr indices_t indices{};
      template <std::size_t... I>
      using seq = awali::internal::index_sequence<I...>;

      /// The Ith valueset type.
      template <std::size_t I>
      using valueset_t = typename std::tuple_element<I, valuesets_t>::type;

    public:
      using self_type = tupleset;
      using is_tupleset_t = bool;

      /// A tuple of values.
      using value_t = std::tuple<typename ValueSets::value_t...>;

      /// A tuple of letters if meaningful, void otherwise.
      using letter_t = typename labelset_types<ValueSets...>::letter_t;
      /// A tuple of gensets if meaningful, void otherwise.
      using genset_t = typename labelset_types<ValueSets...>::genset_t;
      /// A tuple of words if meaningful, void otherwise.
      using word_t = typename labelset_types<ValueSets...>::word_t;

      /// To be iterable.
      using value_type = letter_t;

      using kind_t = labels_are_tuples;

      tupleset(valuesets_t vs)
        : sets_(vs)
      {}

      tupleset(ValueSets... ls)
        : sets_(ls...)
      {}

      static std::string sname()
      {
        return sname_(indices);
      }

      std::string vname(bool full = true) const
      {
        return vname_(full, indices);
      }

      static constexpr std::size_t size()
      {
        return sizeof...(ValueSets);
      }

      template <std::size_t... I>
      static constexpr bool
      is_commutative_semiring()
      {
        return all_<valueset_t<I>::is_commutative_semiring()...>();
      }

      /// Build from the description in \a is.
      static tupleset make(std::istream& is)
      {
        // name: lat<law_char(abc), law_char(xyz)>
        kind_t::make(is);
        auto res = make_(is, indices);
        eat(is, '>');
        return res;
      }

      /// The componants valuesets, as a tuple.
      const valuesets_t& sets() const
      {
        return sets_;
      }

      /// The Ith component valueset.
      template <size_t I>
      const valueset_t<I>& set() const
      {
        return std::get<I>(sets());
      }

      /// Whether unknown letters should be added, or rejected.
      /// \param o   whether to accept
      /// \returns   the previous status.
      bool open(bool o) const
      {
        return open_(o, indices);
      }

      /// Construct a value.
      template <typename... Args>
      value_t value(const std::tuple<Args...>& args) const
      {
        return value_(args, indices);
      }

      /// The generators.  Meaningful for labelsets only.
      genset_t
      genset() const
      {
        return genset_(indices);
      }

      static constexpr bool is_free()
      {
        return is_free_(indices);
      }

    private:
      /// Must be declared before, as we use its result in decltype.
      template <typename Value, std::size_t... I>
      static auto
      letters_of_(const Value& v, seq<I...>)
        -> decltype(internal::zip(valueset_t<I>::letters_of(std::get<I>(v))...))
      {
        return internal::zip(valueset_t<I>::letters_of(std::get<I>(v))...);
      }

    public:
      /// Iterate over the letters of v.
      ///
      /// Templated by Value so that we work for both word_t and label_t.
      /// Besides, avoids the problem of instantiation with weighset that
      /// do not provide a word_t type.
      template <typename Value>
      static auto
      letters_of(const Value& v)
        -> decltype(letters_of_(v, indices))
      {
        return letters_of_(v, indices);
      }

      /// Convert to a word.
      template <typename... Args>
      auto
      word(const std::tuple<Args...>& v) const
        -> word_t
      {
        return word_(v, indices);
      }

      static bool
      equals(const value_t& l, const value_t& r)
      {
        return equals_(l, r, indices);
      }

      /// Whether \a l < \a r.
      static bool
      less_than(const value_t& l, const value_t& r)
      {
        return less_than_(l, r, indices);
      }

      static value_t
      special()
      {
        return special_(indices);
      }

      static bool
      is_special(const value_t& l)
      {
        return is_special_(l, indices);
      }

      value_t
      zero() const
      {
        return zero_(indices);
      }

      bool
      is_zero(const value_t& l) const
      {
        return is_zero_(l, indices);
      }

      static constexpr bool
      has_one()
      {
        return has_one_(indices);
      }

      static constexpr bool
      is_ratexpset()
      {
        return is_ratexpset_(indices);
      }

      static constexpr bool
      is_letterized()
      {
        return is_letterized_(indices);
      }

      static value_t
      one()
      {
        return one_(indices);
      }

      static bool
      is_one(const value_t& l)
      {
        return is_one_(l, indices);
      }

      static bool
      show_one()
      {
        return show_one_(indices);
      }

      bool
      is_letter(const value_t&) const
      {
        return false;
      }

      value_t
      add(const value_t& l, const value_t& r) const
      {
        return add_(l, r, indices);
      }

      value_t
      mul(const value_t& l, const value_t& r) const
      {
        return mul_(l, r, indices);
      }

      value_t
      rdiv(const value_t& l, const value_t& r) const
      {
        return rdiv_(l, r, indices);
      }

      value_t
      ldiv(const value_t& l, const value_t& r) const
      {
        return ldiv_(l, r, indices);
      }

      value_t
      star(const value_t& l) const
      {
        return star_(l, indices);
      }

      value_t
      plus(const value_t& l) const
      {
        return plus_(l, indices);
      }
      
      /// Add the special character first and last.
      ///
      /// Templated by Value so that we work for both word_t and label_t.
      /// Besides, avoids the problem of instantiation with weighset that
      /// do not provide a word_t type.
      template <typename Value>
      Value
      delimit(const Value& l) const
      {
        return delimit_(l, indices);
      }

      /// Remove first and last characters, that must be "special".
      template <typename Value>
      Value
      undelimit(const Value& l) const
      {
        return undelimit_(l, indices);
      }

      // FIXME: this needs to be computed.
      static constexpr star_status_t star_status()
      {
        return star_status_t::STARRABLE;
      }

      template <typename LhsValue, typename RhsValue>
      word_t
      concat(const LhsValue& l, const RhsValue& r) const
      {
        return concat_(l, r, indices);
      }

      value_t
      transpose(const value_t& l) const
      {
        return transpose_(l, indices);
      }

      static size_t
      hash(const value_t& v)
      {
        return hash_(v, indices);
      }

      static value_t
      conv(self_type, value_t v)
      {
        return v;
      }

      template<typename T>
      value_t
      conv(const T& tset, typename T::value_t v) const
      {
        return conv_(tset, v, indices);
      }

      value_t
      conv(b, b::value_t v) const
      {
        return v ? one() : zero();
      }

      /// Read one letter from i, return the corresponding value.
      value_t
      conv(std::istream& i) const
      {
        value_t res = conv_(i, indices);
        eat(i, ']');
        return res;
      }

      std::set<value_t> convs(std::istream&) const
      {
        raise("tupleset: ranges not implemented");
      }

      std::ostream&
      print_set(std::ostream& o, const std::string& format = "text") const
      {
        return print_set_(o, format, indices);
      }

      
      template<unsigned version = version::fsm_json>
      json::node_t* to_json() 
      const 
      {
        version::check_fsmjson<version>();
        switch (version) {
          case 1:
          default:
            return to_json<version>(indices);
        }
      }
      

      template<unsigned version = version::fsm_json>
      json::node_t* value_to_json(value_t v) 
      const 
      {
        version::check_fsmjson<version>();
        switch (version) {
          case 1:
          default:
            return value_to_json<version>(v, indices);
        }
      }



      template<unsigned version = version::fsm_json>
      value_t
      value_from_json(json::node_t const* p) const
      {
        version::check_fsmjson<version>();
        return value_from_json<version>(p->array()->values, indices);
      }

      value_t
      parse(const std::string& s, size_t& p, bool fixed_domain=true) const {
        if(s[--p]!=']')
          throw parse_exception("Invalid tuple");
#  if AWALI_HAVE_CORRECT_LIST_INITIALIZER_ORDER
        value_t r= parse_(s, p, fixed_domain, typename awali::internal::reverseseq<typename indices_t::type>::type());
#else
        value_t r= parse_(s, p, fixed_domain, indices);
#endif
        if(s[--p]!='[')
          throw parse_exception("Invalid tuple");
        return r;
      }

      template <std::size_t... I>
      void comma(const std::string& s, size_t& p, char& c) const {
        if(c=='x') {
          c=',';
          return;
        }
        if(s[--p]!=',')
          throw parse_exception("Invalid letter");
      }

      template <std::size_t... I>
      value_t
      parse_(const std::string& s, size_t& p, bool fixed_domain, seq<I...>) const {
        char c = 'x';
#  if AWALI_HAVE_CORRECT_LIST_INITIALIZER_ORDER
        value_t r=
          awali::internal::make_inv_tuple((comma<I>(s, p, c),
                                           std::get<I>(sets_).parse(s,p,fixed_domain))...);
#else
        value_t r =  std::make_tuple((comma<I>(s, p, c),
                                      set<I>().parse(s,p,fixed_domain))...);
#endif
        return r;
      }


      std::ostream&
      print(const value_t& l, std::ostream& o,
            const std::string& format = "text") const
      {
        return print_(l, o, format, indices);
      }

    private:
      template <std::size_t... I>
      static std::string sname_(seq<I...>)
      {
        std::string res = "lat<";
        const char *sep = "";
        for (auto n: {valueset_t<I>::sname()...})
          {
            res += sep;
            res += n;
            sep = ",";
          }
        res.push_back('>');
        return res;
      }

      template <typename T, std::size_t... I>
      value_t
      conv_(const T& tset, typename T::value_t v, seq<I...>) const
      {
        return std::make_tuple(std::get<I>(this->sets()).conv(std::get<I>(tset.sets()),std::get<I>(v))...);
      }


      template <std::size_t... I>
      std::string vname_(bool full, seq<I...>) const
      {
        std::string res = "lat<";
        const char *sep = "";
        for (auto n: {set<I>().vname(full)...})
          {
            res += sep;
            res += n;
            sep = ",";
          }
        res.push_back('>');
        return res;
      }

      template <std::size_t... I>
      static tupleset make_(std::istream& i, seq<I...>)
      {
#  if AWALI_HAVE_CORRECT_LIST_INITIALIZER_ORDER
        return tupleset{(eat_separator_<I>(i, '<', ','),
                         valueset_t<I>::make(i))...};
#  else
        return make_inv_tuple
          ((eat_separator_<sizeof...(ValueSets)-1 -I>(i, '<', ','),
            valueset_t<sizeof...(ValueSets)-1 -I>::make(i))...);
#  endif
      }

      template <std::size_t... I>
      bool open_(bool o, seq<I...>) const
      {
        using swallow = int[];
        (void) swallow { set<I>().open(o)... };
        std::swap(o, open__);
        return o;
      }

      template <typename... Args, std::size_t... I>
      value_t value_(const std::tuple<Args...>& args, seq<I...>) const
      {
        return value_t{set<I>().value(std::get<I>(args))...};
      }

      template <std::size_t... I>
      genset_t
      genset_(seq<I...>) const
      {
        return internal::cross(set<I>().genset()...);
      }

      template <std::size_t... I>
      static constexpr bool
      is_free_(seq<I...>)
      {
        return all_<valueset_t<I>::is_free()...>();
      }

      template <typename... Args, std::size_t... I>
      word_t
      word_(const std::tuple<Args...>& l, seq<I...>) const
      {
        return word_t{set<I>().word(std::get<I>(l))...};
      }

      template <std::size_t... I>
      static bool
      equals_(const value_t& l, const value_t& r, seq<I...>)
      {
        for (auto n: {valueset_t<I>::equals(std::get<I>(l),
                                            std::get<I>(r))...})
          if (!n)
            return false;
        return true;
      }

      template <std::size_t... I>
      static bool
      less_than_(const value_t& l, const value_t& r, seq<I...>)
      {
        for (auto n: {std::make_pair(valueset_t<I>::less_than(std::get<I>(l),
                                                              std::get<I>(r)),
                                     valueset_t<I>::less_than(std::get<I>(r),
                                                              std::get<I>(l)))...})
          if (n.first)
            return true;
          else if (n.second)
            return false;
        return false;
      }

      template <std::size_t... I>
      static std::size_t
      hash_(const value_t& v, seq<I...>)
      {
        std::size_t res = 0;
        for (auto h: {valueset_t<I>::hash(std::get<I>(v))...})
          std::hash_combine(res, h);
        return res;
      }

      template <std::size_t... I>
      static value_t
      special_(seq<I...>)
      {
        return std::make_tuple(valueset_t<I>::special()...);
      }

      template <std::size_t... I>
      static bool
      is_special_(const value_t& l, seq<I...>)
      {
        for (auto n: {valueset_t<I>::is_special(std::get<I>(l))...})
          if (!n)
            return false;
        return true;
      }

      template <std::size_t... I>
      value_t
      zero_(seq<I...>) const
      {
        return value_t{set<I>().zero()...};
      }

      template <std::size_t... I>
      bool
      is_zero_(const value_t& l, seq<I...>) const
      {
        for (auto n: {set<I>().is_zero(std::get<I>(l))...})
          if (!n)
            return false;
        return true;
      }

      template <std::size_t... I>
      static constexpr bool
      has_one_(seq<I...>)
      {
        return all_<valueset_t<I>::has_one()...>();
      }

      template <std::size_t... I>
      static constexpr bool
      is_ratexpset_(seq<I...>)
      {
        return all_<valueset_t<I>::is_ratexpset()...>();
      }

      template <std::size_t... I>
      static constexpr bool
      is_letterized_(seq<I...>)
      {
        return all_<valueset_t<I>::is_letterized()...>();
      }

      template <std::size_t... I>
      static value_t
      one_(seq<I...>)
      {
        return value_t{valueset_t<I>::one()...};
      }

      template <std::size_t... I>
      static bool
      is_one_(const value_t& l, seq<I...>)
      {
        for (auto n: {valueset_t<I>::is_one(std::get<I>(l))...})
          if (!n)
            return false;
        return true;
      }

      template <std::size_t... I>
      static bool
      show_one_(seq<I...>)
      {
        for (auto n: {valueset_t<I>::show_one()...})
          if (n)
            return true;
        return false;
      }

      template <std::size_t... I>
      value_t
      add_(const value_t& l, const value_t& r, seq<I...>) const
      {
        return value_t{set<I>().add(std::get<I>(l), std::get<I>(r))...};
      }

      template <std::size_t... I>
      value_t
      mul_(const value_t& l, const value_t& r, seq<I...>) const
      {
        return value_t{set<I>().mul(std::get<I>(l), std::get<I>(r))...};
      }

      template <std::size_t... I>
      value_t
      rdiv_(const value_t& l, const value_t& r, seq<I...>) const
      {
        return value_t{set<I>().rdiv(std::get<I>(l), std::get<I>(r))...};
      }

      template <std::size_t... I>
      value_t
      ldiv_(const value_t& l, const value_t& r, seq<I...>) const
      {
        return value_t{set<I>().ldiv(std::get<I>(l), std::get<I>(r))...};
      }

      template <std::size_t... I>
      value_t
      star_(value_t const& l, seq<I...>) const
      {
        return value_t{set<I>().star(std::get<I>(l))...};
      }

      template <std::size_t... I>
      value_t
      plus_(value_t const& l, seq<I...>) const
      {
        return value_t{set<I>().plus(std::get<I>(l))...};
      }

      template <typename Value, std::size_t... I>
      Value
      delimit_(Value const& l, seq<I...>) const
      {
        return Value{set<I>().delimit(std::get<I>(l))...};
      }

      template <typename Value, std::size_t... I>
      Value
      undelimit_(Value const& l, seq<I...>) const
      {
        return Value{set<I>().undelimit(std::get<I>(l))...};
      }

      template <typename LhsValue, typename RhsValue, std::size_t... I>
      word_t
      concat_(const LhsValue& l, const RhsValue& r, seq<I...>) const
      {
        return word_t{set<I>().concat(std::get<I>(l), std::get<I>(r))...};
      }

      template <std::size_t... I>
      value_t
      conv_(std::istream& i, seq<I...>) const
      {
#  if AWALI_HAVE_CORRECT_LIST_INITIALIZER_ORDER
        return std::make_tuple((eat_separator_<I>(i, '[', ','),
                                set<I>().conv(i))...);
#  else
        return
          awali::internal::make_inv_tuple((eat_separator_<sizeof...(ValueSets)-1 - I>(i, '[', ','),
                                           std::get<sizeof...(ValueSets)-1 - I>(sets_).conv(i))...);
#  endif
      }

      /// Read the separator from the input stream \a i.
      /// If \a I is 0, then the separator is '(',
      /// otherwise it is ',' (possibly followed by spaces).
      template <std::size_t I>
      static void
      eat_separator_(std::istream& i, char first, char tail)
      {
        eat(i, I == 0 ? first : tail);
        while (isspace(i.peek()))
          i.ignore();
      }


      template <std::size_t... I>
      std::ostream&
      print_(value_t const& l, std::ostream& o,
             const std::string& format, seq<I...>) const
      {
        if (!is_special(l))
          {
            using swallow = int[];
            (void) swallow
              {
                (o << (I == 0 ? '[' : ','),
                 set<I>().print(std::get<I>(l), o, format),
                 0)...
                  };
            o << ']';
          }
        return o;
      }

      template <std::size_t... I>
      std::ostream&
      print_set_(std::ostream& o, const std::string& format,
                 seq<I...>) const
      {
        const char *sep = "";
        if (format == "latex")
          sep = " \\times ";
        else if (format == "text")
          {
            o << "lat<";
            sep = ",";
          }
        else
          raise("invalid format: ", format);
        using swallow = int[];
        (void) swallow
          {
            (o << (I == 0 ? "" : sep),
             set<I>().print_set(o, format),
             0)...
              };
        if (format == "text")
          o << '>';
        return o;
      }

      template <unsigned version, std::size_t... I>
      value_t
      value_from_json(std::vector<json::node_t*> const& v, seq<I...>) 
      const 
      {return value_t{set<I>().template value_from_json<version>(v[I])...};}

      template <unsigned version,std::size_t... I>
      json::object_t*
      to_json(seq<I...>) const
      {
        switch(version) {
          case 1:
          default:
            json::object_t* obj = new json::object_t();
            obj->push_back("labelKind", new json::string_t("Multi-tape"));
            json::array_t* l = new json::array_t();
            for (auto x: {this->set<I>().template to_json<version>()...})
              l->push_back(x);
            obj->push_back("tapeLabels", l);
            return obj;
        }
      }
      

      template <unsigned version,std::size_t... I>
      json::array_t*
      value_to_json(value_t value, seq<I...> const) const
      {
        switch(version) {
          case 1:
          default:
            json::array_t* l = new json::array_t();
            for (auto a : {this->set<I>().template value_to_json<version>(std::get<I>(value))...})
              l->push_back(a);
            return l;
        }
      }


      template <std::size_t... I>
      value_t
      transpose_(value_t const& l, seq<I...>) const
      {
        return value_t{(set<I>().transpose(std::get<I>(l)))...};
      }

      /// The intersection with another tupleset.
      template <std::size_t... I>
      tupleset
      meet_(const tupleset& rhs, seq<I...>) const
      {
        return tupleset{meet(set<I>(), rhs.set<I>())...};
      }

      /// The join with another tupleset.
      template <std::size_t... I>
      tupleset
      join_(const tupleset& rhs, seq<I...>) const
      {
        return tupleset{join(set<I>(), rhs.set<I>())...};
      }

      /// The meet with another tupleset.
      friend
      tupleset
      meet(const tupleset& lhs, const tupleset& rhs)
      {
        return lhs.meet_(rhs, indices);
      }

      /// The meet with the B weightset.
      friend
      tupleset
      meet(const tupleset& lhs, const b&)
      {
        return lhs;
      }

      /// The meet with the B weightset.
      friend
      tupleset
      meet(const b&, const tupleset& rhs)
      {
        return rhs;
      }

      /// The join with another tupleset.
      friend
      tupleset
      join(const tupleset& lhs, const tupleset& rhs)
      {
        return lhs.join_(rhs, indices);
      }

      /// The join with the B weightset.
      friend
      tupleset
      join(const tupleset& lhs, const b&)
      {
        return lhs;
      }

      /// The join with the B weightset.
      friend
      tupleset
      join(const b&, const tupleset& rhs)
      {
        return rhs;
      }

      valuesets_t sets_;
      mutable bool open__ = false;
    };

    template<typename... Valuesets>
    auto
    make_tupleset(std::tuple<Valuesets...> t) -> tupleset<Valuesets...> {
      return {t};
    }


    namespace internal
    {
      template <typename T1, typename T2>
      struct concat_tupleset;

      template <typename... T1, typename... T2>
      struct concat_tupleset<tupleset<T1...>, tupleset<T2...>>
      {
        using type = tupleset<T1..., T2...>;
      };

    }


  }
}//end of ns awali::stc

#endif // !AWALI_LABELSET_TUPLESET_HH
