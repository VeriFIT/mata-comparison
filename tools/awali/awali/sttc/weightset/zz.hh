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

#ifndef AWALI_WEIGHTSET_F_HH
# define AWALI_WEIGHTSET_F_HH

# include <cassert>
# include <ostream>
# include <string>

#include <awali/sttc/misc/escape.hh>
#include <awali/utils/hash.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/weightset/weightset.hh>

namespace awali {
  namespace sttc {
    /** The cyclic semiring with characteristic N
     *
     * The type of values are int.
     * @tparam N an integer larger than 1: the characteristic of the semiring.
     */
    template <unsigned int N>
    class zz {
    public:
      using self_type = zz<N>;

      zz() {
        if(N<2)
          raise("Cyclic semirings should have a characteristic larger than 1");
      }

      static std::string sname()
      {
        return "zz"+std::to_string(N);
      }

      std::string vname(bool = true) const
      {
        return sname();
      }

      /// Build from the description in \a is.
      static zz<N> make(std::istream& is)
      {
        eat(is, sname());
        return {};
      }

      using value_t = int;
      using finite_t = bool;
      static const finite_t finite=true;

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
        return (l+r)%N;
      }

      static value_t
      sub(const value_t l, const value_t r)
      {
        return (l-r)%N;
      }

      static value_t
      mul(const value_t l, const value_t r)
      {
        return (l*r)%N;
      }

      static value_t
      rdiv(const value_t l, const value_t r)
      {
        require(!is_zero(r), sname()+" div: division by zero");
        int calc=0;
        for (int i=1; i<N; i++) {
          calc = (calc+r)%N; // calc is always equal to (i*r)%N
          if ( calc == 1 )
            return ( (l*i)%N );
          if ( calc == 0 )
            throw std::runtime_error(sname()+": division by a divisor of 0:"+std::to_string(r));
        }
        throw std::runtime_error(sname()+": unreachable code");
      }

      static value_t
      ldiv(const value_t l, const value_t r)
      {
        return rdiv(r, l);
      }

      static value_t
      star(const value_t v)
      {
        if ( v == 0 )
          return 1;
        else
          throw std::domain_error("Weightset "+sname()+": given value ("+std::to_string(v)+") is not starrable");
      }

      static bool
      equals(const value_t l, const value_t r)
      {
        return l%N == r%N ;
      }

      /// Whether \a lhs < \a rhs.
      static bool less_than(value_t lhs, value_t rhs)
      {
        return lhs%N < rhs%N;
      }

      constexpr static bool is_special(value_t)
      {
        return false;
      }

      static bool
      is_zero(const value_t v)
      {
        return ( (v%N) == 0);
      }

      static bool
      is_one(const value_t v)
      {
        return ( (v%N) == 1);
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
        return v%N;
      }

      static value_t
      conv(std::istream& stream)
      {
        int i;
        if (stream >> i) {
          if (i>= (signed) N)
            throw parse_exception("Weight-Set "+sname()+": parsed value ("+std::to_string(i)+") is too great.");
          return i;
        } else
          sttc::fail_reading(stream, sname() + ": invalid value");
      }

      static value_t
      parse(const std::string & s, size_t& p) {
        size_t i=p;
        for(; i>0 && s[i-1]>='0' && s[i-1]<='9'; --i)
            ;
        if(i==p)
            throw parse_exception("Wrong "+sname()+" value");
        std::istringstream st(s.substr(i, p-i));
        value_t x;
        st >> x;
        p=i;
        if (x>= (signed)N)
          throw parse_exception("Weight-Set "+sname()+": parsed value ("+s.substr(i, p-i)+") is too great.");
        return x;
      }

      static std::ostream&
      print(const value_t v, std::ostream& o,
            const std::string& format= "text")
      {
        return o << v;
      }

      std::ostream&
      print_set(std::ostream& o, const std::string& format = "text") const
      {
        if (format == "latex")
          o << "\\mathbb{F}_" << N;
        else if (format == "text")
          o << "Z/" << N << 'Z';
        else
          raise("invalid format: ", format);
        return o;
      }

      
      template<unsigned version = version::fsm_json>
      static json::node_t* 
      to_json()
      {
      switch (version) {
        case 0:
          throw parse_exception("[zz] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          json::object_t* obj = new json::object_t();
          obj->push_back("semiring", new json::string_t("Cyclic"));
          obj->push_back("characteristic", new json::int_t(N));
          return (obj);
        }
      }


      template<unsigned version = version::fsm_json>
      json::node_t* value_to_json(value_t v) 
      const
      {
        switch (version) {
          case 0:
            throw parse_exception("[zz] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return new json::int_t(v);
        }
      }
    

      template<unsigned version = version::fsm_json>
      value_t
      value_from_json(json::node_t* p) 
      const 
      {
        switch (version) {
          case 0:
            throw parse_exception("[letterset] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return p->to_int();
        }
      }

    };
  
    template<unsigned N>
    inline
    zz<N> join(const zz<N>&, const zz<N>&) { return {}; }

  }
}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_F_HH
