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

#ifndef AWALI_WEIGHTSET_MAXMIN_HH
# define AWALI_WEIGHTSET_MAXMIN_HH

# include <ostream>
# include <string>
# include <sstream>
# include <utility>

#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
# include <limits>
#include <awali/sttc/misc/stream.hh> // eat
#include <awali/sttc/weightset/fwd.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/weightset/r.hh>
#include <awali/sttc/weightset/weightset.hh>

namespace awali {
  namespace sttc {
    /** The max-min semiring over floating numbers
     *
     * The type of values are double.
     * The semirings contains + and - infinity.
     */
  class maxmin
  {
  public:
    using self_type = maxmin;

    static std::string sname()
    {
      return "maxmin";
    }

    std::string vname(bool = true) const
    {
      return sname();
    }

    /// Build from the description in \a is.
    static maxmin make(std::istream& is)
    {
      eat(is, sname());
      return {};
    }

    using value_t = double;
    using locally_finite_t = bool;
    static const locally_finite_t locally_finite=true;
    
    static value_t
    add(const value_t l, const value_t r)
    {
      return std::max(l, r);
    }

    static value_t
    mul(const value_t l, const value_t r)
    {
      return std::min(l, r);
    }

    static value_t
    rdiv(const value_t l, const value_t r)
    {
      raise(sname(), "No division");
    }

    static value_t
    ldiv(const value_t l, const value_t r)
    {
      raise(sname(), "No division");
    }

    value_t
    star(const value_t v) const
    {
      return one();
    }

    value_t
    plus(const value_t v) const
    {
      return v;
    }

    static value_t
    one()
    {
      return std::numeric_limits<value_t>::max();
    }

    static value_t
    zero()
    {
      return std::numeric_limits<value_t>::min();
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
      return v == zero();
    }

    static bool
    is_one(const value_t v)
    {
      return v == one();
    }

    static constexpr bool is_commutative_semiring() { return true; }

    static constexpr bool show_one() { return true; }
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
            return one();
          else
            throw std::domain_error(sname() + ": invalid value: o" + str_escape(i));
        case '-':
          if ((i = stream.get()) == 'o')
            if ((i = stream.get()) == 'o')
              return zero();
            else
              throw std::domain_error(sname() + ": invalid value: -o" + str_escape(i));
          else
            if (stream >> i)
              return -i;
            else
              sttc::fail_reading(stream, sname() + ": invalid value");
        default:
          if (stream >> i)
            return i;
          else
            sttc::fail_reading(stream, sname() + ": invalid value");
        }
    }

    static value_t
    parse(const std::string & s, size_t& p) {
      // TODO !!
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
      if (is_zero(v)) {
	if(format == "dot")
	  o << "_";
	else
	  o << "\"-oo\"";
      }
      else
        if (is_one(v)) {
	  if(format == "dot")
	    o << "T";
	  else
	    o << "\"oo\"";
	}
        else
          o << v;
      return o;
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        o << "\\mathbb{R}_{\\max,\\min}";
      else if (format == "text")
        o << "Fuzzy";
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
          throw parse_exception("[Fuzzy] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
        json::object_t* obj = new json::object_t();
        obj->push_back("semiring",new json::string_t("Fuzzy"));
        return (obj);
      }
    }


    template<unsigned version = version::fsm_json>
    json::node_t* value_to_json(value_t v) 
    const
    {
      switch (version) {
        case 0:
          throw parse_exception("[Fuzzy] Unsupported fsm-json version:"
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
          try {
            return p->to_double();
          } catch (json::coercion_exception&) {}
          try {
            std::string str = p->to_string();
            if (str == "oo")
              return zero();
            if (str == "-oo")
              return one();
          } catch (json::coercion_exception&) {}
          throw json::coercion_exception(
            "Cannot be coerced to a value of Z-min-max", 
            "maxmin::value_from_json", 
            p);
      }
    }
    
  };
    
    inline maxmin join(const maxmin&, const maxmin&) { return {}; }

    inline maxmin join(const b&, const maxmin&) { return {}; }
    inline maxmin join(const maxmin&, const b&) { return {}; }

}}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_MAXMIN_HH
