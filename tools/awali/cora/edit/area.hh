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

#include <cora/edit/bless.hh>
#include <cora/edit/token.hh>
#include <cora/edit/sentence.hh>

#ifndef CORA_AREA_HH
#define CORA_AREA_HH

namespace awali { 
namespace cora {
namespace editter {
  
  // Structure defining a printing (square area)
  typedef struct area {
    int r; // upper left corner row
    int c; // upper left corner column
    int h; // height
    int w; // width

    /** Erases this area, that is fill it with spaces; given font_style_t is
        applied to those spaces.
    */
    void erase(font_style_t fs = NO_STYLE);
    void print_left(const sentence_t& t) {size_t y = 0, x =0; print_left(t,y,x);}
    void print_left(const sentence_t& tokens, size_t& y_offset, 
      size_t& x_offset);
    void new_line(size_t& y_offset, size_t& x_offset, int i = 1) {
      y_offset+=i; x_offset=0;}

    // Print the title of the area, centered.
    void print_title(std::string str, bool selected = false, font_style_t fs = NO_STYLE);

    /* Puts the cursor on the first cell of this area_t.*/
    void start();
    bool overwrite_ch(int i);
    bool overwrite_ch(int i, font_style_t fs);
    bool overwrite(token_t const& token, bool allow_cut = false);
    bool overwrite(sentence_t const& tokens);
    bool insert_ch(int i, font_style_t = NO_STYLE);

    /* style given is for the very last space added*/
    bool delete_ch(font_style_t = NO_STYLE);
    bool holdscur();
    
    /* move one char left in the area */
    void move_left();

    /* move `amount` char left in the area */
    void move_left(size_t amount);

    /* move one char right in the area */
    void move_right();

    /* move `amount` char right in the area */
    void move_right(size_t amount);


    void new_line();

    /* Returns relative to upper left corner of this area.
     * If outside the area sets   `y` to `this->h`   and   `x` to `this->w`
     */
    void get_yx(size_t &y, size_t &x);


    void move_at(size_t y_offset, size_t x_offset);
    bool overwrite_str(std::string const& str, font_style_t fs = NO_STYLE);

  } area_t;

  //Specialization of area_t for printing vertical lists
  typedef struct list_area : public area_t {
    int index = 0; // currently selected element
    int start = 0; // current first displayed element 
                   // (not 0 if too many elements to display)
    int size = 0;  // current size of the list

    // Last index printed
    int end();


    // Correct  `index` and `start` so that slected
    void normalize();

    // Action of UP_ARROW on this list;
    void action_up();
    
    // Action of DOWN_ARROW on this list;
    void action_down();

    // Notify the area of how many elements are to be displayed
    void set_size(int i);
    
  
    // Print the given string at given row.
    // The row is given to the position in the list and not in the viewport: if
    // the row is not visible, nothing is printed.
    // The boolean `selected` tells whether the row `index` should be
    // highlighted or not.
//     void print_row(bool selected, int i, std::string str);

    void print_row(bool selected, int i, sentence_t& tokens, 
        int before = 0, int after = 0);
//         bool record = true);

//     struct row_record_t {
//       bool selected;
//       int i;
//       sentence_t tokens;
//       int before;
//       int after;
//     };
// 
//     std::vector<row_record_t> row_records;
// 
//     void clear_records() { row_records.clear(); }
//     void draw_from_records(bool clear = true);

  } list_area_t;


} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali


#endif

