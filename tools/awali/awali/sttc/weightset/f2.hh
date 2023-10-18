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

#ifndef AWALI_WEIGHTSET_F2_HH
# define AWALI_WEIGHTSET_F2_HH

# include <cassert>
# include <ostream>

#include <awali/sttc/misc/escape.hh>
#include <awali/utils/hash.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/weightset/weightset.hh>

namespace awali {
  namespace sttc {
    /** The field F2
     *
     * The type of values is bool.
     */
  class f2
  {
  public:
    using self_type = f2;

    static std::string sname()
    {
      return "f2";
    }

    std::string vname(bool = true) const
    {
      return sname();
    }

    /// Build from the description in \a is.
    static f2 make(std::istream& is)
    {
      eat(is, sname());
      return {};
    }

    using value_t = bool;
    using finite_t = bool;
    static const finite_t finite=true;

    static value_t
    zero()
    {
      return false;
    }

    static value_t
    one()
    {
      return true;
    }

    static value_t
    add(const value_t l, const value_t r)
    {
      return l ^ r;
    }

    static value_t
    sub(const value_t l, const value_t r)
    {
      return l ^ r;
    }

    static value_t
    mul(const value_t l, const value_t r)
    {
      return l && r;
    }

    static value_t
    rdiv(const value_t l, const value_t r)
    {
      require(!is_zero(r), "div: division by zero");
      return l;
    }

    static value_t
    ldiv(const value_t l, const value_t r)
    {
      return rdiv(r, l);
    }

    static value_t
    star(const value_t v)
    {
      if (!v)
        return true;
      else
        throw std::domain_error("f2: star: invalid value: 1");
    }

    static value_t
    plus(const value_t v)
    {
      if (!v)
        return false;
      else
        throw std::domain_error("f2: star: invalid value: 1");
    }
    
    static bool
    equals(const value_t l, const value_t r)
    {
      return l == r;
    }

    /// Whether \a lhs < \a rhs.
    static bool less_than(value_t lhs, value_t rhs)
    {
      return lhs < rhs;
    }

    constexpr static bool is_special(value_t)
    {
      return false;
    }

    static bool
    is_zero(const value_t v)
    {
      return !v;
    }

    static bool
    is_one(const value_t v)
    {
      return v;
    }

    static constexpr bool is_commutative_semiring() { return true; }

    static constexpr bool show_one() { return false; }

    static constexpr
    star_status_t star_status() { return star_status_t::NON_STARRABLE; }

    static value_t
    transpose(const value_t v)
    {
      return v;
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

    static value_t
    conv(std::istream& stream)
    {
      int i;
      if ((stream >> i) && (i == 0 || i == 1))
        return i;
      else
        sttc::fail_reading(stream, sname() + ": invalid value");
    }

    static value_t
    parse(const std::string & s, size_t& p) {
      if ((p>=4)&&(!s.compare(p-4,4,"true"))) {
          p -= 4;
          return true;
      }
      if ((p>=5)&&(!s.compare(p-5,5,"false"))) {
          p -= 5;
          return false;
      }
      if(s[--p] == '0')
        return false;
      if(s[p] == '1')
        return true;
      p++;
      throw parse_exception("Wrong F2 value");
    }

    static std::ostream&
    print(const value_t v, std::ostream& o,
          const std::string& = "text")
    {
      return o << (v ? '1' : '0');
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        o << "\\mathbb{F}_2";
      else if (format == "text")
        o << "F2";
      else
        raise("invalid format: ", format);
      return o;
    }

    
    template<unsigned version = version::fsm_json>
    json::node_t* to_json()
    const
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          json::object_t* obj = new json::object_t();
          obj->push_back("semiring", new json::string_t("F2"));
          return (obj);
      }
    }

    template<unsigned version = version::fsm_json>
    json::node_t* value_to_json(value_t v) 
    const
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          return new json::int_t(v);
      }
    }
    
    template<unsigned version = version::fsm_json>
    value_t value_from_json(json::node_t const* p) 
    const
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          return p->to_bool();
      }
    }

  };

  inline f2 join(const f2&, const f2&) { return {}; }


}}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_F2_HH
