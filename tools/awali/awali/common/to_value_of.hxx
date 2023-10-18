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

#ifndef COMMON_VALUE_OF_STRING_HXX
#define COMMON_VALUE_OF_STRING_HXX

#include <string>
#include <sstream>
#include <awali/common/parse_exception.hh>

namespace awali { namespace internal {

  template<typename M>
  typename M::value_t to_value_of(M const& set, const std::string& str) {
    size_t i= str.size();  
    try {
      auto result = set.parse(str,i);
      if (i==0)
        return result;
    } 
    catch (const parse_exception& e) { }
    std::stringstream ss;
    ss << "String " << str << " is not a proper representation of a value of ";
    set.print_set(ss);
    ss << ".";
    throw parse_exception (ss.str());
  }
  
    
  template<typename M>
  typename M::genset_t::word_t 
  to_word_of(M const& set, const std::string& str)
  {
    try {
      auto result = get_wordset(set).parse(str);
      return result;
    }
    catch (parse_exception const&) {}
    std::stringstream ss;
    ss << "String " << str << " is not a proper representation of a word of ";
    set.print_set(ss);
    ss << ".";
    throw parse_exception (ss.str());
  }

  template<typename M>
  typename M::genset_t::letter_t 
  to_letter_of(M const& set, const std::string& str) 
  {
    size_t i = str.size();
    try {
      auto result = set.genset().parse_one_letter(str,i);
      if (i ==  0)
        return result;
    }
    catch (parse_exception const&) {}
    std::stringstream ss;
    ss << "String " << str << " is not a proper representation of a letter of ";
    set.print_set(ss);
    ss << ".";
    throw parse_exception (ss.str());
  }

}} // end of namespaces awali::internal and awali::dyn

#endif
