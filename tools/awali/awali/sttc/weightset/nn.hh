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

#ifndef AWALI_WEIGHTSET_NK_HH
# define AWALI_WEIGHTSET_NK_HH

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
    /** The semiring of natural numbers bounded by \p K
     *
     * The semiring has K+1 elements.
     * The type of values is unsigned int
     *
     * @tparam K a positive integer: the bound on the semiring
     */
    template <unsigned int K>
    class nn
    {
    public:
      using self_type = nn<K>;

      nn() {
        if(K==0)
          raise("Bounded semirings should have non zero characteristic");
      }

      static std::string sname()
      {
        return "nn"+std::to_string(K);
      }

      std::string vname(bool = true) const
      {
        return sname();
      }

      /// Build from the description in \a is.
      static nn<K> make(std::istream& is)
      {
        eat(is, sname());
        return {};
      }

      using value_t = unsigned int;
      using finite_t = bool;
      static const finite_t finite=true;
      
      static value_t
      zero()
      {
        return 0u;
      }

      static value_t
      one()
      {
        return 1u;
      }

      static value_t
      add(const value_t l, const value_t r)
      {
        return (l+r<K)?l+r:K;
      }

      static value_t
      mul(const value_t l, const value_t r)
      {
        return (l*r<K)?l*r:K;
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

      static value_t
      star(const value_t v)
      {
        if ( v == 0u )
          return 1u;
        else
          return K;
      }

      static bool
      equals(const value_t l, const value_t r)
      {
        return ((l<K)?l:K) == ((r<K)?r:K) ;
      }

      /// Whether \a lhs < \a rhs.
      static bool less_than(value_t lhs, value_t rhs)
      {
        return lhs < rhs && lhs <K;
      }

      constexpr static bool is_special(value_t)
      {
        return false;
      }

      static bool
      is_zero(const value_t v)
      {
        return ( v == 0u );
      }

      static bool
      is_one(const value_t v)
      {
        return ( v == 1u );
      }

      static constexpr bool is_commutative_semiring() { return true; }

      static constexpr bool show_one() { return false; }

      static constexpr
      star_status_t star_status() { return star_status_t::STARRABLE; }

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
        return (v<K)?v:K;
      }

      static value_t
      conv(std::istream& stream)
      {
        int i;
        if (stream >> i) {
          if (i> (signed) K)
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
        if (x> (signed)K)
          throw parse_exception("Weight-Set "+sname()+": parsed value ("+s.substr(i, p-i)+") is too great.");
        return x;
      }

      static std::ostream&
      print(value_t v, std::ostream& o,
            const std::string& format= "text")
      {
        if(v>K)
          v=K;
        return o << v;
      }
      
      std::ostream&
      print_set(std::ostream& o, const std::string& format = "text") const
      {
        if (format == "latex")
          o << "\\mathbb{N}_" << K;
        else if (format == "text")
          o << "N" << K;
        else
          raise("invalid format: ", format);
        return o;
      }


      template<unsigned version = version::fsm_json>
      json::node_t*
      to_json() const
      {
        switch (version) {
        case 0:
          throw parse_exception("[nn] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          json::object_t* obj = new json::object_t();
          obj->push_back("semiring", new json::string_t("Bounded"));
          obj->push_back("characteristic", new json::int_t(K));
          return (obj);
        }
      }

    template<unsigned version = version::fsm_json>
    json::node_t*
    value_to_json(value_t v) const
    {
      switch (version) {
        case 0:
          throw parse_exception("[nn] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          return new json::int_t(v);
      }
    }

    template<unsigned version = version::fsm_json>
    value_t
    static value_from_json(json::node_t const* p)
    {
      switch (version) {
        case 0:
          throw parse_exception("[nn] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          int a=p->to_int();
          if(a<0)
            throw parse_exception("[nn] No negative value in bounded semiring");
          if(a>K)
            a=K;
          return a;
      }
    }
      
    };

    template<unsigned K>
    nn<K> join(const nn<K>&, const nn<K>&) { return {}; }

  }
}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_NK_HH
