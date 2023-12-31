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

#ifndef AWALI_WEIGHTSET_ZMIN_HH
# define AWALI_WEIGHTSET_ZMIN_HH

# include <limits>
# include <ostream>
# include <string>
# include <sstream>
# include <utility>

#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh> // eat
#include <awali/sttc/weightset/fwd.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/weightset/weightset.hh>

namespace awali {
  namespace sttc {

    /** The Z-min-plus semiring
     *
     * The type of values are int.
     * The semiring contains infinity.
     */
  class zmin
  {
  public:
    using self_type = zmin;

    static std::string sname()
    {
      return "zmin";
    }

    std::string vname(bool = true) const
    {
      return sname();
    }

    /// Build from the description in \a is.
    static zmin make(std::istream& is)
    {
      eat(is, sname());
      return {};
    }

    using value_t = int;

    static value_t
    add(const value_t l, const value_t r)
    {
      return std::min(l, r);
    }

    static value_t
    mul(const value_t l, const value_t r)
    {
      return (is_zero(l) || is_zero(r) ? zero()
              : l + r);
    }

    static value_t
    rdiv(const value_t l, const value_t r)
    {
      require(!is_zero(r), "div: division by zero");
      return l - r;
    }

    static value_t
    ldiv(const value_t l, const value_t r)
    {
      return rdiv(r, l);
    }

    value_t
    star(const value_t v) const
    {
      if (0 <= v)
        return one();
      else
        raise(sname(), ": star: invalid value: ", format(*this, v));
    }

    value_t
    plus(const value_t v) const
    {
      if (0 <= v)
        return v;
      else
        raise(sname(), ": star: invalid value: ", format(*this, v));
    }

    static value_t
    one()
    {
      return 0;
    }

    static value_t
    zero()
    {
      return std::numeric_limits<value_t>::max();
    }

    static bool
    equals(const value_t l, const value_t r)
    {
      return l == r;
    }

    /// Whether \a lhs < \a rhs.
    static bool less_than(value_t lhs, value_t rhs)
    {
      return lhs > rhs;
    }

    constexpr static bool is_special(value_t)
    {
      return false;
    }

    static bool
    is_zero(const value_t v)
    {
      return v == zero();
    }

    static bool
    is_one(const value_t v)
    {
      return v == one();
    }

    static constexpr bool is_commutative_semiring() { return true; }

    static constexpr bool show_one() { return true; }
    static constexpr star_status_t star_status() { return star_status_t::TOPS; }

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
      return v ? one() : zero();
    }

    static value_t
    conv(std::istream& stream)
    {
      switch (int i = stream.peek())
        {
        case 'o':
          stream.ignore();
          if ((i = stream.get()) == 'o')
            return zero();
          else
            throw std::domain_error(sname() + ": invalid value: o" + str_escape(i));
        default:
          if (stream >> i)
            return i;
          else
            sttc::fail_reading(stream, sname() + ": invalid value");
        }
    }

    static value_t
    parse(const std::string & s, size_t& p) {
      if(p>=2 && s[p-2]=='o' && s[p-1]=='o'){
        p -= 2;
        return zero();
      }
      size_t i=p;
      for(; i>0 && s[i-1]>='0' && s[i-1]<='9'; --i)
        ;
      if(i==p)
        throw parse_exception("Wrong Z-min value");
      if(i>0 && (s[i-1]=='-' || s[i-1]=='+'))
        --i;
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
      if(format == "json") {
        if (is_zero(v))
          o << "\"oo\"";
        else
          o<< v;
        return o;
      }
      if (is_zero(v))
        o << (format == "latex" ? "\\infty" : "oo");
      else
        o << v;
      return o;
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        o << "\\mathbb{Z}_{min}";
      else if (format == "text")
        o << "Z-min-plus";
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
          try {
            return p->to_int();
          } catch (json::coercion_exception&) {}
          try {
            if (p->to_string() == "oo")
              return zero();
          } catch (json::coercion_exception&) {}
          std::stringstream ss;
          ss << "[zmin] At position " << p->path_to_root() 
             << " node cannot be coerced to a Z-min-plus value.";
          throw json::coercion_exception(ss.str());
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
          if (v == zero())
            return new json::string_t("oo");
          return new json::int_t(v);
      }
    }

    template<unsigned version = version::fsm_json>
    json::node_t*
    to_json() const
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          return new json::object_t("semiring",new json::string_t("Z-min-plus"));
      }
    }

  };

    inline zmin join(const zmin&, const zmin&) { return {}; }

    inline zmin join(const b&, const zmin&) { return {}; }
    inline zmin join(const zmin&, const b&) { return {}; }

  }}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_ZMIN_HH
