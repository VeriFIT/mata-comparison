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

#ifndef CORA_EDIT_SENTENCE_HH
#define CORA_EDIT_SENTENCE_HH

#include <initializer_list>
#include <list>
#include <ncurses.h>
#include <string>
#include <unordered_map>

#include <cora/edit/bless.hh>
#include <cora/edit/token.hh>

namespace awali { 
namespace cora {
namespace editter {


  class sentence_t : public std::list<token_t> {
  public:
    token_t sep = "";
    bool sep_ignorable = true; /* whether sep should be displayed at the start of a new line */
    font_style_t font_style = NO_STYLE;
    sentence_t(font_style_t ft) : font_style(ft) {}
    sentence_t() {}
    sentence_t(const std::string& str, font_style_t = NO_STYLE, const std::string& sep =" ");
//     sentence_t(const sentence_t& tokens);


    /* If apply is `true` insert spaces before first token and after last token. */
    std::pair<int,int> center(unsigned width, bool apply =false);

    // Computes and returns blank size to put in front and back of this so that
    // the total length of tokens is c_w and that the token at position t_pos
    // starts at position c_pos.
    // If the sentence_t is too long, it is trimmed !
    std::pair<int,int> align_before(unsigned t_pos, unsigned c_pos, 
        unsigned c_w);

    size_t length() const;

    /* Replace an ARG by a token in this sentence.*/
    sentence_t& operator/= (const token_t& rep);
    bool operator==(sentence_t const& other) const;
    
    void print_at(int y, int x) const; 

  };

  sentence_t& operator <<(sentence_t& tokens, token_t&& token);
  sentence_t& operator <<(sentence_t& tokens, const token_t& token);
  sentence_t& operator <<(sentence_t& tokens, const std::string& str);
  sentence_t& operator <<(sentence_t& tokens, const sentence_t& token);
  sentence_t& operator <<(sentence_t& tokens, const char* str);
  std::ostream& operator<< (std::ostream&, const sentence_t& tokens);

} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
