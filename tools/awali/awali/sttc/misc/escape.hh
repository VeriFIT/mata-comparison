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

#ifndef AWALI_MISC_ESCAPE_HH
# define AWALI_MISC_ESCAPE_HH

# include <iosfwd>
# include <string>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace awali { namespace sttc {

  std::ostream&
  str_escape(std::ostream& os, const int c)
  {
    std::ios_base::fmtflags flags = os.flags(std::ios_base::hex);
    char fill = os.fill('0');
    switch (c)
      {
      case -1:   os << "<end-of-file>"; break;
      case '\\': os << "\\\\";          break;
      case '"':  os << "\\\"";          break;
      case '\n': os << "\\n";           break;
      default:
        if (0 <= c && c <= 0177 && std::isprint(c))
          os << char(c);
        else
          os << "\\0x" << std::setw(3) << c;
        break;
      }
    os.fill(fill);
    os.flags(flags);
    return os;
  }

  std::string
  str_escape(const int c)
  {
    std::ostringstream o;
    str_escape(o, c);
    return o.str();
  }

  std::ostream&
  str_escape(std::ostream& os, const std::string& str)
  {
    for (auto c: str)
      str_escape(os, c);
    return os;
  }

  std::string
  str_escape(const std::string& s)
  {
    std::ostringstream o;
    str_escape(o, s);
    return o.str();
  }

}}//end of ns awali::stc

#endif // !AWALI_MISC_ESCAPE_HH
