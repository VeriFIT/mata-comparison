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


#include <cora/edit/area.hh>
#include <awali/dyn/algos/sys.hh> //  dyn::warning_stream
#include <iostream>

namespace awali {
namespace cora {
namespace editter {

void
area_t::erase(font_style_t fs)
{
  int x, y;
  getyx(stdscr, y, x);
  const std::string& bl= blank(w);
  for (int x = 0; x<h; x++)
    print_at(fs, r+x, c, bl);
  move(y, x);
}


void
area_t::print_left(const sentence_t& tokens, size_t& y_offset, size_t& x_offset)
{
  token_t sep = tokens.sep;
  attron(tokens.font_style);
  bool first = true;
  for(const token_t& token : tokens) {
    size_t size = token.size();
    if ((long) size >= this->w) {
      if ((!first) && (!tokens.sep_ignorable
                       || ((long) (x_offset + sep.size())+1 < this->w))) {
        if (!((long) (x_offset + sep.size()) < this->w)) {
          x_offset= 0;
          y_offset++;
        }
        sep.print_at(this->r+y_offset, this->w+x_offset);
        x_offset+= sep.size();
      }
      size_t start = 0;
      while (start < size) {
        token.subtoken(start, this->w-x_offset)
        .print_at(this->r+y_offset, this->c+x_offset);
        start+=this->w-x_offset;
        if (start < size) {
          x_offset = 0;
          y_offset++;
        }
        else
          x_offset = this->w-(start-size);
      }
    }
    else {
      if ((long)(x_offset + size + sep.size()) > this->w) {
        x_offset = 0;
        y_offset++;
      }
      else
        if (!first) {
          sep.print_at(this->r+y_offset, this->c+x_offset);
          x_offset += sep.size();
        }
      token.print_at(this->r+y_offset, this->c+x_offset);
      x_offset += token.size();
    }
    first = false;
  }
  attroff(tokens.font_style);
}

/* ******************************************************** Class list_area_t */
int
list_area_t::end()
{
  int e = start+h-1;
  return (e >= size) ? size-1 : e-1;
}

void
list_area_t::normalize()
{
  // Normalizing index;
  if (index < 0 || size == 0)
    index = 0;
  else
    if (index >= size)
      index = size-1;

  if (start > size - (h-1)) {
    if ((h-1)>=size)
      start = 0;
    else
      start = size-(h-1);
  }
  if (index < start)
    start = index;
  if (index == start && (index != 0))
    start--;
  if (index > end()) {
    start = index-(h-1)+1;
    if (start < 0)
      start = 0;
  }
  if (index == end() && index != (size-1))
    start++;
}

void
list_area_t::action_up()
{
  index--;
  normalize();
}


void
list_area_t::action_down()
{
  index++;
  normalize();
}


void
list_area_t::set_size(int i)
{
  size = i;
  normalize();
}


void
area_t::print_title(std::string str, bool selected, font_style_t fs)
{
  sentence_t tokens(fs);
  tokens.sep = " ";
  if (selected) {
    token_t t(">>", NO_STYLE);
    tokens << t;
  }
  tokens << token_t(str, NO_STYLE);
  if (selected) {
    token_t t("<<", NO_STYLE);
    tokens << t;
  }
  tokens.center(this->w, true);
  this->start();
  this->overwrite(tokens);
}


void 
area_t::start() {
  this->move_at(0,0);
}

bool
area_t::overwrite_ch(int i, font_style_t fs)
{
  return overwrite_ch(i | fs);
}


bool 
area_t::overwrite_ch(int i)
{
  if (!this->holdscur()) {
    *dyn::warning_stream << "Attempting to write outside area." << std::endl;
    this->move_right();
    return false;
  }
  addch(i);
  size_t x, y;
  this->get_yx(y, x);
  if ((long) x == (long) this->w)
    new_line();
  return true;
}


bool 
area_t::overwrite(token_t const& token, bool allow_cut) 
{
  attron(token.font_style);
  bool ret = true;
  if (!allow_cut && ((long) token.size() <= this->w)) {
    size_t y, x;
    this->get_yx(y,x);
    if ((long) token.size() > (long)(this->w-x))
      this->overwrite_str(blank(this->w-x));
  }
  for (int i : token.text)
    ret = this->overwrite_ch(i);
  attroff(token.font_style);
  return ret;
}


void 
area_t::get_yx(size_t &y, size_t &x) 
{
  int x2, y2;
  getyx(stdscr, y2,x2);
  if (!this->holdscur()) {
    x = this->w;
    y = this->h;
  }
  else {
    x = x2 - this->c;
    y = y2 - this->r;
  }
}


bool 
area_t::overwrite(sentence_t const& tokens) 
{
  attron(tokens.font_style);
  bool ret = true;
  bool first = true;
  token_t const& sep = tokens.sep;
  size_t sepsize = sep.size();
  for(const token_t& token : tokens) {
    size_t size = token.size();
    if (!first){
      if (!tokens.sep_ignorable)
        ret = this->overwrite(sep);
      else {
        size_t y, x;
        this->get_yx(y,x);
        if ( ((long)size+sepsize) <= ((long)this->w-x) )
          ret = this->overwrite(sep);
      }
    }
    ret = this->overwrite(token);
    first = false;
  }
  attroff(tokens.font_style);
  return ret;
}


bool 
area_t::holdscur()
{
  int x, y;
  getyx(stdscr, y, x);
  return !( (y < this->r) || (y >= (this->r+this->h))
            || (x < this->c) || (x >= (this->c+this->w)) );
}


bool 
area_t::delete_ch(font_style_t fs)
{
  if (!this->holdscur()) {
    *dyn::warning_stream << "Deleting outside area." << std::endl;
    return false;
  }
  int x, y;
  getyx(stdscr, y, x);
  bool b;
  do {
    this->move_right();
    b = this->holdscur();
    int c = b ? inch() : ' ' | fs;
    this->move_left();
    this->overwrite_ch(c);
  }
  while (b);
  move(y, x);
  return true;
}


bool 
area_t::insert_ch(int i, font_style_t fs)
{
  if (!this->holdscur()) {
    *dyn::warning_stream << "Inserting outside area." << std::endl;
    return false;
  }
  int x, y;
  getyx(stdscr, y, x);
  i = i | fs;
  while (this->holdscur()) {
    int old = inch();
    this->overwrite_ch(i);
    i = old;
  }
  move(y, x);
  this->move_right();
  return false;
}


void 
area_t::move_right(size_t amount)
{
  int x, y;
  getyx(stdscr, y, x);
  int z= x+amount-this->c;
  move( y + z/this->w, this->c + z%this->w);
}

  
void 
area_t::move_right() {
  int x,y;
  getyx(stdscr, y, x);
  if ((long) x+1 == this->w)
    move(y+1,this->c);
  else
    move(y,x+1);
}


void
area_t::move_left() 
{
  int x,y;
  getyx(stdscr, y, x);
  if ((long) x == this->c)
    move(y-1,this->w-1);
  else
    move(y,x-1);
}


void
area_t::move_left(size_t amount)
{
  int x, y;
  getyx(stdscr, y, x);
  int z= x-amount-this->c;
  move( y + z/this->w, this->c + z%this->w);
}

void
area_t::new_line()
{
  int x,y;
  getyx(stdscr, y, x);
  move(y+1,this->c);
}

void
area_t::move_at(size_t y_offset, size_t x_offset)
{
  move(this->r+y_offset, this->c+x_offset);
}

bool
area_t::overwrite_str(std::string const& str, font_style_t fs)
{
  attron(fs);
  bool b = true;
  size_t i = 0;
  for(i=0; i < str.length(); i++) {
    if (b)
      b = this->overwrite_ch(str[i]);
    else
      break;
  }
  attroff(fs);
  if (!b)
    this->move_right(str.length()-i);
  return b;
}


void
list_area_t::print_row(bool selected, int i, sentence_t& tokens,
                       int before, int after)
{
//     if (record)
//       row_records.push_back(row_record_t{selected,i,tokens,before,after});
  if ((i+1-start >= h) || i < start)
    return;
  if (i == start && i != 0) {
    std::stringstream ss;
    ss << "^^^ " << i+1 << " more ^^^";
    std::string str = ss.str();
    center(str, w-2);
    print_at(r+i+1-start, c+1, str);
  }
  else
    if (i+1-start == h-1  && i != size-1) {
      std::stringstream ss;
      ss << "vvv " << (size-i) << " more vvv";
      std::string str = ss.str();
      center(str, w-2);
      print_at(r+i+1-start, c+1, str);
    }
    else {
//       center(str,w-2);
      if (selected && i==index) {
//         tokens.font_style = STANDOUT;
        attron(STANDOUT);
        print_at (r+i+1-start, c+1, blank(before).c_str());
        print_at (r+i+1-start, c+w-1-after, blank(after).c_str());
//         attroff(STANDOUT);
      }
      tokens.print_at(r+i+1-start, c+1+before);
      attroff(STANDOUT);
//       tokens.font_style = NO_STYLE;
//       for (size_t i = 1; i<tokens.size(); i++)
//         print_token(tokens[i]);
    }
}

} // end of namespaxe awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali
