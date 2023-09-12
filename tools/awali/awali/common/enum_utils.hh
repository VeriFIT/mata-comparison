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

#ifndef AWALI_COMMON_ENUMS_UTILS_HH
#define AWALI_COMMON_ENUMS_UTILS_HH


#include <awali/common/enums.hh>

#include <string>

namespace awali {

  /** Builds a {@link direction_t} from a string describing its name.
   *  @param name String-representation of the enum value to return;
   *  case-insensitive; char `'-'` or `'_'` can be used indifferently.
   *  @throws std::domain_error if @pname{name} does not represent any value of
   *  {@link direction_t}
   */
  direction_t make_direction(std::string const& name);


  /** Return the canonical string reprensation of given {@link direction_t}. */
  std::string name_of(direction_t val);


  /** Builds a {@link layout_t} from a string describing its name.
   *  @param name String-representation of the enum value to return;
   *  case-insensitive; char `'-'` or `'_'` can be used indifferently.
   *  @throws std::domain_error if @pname{name} does not represent any value of
   *  {@link layout_t}
   */
  layout_t make_layout(std::string const& name);
  

  /** Return the canonical string reprensation of given {@link layout_t}. */
  std::string name_of(layout_t val);


  /** Builds a {@link quotient_algo_t} from a string describing its name.
   *
   * @param name String-representation of the enum value to return;
   * case-insensitive; char `'-'` or `'_'` can be used indifferently.
   * 
   * @throws std::domain_error if @pname{name} does not represent any value of
   * {@link quotient_algo_t}
   */
  quotient_algo_t make_quotient_algo(std::string const& name);
  

  /** Return the canonical string reprensation of given 
   * {@link quotient_algo_t}. 
   */
  std::string name_of(quotient_algo_t val);


  /** Builds a {@link exp_to_aut_algo_t} from a string describing its name.
   *
   * @param name String-representation of the enum value to return;
   * case-insensitive; char `'-'` or `'_'` can be used indifferently.
   * 
   * @throws std::domain_error if @pname{name} does not represent any value of
   * {@link exp_to_aut_algo_t}
   */
  exp_to_aut_algo_t make_exp_to_aut_algo(std::string const& name);
  
  
  /** Return the canonical string reprensation of given 
   * {@link exp_to_aut_algo_t}. 
   */
  std::string name_of(exp_to_aut_algo_t val);


  /** Builds a {@link io_format_t} from a string describing its name.
   *
   * @param name String-representation of the enum value to return;
   * case-insensitive; char `'-'` or `'_'` can be used indifferently.
   * 
   * @throws std::domain_error if @pname{name} does not represent any value of
   * {@link io_format_t}
   */
  io_format_t make_io_format(std::string const& name);
  

  /** Return the canonical string reprensation of given {@link io_format_t}. */
  std::string name_of(io_format_t val);


  /** Return the default extension of a file written in given format. */
  std::string default_extension_of(io_format_t val);


  /** Builds a {@link state_elim_order_t} from a string describing its name.
   *
   * @param name String-representation of the enum value to return;
   * case-insensitive; char `'-'` or `'_'` can be used indifferently.
   * 
   * @throws std::domain_error if @pname{name} does not represent any value of
   * {@link state_elim_order_t}
   */
  state_elim_order_t make_state_elim_order_t(std::string const& name);
  

  /** Return the canonical string reprensation of given 
   * {@link state_elim_order_t}. 
   */
  std::string name_of(state_elim_order_t val);

  namespace internal {


    int enum_of_string (const std::string& enum_name,
                        const std::string& value);

#define GENERATE_MAKE_ENUM(enum_t) \
  template<typename T=enum_t> \
  auto make_enum(std::string const& str) \
  -> typename std::enable_if<std::is_same<T,enum_t>::value,T>::type \
  { return (enum_t) enum_of_string(#enum_t,str); }


    GENERATE_MAKE_ENUM(direction_t)
    GENERATE_MAKE_ENUM(layout_t)
    GENERATE_MAKE_ENUM(quotient_algo_t)
    GENERATE_MAKE_ENUM(minim_algo_t)
    GENERATE_MAKE_ENUM(exp_to_aut_algo_t)
    GENERATE_MAKE_ENUM(io_format_t)
    GENERATE_MAKE_ENUM(state_elim_order_t)
    GENERATE_MAKE_ENUM(star_status_t)


#undef GENERATE_MAKE_ENUM



  }//end of namespace awali::internal
} // end of namespace awali

#endif
