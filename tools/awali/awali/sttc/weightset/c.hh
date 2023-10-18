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

#ifndef AWALI_WEIGHTSET_C_HH
#define AWALI_WEIGHTSET_C_HH

#include <string>
#include <ostream>
#include <sstream>

#include <awali/utils/arith.hh>

#include <complex>
#include <awali/common/enums.hh>

#include <awali/sttc/misc/attributes.hh>
#include <awali/utils/hash.hh>

#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/misc/stream.hh> // eat
#include <awali/sttc/weightset/r.hh>
#include <awali/sttc/weightset/lr_parse_number.hh>

namespace awali {
  namespace sttc {
    /**The semiring of complex numbers
     *
     * The values are impleted by std::complex<double>
     */
      class c
      {
      public:
        using self_type = c;

        static std::string sname()
        {
          return "c";
        }

        std::string vname(bool = true) const
        {
          return sname();
        }

        /// Build from the description in \a is.
        static c make(std::istream& is)
        {
          eat(is, sname());
          return {};
        }

        using value_t = std::complex<double>;


        static value_t zero()
        {
          return value_t();
        }

        static value_t one()
        {
          return value_t(1);
        }

        static value_t add(const value_t l, const value_t r)
        {
          return l+r;
        }

        static value_t sub(const value_t l, const value_t r)
        {
          return l-r;
        }

        static value_t mul(const value_t l, const value_t r)
        {
          return l*r;
        }

        static value_t
        rdiv(const value_t l, const value_t r)
        {
          require(!is_zero(r), "div: division by zero");
          return l/r;
        }

        static value_t
        ldiv(const value_t l, const value_t r)
        {
          return rdiv(r, l);
        }

        value_t star(const value_t v) const
        {
          // Bad casting when v.den is too big
          if (std::norm(v) < 1)
            // No need to reduce: numerator and denominators are primes.
            return value_t(1)/(value_t(1)-v);
          else
            raise(sname(), ": star: invalid value: ", format(*this, v));
        }

        value_t plus(const value_t v) const
        {
          // Bad casting when v.den is too big
          if (std::norm(v) < 1)
            // No need to reduce: numerator and denominators are primes.
            return value_t(v)/(value_t(1)-v);
          else
            raise(sname(), ": star: invalid value: ", format(*this, v));
        }

        static bool is_special(const value_t) // C++11: cannot be constexpr.
        {
          return false;
        }

        static bool is_zero(const value_t v)
        {
          return v.real() == 0 && v.imag() == 0;
        }

        static bool is_one(const value_t v)
        {
          // All values are normalized.
          return v.real() == 1 && v.imag() == 0;
        }

        static bool equals(const value_t l, const value_t r)
        {
          return l==r;
        }

        /// Whether \a lhs < \a rhs.
        static bool less_than(value_t lhs, value_t rhs)
        {
          if(std::norm(lhs) == std::norm(rhs))
            return std::arg(lhs) < std::arg(rhs);
          return std::norm(lhs) < std::norm(rhs);
        }

        static constexpr bool is_commutative_semiring() { return true; }

        static constexpr bool show_one() { return false; }
        static constexpr star_status_t star_status() { return star_status_t::ABSVAL; }

        static value_t
        abs(const value_t v)
        {
          return std::abs(v);
        }

        static value_t
        transpose(const value_t v)
        {
          return v;
        }

        static size_t hash(value_t v)
        {
          size_t res = 0;
          std::hash_combine(res, utils::hash_value(v.real()));
          std::hash_combine(res, utils::hash_value(v.imag()));
          return res;
        }

        static value_t
        conv(self_type, value_t v)
        {
          return v;
        }

        static value_t
        conv(r, r::value_t v)
        {
          return {v};
        }

        static value_t
        conv(q, q::value_t v)
        {
          return {double(v.num) / double(v.den)};
        }

        static value_t
        conv(z, z::value_t v)
        {
          return {(double)v};
        }

        static value_t
        conv(n, n::value_t v)
        {
          return {(double)v};
        }
        
        static value_t
        conv(b, b::value_t v)
        {
          return v?one():zero();
        }

        static double greedy(std::istream& i) {
          std::ostringstream os;
          char maybe, sig=0;
          if (i.peek()=='+' || i.peek()=='-')
            i>> sig;
          while((maybe = i.peek()) == '.' || (maybe >='0' && maybe <='9')) {
            i >> maybe;
            os << maybe;
          }
          if(os.str().empty()) {
            if(sig=='+')
              return 1;
            if(sig=='-')
              return -1;
            throw std::runtime_error(sname() + " : invalid real");
          }
          double r;
          std::istringstream is(os.str());
          is >> r;
          if(sig=='-')
            return -r;
          return r;
        }

        static value_t
        conv(std::istream& i)
        {
          double re,im;
          char maybe;
          if ((maybe = i.peek()) == 'i') { //no real part
            sttc::eat(i, 'i');
            return value_t{0,1};
          }
          re = greedy(i);
          if ((maybe = i.peek()) == 'i') { //no real part
            sttc::eat(i, 'i');
            return value_t{0,re};
          }
          if ((maybe = i.peek()) == '+' || maybe == '-'){ // imaginary part
            im=greedy(i);
            if (i.peek()!='i')
              sttc::fail_reading(i, sname() + ": invalid imaginary complex part");
            sttc::eat(i, 'i');
            return value_t{re, im};
          }
          return value_t{re}; // no imaginary part
        }

//         static double parse_simple_real(const std::string & s, size_t& p, double def=0) {
//           size_t i=p;
//           for(; i>0 && ((s[i-1]>='0' && s[i-1]<='9') || s[i-1]=='.'); --i)
//             ;
//           double r=def;
//           if(p>i) {
//             std::istringstream st(s.substr(i, p-i));
//             st >> r;
//           }
//           if(s[i-1]=='-') {
//             --i;
//             r=-r;
//           }
//           if(s[i-1]=='+')
//             --i;
//           p=i;
//           return r;
//         }


        static value_t
        parse(const std::string & s, size_t& p) {
          return internal::lr_parse_complex(s,p);
        }


        static std::ostream&
        print(const value_t v, std::ostream& o,
              const std::string& format = "text")
        {
          if (format == "json") {
            o<< '{';
            if(v.real()!=0 || v.imag()==0) {
              o << "\"re\":" << v.real();
              if(v.imag()!=0)
                o<< ", \"im\":" << v.imag();
            }
            else
              o<< "\"im\":" << v.imag();
            o <<'}';
            return o;
          }
          if(v.real()!=0) {
            o << v.real();
            if(v.imag()>0)
              o << '+';
          }
          if(v.imag()!=0) {
            double im = v.imag();
            if(im==1)
              o << 'i';
            else if(im==-1)
              o << "-i";
            else
              o << im << 'i';
          }
          if(v.real()==0 && v.imag()==0)
            o << 0.0;
          return o;
        }

        std::ostream&
        print_set(std::ostream& o, const std::string& format = "text") const
        {
          if (format == "latex")
            o << "\\mathbb{C}";
          else if (format == "text")
            o << "C";
          else
            raise("invalid format: ", format);
          return o;
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
          return new json::object_t("semiring", new json::string_t("C"));
      }
    }

    
    template<unsigned version = version::fsm_json>
    json::node_t*
    value_to_json(value_t v) 
    const
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          json::object_t* l = new json::object_t();
          if (v.imag () == 0.)
            l->push_back("real",new json::float_t(v.real()));
          else if (v.real() == 0.)
            l->push_back("imag",new json::float_t(v.imag()));
          else {
            l->push_back("real",new json::float_t(v.real()));
            l->push_back("imag",new json::float_t(v.imag()));
          }
          return l;
      }
    }

    template<unsigned version = version::fsm_json>
    value_t
    value_from_json(json::node_t const* p)
    const
    {
      version::check_fsmjson<version>();
      switch (version) {
        case 0: /* Never occurs due to above check. */
        case 1:
        default:
          try { return value_t{p->to_double(),0}; }
          catch (json::exception const&){}
          switch(p->kind) {
            case json::ARRAY: {
              std::vector<json::node_t*> const& v = p->array()->values;
              if (p->arity() !=2)
                  throw json::coercion_exception("This ARRAY node should have exactly two elements.","c",p);
              double a[2];
              for(int i=0;i<2;i++)
                a[i]=v[i]->to_double();
              return value_t{a[0],a[1]};
            }
            case json::OBJECT: {
              if(   !p->has_child("real") && !p->has_child("imag") 
                 && !p->has_child("re") && !p->has_child("im"))
                throw json::coercion_exception("Should have either a field \"real\" or \"imag\".","c",p);
              double a[2] = {0,0};
              if(p->has_child("real"))
                a[0]=p->at("real")->to_double();
              else if(p->has_child("re"))
                a[0]=p->at("re")->to_double();
              if(p->has_child("imag"))
                a[1]=p->at("imag")->to_double();
              if(p->has_child("im"))
                a[1]=p->at("im")->to_double();
              return value_t{a[0],a[1]};
            }
            case json::STRING: {
              std::string const& str = p->string()->value;
              size_t pos = str.length();
              value_t v = parse(str, pos);
              if (pos != 0)
                break;
              return v;
            }
            default : {}
          }
          throw json::coercion_exception("Unable to coerce this "
                                         + json::string_of(p->kind)
                                         + " node to a complex number.",
                                         "c",p);
      }
    }

  };
    
  inline c join(const c&, const c&) { return {}; }

  inline c join(const r&, const c&) { return {}; }
  inline c join(const c&, const r&) { return {}; }

  inline c join(const q&, const c&) { return {}; }
  inline c join(const c&, const q&) { return {}; }

  inline c join(const z&, const c&) { return {}; }
  inline c join(const c&, const z&) { return {}; }

  inline c join(const n&, const c&) { return {}; }
  inline c join(const c&, const n&) { return {}; }

  inline c join(const b&, const c&) { return {}; }
  inline c join(const c&, const b&) { return {}; }

  }
}//end of ns awali::stc




#endif // !AWALI_WEIGHTSET_C_HH
