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
#include <vector>

#include <ncurses.h>

#include <cora/edit/bless.hh>

#ifndef CORA_EDIT_TOKEN_HH
#define CORA_EDIT_TOKEN_HH

namespace awali { 
namespace cora {
namespace editter {

  static std::basic_string<chtype> taaaest;

  // A token is essentially a string that holds a font_style_t.
  // - each character is in fact stored as an int in order to allow usage
  //   of the special characters of ncurses.
  // - we use the special token_t::ARG to allow a replacement mechanism
  //   (operator /=)
  class token_t {
   public:
    std::vector<int> text;
    font_style_t font_style = NO_STYLE;
    static const int ARG;
    unsigned int n_args = 0;

    token_t(font_style_t ft = NO_STYLE);
    token_t(const int& i, font_style_t ft = NO_STYLE);
    token_t(const chtype& i, font_style_t ft = NO_STYLE);
    token_t(const std::vector<int>& v,font_style_t ft = NO_STYLE);
    token_t(std::initializer_list<int> l, font_style_t ft = NO_STYLE);
    token_t(const std::string& str, font_style_t ft = NO_STYLE);
    token_t(const char* str, font_style_t ft = NO_STYLE);
    token_t(const std::string& str, bool standout);
    token_t(const char& c, font_style_t ft = NO_STYLE);
    
    // Replace in this token any token_t::ARG with the content of the token
    // given in argument.
    token_t& operator /= (const token_t& token);
    
    size_t size() const { return text.size(); }
    void print_at(int y, int x) const;
    void print() const;
    
    bool operator !=(token_t const& other) const;
    bool operator ==(token_t const& other) const;
    
    token_t subtoken(size_t start, size_t len) const;
  };

  token_t& operator <<(token_t& token, const std::string& str);
  token_t& operator <<(token_t& token, int i);




   std::ostream& operator<< (std::ostream&, const token_t& token);
  

} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
