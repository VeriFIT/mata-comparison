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

#ifndef AWALI_WEIGHTSET_N_HH
#define AWALI_WEIGHTSET_N_HH

# include <ostream>
# include <string>
# include <sstream>

#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/weightset/lr_parse_number.hh>

namespace awali {
  namespace sttc {
    ///The semiring of Natural numbers
  class n
  {
  public:
    using self_type = n;

    static std::string sname()
    {
      return "n";
    }

    std::string vname(bool = true) const
    {
      return sname();
    }

    /// Build from the description in \a is.
    static n make(std::istream& is)
    {
      eat(is, sname());
      return {};
    }

    using value_t = unsigned int;

    constexpr static value_t
    zero()
    {
      return 0u;
    }

    constexpr static value_t
    one()
    {
      return 1u;
    }

    static value_t
    add(const value_t l, const value_t r)
    {
      return l + r;
    }

    static value_t
    sub(const value_t l, const value_t r)
    {
      require(l>=r, "N: sub: invalid subtraction: ", l , '-', r);
      return l - r;
    }

    static value_t
    mul(const value_t l, const value_t r)
    {
      return l * r;
    }

    static value_t
    rdiv(const value_t l, const value_t r)
    {
      require(!is_zero(r), "div: division by zero");
      require(!(l % r),
              "N: div: invalid division: ", l, '/', r);
      return l / r;
    }

    static value_t
    ldiv(const value_t l, const value_t r)
    {
      return rdiv(r, l);
    }

    value_t
    star(const value_t v) const
    {
      if (is_zero(v))
        return one();
      else
        raise("N: star: invalid value: ", format(*this, v));
    }

    value_t
    plus(const value_t v) const
    {
      if (is_zero(v))
        return zero();
      else
        raise("N: star: invalid value: ", format(*this, v));
    }

    constexpr static bool is_special(value_t)
    {
      return false;
    }

    static bool
    is_zero(const value_t v)
    {
      return v == 0u;
    }

    static bool
    is_one(const value_t v)
    {
      return v == 1u;
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

    static constexpr bool is_commutative_semiring() { return true; }

    static constexpr bool show_one() { return false; }
    static constexpr star_status_t star_status() { return star_status_t::NON_STARRABLE; }

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
    conv(b, b::value_t v)
    {
      // Conversion from bool to int.
      return v;
    }

    static value_t
    parse(const std::string & s, size_t& p) {
      return internal::lr_parse_int(s,p);
    }

    static value_t
    conv(std::istream& stream)
    {
      value_t res;
      if (stream >> res)
        return res;
      else
        sttc::fail_reading(stream, sname() + ": invalid value");
     }

    static std::ostream&
    print(const value_t v, std::ostream& o,
          const std::string& /*fmt*/ = "text")
    {
      return o << v;
    }

    static std::ostream&
    print_set(std::ostream& o, const std::string& format = "text")
    {
      if (format == "latex")
        o << "\\mathbb{N}";
      else if (format == "text")
        o << "N";
      else
        raise("invalid format: ", format);
      return o;
    }



    template<unsigned version = version::fsm_json>
    value_t
    static value_from_json(json::node_t const* p)
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          int a=p->to_int();
          if(a<0)
            throw parse_exception("[N] Negative integers are not allowed.");
          return a;
      }
    }
    

    template<unsigned version = version::fsm_json>
    json::node_t*
    value_to_json(value_t v) const
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
    json::object_t*
    to_json() const
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          json::object_t* obj = new json::object_t();
          obj->push_back("semiring",new json::string_t("N"));
          return obj;
      }
    }
  };

    inline n join(const n&, const n&) { return {}; }

    inline n join(const b&, const n&) { return {}; }
    inline n join(const n&, const b&) { return {}; }

}}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_N_HH
