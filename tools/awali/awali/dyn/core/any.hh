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

#ifndef DYN_ANY_HH
#define DYN_ANY_HH

#include <awali/dyn/core/internal/untyped_value.hh>
#include <awali/dyn/core/internal/value.hh>
#include <awali/common/to_value_of.hxx>
#include <awali/common/demangle.hxx>

#include<string>
#include<iostream>
#include<sstream>
#include<list>

namespace awali {
  namespace dyn {

   namespace internal {
      template<typename T>
      bool any_typeof(const any_t& a);
      
      template<typename T>
      T any_cast(const any_t& a);
    }

    std::ostream& operator<<(std::ostream& o, const dyn::any_t& a);
    

    /** @brief Structure used to erase the type of labels/weights at the dyn
     * layer.
     *
     * Anything may be stored into an \link any_t \endlink but it should be of
     * exactly the correct type expected by the automaton.
     * Usually, automatic type conversion will *not* work (typically long->int,
     * etc) and provoke a runtime error.
     */
    struct any_t {
      template<typename T>
      friend T internal::any_cast(const any_t& a);

      template<typename T>
      friend bool internal::any_typeof(const any_t& a);
      friend std::ostream& operator<<(std::ostream& o, const dyn::any_t& a);

    private:
      internal::untyped_value* val;

      bool is_tuple;
      
      std::ostream& output (std::ostream& o) const {
        if (is_tuple) {
          o << '[';
          bool b = true; 
          internal::Value<std::list<any_t>>& l
            = dynamic_cast<internal::Value<std::list<any_t>>&>
              (*(this->val));
          for (auto it =l.val.begin(); it != l.val.end(); it++) {
            if (b)
              b=false;
            else
              o << ',';
            it->output(o);
          }
          o << ']';
          return o;
        }
        else
          return val->output(o);
      }
    

      template<typename H>
      void static add_to_vect(std::list<any_t>&accu, const H& head) {
        accu.emplace_back(head);
      }

      template <typename H, typename...T>
      void static add_to_vect
        (std::list<any_t>& accu, const H& head, const T&... tail)
      {
        add_to_vect(accu,head);
        add_to_vect(accu,tail...);
      }

    public:
      template<typename T>
      any_t(const T& t) : val(new internal::Value<T>(t)), is_tuple(false) {}
    
      any_t() : val(nullptr), is_tuple(false) {}

      template<typename H, typename... T>
      any_t(const H& head, const T&... tail) 
      : val(new internal::Value<std::list<any_t>>({})),
        is_tuple(true) 
      {
        internal::Value<std::list<any_t>>& test
            = dynamic_cast<internal::Value<std::list<any_t>>&>
              (*(this->val));
        add_to_vect(test.val, head, tail...);
      }

      any_t(const char *s) : val(new internal::Value<std::string>(s)), is_tuple(false) {}

      any_t(const any_t& a) : val(a.val->clone()), is_tuple(a.is_tuple) {}

      any_t& operator= (const any_t& t) {
        if(&t==this) return *this;
        delete val;
        val=t.val->clone();
        is_tuple=t.is_tuple;
        return *this;
      }

      bool operator< (const any_t& a) const {
        return val->less(*(a.val));
      }


      bool operator==(const any_t& a) const {
        return val->equal(*(a.val));
      }

      bool operator!=(const any_t& a) const {
        return !val->equal(*(a.val));
      }

      bool operator>=(const any_t& a) const {
        return !val->less(*(a.val));
      }

      bool operator<=(const any_t& a) const {
        return val->equal(*(a.val)) || val->less(*(a.val));
      }

      bool operator>(const any_t& a) const {
        return !val->equal(*(a.val)) && !val->less(*(a.val));
      }

      template<typename T>
      explicit
      operator T() const {
        return internal::any_cast<T>(*this);
      }

      std::ostream& real_type_id(std::ostream &o) const {
        return val->real_type_id(o);
      }
      std::ostream& real_type_name(std::ostream &o) const {
        return val->real_type_name(o);
      }

      ~any_t() {
        delete val;
      }
    };

  std::ostream& operator<< (std::ostream& o, const any_t& a);

  struct any_cast_exception : public std::runtime_error {
    any_cast_exception(const std::string& s) : std::runtime_error(s) {}
  };


  namespace internal {
    template<typename T>
    T any_cast(const any_t& a){
      try {
        const internal::Value<T>& test
          = dynamic_cast<const internal::Value<T>&>(*(a.val));
        return test.val;
      }
      catch (const std::bad_cast& e) {
        std::stringstream ss;
        ss << "Failed to extract content of "<< a << ". "
           << "Tried to cast it as a " 
           << awali::internal::demangle(typeid(T).name())  
           << " but is actually of type ";
        a.real_type_name(ss) << ".";
        throw any_cast_exception(ss.str());
      }
    }
    template<typename T>
    bool any_typeof(const any_t& a){
      try {
        const internal::Value<T>& test
          = dynamic_cast<const internal::Value<T>&>(*(a.val));
        return true;
      }
      catch(std::bad_cast& e) {
         return false;
      }
    }

  }



 
}}//end of namespaces awali::dyn, and awali

#endif //DYN_ANY_HH
