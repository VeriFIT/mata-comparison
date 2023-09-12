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

#ifndef COMMON_JSON_PARSER_HH
#define COMMON_JSON_PARSER_HH

#include<unordered_map>
#include<iostream>

#include <awali/common/json/node.hh>

namespace awali {
namespace json {

class parser_t {
public:
  struct position_t {
    /** Only positive values are allowed. Negative value indicate errors.*/
    int line;
    int column;

    static inline position_t tellg_unsupported() { return {-1,0}; }
    inline bool is_tellg_unsupported() { return (line == -1); }
    static inline position_t eof() { return {-2,0}; }
    inline bool is_eof() { return (line == -2); }
  };
protected:
  static std::string const null_repr;
  static std::string const true_repr;
  static std::string const false_repr;

    
  node_t* parse_array();
  node_t* parse_object();
  node_t* parse_number();
  node_t* parse_string();
  node_t* parse_constant(std::string const&);


  position_t position_of(int pos);
  
  void ignore_spaces();

  bool check(char e, std::string oth = "", std::unordered_map<char,std::string> names_override = {});
  bool inline check(char e, std::unordered_map<char,std::string> names_override)
  {return check(e, "", names_override);}

  int peek();
  std::string extract_and_unescape_string(bool quote = true);

  /** Whether on error occured. */
  bool _error;
  
  /** Error message if on error occured. */
  std::stringstream _error_message;
  
  /** Attempt to give localisation of parsing error (only set if an error
      occured). */
  std::list<uint_or_string_t> _error_path;
  
  std::istream& _in;
public:

  inline bool error() { return _error; }
  inline std::string error_message() { return _error_message.str(); }
 
  /* Can only be called once. */
  inline std::list<uint_or_string_t> move_error_path() 
  { return std::move(_error_path); }

  node_t* parse_node();
  parser_t(std::istream& i) : _error(false), _in(i) {}

  inline position_t position() { return position_of(_in.tellg()); };
  
  
  static std::ostream& escape_and_put(std::ostream&, std::string const&);
  static std::string const& escape(char);
  static std::string escape(std::string const&);
  static std::string unescape(std::string const&, bool quote = false);
};


node_t* parse(std::istream&);

}// end of namespace awali::json
}// end of namespace awali

#endif
