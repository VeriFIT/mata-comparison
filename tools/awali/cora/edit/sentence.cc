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

#include <string>
#include <unordered_map>
#include <initializer_list>
#include <vector>

#include <ncurses.h>

#include <cora/edit/sentence.hh>

#ifndef CORA_EDIT_SENTENCE_CC
#define CORA_EDIT_SENTENCE_CC

namespace awali { 
namespace cora {
namespace editter {

  sentence_t& 
  operator <<(sentence_t& tokens, const token_t& token) 
  {
    tokens.push_back(token);
    return tokens;
  }
  

  sentence_t& 
  operator <<(sentence_t& tokens, token_t&& token) 
  {
    tokens.push_back(token);
    return tokens;
  }


  sentence_t& 
  operator <<(sentence_t& tokens, const sentence_t& other) 
  {
    for (const token_t& token: other)
      tokens << token;
    return tokens;
  }


  sentence_t& 
  operator<<(sentence_t& tokens, const std::string& str) 
  {
    return tokens << token_t(str);
  }


  sentence_t& 
  operator<<(sentence_t& tokens, const char* str) 
  {
    return tokens << token_t(str);
  }


  sentence_t::sentence_t(const std::string& str, font_style_t fs,
                     const std::string& sep_) 
  : sep(sep_), font_style(fs)
  {
    if (str != "") {
      size_t start= 0;
      size_t end = str.find(sep_,start);
      while (end != std::string::npos) {
        push_back(token_t(str.substr(start,end-start)));
        start = end+sep_.size();
        end= str.find(sep_,start);
      }
      push_back(token_t(str.substr(start)));
    }
  }

  
//   sentence_t::sentence_t(const sentence_t& token) 
//   : std::list<token_t>(token),
//     sep(token.sep), font_style(token.font_style)
//   {}

  
  size_t 
  sentence_t::length() 
  const 
  {
    size_t res = 0;
    size_t sep_size = sep.size();
    for (const token_t& t : *this) {
      if (res != 0)
        res += sep_size;
      res += t.size();
    }
    return res;
  }


  void 
  sentence_t::print_at(int y, int x) 
  const
  { 
    attron(this->font_style);
    move(y,x);
    bool b = false;
    for (const token_t &token : *this) {
      if (b)
        sep.print();
      else
        b = true;
      token.print();
    }
    attroff(this->font_style);
  }


  std::pair<int,int> 
  sentence_t::center(unsigned width, bool apply) 
  { 
    size_t length = this->length();
    if (length > width) {
      return {0,0};
    } else {
      int left = (width-length)/2;
      int right = width - length - left;
      if (apply) {
        if (left > 0 && ((unsigned) left) > sep.size())
          this->insert(this->begin(),token_t(blank(left-sep.size())));
        if (right > 0 && ((unsigned) right) > sep.size())
          this->push_back(token_t(blank(right-sep.size())));
      }
      return {left,right};
    }
  }


  std::pair<int,int> 
  sentence_t::align_before(unsigned t_pos, unsigned c_pos, unsigned c_w) 
  {
    std::pair<int,int> res = {0,0};
    size_t left = 0;
    size_t right = 0;
    {
      auto it = begin();
      for (unsigned i = 0; i<size(); i++) {
        if (i<t_pos)
          left += it->text.size();
        else
          right += it->text.size();
        it++;
      }
    }
    if (left+right > c_w) {
      left = 0;
      size_t old = 0;
      auto it = begin();
      for (unsigned i = 0; i<size(); i++) {
        size_t next = old + it->text.size();
        if (next >= (c_w-2)) {
          this->erase(++it,this->end());
          token_t& t = this->back();
          t.text.erase(t.text.begin()+c_w-old-2,t.text.end());
          break;
        }
        old = next;
        it++;
      }
      push_back(token_t("..",UNDERLINE));
      return std::pair<int,int>{0,0};
    }
    else if (left > c_pos)
      res.second = c_w-(left+right);
    else if (right > c_w-c_pos)
      res.first = c_w-(left+right);
    else {
      res.first = c_pos-left;
      res.second = c_w-c_pos-right;
    }
    return res;
  }


  sentence_t& 
  sentence_t::operator/= (const token_t& rep) 
  {
    for (token_t& t : *this)
      if (t.n_args>0) {
        t /= rep;
        break;
      }
    return *this;
  }
  

  bool 
  sentence_t::operator==(sentence_t const& other) const
  {
    if (size() != other.size())
      return false;
    auto it1 = this->cbegin();
    auto it2 = other.cbegin(); 
    while (it1 != this->cend()) {
      if (*it1 != *it2)
        return false;
      it1++;
      it2++;
    }
    return true;
  }


  // This is a debug function only
  std::ostream& 
  operator<< (std::ostream& o, const token_t& token) 
  {
   for (int i : token.text) {
     if (i == token_t::ARG)
       o << "%s";
     else if (i == (long)ACS_LARROW)
       o << "%<";
     else if (i == (long)ACS_RARROW)
       o << "%>";
     else if (i == (long)ACS_UARROW)
       o << "%^";
     else if (i == (long)ACS_DARROW)
       o << "%v";
     else if (i == (long)ACS_HLINE)
       o << "%-";
     else if (i == (long)ACS_VLINE)
       o << "%|";
     else if (i == (long)ACS_DIAMOND)
       o << "%#";
     else if (i == (long)ACS_BULLET)
       o << "%.";
     else
       o << (char) i;
   }
   return o;
  }


  std::ostream&
  operator<< (std::ostream& o, const sentence_t& tokens) 
  {
   bool b = false;
   for (const token_t& t: tokens) {
     if (b)
       o << tokens.sep;
     else
       b = true;
     o << t;
   }
   return o;

  }

} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
