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

#ifndef AWALI_DYN_OPTIONS_OPTIONS_HH
#define AWALI_DYN_OPTIONS_OPTIONS_HH

#include <string>
#include <typeinfo>
#include <unordered_map>

#include <awali/common/demangle.hxx>
#include <awali/common/enum_utils.hh>
#include <awali/common/enums.hh>
#include <awali/common/priority.hh>
#include <awali/dyn/options/option.hh>

namespace awali { namespace dyn {


  /**
   An {@link options_t} is a set of optional parameters that is passed on
   to called functions.

   More precisely, it is a map from registered {@link internal::option_t} to
   their expected value.  Hence in general, adding new mappings to an
   #options_t might override a previous mapping.

   This class is an attempt to reduce the number of optional parameters
   in functions.
   It emulates the named optional parameters paradigm
   of high level languages (typically, in Python).

   (Examples below assume `using namespace awali` and `using namespace
   awali::dyn`.)
   @example (Providing one option)
   \code
      ratexp_t exp = ratexp_t::from("(aa)*a(aa)*");
      // Computing the Thompson automaton of exp
      automaton_t aut1 = exp_to_aut(exp, {EXP_TO_AUT_ALGO=THOMPSON} );
      // Compute the automaton resulting from the "Derived Term" algorithm
      automaton_t aut2 = exp_to_aut(exp, {EXP_TO_AUT_ALGO=DERIVED_TERM} );
   \endcode
   @example (Providing multiple options)
   \code
      ratexp_t exp = ratexp_t::from("(aa)*aa(aa)*");
      // Next line computes the minimal automaton accepting the language
      // of `exp`.
      // It basically calls in order:
      //   1. exp_to_aut, 2. proper, 3. determinize, 4. min_quotient
      // Optional parameters for all these functions may be provided.
      automaton_t aut = minimal_automaton(exp,
          { EXP_TO_AUT_ALGO=THOMPSON,
            MINIM_ALGO=DETERMINIZE_QUOTIENT,
            QUOTIENT_ALGO=MOORE              } );
   \endcode

   @example (Meaningless options are silently ignored)
   \code
     options_t opt = { IN_PLACE=true,
                       KEEP_HISTORY=true, //This is useless since KEEP_HISTORY
                                          //default to `true` anyway
                       EXP_TO_AUT_ALGO=THOMPSON
                     };
     ratexp_t exp = ratexp_t::from("(a+b)*aba(a+b)*");
     automaton_t aut = exp_to_aut(exp, opt); // Only EXP_TO_AUT_ALGO is
                                             // meaningful, Thompson's
                                             // algorithm is used.
     proper(aut, opt); // IN_PLACE is meaningful, hence this modifies `aut`
   \endcode


   See group {@link Options} for details on this mechanism.
   */
  class options_t
  {
    std::unordered_map<size_t, internal::opt_any_t> map;

  public:
    /** Creates an #options_t from a list of option/value pairs.
     *
     *  @param list List of option/value pairs.
     *
     * */
    options_t(std::initializer_list<internal::option_value_pair_t> list);

    options_t(){};

    /** Creates an #options_t from one option/value pair.
     *
     * @param pair Option/value pair.
     */
    options_t(internal::option_value_pair_t pair) : options_t({pair}) {}


    /** Creates an #options_t from an {@link internal::option_t} by mapping it
     * to its default value.
     *
     */
    template <typename T>
    options_t(internal::option_t<T> opt)
        : options_t((internal::option_value_pair_t) opt)
    {}

    /** Adds an option/value pair to this #options_t, possibly overriding
     * a previous value.
     *  @param other New pair to add in this #options_t.
     *  @return A reference to this #options_t
     * */
    options_t& operator+=(internal::option_value_pair_t const& other);


    /** Adds all option/value mapping in @pname{other} to this #options_t,
     * possibly overriding previous values.
     *
     * @param other
     * @return A reference to this #options_t.
     */
    options_t& operator+=(options_t const& other);


    /** Gets the value associated by to given @pname{option} by this
     * #options_t.
     * @param option
     * @return An object of the type declared by @pname{option}.
     */
    template <typename T>
    typename internal::option_t<T>::value_t
    operator[](internal::option_t<T> option)
    {
      //          auto value = map.find(option.id);
      //          if (value == map.end())
      //            value = map.emplace(option.id,
      //            option.get_default_value()).first;
      //          return ((typename internal::option_t<T>::value_t&)
      //          value->second);
      auto value = map.find(option.id);
      if (value == map.end())
        return option.get_default_value();
      return ((typename internal::option_t<T>::value_t) value->second);
    }

    /** Creates a new #options_t that is the union of this #options_t
     * with another option/value pair.
     *
     * If @pname{pair} gives a value to an option that is already defined in
     * this #options_t, the one in @pname{pair} in kept and the one in this
     * #options_t is ignored.
     *
     * @param pair option/value pair to be added.
     * @return A new #options_t
     */
    options_t operator+(internal::option_value_pair_t const& pair) const;


    /** Creates a new #options_t that is the union of this #options_t
     * with another #options_t.
     *
     * If @pname{other} and this #options_t give a value to the same option,
     * only the one in @pname{other} is kept in the returned #options_t.
     *
     * @param other Right operand of the union.
     * @return A new #options_t
     */
    options_t operator+(options_t const& other) const;
  };


}} // namespace awali::dyn


#endif
