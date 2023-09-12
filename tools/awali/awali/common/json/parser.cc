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
    std::stringstream o;
    switch (c) {
      case '\b':  o << "\\b"; break;
      case '\f':  o << "\\f"; break;
      case '\n':  o << "\\n"; break;
      case '\r':  o << "\\r"; break;
      case '\t':  o << "\\t"; break;
      case '"':   o << "\\\""; break;
      case '\\':  o << "\\\\"; break;
      default:
        if ('\x00' <= c && c <= '\x1f') {
            o << "\\u"
              << std::hex << std::setw(4) << std::setfill('0') << (int)c;
        }
        else
            o << c;
     }
     res[(unsigned char) c] = o.str();
  }
  return res;
}

static std::vector<std::string> const converter = init_converter();


void
parser_t::ignore_spaces() 
{
  while (_in.good() && std::isspace(_in.peek()))
    _in.get(); 
}

bool 
parser_t::check(
  char e, std::string oth, std::unordered_map<char,std::string> names_override) 
{ 
  ignore_spaces();
  int c;
  if ((c = _in.get()) != (int) e) {
    if (_in.eof())
      _error_message << "Reached end of input while looking for ";
    else {
      if (!_in.eof())
        _error_message << "Got unexpected character (";
        if (c >=20 && c <= 126)
          _error_message << (char) c;
        else
          _error_message << "\\" << (int) c ;
        _error_message << "). Expecting ";
    }

    for (char c : oth) {
      _error_message 
        << (names_override.count(c) ? names_override.at(c) : names.at(c))
        << " (";
      if (c >=20 && c <= 126)
        _error_message << (char) c;
      else
        _error_message << "\\" << (int) c ;
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
      _in.unget();
    _error = true;
    return false;
  }
  return true;
}

int
parser_t::peek() 
{
  ignore_spaces();
  return _in.peek();
}

std::string 
parser_t::extract_and_unescape_string(bool quote) 
{
  if (quote && !check('"',{{'"',"opening string delimiter"}}))
    return "";
  unsigned start_pos = _in.tellg();
  char c;
  std::ostringstream o;
  while(true) {
    if ( (quote && _in.peek()=='"') ||  _in.eof())
      break; 
    _in.get(c);
    if(c=='\\') {
      _in.get(c);
      switch(c) {
        case '\\':
        case '"':
          o << c;
          break;
        case 'b': o << '\b'; break;
        case 'n': o << '\n'; break;
        case 'f': o << '\f'; break;
        case 'r': o << '\r'; break;
        case 't': o << '\t'; break;
        case 'u': {
          char c1, c2, c3, c4 = '\0';
          _in >> c1;
          _in >> c2;
          _in >> c3;
          _in >> c4;
          if(c1=='0' && c2 =='0' && (c3 == '0' || c3 == '1')) {
            char c = (c3 == '1')?16:0;
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
              return o.str();
            }
            o << c;
          }
          else {
            _error_message << "Only unicode escaped character below "
              "1F are supported, i.e., of the form \\u00YZ, where Y = 0 or "
              "1.";
            _error = true;
            return o.str();
          }

        }
        break;
        default:
          o << '\\' << c;
      }
    }
    else
      o << c;
  }
  if (quote && !check('"', {{'"',"closing string delimiter"}})) {
    _in.clear();
    _in.seekg(start_pos); /* We return to starting position for error message
                            * purposes. */
    return "";
  }
  return o.str();
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
    _in.get();
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
    _in.get();
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
    _in.get();
    return o;
  }

  while(true) {
    std::string k = extract_and_unescape_string();
    if (_error || !check(':'))
      return o;
    node_t* child = parse_node();
    if (child != nullptr)
      o->push_back(k,child);
    if (_error) {
      _error_path.push_front(k);
      return o;
    }
    if(peek() != ',')
      break;
    _in.get();
  }
  check('}',",");
  return o;
}


node_t* 
parser_t::parse_number() 
{
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
  char c[6];
  unsigned p = _in.tellg();
  for (int x=0; x<target.size(); x++) {
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
parse(std::istream& in) 
{
  parser_t parser(in);
  node_t* node = parser.parse_node();
  if (parser.error()) {
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
  while ((_in.tellg() < pos) && _in.get(c)) {
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
  return converter[(unsigned char) c];
}

std::ostream& 
parser_t::escape_and_put(std::ostream& o, std::string const & str)
{
  o << '"';
  for (char c : str)
    o << escape(c);
  return (o << '"'); 
}


std::string 
parser_t::escape(std::string const& str) {
  std::stringstream ss;
  parser_t::escape_and_put(ss,str);
  return ss.str();
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
