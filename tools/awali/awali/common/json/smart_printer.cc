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
heighter_t::after_child(node_t const* node, uint_or_string_t const&,
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
  map[node]=parser_t::escape(node->value).size();
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
inline_sizer_t::after_child(array_t const* node, unsigned,
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
  return map.at(node);
}



unsigned
inline_sizer_t::height_of(node_t const* node)
{
  return heighter[node];
}



/*==============================================================================
   smart_printer_t
==============================================================================*/

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
  parser_t::escape_and_put(_out,str);
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
    if (node->arity() > 0) {
      unsigned max_child_size = 0;
      for (node_t const* child : *node)
        if (sizer[child] > max_child_size)
          max_child_size = sizer[child];
      unsigned item_width = max_child_size+3;
      unsigned count = (max()-indent_amount) / item_width;
      //item_width = (max()-indent_amount) / count;
      if (count > 1) {
        unsigned x = 0;
        unsigned count_down = node->arity();
//         unsigned remainder = ((node->arity()) % count);
//         if (remainder > 0)
//           remainder = count - remainder;
        for (node_t const* child : *node) {
          put_inline(_out, child);
          if (--count_down > 0) {
            _out << ",";
            if (++x == count) {
              indent();
              x = 0;
            }
            else
              _out << std::string (item_width-sizer[child]-1, ' ');
          }
//           else 
//               _out << std::string ((remainder+1)*item_width-sizer[child]-3, ' ');
        }
        stop(visit_t::IGNORE_UNVISITED_CHILDREN);
      }
    }
  }
  else
    _out << "[";
}


void
smart_printer_t::leave(array_t const*)
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
    if (node->arity() > 0) {
      unsigned max_item_size = 0;
      for (auto const& pair : *node) {
        unsigned item_size = sizer[pair.second]
                             + parser_t::escape(pair.first).size();
        if (item_size > max_item_size)
          max_item_size = item_size;
      }
      max_item_size += 2; // comma and colon
      max_item_size += 2; // # of spaces between items
      unsigned count = (max()-indent_amount+2) / max_item_size;
      if (count > 1) {
//         Next line makes takes all available horizontal space
//         max_item_size = (max()-indent_amount) / (count-1);
        unsigned x = 0;
        unsigned count_down = node->arity();
        for (auto const& pair : *node) {
          parser_t::escape_and_put(_out, pair.first) << ":";
          put_inline(_out, pair.second);
          if (--count_down > 0) {
            _out << ",";
            if (++x == count) {
              indent();
              x = 0;
            }
            else {
              unsigned item_size = sizer[pair.second] 
                                   + parser_t::escape(pair.first).size()+2;
              _out << std::string (max_item_size-item_size, ' ');
            }
          }
        }
        stop(visit_t::IGNORE_UNVISITED_CHILDREN);
      }
    }
  }
  else
    _out << "{";
}



void
smart_printer_t::leave(object_t const*)
{
  if (inliner == nullptr && _max != 0)  {
    decr_indent();
    _out << "}";
  }
  else
    _out << "}";
}



void
smart_printer_t::between_children(object_t const*)
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
smart_printer_t::after_child(node_t const*,
                             uint_or_string_t const&,
                             node_t const* child)
{
  if (!is_last(child))
    max_vect.pop_back();
}



void
smart_printer_t::before_child(node_t const*, uint_or_string_t const&,
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
smart_printer_t::before_child(array_t const*, unsigned,
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
smart_printer_t::before_child(object_t const*, std::string const& key,
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
: _out(o), indent_amount(0), columns(0), inliner(nullptr), _max(m)
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
put_inline(std::ostream& out, node_t const* node)
{
  smart_printer_t printer(out,0);
  printer.run(node);
  return out;
}

std::ostream& 
put(std::ostream& out, node_t const* node)
{
  smart_printer_t printer(out);
  printer.run(node);
  return out;
}


} //end of namespace json
} //end of namespace awali
#endif
