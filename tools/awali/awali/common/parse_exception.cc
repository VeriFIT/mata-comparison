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

#ifndef AWALI_PARSE_EXCEPTION_CC
#define AWALI_PARSE_EXCEPTION_CC

#include<awali/common/parse_exception.hh>
#include<sstream>

namespace awali { 

  static std::string build_message(std::istream& i, const std::string& s) {
    int line_count = 1;
    int start_line_count = 0;
    i.clear();
    int current_pos = i.tellg();
    if (current_pos < 0) 
      throw parse_exception(s);
    i.seekg(0);
    char c;
    while ((i.tellg() < current_pos) && i.get(c)) {
      if (c == '\n') {
        ++line_count;
        start_line_count = i.tellg();
      }
    }
    std::ostringstream os;
    os << "line " << line_count
       << " col " << (current_pos - start_line_count)
       << " : " << s;
    return os.str();
  }

  static std::string build_message(size_t p, const std::string& s) {
    std::ostringstream os;
    os << "position " << p
       << " : " << s;
    return os.str();
  }

  parse_exception::parse_exception(std::istream& i, const std::string& s) :
    std::runtime_error(build_message(i,s)) {}
  
  parse_exception::parse_exception(size_t p, const std::string& s) :
    std::runtime_error(build_message(p,s)) {}
}//end of ns awali

#endif
