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

#ifndef COMMON_JSON_PARSER_CC
#define COMMON_JSON_PARSER_CC

#include <awali/common/json/parser.hh>

namespace awali {
namespace json {

static std::unordered_map<char,std::string> const names = {
  {',', "collection separator"},
  {'[', "array opening delimiter"},
  {']', "array closing delimiter"},
  {'{', "object opening delimiter"},
  {'}', "object closing delimiter"},
  {':', "key-value separator"},
  {'"', "string delimiter"}
};

std::string const parser_t::null_repr = "null";
std::string const parser_t::true_repr = "true";
std::string const parser_t::false_repr = "false";

std::vector<std::string> init_converter () {
  std::vector<std::string> res;
  for (int i = std::numeric_limits<char>::min(); 
       i <= (int) std::numeric_limits<char>::max();
       i++)
    res.push_back("");
  for (int i = std::numeric_limits<char>::min(); 
       i <= std::numeric_limits<char>::max();
       i++)
  {
    char c = (char) i;
    std::string s;
    switch (c) {
      case '\b':  s += "\\b"; break;
      case '\f':  s += "\\f"; break;
      case '\n':  s += "\\n"; break;
      case '\r':  s += "\\r"; break;
      case '\t':  s += "\\t"; break;
      case '"':   s += "\\\""; break;
      case '\\':  s += "\\\\"; break;
      default:
        if ('\x00' <= c && c <= '\x1f') {
            s += "\\u00";
            s += "00";
            s += (c >= '\x10'?'1':'0');
            int d = c%16;
            if (d<10)
              s += (d+'0');
            else
              s += ((d-10)+'A');
        }
        else
            s += c;
     }
     res[(unsigned char) c] = std::move(s);
  }
  return res;
}



bool 
parser_t::check( char e, std::string const& oth, 
                 std::unordered_map<char,std::string> const& names_override) 
{ 
  int c;
  if ((c = get()) != (int) e) {
    if (_in.eof())
      _error_message << "Reached end of input while looking for ";
    else {
      _error_message << "Got unexpected character (";
      if (c >=20 && c <= 126)
        _error_message << (char) c;
      else
        _error_message << "\\" << (int) c ;
      _error_message << ") while looking for ";
    }

    for (char x : oth) {
      _error_message 
        << (names_override.count(x) ? names_override.at(x) : names.at(x))
        << " (";
      if (x >=20 && x <= 126)
        _error_message << (char) x;
      else
        _error_message << "\\" << (int) x ;
      _error_message << "), ";
    }
    if (!oth.empty())
      _error_message << "or ";
    _error_message 
      << (names_override.count(e) ? names_override.at(e) : names.at(e))
      << " (";
    if (e >=20 && e <= 126)
      _error_message << (char) e;
    else
      _error_message << "\\" << (int) e ;
    _error_message << ").";

    if (!_in.eof())
      unget(c);
    _error = true;
    return false;
  }
  return true;
}

int
parser_t::peek(bool ignore_spaces) 
{
  if (_current_char == none) {
    if (ignore_spaces)
      while (_in.good() && std::isspace(_current_char = _in.get())) {}
    else
      _current_char = _in.get();
  }
  return _current_char;
}

int parser_t::get(bool ignore_spaces) 
{
  int i = peek(ignore_spaces);
  _current_char = none;
  return i;
}

void parser_t::unget(int c) 
{
  _current_char = c;
}

void parser_t::reset_lookahead() 
{
  if(_current_char != none) {
    _in.unget();
    _current_char = none;
  }
}


std::string 
parser_t::extract_and_unescape_string(bool quote) 
{
  static std::unordered_map<char,std::string> const open_map = {{'"',"opening string delimiter"}};
  static std::unordered_map<char,std::string> const close_map = {{'"',"closing string delimiter"}};
  if (quote && !check('"',open_map))
    return "";
  unsigned start_pos = _in.tellg();
  char c;
  std::string res;
  while(true) {
    if ( (quote && peek(false)=='"') ||  _in.eof())
      break; 
    c = get(false);
    if(c=='\\') {
      c = get(false);
      switch(c) {
        case '\\':
        case '"':
          res += c;
          break;
        case 'b': res += '\b'; break;
        case 'n': res += '\n'; break;
        case 'f': res += '\f'; break;
        case 'r': res += '\r'; break;
        case 't': res += '\t'; break;
        case 'u': {
          char c1, c2, c3, c4 = '\0';
          c1 = get(false);
          c2 = get(false);
          c3 = get(false);
          c4 = get(false);
          if(c1=='0' && c2 =='0' && (c3 == '0' || c3 == '1')) {
            c = (c3 == '1')?16:0;
            if ('0'<= c4 && c4 <= '9')
              c +=  (c4-'0');
            else if ('a'<= c4 && c4 <= 'f')
              c += 10 + c4 -'a';
            else if ('A'<= c4 && c4 <= 'F')
              c += 10 + c4 -'A';
            else {
              _error_message << "Only unicode escaped character "
                               "below 1F are supported, i.e., of the form "
                               "\\u00YZ, where Y = 0 or 1.";
              _error = true;
              return res;
            }
            res += c;
          }
          else {
            _error_message << "Only unicode escaped character below "
              "1F are supported, i.e., of the form \\u00YZ, where Y = 0 or "
              "1.";
            _error = true;
            return res;
          }

        }
        break;
        default:
          res +=  '\\';
          res += c;
      }
    }
    else
      res += c;
  }
  if (quote && !check('"', close_map)) {
    reset_lookahead();
    _in.clear();
    _in.seekg(start_pos); /* We return to starting position for error message
                            * purposes. */
    return "";
  }
  return res;
}


node_t* 
parser_t::parse_node() {
  int c;
  switch (c = peek()) {
    case '{' : return parse_object();
    case '[' : return parse_array();
    case '"' : return parse_string();
    case 'n' : return parse_constant(null_repr);
    case 'f' : return parse_constant(false_repr);
    case 't' : return parse_constant(true_repr);
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case 'e': case 'E': case '.': case '-': case '+':
      return parse_number();
    default: 
      _error_message << "Unexpected character (";
      if ( c>=20 && c <127) 
        _error_message << (char) c;
      else if (c == -1)
        _error_message << "EOF";
      else
        _error_message << '\\' << (int) c;
      _error_message << "). Expecting the start of a json value.";
      _error = true;
  }
  return nullptr;
}


node_t* 
parser_t::parse_array() 
{
  array_t* a=new array_t();
  if (!check('['))
    return a;
  if(peek()==']') {
    get();
    return a;
  }

  unsigned i = 0;
  while (true) {
    node_t* child = parse_node();
    if (child != nullptr)
      a->push_back(child);
    if (_error) {
      _error_path.push_front(i);
      return a;
    }
    if(peek() != ',')
      break;
    get();
    i++;
  }
  check(']');
  return a;
}


node_t* 
parser_t::parse_object() 
{
  object_t* o = new object_t();

  if (!check('{'))
    return o;
  if(peek()=='}') {
    get();
    return o;
  }

  while(true) {
    std::string k = extract_and_unescape_string();
    bool found = _only_metadata && k == "metadata";
    
    if (_error || !check(':'))
      return o;
    node_t* child = parse_node();
    if (_error)
      _error_path.push_front(k);
    if (child != nullptr)
      o->push_back(std::move(k),child);
    if (_error)
      return o;
    if (found) {
      _error = true;
      _error_message << "metadata found";
      return o;
    }
    if(peek() != ',')
      break;
    get();
  }
  check('}',",");
  return o;
}


node_t* 
parser_t::parse_number() 
{
  reset_lookahead();
  std::streampos pos = _in.tellg();
  
  int z;
  _in >> z;
  std::streampos pos2 = _in.tellg();
  
  _in.seekg(pos);
  
  double y;
  _in >> y;
  std::streampos pos3 = _in.tellg();
  
  if (pos2 == pos3)
    return new int_t(z);
  return new float_t(y);
}



node_t* 
parser_t::parse_string() 
{
  return new string_t(extract_and_unescape_string());
}




node_t* 
parser_t::parse_constant(std::string const& target) 
{
  reset_lookahead();
  char c[6];
  unsigned p = _in.tellg();
  for (unsigned x=0; x<target.size(); x++) {
    _in >> c[x];
    if (c[x] != target[x]) {
      parser_t::position_t pos = position_of(p);
      c[x+1]= '\0';
      _error_message << "Expecting JSON value (probably constant " << target 
                    << ") at line " << pos.line
                    << " and column " << pos.column
                    << ". Got string starting with " << c;
      _error = true;
      return nullptr;
    }
  }
  if (&target == &null_repr)
    return new null_t();
  if (&target == &true_repr)
    return new bool_t(true);
  if (&target == &false_repr)
    return new bool_t(false);
  _error_message << "Unknown constant: " << target ;
  _error = true;
  return nullptr;
}


node_t* 
parse(std::istream& in, bool stop_after_metadata) 
{
  parser_t parser(in, stop_after_metadata);
  node_t* node = parser.parse_node();
  if (parser.error() && !stop_after_metadata) {
    std::list<uint_or_string_t> list = parser.move_error_path();
    path_t path { std::make_move_iterator(std::begin(list)), 
                  std::make_move_iterator(std::end(list))   };
    if (node != nullptr)
      delete(node);
    parser_t::position_t pos = parser.position();
    throw parse_exception(parser.error_message(), "json::parser_t", path, 
                          pos.line, pos.column);
  }
  return node;
}


parser_t::position_t
parser_t::position_of(int pos) {
  if (_in.eof())
    return position_t::eof();
  if (pos < 0)
    return position_t::tellg_unsupported();
  std::ios_base::iostate state = _in.rdstate();
  _in.clear();
  _in.seekg(0);
  int line_count = 1;
  int start_line_count = _in.tellg();
  if (start_line_count < 0) /* tellg() is not supported by internal steam. */
    return position_t::tellg_unsupported();
  char c;
  while (_in.tellg() < pos) {
    c = get();
    if (c == '\n') {
      ++line_count;
      start_line_count = _in.tellg();
    }
  }
  _in.setstate(state);
  return {line_count,(((int) pos) - start_line_count+1)};
}


std::string const& 
parser_t::escape(char c) { 
  static std::vector<std::string> const converter = init_converter();
  return converter[(unsigned char) c];
}

bool
static need_escape(std::string const& str) {
  for (auto const& c : str)
    if (parser_t::escape(c).size() != 1)
      return true;
  return false;

}

std::ostream& 
parser_t::escape_and_put(std::ostream& o, std::string const & str)
{
  o << '"';
  for (char c : str)
    o << parser_t::escape(c);
  return (o << '"'); 
}


std::string 
parser_t::escape(std::string const& str) {
  if (need_escape(str)) {
    std::stringstream ss;
    parser_t::escape_and_put(ss,str);
    return ss.str();
  }
  else
    return '"'+str+'"';
}


std::string 
parser_t::unescape(std::string const& str, bool quote) {
  std::stringstream in(str);
  parser_t parser(in);
  std::string res = parser.extract_and_unescape_string(quote);
  if (parser.error()) {
    parser_t::position_t pos = parser.position();
    throw parse_exception(parser.error_message(), "parser_t::unescape",
                      {}, pos.line, pos.column );
  }
  return str;
}

}// end of namespace awali::json
}// end of namespace awali

#endif
