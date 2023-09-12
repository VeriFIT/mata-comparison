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

#ifndef AWALI_WEIGHTSET_R_HH
# define AWALI_WEIGHTSET_R_HH

# include <string>
# include <ostream>

#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/weightset/q.hh>

namespace awali {
  namespace sttc {
    /** The semiring of floating Numbers
     *
     * The values are implemented with double.
     */
    class r {
  public:
    using self_type = r;

    static std::string sname()
    {
      return "r";
    }

    std::string vname(bool = true) const
    {
      return sname();
    }

    /// Build from the description in \a is.
    static r make(std::istream& is)
    {
      eat(is, sname());
      return {};
    }

    using value_t = double;

    static value_t
    zero()
    {
      return 0.;
    }

    static value_t
    one()
    {
      return 1.;
    }

    static value_t
    add(const value_t l, const value_t r)
    {
      return l + r;
    }

    static value_t
    sub(const value_t l, const value_t r)
    {
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
      if (-1 < v && v < 1)
        return 1/(1-v);
      else
        raise(sname(), ": star: invalid value: ", format(*this, v));
    }

    constexpr static bool is_special(value_t)
    {
      return false;
    }

    static bool
    is_zero(const value_t v)
    {
      return v == 0;
    }

    static bool
    is_one(const value_t v)
    {
      return v == 1;
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
    static constexpr star_status_t star_status() { return star_status_t::ABSVAL; }

    static value_t
    abs(const value_t v)
    {
      return v < 0 ? -v : v;
    }

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
    conv(q, q::value_t v)
    {
      return value_t(v.num) / value_t(v.den);
    }

    static value_t
    conv(z, z::value_t v)
    {
      return v;
    }

    static value_t
    conv(n, n::value_t v)
    {
      return (value_t)v;
    }

    static value_t
    conv(b, b::value_t v)
    {
      return v;
    }

    static value_t
    conv(std::istream& i)
    {
      value_t res;
      if (i >> res)
        return res;
      else
        fail_reading(i, sname() + ": invalid value");
    }

    static value_t
    parse(const std::string & s, size_t& p) {
      size_t i=p;
      for(; i>0 && ((s[i-1]>='0' && s[i-1]<='9') || s[i-1]=='e' || s[i-1]=='E' || s[i-1]=='.' || s[i-1]=='+' || s[i-1]=='-'); --i)
        ;
      if(i==p)
        throw parse_exception("Wrong R value");
      std::istringstream st(s.substr(i, p-i));
      value_t x;
      st >> x;
      p=i;
      return x;
    }

    static std::ostream&
    print(const value_t v, std::ostream& o,
          const std::string& format = "text")
    {
      o << v;
      return o;
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        o << "\\mathbb{R}";
      else if (format == "text")
        o << "R";
      else
        raise("invalid format: ", format);
      return o;
    }
    
    template<unsigned version = version::fsm_json>
    static
    json::node_t* to_json()
    {
      switch (version) {
        case 0:
          throw parse_exception("[R] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          return new json::object_t("semiring",new json::string_t("R"));
      }
    }

    template<unsigned version = version::fsm_json>
    json::node_t* value_to_json(value_t v) 
    const
    {
      switch (version) {
        case 0:
          throw parse_exception("[R] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          return new json::float_t(v);
      }
    }
    
    template<unsigned version = version::fsm_json>
    value_t value_from_json(json::node_t const* p) 
    const
    {
      switch (version) {
        case 0:
        case 1:
        default:
          return p->to_double();
      }
    }
    
  };

  inline r join(const r&, const r&) { return {}; }

  inline r join(const q&, const r&) { return {}; }
  inline r join(const r&, const q&) { return {}; }

  inline r join(const z&, const r&) { return {}; }
  inline r join(const r&, const z&) { return {}; }

  inline r join(const n&, const r&) { return {}; }
  inline r join(const r&, const n&) { return {}; }

  inline r join(const b&, const r&) { return {}; }
  inline r join(const r&, const b&) { return {}; }

}}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_R_HH
