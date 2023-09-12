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

#ifndef COMMON_JSON_SMART_PRINTER_CC
#define COMMON_JSON_SMART_PRINTER_CC

#include <awali/common/json/smart_printer.hh>

namespace awali {
namespace json {

/* =============================================================================
 * heighter_t
============================================================================= */

void heighter_t::enter(node_t const* node)
{
  if (map.count(node) > 0)
    this->stop(visit_t::IGNORE_UNVISITED_CHILDREN);
  else
    map[node] = 0;
}



void
heighter_t::after_child(node_t const* node, uint_or_string_t const& uos,
                        node_t const* child)
{
  unsigned x  = map[child];
  unsigned& y = map[node];
  if (x >= y)
    y = x+1;
}



unsigned heighter_t::operator[] (node_t const* node)
{
  auto it = map.find(node);
  if (it != map.end())
    return it->second;
  this->visit(node);
  return map[node];
}



/* =============================================================================
 * inline_sizer_t
============================================================================= */

void
inline_sizer_t::leave(bool_t const* node)
{
  map[node] = node->value ? 4 : 5;
}



void
inline_sizer_t::leave(int_t const* node)
{
  std::stringstream ss;
  ss << node->value;
  map[node] = ss.str().length();
}



void
inline_sizer_t::leave(string_t const* node)
{
  map[node]=escape(node->value).size();
}



void inline_sizer_t::leave(float_t const* node)
{
  std::stringstream ss;
  ss << node->value;
  map[node] = ss.str().length();
}



void
inline_sizer_t::leave(null_t const* node)
{
  map[node] = 4;
}



void
inline_sizer_t::enter(object_t const* node)
{
  map[node] = 2;
}



void
inline_sizer_t::enter(array_t const* node)
{
  map[node] = 2;
}



void
inline_sizer_t::after_child(array_t const* node, unsigned i,
                            node_t const* child)
{
  map[node] += map[child];
}



void
inline_sizer_t::after_child(object_t const* node, std::string const& key,
                            node_t const* child)
{
  map[node] += key.length() + 2 + 1 + map[child];
  if (   (child->kind == OBJECT && child->arity() > 1)
      || (heighter[child] > 1) )
    map[node] += 1;
}



void
inline_sizer_t::between_children(object_t const* node)
{
  map[node] += 2;
}



void
inline_sizer_t::between_children(array_t const* node)
{
  if (heighter[node] == 1)
    map[node] += 1;
  else
    map[node] += 2;
}



unsigned
inline_sizer_t::operator[] (node_t const* node)
{
  return map.find(node)->second;
}



unsigned
inline_sizer_t::height_of(node_t const* node)
{
  return heighter[node];
}



/* =============================================================================
 * smart_printer_t
============================================================================= */

unsigned
smart_printer_t::max ()
{
  return max_vect.back();
}


void
smart_printer_t::indent(int i)
{
  _out << '\n' << std::string(indent_amount+i, ' ');
}


void
smart_printer_t::smart_printer_t::leaf(double d)
{
  std::stringstream ss;
  ss << d;
  std::string dbl_string = ss.str();
  _out << dbl_string;
  if (dbl_string.find_first_of(".Ee") == std::string::npos)
    _out << ".";
}


void
smart_printer_t::leaf(std::string const& str)
{
  escape_and_print(_out,str);
}


void
smart_printer_t::leaf(int i)
{
  _out << i;
}


void
smart_printer_t::leaf(std::nullptr_t)
{
  _out << "null";
}



void
smart_printer_t::leaf(bool b)
{
  if (b)
    _out << "true";
  else
    _out << "false";
}



void
smart_printer_t::incr_indent()
{
 if (path().empty() || path().back().is_integer())
   indent_amount += 1;
 else
   indent_amount += 3;
}



void
smart_printer_t::decr_indent()
{
 if (path().empty() || path().back().is_integer())
   indent_amount -= 1;
 else
   indent_amount -= 3;
}



bool
smart_printer_t::is_last (node_t const* child)
{
  if (child->parent == nullptr)
    return false;
  if (   child->parent->is(OBJECT)
      && (--(child->parent->object()->end()))->second == child)
    return true;
  if (   child->parent->is(ARRAY)
      && (*(--(child->parent->array()->end())) == child))
    return true;
  return false;
}



void
smart_printer_t::enter(array_t const* node)
{
  if (inliner == nullptr  && _max != 0) {
    incr_indent();
    if (path().empty() || path().back().is_integer())
      _out << "[";
    else
      _out << "  [";
  }
  else
    _out << "[";
}


void
smart_printer_t::leave(array_t const* node)
{
  if (inliner == nullptr && _max != 0) {
    _out << "]";
    decr_indent();
  }
  else
    _out << "]";
}



void
smart_printer_t::enter(object_t const* node)
{
  if (inliner == nullptr && _max != 0) {
    incr_indent();
    if (path().empty() || path().back().is_integer())
      _out << "{";
    else
      _out << "  {";
  }
  else
    _out << "{";
}



void
smart_printer_t::leave(object_t const* node)
{
  if (inliner == nullptr && _max != 0)  {
    decr_indent();
    _out << "}";
  }
  else
    _out << "}";
}



void
smart_printer_t::between_children(object_t const* node)
{
  _out << ",";
  if ((inliner == nullptr) && (_max != 0))
    indent();
  else
    _out << " ";
}



void
smart_printer_t::between_children(array_t const* node)
{
  _out << ",";
  if ((inliner == nullptr) && (_max != 0))
    indent();
  else if (sizer.height_of(node) != 1)
    _out << " ";
}



void
smart_printer_t::after_child(node_t const* node,
                             uint_or_string_t const& uos,
                             node_t const* child)
{
  if (!is_last(child))
    max_vect.pop_back();
}



void
smart_printer_t::before_child(node_t const* node, uint_or_string_t const& uos,
                              node_t const* child)
{
  if (is_last(child)) {
    if (is_last(child->parent))
      max_vect.back()--;
  }
  else
    max_vect.push_back(_max-1);
}



void
smart_printer_t::before_child(array_t const* node, unsigned i,
                              node_t const* child)
{
  if (inliner == nullptr && !child->is_leaf()) {
    if (    _max == 0
         || indent_amount+sizer[child] <= max() ) {
     inliner = child;
    }
  }
}



void
smart_printer_t::before_child(object_t const* node, std::string const& key,
                              node_t const* child)
{
  _out << '\"' << key<< "\":";
  bool b = ((child->kind == OBJECT && child->arity() > 1) || sizer.height_of(child) > 1);
  if ((inliner == nullptr || _max == 0) && !child->is_leaf()) {
    if (   _max == 0
        || (indent_amount+sizer[child]+2<=max() && sizer.height_of(child)<=3))
    {
      inliner = child;
      if (indent_amount+sizer[child]+key.length()+2+((int) b)>=max()) {
        indent(2);
      }
      else if (b)
        _out << " ";
    }
    else
      indent();
  }
  else if (b)
    _out << " ";
}



void
smart_printer_t::leave(node_t const* node)
{
  if (inliner == node) 
    inliner = nullptr;
}



smart_printer_t::smart_printer_t(std::ostream& o, unsigned m)
: _out(o), indent_amount(0), inliner(nullptr), _max(m)
{
  max_vect.push_back(_max-1);
}



void
smart_printer_t::run(json_ast_t tree)
{
  run(&(*tree));
}



void
smart_printer_t::run(node_t const* tree)
{
  if (_max != 0)
    sizer = inline_sizer_t(tree);
  visit(tree);
}



std::ostream& 
put(node_t const* node, std::ostream& out)
{
  smart_printer_t printer(out);
  printer.run(node);
  return out;
}


} //end of namespace json
} //end of namespace awali
#endif
