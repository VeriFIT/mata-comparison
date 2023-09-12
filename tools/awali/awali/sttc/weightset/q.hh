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

#ifndef AWALI_WEIGHTSET_Q_HH
#define AWALI_WEIGHTSET_Q_HH

#include <string>
#include <ostream>

#include <awali/utils/arith.hh>

#include <awali/common/qfraction.cc>
#include <awali/common/enums.hh>

#include <awali/sttc/misc/attributes.hh>
#include <awali/utils/hash.hh>

#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/misc/stream.hh> // eat
#include <awali/sttc/weightset/z.hh>

namespace awali {
  namespace sttc {
    /**The semiring of rational numbers
     *
     * The values are q_fraction_t
     */
      class q {
      public:
        using self_type = q;

        static std::string sname()
        {
          return "q";
        }

        std::string vname(bool = true) const
        {
          return sname();
        }

        /// Build from the description in \a is.
        static q make(std::istream& is)
        {
          eat(is, sname());
          return {};
        }

        using value_t = q_fraction_t;

        static unsigned int abs(int a)
        {
          return a < 0 ? -a : a;
        }


        static value_t zero()
        {
          return value_t{0, 1};
        }

        static value_t one()
        {
          return value_t{1, 1};
        }

        static value_t add(const value_t l, const value_t r)
        {
          unsigned int cm = utils::lcm(l.den, abs(r.den));
          return value_t{l.num * int (cm / l.den) + r.num * int (cm / r.den),
              cm}.reduce();
        }

        static value_t sub(const value_t l, const value_t r)
        {
          unsigned int cm = utils::lcm(l.den, abs(r.den));
          return value_t{l.num * int (cm / l.den) - r.num * int (cm / r.den),
              cm}.reduce();
        }

        static value_t mul(const value_t l, const value_t r)
        {
          return value_t{l.num * r.num, l.den * r.den}.reduce();
        }

        static value_t
        rdiv(const value_t l, const value_t r)
        {
          require(!is_zero(r), "div: division by zero");
          if (0 < r.num)
            return value_t{l.num * int(r.den), l.den * r.num}.reduce();
          else
            return value_t{-l.num * int(r.den), l.den * -r.num}.reduce();
        }

        static value_t
        ldiv(const value_t l, const value_t r)
        {
          return rdiv(r, l);
        }

        value_t star(const value_t v) const
        {
          // Bad casting when v.den is too big
          if (abs(v.num) < v.den)
            // No need to reduce: numerator and denominators are primes.
            return {int(v.den), v.den - v.num};
          else
            raise(sname(), ": star: invalid value: ", format(*this, v));
        }

        static bool is_special(const value_t) // C++11: cannot be constexpr.
        {
          return false;
        }

        static bool is_zero(const value_t v)
        {
          return v.num == 0;
        }

        static bool is_one(const value_t v)
        {
          // All values are normalized.
          return v.num == 1 && v.den == 1;
        }

        static bool equals(const value_t l, const value_t r)
        {
          return l==r;
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
          return v.num < 0 ? (value_t{-v.num, v.den}) : v;
        }

        static value_t
        transpose(const value_t v)
        {
          return v;
        }

        static size_t hash(value_t v)
        {
          size_t res = 0;
          std::hash_combine(res, utils::hash_value(v.num));
          std::hash_combine(res, utils::hash_value(v.den));
          return res;
        }

        static value_t
        conv(self_type, value_t v)
        {
          return v;
        }

        static value_t
        conv(z, z::value_t v)
        {
          return {v, 1};
        }

        static value_t
        conv(n, n::value_t v)
        {
          return {(int)v, 1};
        }

        static value_t
        conv(b, b::value_t v)
        {
          return {v, 1};
        }

        static value_t
        conv(std::istream& i)
        {
          int num;
          if (! (i >> num))
            fail_reading(i, sname() + ": invalid numerator");

          // If we have a slash after the numerator then we have a
          // denominator as well.
          char maybe_slash;
          if ((maybe_slash = i.peek()) != '/')
            return value_t{num, 1};
          eat(i, '/');

          // operator>> with an istream and an unsigned int silently
          // mangles a negative number into its two's complement
          // representation as a positive number.
          if (i.peek() == '-')
            {
              num = - num;
              eat(i, '-');
            }

          unsigned int den;
          if (i >> den)
            {
              // Make sure our rational respects our constraints.
              if (den == 0)
                throw std::domain_error(sname() + ": zero denominator");
              return value_t{num, den}.reduce();
            }
          else
            fail_reading(i, sname() + ": invalid denominator");
        }

        static value_t
        parse(const std::string & s, size_t& p) {
          size_t i;

    //Compute denominator
    unsigned int y = 1;
    for(i=p; i>0 && (s[i-1]>='0' && s[i-1]<='9');--i)
            ;
          if(i==p)
            throw parse_exception("Wrong Q value (right):"+s);
    
    if (i>0 && s[i-1] == '/') { //There is a denominator
            std::istringstream sty(s.substr(i, p-i));
            sty >> y;
      p=i-1;
    }

    //Compute numerator
    int x;
    for(i=p; i>0 && (s[i-1]>='0' && s[i-1]<='9');--i)
            ;

    if (i==p)
      throw parse_exception("Wrong Q value (left):"+s);
          if(i>0 && (s[i-1]=='-' || s[i-1]=='+'))
            --i;
          std::istringstream stx(s.substr(i, p-i));
          stx >> x;
          p=i;
          return value_t{x,y};
        }
        

        static std::ostream&
        print(const value_t v, std::ostream& o,
              const std::string& format = "text")
        {
          if (format == "json") {
            if(v.den ==1u)
              return o << v.num;
            o<< "{\"num\":" << v.num << ", \"den\":" << v.den << '}';
            return o;
          }
          if (format == "latex")
            {
              if (v.den == 1)
                o << v.num;
              else
                o << "\\frac{" << v.num << "}{" << v.den << '}';
            }
          else
            {
              o << v.num;
              if (v.den != 1)
                o << '/' << v.den;
            }
          return o;
        }

        std::ostream&
        print_set(std::ostream& o, const std::string& format = "text") const
        {
          if (format == "latex")
            o << "\\mathbb{Q}";
          else if (format == "text")
            o << "Q";
          else
            raise("invalid format: ", format);
          return o;
        }
    
    template<unsigned version = version::fsm_json>
    json::object_t*
    to_json() const
    {
      switch (version) {
        case 0:
          throw parse_exception("[q] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          return new json::object_t("semiring", new json::string_t("Q"));
      }
    }

    template<unsigned version = version::fsm_json>
    json::node_t* value_to_json(value_t v) 
    const
    {
      switch (version) {
        case 0:
          throw parse_exception("[q] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
        if(v.den ==1u)
          return new json::int_t(v.num);
        json::object_t* l = new json::object_t();
        l->push_back("num",new json::int_t(v.num));
        l->push_back("den",new json::int_t(v.den));
        return l;
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
          switch(p->kind) {
            case json::FLOATING:
              throw json::coercion_exception(
                "[Q] value_from_json: node is of kind FLOATING and we do "
                "not support double to qfraction conversion");
            case json::BOOLEAN:
            case json::INTEGER:
            case json::STRING: {
              try {return value_t{p->to_int(),1u};}
              catch (json::coercion_exception&) {}
              std::stringstream ss(p->string()->value);
              value_t v = conv(ss); // This might be wrong
              if (ss.eof())
                return v;
              throw json::coercion_exception(
                "[Q] value_from_json: node is of kind STRING and is not a "
                "proper qfraction representation.");
            }
            case json::ARRAY :{
              std::vector<json::node_t*> const& v = p->array()->values;
              if (p->arity()!=2) 
                throw json::coercion_exception(
                  "[Q] value_from_json: node is of kind ARRAY and needs to "
                  " have two children to be interpreted as a qfraction.");
              return value_t(p->at(0)->to_int(),p->at(1)->to_int());
            }
            case json::OBJECT :{
                int n;
                if(!p->has_child("num")) {
                  throw json::coercion_exception(
                    "[Q] value_from_json: node is of kind OBJECT and needs to "
                    "have a \"num\" field to be interpreted as a qfraction.");
                }
                try {
                  n = p->at("num")->to_int();
                } catch (json::coercion_exception& e) {
                    throw json::coercion_exception(
                      "[Q] value_from_json:: node is of kind OBJECT, hence the "
                      "node associated with \"num\" needs to be coercible to "
                      "int. " + std::string(e.what()));
                }
                if (p->has_child("den"))
                  return value_t(n, p->at("den")->to_int());
                else
                  return value_t(n);
            }
            case json::_NULL:
              throw json::coercion_exception(
                "[Q] value_from_json: node is of kind NULL and cannot be "
                "interpreted as a qfraction.");
          }
          throw std::runtime_error("json parser Q");
      }
    }

      };
      
  inline q join(const q&, const q&) { return {}; }

  inline q join(const z&, const q&) { return {}; }
  inline q join(const q&, const z&) { return {}; }

  inline q join(const n&, const q&) { return {}; }
  inline q join(const q&, const n&) { return {}; }

  inline q join(const b&, const q&) { return {}; }
  inline q join(const q&, const b&) { return {}; }

  }
}//end of ns awali::stc




#endif // !AWALI_WEIGHTSET_Q_HH
