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

#ifndef AWALI_LABELSET_ONESET_HH
# define AWALI_LABELSET_ONESET_HH

# include <iostream>
# include <set>
# include <stdexcept>

#include <awali/common/version.hh>
#include <awali/common/json/node.cc>
#include <awali/common/parse_exception.cc>
#include <awali/sttc/core/kind.hh>
#include <awali/sttc/empty.hh>
//#include <awali/sttc/labelset/labelset.hh>
#include <awali/utils/hash.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc {

  /// Implementation of labels are ones: there is a single instance
  /// of label.
  class oneset
  {
  public:
    using self_type = oneset;
    using value_t = empty_t;

    using kind_t = labels_are_one;

    oneset() = default;

    static std::string sname()
    {
      return "lao";
    }

    static std::string vname(bool = true)
    {
      return sname();
    }

    /// Build from the description in \a is.
    static oneset make(std::istream& is)
    {
      // name: lao_ratexpset<law_char(xyz)_b>
      //       ^^^ ^^^^^^^^^^^^^^^^^^^^^^^^^^
      //       kind         weightset
      //
      // There is no "char(...)_".
      kind_t::make(is);
      return {};
    }

    /// Does not make a lot of sense.
    bool open(bool o) const
    {
      return !o;
    }

    static constexpr bool is_free()
    {
      // This is debatable.  However, in Vaucanson, if a labelset
      // is_free, then we expect to be able to iterate on its genset,
      // and I don't plan to provide a genset here.
      return false;
    }

    /// Value constructor.
    template <typename... Args>
    value_t value(Args&&... args) const
    {
      return value_t{std::forward<Args>(args)...};
    }

    /// Whether \a l == \a r.
    static bool
    equals(const value_t, const value_t)
    {
      return true;
    }

    /// Whether \a l < \a r.
    static bool less_than(const value_t, const value_t)
    {
      return false;
    }

    static value_t special()
    {
      return {};
    }

    /// The special label is indistinguishable for the others.
    constexpr static bool
    is_special(value_t)
    {
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
      return true;
    }

    static empty_t one()
    {
      return {};
    }

    static bool is_one(empty_t)
    {
      return true;
    }

    static empty_t transpose(empty_t)
    {
      return {};
    }

    static bool is_letter(empty_t)
    {
      return false;
    }

    template<typename T>
    static bool has(const T&)
    {
      return false;
    }

    static empty_t concat(empty_t, empty_t)
    {
      return {};
    }

    static std::ostream& print(empty_t, std::ostream& o,
                               const std::string& = "text")
    {
      return o;
    }

    static size_t size(value_t)
    {
      return 0;
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

    static value_t conv(std::istream& i)
    {
      if (i.peek() == '\\')
        {
          i.ignore();
          char c = i.peek();
          require(c == 'e',
                  "invalid label: unexpected \\", c);
          i.ignore();
        }
      return {};
    }

    static std::set<value_t> convs(std::istream&)
    {
      raise("oneset: ranges not implemented");
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        o << "1";
      else if (format == "text")
        o << vname(true);
      else
        raise("invalid format: ", format);
      return o;
    }

    std::set<value_t> genset() const {
        return std::set<value_t>();
    }

    
    template<unsigned version = version::fsm_json>
    static 
    json::node_t* to_json()
    {
      switch (version) {
        case 0:
          throw parse_exception("[One] Unsupported fsm-json version: "
                                + std::to_string(version));
        case 1:
        default:
          json::object_t* obj = new json::object_t();
          obj->push_back("labelKind", new json::string_t("No Label"));
          return obj;
      }
    }

    template<unsigned version = version::fsm_json>
    json::node_t* value_to_json(value_t) const
    {
      switch (version) {
        case 0:
          throw parse_exception("[One] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          return new json::string_t("");
      }
    }

    template<unsigned version = version::fsm_json>
    value_t value_from_json(json::node_t const* p) const {
      switch (version) {
        case 0:
          throw parse_exception("[One] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          std::string s = p->to_string();
          require(s.empty(),"json parser Letterset","empty string expected");
          return one();
      }
    }

    value_t
    parse(const std::string & s, size_t& p, bool fixed_alphabet=true) const {
      return one();
    }

  };

  /// The meet of two labelsets.
  inline
  oneset
  meet(const oneset&, const oneset&)
  {
    return {};
  }

  /// The union of two labelsets.
  inline
  oneset
  join(const oneset&, const oneset&)
  {
    return {};
  }


}}//end of ns awali::stc

#endif // !AWALI_LABELSET_ONESET_HH
