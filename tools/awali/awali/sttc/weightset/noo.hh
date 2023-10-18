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

#ifndef AWALI_WEIGHTSET_NOO_HH
# define AWALI_WEIGHTSET_NOO_HH

# include <ostream>
# include <string>
# include <sstream>
# include <limits>

#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/weightset/weightset.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/weightset/fwd.hh>

namespace awali {
  namespace sttc {
    /** The semiring of extended natural numbers
     *
     * This semiring includes infinity.
     */
  class noo
  {
  public:
    using self_type = noo;

    static std::string sname()
    {
      return "noo";
    }

    std::string vname(bool = true) const
    {
      return sname();
    }

    /// Build from the description in \a is.
    static noo make(std::istream& is)
    {
      eat(is, sname());
      return {};
    }

    using value_t = unsigned ;

    constexpr static value_t
    zero()
    {
      return 0u;
    }

    constexpr static value_t
    infinity()
    {
      return std::numeric_limits<value_t>::max();
    }
    
    constexpr static value_t
    one()
    {
      return 1u;
    }

    static value_t
    add(const value_t l, const value_t r)
    {
      if(is_infinity(l) || is_infinity(r))
        return infinity();
      return l + r;
    }

    static value_t
    sub(const value_t l, const value_t r)
    {
      require(l>=r, "N: sub: invalid subtraction: ", l , '-', r);
      require(!is_infinity(r), "N: sub: invalid subtraction: ", l , '-', r);
      if(is_infinity(l))
        return l;
      return l - r;
    }

    static value_t
    mul(const value_t l, const value_t r)
    {
      if(is_zero(l) || is_zero(r))
        return zero();
      if(is_infinity(l) || is_infinity(r))
        return infinity();
      return l * r;
    }

    static value_t
    rdiv(const value_t l, const value_t r)
    {
      require(!is_zero(r), "div: division by zero");
      if(is_infinity(r))
        return zero();
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
      return infinity();
    }

    value_t
    plus(const value_t v) const
    {
      if (is_zero(v))
        return zero();
      return infinity();
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
    is_infinity(const value_t v)
    {
      return v == infinity();
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
    static constexpr star_status_t star_status() { return star_status_t::STARRABLE; }

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
      if(s[p-1]=='o'){
        --p;
        if(s[p-1]=='o')
          --p;
        else
          throw parse_exception("Wrong N-oo value");
        return infinity();
      }
      size_t i=p;
      for(; i>0 && s[i-1]>='0' && s[i-1]<='9'; --i)
        ;
      if(i==p)
        throw parse_exception("Wrong N value");
      std::istringstream st(s.substr(i, p-i));
      value_t x;
      st >> x;
      p=i;
      return x;
    }

    static value_t
    conv(std::istream& stream)
    {
      if(stream.peek()=='o') {
        stream.get();
        if(stream.get()=='o')
          return infinity();
        sttc::fail_reading(stream, sname() + ": invalid value");
      }
      value_t res;
      if (stream >> res)
        return res;
      else
        sttc::fail_reading(stream, sname() + ": invalid value");
     }

    static std::ostream&
    print(const value_t v, std::ostream& o,
          const std::string& fmt = "text")
    {
      if(is_infinity(v)) {
        if(fmt == "json")
          return o<< "\"oo\"";
        else
          return o << "oo";
      }
      return o << v;
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        o << "\\mathbb{N}_\\infty";
      else if (format == "text")
        o << "N-oo";
      else
        raise("invalid format: ", format);
      return o;
    }

    template<unsigned version = version::fsm_json>
    static json::node_t* 
    to_json()
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          return new json::object_t("semiring",new json::string_t("N-oo"));
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
          if (is_infinity(v))
            return new json::string_t("oo");
          else
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
          try {
            int i = p->to_int();
            if(i>= 0)
              return i;
            break;
          } catch (json::coercion_exception&) {}
          try {
            if (p->to_string() == "oo")
              return zero();
          } catch (json::coercion_exception&) {}
      }
      std::stringstream ss;
      ss << "[noo] At position " << p->path_to_root() 
         << " node cannot be coerced to a N-oo value.";
      throw json::coercion_exception(ss.str());
    }

  };

    inline noo join(const noo&, const noo&) { return {}; }

    inline noo join(const b&, const noo&) { return {}; }
    inline noo join(const noo&, const b&) { return {}; }

}}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_NOO_HH
