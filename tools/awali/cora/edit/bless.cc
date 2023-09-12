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

#include <cora/edit/bless.hh>
#include <cora/edit/token.hh>
#include <cora/edit/area.hh>

namespace awali { 
namespace cora {
namespace editter {

  void center(std::string& result, size_t width) {
   if (result.size() < width) {
     size_t extra = width - result.size();
     result = blank(extra/2) + result + blank((extra+1)/2);
   }
  }
  

  const std::string& 
  blank (size_t width) {
    static std::unordered_map<int,std::string> blanks;
    auto got = blanks.find(width); 
    if (got == blanks.end()) {
      std::string s(width,' ');
      blanks[width] = s;
      return blanks[width];
    } 
    else
      return got->second;
  }

  void 
  print_at(font_style_t fs, int y, int x, const std::string& str) {
    attron(fs);
    mvprintw(y,x,str.c_str());
    attroff(fs);
  }

  void
  print_at(bool b, int y, int x, const std::string& str)
  {
    print_at(b?STANDOUT:NO_STYLE,y,x,str);
  }

  void 
  print_at(int y, int x, const std::string& str) {
    print_at(NO_STYLE,y,x,str);
  }

  void
  print(bool standout,const std::string& str)
  {
    if (standout)
      attron(A_STANDOUT);
    printw(str.c_str());
    if (standout)
      attroff(A_STANDOUT);
  }
  

} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali 
