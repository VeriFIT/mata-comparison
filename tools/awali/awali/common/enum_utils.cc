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

#ifndef AWALI_COMMON_ENUMS_UTILS_CC
#define AWALI_COMMON_ENUMS_UTILS_CC

#include<utility>
#include<algorithm>
#include<unordered_map>
#include<sstream>
#include<iostream>

#include<awali/common/enum_utils.hh>
#include<awali/common/demangle.hxx>

namespace awali {
  namespace internal {

    std::unordered_map< std::string,
        std::pair< std::unordered_map<std::string, int>,
        std::unordered_map<int, std::string> > >
        init_enum_map ()
    {
      std::unordered_map<std::string,
          std::pair< std::unordered_map<std::string, int>,
          std::unordered_map<int, std::string> > > map;

#define REGISTER_ENUM_VALUE(type, name) \
  if (map.find(#type) == map.end()) {\
    map[#type] = {{},{}}; \
    /* Next two lines are here to check that make_enum was correctly */ \
    /* generated in enum_utils.hh */ \
    if (false) \
      make_enum<type>(""); \
  }\
  map[#type].first[#name] = name; \
  /* Next line is only to typecheck name to type */ \
  {type test = name; test=test; }  \
  if (map[#type].second.find(name) == map[#type].second.end())\
    map[#type].second[name] = #name

      REGISTER_ENUM_VALUE(direction_t, FORWARD);
      REGISTER_ENUM_VALUE(direction_t, BACKWARD);

      REGISTER_ENUM_VALUE(layout_t, VERTICAL);
      REGISTER_ENUM_VALUE(layout_t, HORIZONTAL);
      REGISTER_ENUM_VALUE(layout_t, CIRCULAR);

      REGISTER_ENUM_VALUE(minim_algo_t, DETERMINIZE_QUOTIENT);
      REGISTER_ENUM_VALUE(minim_algo_t, BRZOZOWSKI);

      REGISTER_ENUM_VALUE(quotient_algo_t, MOORE);
      REGISTER_ENUM_VALUE(quotient_algo_t, HOPCROFT);

      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, GLUSHKOV);
      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, STANDARD);
      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, DERIVED_TERM);
      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, BREAKING_DERIVED_TERM);
      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, BREAKING);
//       REGISTER_ENUM_VALUE(exp_to_aut_algo_t, BREAKING_DERIVED_TERM);
      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, THOMPSON);
      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, COMPACT_THOMPSON);
      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, WEIGHTED_THOMPSON);
      REGISTER_ENUM_VALUE(exp_to_aut_algo_t, STANDARD_AND_QUOTIENT);

      REGISTER_ENUM_VALUE(io_format_t, DOT);
      REGISTER_ENUM_VALUE(io_format_t, FADO);
      REGISTER_ENUM_VALUE(io_format_t, GRAIL);
      REGISTER_ENUM_VALUE(io_format_t, FSM_JSON_V1);
      REGISTER_ENUM_VALUE(io_format_t, JSON);
      REGISTER_ENUM_VALUE(io_format_t, PDF);
      REGISTER_ENUM_VALUE(io_format_t, TEXT);
      REGISTER_ENUM_VALUE(io_format_t, SVG);
      REGISTER_ENUM_VALUE(io_format_t, FSM_JSON_V0);

      REGISTER_ENUM_VALUE(state_elim_order_t, MIN_INOUT_DEGREE);
      REGISTER_ENUM_VALUE(state_elim_order_t, MIN_ID);
      REGISTER_ENUM_VALUE(state_elim_order_t, ID_ORDER);

      REGISTER_ENUM_VALUE(star_status_t, STARRABLE);
      REGISTER_ENUM_VALUE(star_status_t, NON_STARRABLE);
      REGISTER_ENUM_VALUE(star_status_t, TOPS);
      REGISTER_ENUM_VALUE(star_status_t, ABSVAL);

#undef REGISTER_ENUM_VALUE
      return map;
    }

    std::unordered_map< std::string,
                        std::pair< std::unordered_map<std::string,int>, 
                                   std::unordered_map<int, std::string> > > 
        const enum_map = init_enum_map();

    int
    enum_of_string (const std::string& enum_name, const std::string& value)
    {
      std::string str = value;
      std::transform(str.begin(), str.end(), str.begin(),
      [](unsigned char c) { return (c=='-')?'_':std::toupper(c);}
                    );
      auto it1 = enum_map.find(enum_name);
      if (it1 == enum_map.end())
        throw std::domain_error("No enum "+enum_name+" is registered.");
      auto it2 = it1->second.first.find(str);
      if (it2 == it1->second.first.end()) {
        std::stringstream ss;
        ss << "Enum " << enum_name << " "
           << "has no value associated with \"" << value << "\".  ";
        std::string sep = "Admissible values (case insensitive): ";
        for (auto pair : it1->second.first) {
          ss << sep << '"' << pair.first << '"';
          std::string str = it1->second.second.find(pair.second)->second;
          if (str != pair.first)
            ss << " (aka \"" << str << "\")";
          sep = ", ";
        }
        ss << ".";//  (Case is not meaningful.)";
        throw std::domain_error(ss.str());
      }
      return (it2->second);
    }

    std::string string_of_enum
    (const std::string& enum_name, int value)
    {
      auto it1 = enum_map.find(enum_name);
      if (it1 == enum_map.end())
        throw std::domain_error("No enum "+enum_name+" is registered.");
      auto it2 = it1->second.second.find(value);
      if (it2 == it1->second.second.end())
        throw std::domain_error("Enum "+enum_name+" has no value number "
                                +std::to_string(value)+".");
      return (it2->second);
    }

  }// End of namespace awali::internal

  direction_t make_direction(const std::string& value) {
    return (direction_t) internal::enum_of_string("direction_t", value);
  }

  layout_t make_layout(const std::string& value) {
    return (layout_t) internal::enum_of_string("layout_t", value);
  }


  quotient_algo_t make_quotient_algo(const std::string& value) {
    return (quotient_algo_t) internal::enum_of_string("quotient_algo_t", value);
  }

  exp_to_aut_algo_t make_exp_to_aut_algo(const std::string& value) {
    return (exp_to_aut_algo_t)
           internal::enum_of_string("exp_to_aut_algo_t", value);
  }

  io_format_t make_io_format(const std::string& value) {
    return (io_format_t) internal::enum_of_string("io_format_t", value);
  }

  state_elim_order_t make_state_elim_order(const std::string& value) {
    return (state_elim_order_t)
           internal::enum_of_string("state_elim_order_t", value);
  }

  std::string name_of(direction_t val) {
    return internal::string_of_enum("direction_t", val);
  }

  std::string name_of(layout_t val) {
    return internal::string_of_enum("layout_t", val);
  }

  std::string name_of(quotient_algo_t val) {
    return internal::string_of_enum("quotient_algo_t", val);
  }

  std::string name_of(exp_to_aut_algo_t val) {
    return internal::string_of_enum("exp_to_aut_algo_t", val);
  }

  std::string name_of(io_format_t val) {
    return internal::string_of_enum("io_format_t", val);
  }

  std::string name_of(state_elim_order_t val) {
    return internal::string_of_enum("state_elim_order_t", val);
  }

  std::string default_extension_of(io_format_t val) {
    switch (val) {
      case DOT: return "dot";
      case FADO: return "fado";
      case GRAIL: return "grail";
      case FSM_JSON_V1: case FSM_JSON_V0: return "json";
      case PDF: return "pdf";
      case TEXT: return "txt";
      case SVG: return "svg";
    }
    return name_of(val);
  }

  }//end of namespace awali
#endif
