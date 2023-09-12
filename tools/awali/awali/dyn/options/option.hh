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

#ifndef AWALI_DYN_OPTIONS_OPTION_HH
#define AWALI_DYN_OPTIONS_OPTION_HH

#include <unordered_map>
#include <typeinfo>

#include <awali/dyn/core/any.hh>
#include <awali/common/priority.hh>
#include <awali/common/enums.hh>
#include <awali/common/enum_utils.hh>
#include <awali/common/demangle.hxx>
#include <awali/dyn/options/value.hh>

namespace awali {
  namespace dyn {
  namespace internal {

    template<typename T>
    struct acceptor_t {
      virtual bool is_valid(T const& value) {
        return true;
      }
      virtual std::string acceptable_values() {
        return ("any value of type "+awali::internal::demangle(typeid(T).name()));
      }
    };

    template<>
    struct acceptor_t<bool> {
      virtual bool is_valid(bool const& value) {
        return true;
      }
      std::string acceptable_values() {
        return ("boolean");
      }
    };

    template<>
    struct acceptor_t<std::string> {
      virtual bool is_valid(std::string const& value) {
        return true;
      }
      virtual std::string acceptable_values() {
        return ("any string");
      }
    };

    /** Global variable that assign a unique identifier to each
     * instance of option<T> independentely of type T.
     */
    extern size_t next_id;

    template<typename T>
    class option_t {
     public:
      using value_t = T;
      const size_t id;

     private:
      std::string option_name;
      acceptor_t<value_t> acceptor;
      value_t default_value;
      bool default_set_by_user = false;

      option_t(option_t<T> other, value_t def) :
        id(other.id),
        option_name(other.option_name),
        acceptor(other.acceptor),
        default_value(other.default_set_by_user?other.def:def),
        default_set_by_user(other.was_user_set)
      {}

      template<typename X, typename P>
      internal::option_value_pair_t make(const X& value, priority::ONE<P>) {
        throw std::runtime_error("Wrong value type for option "+option_name
                                 + ". Expecting: "
                                 + acceptor.acceptable_values()
                                 + "." );
      }

      template<typename X=T, typename P>
      internal::option_value_pair_t
      make(const char* value, priority::TWO<P>)
      { return make(std::string(value), priority::value); }

      template<typename X=T, typename P>
      auto make(std::string const& value, priority::THREE<P>)
      -> decltype(
          awali::internal::make_enum<X>(value),
          internal::option_value_pair_t(id, default_value)
      ) {
        if (value == "default")
          return {id, default_value};
        return {id, awali::internal::make_enum<X>(value)};
      }

      template<typename P>
      internal::option_value_pair_t make(const T& value, priority::FOUR<P>) {
        if (!acceptor.is_valid(value))
          make(value, priority::one);
        return {id, value};
      }



     public:
      option_t(std::string name, T def, acceptor_t<T> acc = acceptor_t<T>())
        : id(internal::next_id++), option_name(name), acceptor(acc),
          default_value(def)
      {}

      bool acceptable(T value) {
        return acceptor(value);
      }

      operator internal::option_value_pair_t() {
        if (std::is_same<T, bool>::value)
          return {id, true};
        throw std::runtime_error("Using option without argument is only possible if they expect boolean values.");
      }


      /** Creates a phony option_t that overrides system default value (but not
       *  user defined default, if set).
       *
       *  \b Beware
       *
       *  Not intended for end users.
       *
       *  \b Example
       *  \code
       *    options_t opt = {}
       *    boolean inplace1 = opt[IN_PLACE] // `false` (system default)
       *    boolean inplace2 = opt[IN_PLACE.with_default_value(true)] // `true` (local default)
       *
       *    IN_PLACE.set_default_value(false); // end user sets a default value
       *    boolean inplace3 = opt[IN_PLACE.with_default_value(true)] // 'false` (user defined default value)
       *  \endcode
       */
      option_t<value_t> with_default_value(value_t new_default_value) {
        return option_t(*this, new_default_value);
      }


      /* Globally sets the default value of this option.
       */
      void set_default_value(value_t new_default_value) {
        default_value = new_default_value;
        default_set_by_user = true;
      }

      value_t get_default_value() {
        return default_value;
      }

     public:

      template<typename X>
      internal::option_value_pair_t operator=(const X& value) {
        return make(value, priority::value);
      }

      /** Create of value of the type expected by this option_t, from string
       * str. Supports the string "default".
       *
       */
      value_t of_string(std::string str) {
        internal::option_value_pair_t pair = operator=(str);
        return (value_t) pair.value;
      }

    };

    } // end of namespace awali::dyn::internal
  } // end of namespace awali::dyn
} //end of namespace awali


#include<awali/dyn/options/declaration.incl.hh>




#endif
