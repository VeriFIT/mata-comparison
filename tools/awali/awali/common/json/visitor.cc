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


#ifndef COMMON_JSON_VISITOR_CC
#define COMMON_JSON_VISITOR_CC

#include <awali/common/json/visitor.hh>

namespace awali {
namespace json {

void visitor_t::visit(node_t const* node) 
{
  enter(node);
  switch (node->kind) {
    case node_kind_t::OBJECT:
      visit(node->object());
      break;
    case node_kind_t::ARRAY:
      visit(node->array());
      break;
    case node_kind_t::INTEGER:
      visit(node->integer());
      break;
    case node_kind_t::FLOATING:
      visit(node->floating());
      break;
    case node_kind_t::STRING:
      visit(node->string());
      break;
    case node_kind_t::BOOLEAN:
      visit(node->boolean());
      break;
    case node_kind_t::_NULL:
      return visit(node->null());
      break;
  }
  leave(node);
}


void 
visitor_t::visit(object_t const* node)
{
  unsigned i = 0;
  enter(node);
  if (!(this->is_stopped())) {
    for(auto a: node->fields)  {
      if (i > 0) {
        between_children((node_t const*)node);
        between_children(node);
      }
      before_child((node_t const*) node, a.first, a.second);
      before_child(node, a.first, a.second);
      _path_from_root.push_back(a.first);
      visit(a.second);
      _path_from_root.pop_back();
      after_child(node, a.first, a.second);
      after_child((node_t const*) node, a.first, a.second);
      i++;
      if (this->is_stopped())
        break;
    }
  }
  step_stop();
  if (!(this->is_stopped()))
    leave(node);
}


void 
visitor_t::visit(array_t const* node)
{
  enter(node);
  if (!(this->is_stopped())) {
    unsigned i = 0;
    for(auto a: node->values) {
      if (i > 0) {
        between_children((node_t const*)node);
        between_children(node);
      }
      before_child((node_t const*)node, i, a);
      before_child(node, i, a);
      _path_from_root.push_back(i);
      visit(a);
      _path_from_root.pop_back();
      after_child(node, i, a);
      after_child((node_t const*)node, i, a);
      i++;
      if (this->is_stopped())
        break;
    }
  }
  step_stop();
  if (!(this->is_stopped()))
    leave(node);
}



}// end of namespace json
}// end of namespace awali

#endif
