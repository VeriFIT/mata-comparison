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

#ifndef AWALI_ATO_CC
#define AWALI_ATO_CC

#include<awali/common/ato.hh>
#include<awali/common/parse_exception.hh>
#include<iostream>
#include<sstream>
#include<limits>
#include<stdexcept>

namespace awali {   
  
  int strict_atoi(const std::string& s) {
    if(s.empty())
      throw parse_exception ("The empty string is not a proper representation of an integer.");
    size_t p;
    int r = std::stoi(s, &p);
    if(p!=s.length()) {
      std::stringstream ss;
      ss << "String " << s << " is not a proper representation of an integer.";
      throw parse_exception (ss.str());
    }
    return r;
    }

  unsigned strict_atou(const std::string& s) {
    static const unsigned long lim = std::numeric_limits<unsigned>::max();
    if(s.empty())
      throw parse_exception ("The empty string is not a proper representation of a natural number.");
    size_t p;
    unsigned long r = std::stoul(s, &p);
    if(r > lim) {
      std::stringstream ss;
      ss << "String " << s << " out of range for unsigned int.";
      throw std::out_of_range(ss.str());
    }
    if(p!=s.length()) {
      std::stringstream ss;
      ss << "String " << s << " is not a proper representation of a natural number.";
      throw parse_exception (ss.str());
    }
    return (unsigned) r;
  }

}//end of ns awali

#endif
