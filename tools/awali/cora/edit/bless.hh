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

#include <string>
#include <unordered_map>
#include <sstream> 
#include <initializer_list>

#include <ncurses.h>

#ifndef CORA_BLESS_HH
#define CORA_BLESS_HH

namespace awali { 
namespace cora {
namespace editter {


  // Changes the given string so that it is now of size `width` by adding
  // anought spaces on both sides.
   void center(std::string& result, size_t width);
   
  // Custom wrapping around ncurses font style
  typedef enum font_style {
    NO_STYLE=A_NORMAL,      // normal
    STANDOUT=A_STANDOUT,    // highlighted (inverted color)
    BOLD=A_BOLD,            // bold font
    UNDERLINE= A_UNDERLINE, // underline font
    DIM= A_DIM,             // dimmed font
    BLINK= A_BLINK          // blink
  } font_style_t;
  

  // Prints given string at given absolute position with given font style.
  void print_at(font_style_t fs, int y, int x, std::string const& str);

  // Shorthand for print_at(NO_STYLE,y,x,str)
  void print_at(int y, int x, const std::string&  str);

  // Prints the string at pos (y,x,) in NO_STYLE or STANDOUT depending on given
  // boolean.
  void print_at(bool standout, int y, int x, std::string const& str);

  // Prints at cursor position in NO_STYLE or STANDOUT style depening on given
  // boolean
  void print(bool standout, std::string const& str);

  //return a reference to a string of size `width` containing only spaces.
  const std::string& blank (size_t width);


  static const int KEY_NUMPAD_0 = 1001;
  static const int KEY_NUMPAD_1 = KEY_NUMPAD_0+1;
  static const int KEY_NUMPAD_2 = KEY_NUMPAD_0+2;
  static const int KEY_NUMPAD_3 = KEY_NUMPAD_0+3;
  static const int KEY_NUMPAD_4 = KEY_NUMPAD_0+4;
  static const int KEY_NUMPAD_5 = KEY_NUMPAD_0+5;
  static const int KEY_NUMPAD_6 = KEY_NUMPAD_0+6;
  static const int KEY_NUMPAD_7 = KEY_NUMPAD_0+7;
  static const int KEY_NUMPAD_8 = KEY_NUMPAD_0+8;
  static const int KEY_NUMPAD_9 = KEY_NUMPAD_0+9;
  static const int KEY_NUMPAD_ENTER = KEY_NUMPAD_0+10;
  
} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali 


#endif // CORA_BLESS_HH
