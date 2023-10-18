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

#ifndef DYN_VALUE_HH
#define DYN_VALUE_HH

#include <awali/common/demangle.hxx>
#include <awali/common/qfraction.hh>
#include <awali/dyn/core/internal/untyped_value.hh>

#include<typeinfo>  

namespace awali { namespace dyn {
  namespace internal {

    template<typename T> T any_cast(const any_t& a);

    template<typename T>
    struct Value : public untyped_value {
      Value(const T& val) : val(val) {}
      T val;

      Value* clone() const override {
        return new Value(val);
      }

      std::ostream& output(std::ostream& o) const override {
        return (o << val);
      }

      bool less(const untyped_value& uv) const override;

      bool equal(const untyped_value& uv) const override {
        const Value& v=dynamic_cast<const Value&>(uv);
        return (val == v.val);
      }

      std::ostream&  real_type_name(std::ostream& o) const override {
        return o << awali::internal::demangle(typeid(val).name());
      }
     
      std::ostream&  real_type_id(std::ostream& o) const override {
        return o << typeid(val).hash_code();
      }

      virtual ~Value() {}
    };

    template<typename T>
    bool Value<T>::less(const untyped_value& uv) const {
      const Value& v=dynamic_cast<const Value&>(uv);
      return (val < v.val);
    }
    
    template<>
    inline
    bool Value<std::string>::less(const untyped_value& uv) const {
      const Value& v=dynamic_cast<const Value&>(uv);
      if(val.length() == v.val.length())
        return (val < v.val);
      else
        return (val.length() < v.val.length());
    }

    template<>
    inline
    bool Value<std::basic_string<int>>::less(const untyped_value& uv) const {
      const Value& v=dynamic_cast<const Value&>(uv);
      if(val.length() == v.val.length())
        return (val < v.val);
      else
        return (val.length() < v.val.length());
    }

}}}//end of namespace awali::dyn::internal, awali::dyn, and awali

#endif //DYN_VALUE_HH
