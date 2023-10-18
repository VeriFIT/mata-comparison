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

#ifndef VAUCR_WEIGHTSET_Z_HH
# define VAUCR_WEIGHTSET_Z_HH

# include <ostream>
# include <string>
# include <sstream>

#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/weightset/fwd.hh>

namespace awali { namespace sttc {

  namespace internal
  {
  class z_impl
  {
  public:
    using self_type = z;

    static std::string sname()
    {
      return "z";
    }

    std::string vname(bool = true) const
    {
      return sname();
    }

    /// Build from the description in \a is.
    static z make(std::istream& is)
    {
      eat(is, sname());
      return {};
    }

    using value_t = int;

    static value_t
    zero()
    {
      return 0;
    }

    static value_t
    one()
    {
      return 1;
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
      require(!(l % r),
              "z: div: invalid division: ", l, '/', r);
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
        raise("z: star: invalid value: ", format(*this, v));
    }

    value_t
    plus(const value_t v) const
    {
      if (is_zero(v))
        return zero();
      else
        raise("z: star: invalid value: ", format(*this, v));
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

    value_t
    parse(const std::string & s, size_t& p) const {
      size_t i=p;
      for(; i>0 && s[i-1]>='0' && s[i-1]<='9'; --i)
        ;
      if(i==p)
        throw parse_exception("Wrong Z value");
      if(i>0 && (s[i-1]=='-' || s[i-1]=='+'))
        --i;
      std::istringstream st(s.substr(i, p-i));
      value_t x;
      st >> x;
      p=i;
      return x;
    }

    static value_t
    conv(std::istream& stream)
    {
      int res;
      if (stream >> res)
        return res;
      else
        sttc::fail_reading(stream, sname() + ": invalid value");
     }

    static std::ostream&
    print(const value_t v, std::ostream& o,
          const std::string& fmt = "text")
    {
      if(fmt == "json")
        return o<< '"' << v << '"';
      return o << v;
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        o << "\\mathbb{Z}";
      else if (format == "text")
        o << "Z";
      else
        raise("invalid format: ", format);
      return o;
    }

    std::ostream&
    js_print(std::ostream& o) const
    {
      o << "{\"Semiring\":\"Z\"}";
      return o;
    }

    value_t
    js_parse(std::istream& i) const
    {
      char c;
      i >> c;
      if(c != '"')
        throw std::runtime_error("json parser Z");
          value_t r;
          i >> r;
      i >> c;
      if(c != '"')
        throw std::runtime_error("json parser Z");
      return r;
    }
  };
  }

  VAUCR_WEIGHTS_BINARY(z, z, z);

  VAUCR_WEIGHTS_BINARY(b, z, z);
  VAUCR_WEIGHTS_BINARY(z, b, z);


}}//end of ns awali::stc

#endif // !VAUCR_WEIGHTSET_Z_HH
