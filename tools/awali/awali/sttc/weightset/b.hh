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

#ifndef AWALI_WEIGHTSET_B_HH
# define AWALI_WEIGHTSET_B_HH

# include <cassert>
# include <ostream>
# include <string>

#include <awali/sttc/misc/escape.hh>
#include <awali/utils/hash.hh>
#include <awali/common/parse_exception.cc>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/weightset/fwd.hh>
#include <awali/sttc/weightset/weightset.hh>
#include <awali/common/json/node.cc>
#include <awali/common/version.hh>

namespace awali {
namespace sttc {
  ///The Boolean semring
    class b {
    public:
      using self_type = b;

      static std::string sname() {
        return "b";
      }

      static std::string vname(bool = true) {
        return sname();
      }

      /// Build from the description in \a is.
      static b make(std::istream& is) {
        eat(is, sname());
        return {};
      }

      using value_t = bool;
      using finite_t = bool;
      static const finite_t finite=true;

      static value_t zero() {
        return false;
      }

      static value_t one() {
        return true;
      }

      static value_t add(value_t l, value_t r) {
        return l || r;
      }

      static value_t mul(value_t l, value_t r) {
        return l && r;
      }

      static value_t rdiv(value_t l, value_t r) {
        require(!is_zero(r), "div: division by zero");
        return l;
      }

      static value_t ldiv(value_t l, value_t r) {
        return rdiv(r, l);
      }

      static value_t star(value_t) {
        return one();
      }

      static bool equals(value_t l, value_t r)
      {
        return l == r;
      }

      /// Whether \a lhs < \a rhs.
      static bool less_than(value_t lhs, value_t rhs)
      {
        return lhs < rhs;
      }

      constexpr static bool is_special(value_t) {
        return false;
      }

      static bool is_zero(value_t v) {
        return !v;
      }

      static bool is_one(value_t v) {
        return v;
      }

      constexpr static bool is_commutative_semiring() { return true; }

      constexpr static bool show_one() { return false; }

      constexpr static star_status_t star_status() {
        return star_status_t::STARRABLE;
      }

      static value_t transpose(value_t v) {
        return v;
      }

      static size_t hash(value_t v) {
        return utils::hash_value(v);
      }

      static value_t conv(self_type, value_t v) {
        return v;
      }

      static value_t conv(std::istream& is) {
        int i;
        if (is >> i)
          {
            if (i == 0 || i == 1)
              return i;
            else
              fail_reading(is,
                           sname() + ": invalid value: " + std::to_string(i));
          }
        else
          fail_reading(is, sname() + ": invalid value");
      }

      static value_t parse(const std::string & s, size_t& p) {
        if ((p>=4)&&(!s.compare(p-4,4,"true"))) {
            p -= 4;
            return true;
        }
        if ((p>=5)&&(!s.compare(p-5,5,"false"))) {
            p -= 5;
            return false;
        }
        if(s[--p] == '0')
          return false;
        if(s[p] == '1')
          return true;
        throw parse_exception("Wrong Boolean value");
      }

      static std::ostream& print(value_t v, std::ostream& o,
                          const std::string& format = "text") {
        if (format == "latex")
          o << (v ? "\\top" : "\\bot");
        else
          o << v;
        return o;
      }

      static std::ostream& print_set(std::ostream& o, const std::string& format = "text") {
        if (format == "latex")
          o << "\\mathbb{B}";
        else if (format == "text")
          o << "B";
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
            throw parse_exception("[B] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return new json::object_t("semiring", new json::string_t("B"));
        }
      }


      template<unsigned version = version::fsm_json>
      static
      json::node_t* value_to_json(value_t v)
      {
        switch (version) {
          case 0:
            throw parse_exception("[B] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return (new json::bool_t(v));
        }
      }

      template<unsigned version = version::fsm_json>
      static value_t value_from_json(json::node_t const* p) {
        switch (version) {
          case 0:
            throw parse_exception("[B] Unsupported fsm-json version:"
                                  + std::to_string(version));
          case 1:
          default:
            return p->to_bool();
        }
      }
    };

  inline b join(const b&, const b&) { return {}; }

}}//end of namespaces awali::sttc, and awali


#endif // !AWALI_WEIGHTSET_B_HH
