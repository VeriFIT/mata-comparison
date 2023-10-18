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

#include <cora/edit/token.hh>


namespace awali { 
namespace cora {
namespace editter {

  const int token_t::ARG = -1;

  token_t::token_t(font_style_t ft) 
  : font_style(ft) 
  {}


  token_t::token_t(const int& i, font_style_t ft)
  : token_t(std::to_string(i),ft)
  {}

  
  token_t::token_t(const chtype& i, font_style_t ft)
  : token_t(ft)
  {
    *this << i;
  }


  token_t::token_t(const std::vector<int>& v,font_style_t ft) 
  : font_style(ft) 
  {
    for (int i : v)
      *this << i;
  }


  token_t::token_t(std::initializer_list<int> l, font_style_t ft) 
  : token_t(std::vector<int>(l),ft)
  {}


  token_t::token_t(const std::string& str, font_style_t ft) 
  : token_t(ft) 
  { 
    (*this) << str; 
  }


  token_t::token_t(const char* str, font_style_t ft)
  : token_t(std::string(str),ft) 
  {}
  

  token_t::token_t(const char& c, font_style_t ft)
  : token_t(ft) 
  { 
    text.push_back(c); 
  }


  token_t::token_t(const std::string& str, bool standout)
  : token_t(str, standout?STANDOUT:NO_STYLE)
  {}
    
  
  token_t& 
  token_t::operator/= (const token_t& token) 
  {
    for (unsigned i =0; i<text.size(); i++) 
      if (text[i] == token_t::ARG) {
        n_args--;
        text.erase(text.begin()+i);
        text.insert(text.begin()+i, token.text.begin(),token.text.end());
        this->font_style=token.font_style;
        break;
      }
    return *this;
  }


  token_t& 
  operator <<(token_t& token, const std::string& str) 
  {
    for (unsigned i = 0; i<str.size(); i++) {
      bool b = true;
      if (str[i] == '%') {
        b = false;
        if(str[i+1] == 's') {
          token.text.push_back(token_t::ARG);
          token.n_args++;
        } 
        else if (str[i+1] == '<')
          token.text.push_back(ACS_LARROW);
        else if (str[i+1] == '>') 
          token.text.push_back(ACS_RARROW);
        else if (str[i+1] == '^')
          token.text.push_back(ACS_UARROW);
        else if (str[i+1] == 'v')
          token.text.push_back(ACS_DARROW);
        else if (str[i+1] == '-')
          token.text.push_back(ACS_HLINE);
        else if (str[i+1] == '|')
          token.text.push_back(ACS_VLINE);
        else if (str[i+1] == '#')
          token.text.push_back(ACS_BLOCK);
        else if (str[i+1] == '.')
          token.text.push_back(ACS_BULLET);
        else if (str[i+1] == 'D')
          token.text.push_back(ACS_DIAMOND);
        else
          b = true;
      }
      if (b)
        token.text.push_back(str[i]);
      else 
        i++;
    }
    return token;
  }
    

  token_t& 
  operator<<(token_t& token, int i) 
  {
    token.text.push_back(i);
    if (i == token_t::ARG)
      token.n_args++;
    return token;
  }


  void 
  token_t::print_at(int y, int x) 
  const
  {
    move(y,x);
    this->print();
  }
  

  void 
  token_t::print() 
  const
  {
    attron(this->font_style);
    for (int i : this->text)
      if (i != token_t::ARG)
        addch(i);
    attroff(this->font_style);
 }


  bool 
  token_t::operator !=(token_t const& other) 
  const
  {
    return !(*this == other);
  }


  bool 
  token_t::operator ==(token_t const& other) 
  const
  {
    if (size() != other.size())
      return false;
    for (unsigned i = 0; i< size(); i++)
      if (this->text.at(i) != other.text.at(i))
        return false;
    return true;
  }

  token_t 
  token_t::subtoken(size_t start, size_t len) 
  const
  {
    if (start+len > text.size())
      len = text.size()-start;
    std::vector<int> newv(text.begin()+start, text.begin()+start+len);
    return token_t(newv,font_style);

  }


} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali
