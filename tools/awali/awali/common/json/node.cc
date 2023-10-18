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
//
#ifndef AWALI_JSON_NODE_CC
#define AWALI_JSON_NODE_CC

#include<sstream>
#include<math.h>
#include<limits>

#include<awali/common/json/node.hh>
#include<awali/common/json/parser.hh>
// #include<awali/common/json/exception.hh>


/* =============================================================================
   uint_or_string  and  navigation path
============================================================================= */
namespace std {

std::ostream&
operator<<(std::ostream& o, awali::json::uint_or_string_t const& uos)
{
  if (uos.is_string())
    return (o << '"' << uos.string() << '"');
  else
    return (o << uos.integer());
}


std::ostream&
operator<<(std::ostream& o, awali::json::path_t const& path)
{
  o << '/';
  for (auto uos: path)
    o << uos << '/';
  return o;
}

}


namespace awali {
namespace json {
/* =============================================================================
   KIND
============================================================================= */
std::string const& string_of(node_kind_t kind) {
  static std::vector<std::string> names
   = {"OBJECT", "ARRAY", "INTEGER", "FLOATING", "STRING", "BOOLEAN", "_NULL"};
  return names[(unsigned)kind];
}

std::ostream& operator<<(std::ostream& o, node_kind_t kind) {
  return o << string_of(kind);
}


/* =============================================================================
   Exceptions
============================================================================= */
exception:: exception(std::string const& message, std::string const& caller, 
                      node_t const* node) 
: message(message), caller(caller), node(node)  
{
  this->path_to_root = node->path_to_root();
  std::stringstream what_stream;
  if (caller != "")
    what_stream << "[" << caller << "] ";
  if (node != nullptr)
    what_stream << "At position " << path_to_root << ":  ";
  what_stream << message;
  what_msg = what_stream.str();
}

exception:: exception(std::string const& message, std::string const& caller,
                      path_t path, int line, int col) 
: message(message), caller(caller), node(nullptr), path_to_root(std::move(path))
{
  std::stringstream what_stream;
  if (caller != "")
    what_stream << "[" << caller << "] ";
  if (line >= 0  &&  col >= 0) {
    what_stream << "At line " << line
                << " and column " << col
                << " of input";
  }
  else if (line == -1)
    what_stream << "At an unknown position in input (tellg() unsupported by stream)";
  else if (line == -2)
    what_stream << "At the end of input";
  else
    what_stream << "At an unknown position in input";
    
  if (!path_to_root.empty())
    what_stream << ", and inside node " << path_to_root << " in tree";
  what_stream << ":  ";
  what_stream << message;
  what_msg = what_stream.str();
}

kind_mismatch::kind_mismatch(std::string const& caller, node_t const* node,
                             std::initializer_list<node_kind_t> types)
  :
  exception(kind_mismatch::message_builder(node->kind, types),
            caller, node),
  expected_types{types}
{}


std::string 
kind_mismatch::message_builder(node_kind_t caller_kind,
                               std::initializer_list<node_kind_t> types) 
{
  std::stringstream ss;
  ss << "Node is of kind " << caller_kind << ", while ";
  if (types.size() <= 1)
    ss << "it is expected to be of kind ";
  else
    ss << "its kind is expected to be among {";
  bool first = true;
  for (node_kind_t kind : types) {
    if (first)
      first = false;
    else
      ss << ",";
    ss << kind;
  }
  if (types.size() > 1)
    ss << "}";
  ss << ".";
  return ss.str();
}


/* =============================================================================
   node_t
============================================================================= */


path_t node_t::path_to_root() const
{
  if (parent==nullptr)
    return {};
  path_t path = parent->path_to_root();
  switch(parent->kind) {
    case ARRAY: {
      unsigned i = 0;
      for (auto c : *parent->array()) {
        if (c == this) {
          path.push_back(i);
          return path;
        }
        i++;
      }
      break;
    }
    case OBJECT:
      for (auto c : *parent->object()) {
        if (c.second == this) {
          path.push_back(c.first);
          return path;
        }
      }
    break;
    default: {}
  }
  throw std::runtime_error("node_t::path_to_root() Should not happen.");
}



node_t const* node_t::at(std::string const&) const
{throw kind_mismatch ("node_t::at(string)", this, node_kind_t::OBJECT);}

node_t* node_t::at(std::string const&)
{throw kind_mismatch ("node_t::at(string)", this, node_kind_t::OBJECT);}

node_t* node_t::at(unsigned)
{throw kind_mismatch ("node_t::at(uint)", this, node_kind_t::ARRAY);}

node_t const* node_t::at(unsigned) const
{throw kind_mismatch ("node_t::at(uint)", this, node_kind_t::ARRAY);}


//node_t* 
//node_t::at(std::initializer_list<std::string> const& l) 
//{
//  node_t * n = this;
//  for (auto x: l)
//    n = this->at(x);
//  return n;
//}


//node_t const* 
//node_t::at(std::initializer_list<std::string> const& l) 
//const
//{
//  node_t const* n = this;
//  for (auto x: l)
//    n = this->at(x);
//  return n;
//}
//
//
//
//bool 
//node_t::has_child(std::initializer_list<std::string> const& l) 
//cons
//{ node_t const* n = this; 
//  for (auto x : l) 
//    if (n->has_child(x)) 
//      n = n->at(x); 
//    else return false; 
//      return true;
//}

bool 
node_t::has_path(json::path_t const& path) const 
{
  node_t const* n = this;
  for (auto x : path) 
    if (x.is_integer()) {
      if (!n->has_child(x.integer()))
        return false;
      else
        n = n->at(x.integer()); 
    }
    else {
      if (!n->has_child(x.string()))
        return false;
      else
        n = n->at(x.string()); 
    }
  return true;
}

// node_t*
// node_t::parse(std::istream& i)
// {
//   switch(awali::internal::peek(i)) {
//     case '{' : return object_t::parse(i);
//     case '[' : return array_t::parse(i);
//     case '"' : return string_t::parse(i);
//     case 'n' : return null_t::parse(i);
//     case 'f' : return bool_t::parse_false(i);
//     case 't' : return bool_t::parse_true(i);
//     default  : return float_t::parse(i);
//   }
// }


node_t*
node_t::at_path(path_t const& path, unsigned i)
{
  if (path.size() == i)
    return this;
  switch (this->kind) {
    case node_kind_t::OBJECT: {
      if (path[i].is_integer()) 
        break;
      std::string str = path.at(i).string();
      if (!this->object()->has_child(str)) {
        std::stringstream ss;
        ss << "Reached object node has no field " << str << ".";
        throw out_of_range(ss.str(), "node_t::at_path(path_t)", this);
      }
      return this->object()->at(str)->at_path(path,i+1);
    }
    case node_kind_t::ARRAY:{
      if (path[i].is_string())
        break;
      unsigned j = path.at(i).integer();
      if (j >= this->array()->values.size()) {
        std::stringstream ss;
        ss << "Reached array node has only "
           << this->array()->values.size()-1
           << "children, hence has no child at position "
           << path[i].integer()
           <<".";
        throw out_of_range(ss.str(), "node_t::at_path(path_t)", this);
      }
      return this->array()->at(i)->at_path(path,i+1);
    }
    default: break;
  }
  throw kind_mismatch("node_t::at_path(path)", this, 
                      {node_kind_t::ARRAY, node_kind_t::OBJECT});
}

node_t const*
node_t::at_path(path_t const& path, unsigned i)
const
{
  if (path.size() == i)
    return this;
  switch (this->kind) {
    case node_kind_t::OBJECT: {
      if (path[i].is_integer()) 
        break;
      std::string str = path.at(i).string();
      if (!this->object()->has_child(str)) {
        std::stringstream ss;
        ss << "Reached object node has no field " << str << ".";
        throw out_of_range(ss.str(), "node_t::at_path(path_t)", this);
      }
      return this->object()->at(str)->at_path(path,i+1);
    }
    case node_kind_t::ARRAY:{
      if (path[i].is_string())
        break;
      unsigned j = path.at(i).integer();
      if (j >= this->array()->values.size()) {
        std::stringstream ss;
        ss << "Reached array node has only "
           << this->array()->values.size()-1
           << "children, hence has no child at position "
           << path[i].integer()
           <<".";
        throw out_of_range(ss.str(), "node_t::at", this);
      }
      return this->array()->at(i)->at_path(path,i+1);
    }
    default: break;
  }
  throw kind_mismatch("node_t::at_path(path_t)", this, 
                      {node_kind_t::ARRAY, node_kind_t::OBJECT});
}


/* =============================================================================
   OBJECT
============================================================================= */
// object_t*
// object_t::parse(std::istream& i)
// {
//   object_t* o = new object_t();
//   awali::internal::check(i,'{');
//   if(awali::internal::peek(i)=='}')
//     return o;
//   while(1) {
//     std::string k(awali::internal::parsestring(i));
//     awali::internal::check(i,':');
//     node_t* child = node_t::parse(i);
//     o->push_back(k,child);
//     if(awali::internal::peek(i)!=',')
//       break;
//     awali::internal::check(i,',');
//   }
//   awali::internal::check(i,'}',",");
//   return o;
// }


object_t::~object_t()
{
  for(auto p : fields)
    delete p.second;
}


object_t*
object_t::push_front(std::string key, node_t* node)
{
  this->_fields.push_front(std::move(key),node);
  node->parent = this;
  return this;
}


object_t*
object_t::push_back(std::string key, node_t* node)
{
  this->_fields[std::move(key)] = node;
  node->parent = this;
  return this;
}


object_t*
object_t::insert_after(std::string const& ref_key, std::string key, 
                       node_t* node) 
{
  auto it = this->_fields.find(ref_key);
  if (it != this->_fields.end())
    it++;
  this->_fields.insert(it, std::move(key), node);
  node->parent = this;
  return this;
}


object_t*
object_t::insert_before(std::string const& ref_key, std::string key, 
                        node_t* node) 
{
  auto it = this->_fields.find(ref_key);
  this->_fields.insert(it,std::move(key),node);
  node->parent = this;
  return this;
}


object_t*
object_t::copy()
const
{
  object_t* obj = new object_t();
  for (auto p: fields)
    obj->push_back(p.first,p.second->copy());
  return obj;
}


node_t* 
object_t::at(std::string const& key)
{
  auto it = _fields.find(key);
  if (it != _fields.end())
    return it->second;
  std::stringstream ss;
  ss << "Node has no child associated with key \"" << key 
     << "\".";  
  throw out_of_range(ss.str(), "object_t::at(string)", this);
}


node_t const* 
object_t::at(std::string const& key) 
const
{
  auto it = _fields.find(key);
  if (it != _fields.end())
    return it->second;
  std::stringstream ss;
  ss << "Node has no child associated with key \"" << key 
     << "\".";  
  throw out_of_range(ss.str(), "object_t::at(string)", this);
}

/* =============================================================================
   ARRAY
============================================================================= */
// array_t*
// array_t::parse(std::istream& i)
// {
//   array_t* a=new array_t();
//   awali::internal::check(i,'[');
//   if(awali::internal::peek(i)==']') {
//     awali::internal::check(i,']');
//     return a;
//   }
//   while (true) {
//     a->push_back(node_t::parse(i));
//     if(awali::internal::peek(i)!=',')
//       break;
//     awali::internal::check(i,',');
//   }
//   awali::internal::check(i,']',",");
//   return a;
// }


array_t* 
array_t::push_back(node_t* p) {
  _values.push_back(p); 
  p->parent = this; 
  return this;
}

node_t* 
array_t::at(unsigned i) 
{
  if (i < values.size())
    return values.at(i);
  std::stringstream ss;
  ss << "This json::array_t has only " << values.size() << " children, "
     << " hence has no child at position " << i << ".";
  throw out_of_range(ss.str(), "array_t::at(unsigned)", this);
}


node_t const* 
array_t::at(unsigned i) 
const
{
  if (i < values.size())
    return values.at(i);
  std::stringstream ss;
  ss << "This json::array_t has only " << values.size() << " children, "
     << " hence has no child at position " << i << ".";
  throw out_of_range(ss.str(), "array_t::at(unsigned)const", this);
}

array_t::~array_t()
{
  for(node_t* p : values)
    delete p;
}


array_t*
array_t::copy()
const
{
  array_t* l = new array_t();
  for (node_t* v : values)
      l->push_back(v->copy());
  return (l);
}


/* =============================================================================
   INTEGER
============================================================================= */

bool 
int_t::to_bool() 
const
{
  if (value == 0 or value == 1)
    return value;
  throw coercion_exception("Only integer values 0 and 1 may be used as boolean."
                           "This int_t holds "+std::to_string(value)+".",
                           "int_t::to_bool()",
                           this
                           );
}


/* =============================================================================
   FLOATING
============================================================================= */
// node_t*
// float_t::parse(std::istream& i)
// {
//   double y;
//   i>>y;
//   if(std::floor(y)==y)
//     return new int_t((int)y);
//   return new float_t(y);
// }


bool 
float_t::to_bool() 
const
{
  try {
    int i = this->to_int();
    if (value == 0 or value == 1)
      return i;
  } catch (coercion_exception&) {}
  throw coercion_exception("Only double values 0.0 and 1.0 may be used as "
                           "boolean.  This float_t holds "
                           + std::to_string(value) + ".",
                           "float_t::to_bool()",
                           this);
}


int float_t::to_int() 
const
{
 if (isfinite(this->value)) { /* excludes NaN and infty double */
   double integral;
   double fractional = std::modf (this->value, &integral);
   if (fractional == .0) {
     if (integral > std::numeric_limits<int>::max())
       throw coercion_exception("The double value held by this `float_t` "
                                "cannot be coerced to int: it is greater than "
                                "max int.  "
                                "Held value is "+std::to_string(value)+"."
                                "float_t::to_int()",
                                this);
     if (integral < std::numeric_limits<int>::min())
       throw coercion_exception("The double value held by this `float_t` "
                                "cannot be coerced to int: it is lesser than "
                                "min int.  "
                                "Held value is "+std::to_string(value)+".",
                                "float_t::to_int()",
                                this);

     return value;
   }
 }
 throw coercion_exception("The double value held by this `float_t` "
                          "is not an integer. "
                          "Held value is "+std::to_string(value)+".",
                          "float_t::to_int()",
                          this
                          );
}


/* =============================================================================
   STRING
============================================================================= */


bool string_t::to_bool() 
const
{
  if (this->value == "true")
    return true;
  if (this->value == "True")
    return true;
  if (this->value == "false")
    return false;
  if (this->value == "False")
    return false;

  try {
   int i = this->to_int();
   if (i == 0 || i == 1)
    return i;
  } catch (coercion_exception&) {}
  
  try {
   double d = this->to_double();
   if (d == 0.0 || d == 1.0)
    return (d==1.0);
  } catch (coercion_exception&) {}

  throw coercion_exception("The string held by this `string_t` "
                           "is not a boolean: only \"true\", \"false\", "
                           "\"True\", \"False\" and string-representation of 0 "
                           "or 1 are considered valid."
                           "Held value is " + this->value + ".",
                            "string_t::to_bool()",
                            this);
}


int 
string_t::to_int() 
const
{
  try {
    size_t idx;
    int i = std::stoi(this->value, &idx);
    if (idx == this->value.length())
      return i;
  } catch (std::invalid_argument&){}
  throw coercion_exception("The string held by this `string_t` "
                           "is not an integer. "
                           "Held value is " + this->value + ".",
                           "string_t::to_int()",
                           this
                           );
}


double 
string_t::to_double()
const
{
  try{
    size_t idx;
    double d = std::stod(this->value, &idx);
    if (idx == this->value.length())
      return d;
  } catch (std::invalid_argument&){}
  throw coercion_exception("The string held by this `string_t` "
                           "is not a double. "
                           "Held value is " + this->value + ".",
                           "string_t::to_double()",
                           this
                           );
}


/* ============================================================================
   BOOLEAN
============================================================================= */
// bool_t*
// bool_t::parse(std::istream& i)
// {
//   switch(awali::internal::peek(i)) {
//     case 't' : return bool_t::parse_true(i);
//     case 'f' : return bool_t::parse_false(i);
//     default: throw parse_exception(
//                     i,
//                     "json parsing: expecting `true` or `false`.");
//   }
// }


// bool_t*
// bool_t::parse_false(std::istream& i)
// {
//   static const std::string target = "false";
//   char c;
//   for (int x=0; x<target.size(); x++) {
//     i >> c;
//     if (c != target[x])
//       throw parse_exception(i, "json parsing: expecting `false`.");
//   }
//   return new bool_t(false);
// }


// bool_t*
// bool_t::parse_true(std::istream& i)
// {
//   static const std::string target = "true";
//   char c;
//   for (int x=0; x<target.size(); x++) {
//     i >> c;
//     if (c != target[x])
//       throw parse_exception(i, "json parsing: expecting `true`.");
//   }
//   return new bool_t(true);
// }


/* =============================================================================
   _NULL
============================================================================= */
// null_t*
// null_t::parse(std::istream& i)
// {
//   static const std::string target = "null";
//   char c;
//   for (int x=0; x<target.size(); x++) {
//     i >> c;
//     if (c != target[x])
//       throw parse_exception(i, "json parsing: expecting `null`");
//   }
//   return new null_t();
// }



}//end of namespace awali::json
}//end of namespace awali


#endif
