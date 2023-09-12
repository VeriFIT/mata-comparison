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

#ifndef COMMON_JSON_EQUALITY_CC
#define COMMON_JSON_EQUALITY_CC

#include <algorithm>

#include <awali/common/json/equality.hh>

namespace awali {
namespace json {



bool 
internal::equal(node_t const* left, node_t const* right,  path_t* path) 
{
  if (left->kind != right->kind)
    return false;

  switch (left->kind) {
    case OBJECT: {
      auto itl = left->object()->begin();
      auto endl = left->object()->end();
      auto itr = right->object()->begin();
      auto endr = right->object()->end();
      while (itl != endl && itr != endr) {
        if (itl->first != itr->first)
          return false;
        if (!internal::equal(itl->second, itr->second, path)) {
          if (path != nullptr)
            path->push_back(itl->first);
          return false;
        }
        itl++;
        itr++;
      }
      return (itl == endl && itr == endr);
    }
    case ARRAY: {
      if (left->arity() != right->arity())
        return false;
      for(unsigned i = 0; i< left->arity(); i++)
        if (!internal::equal(left->at(i), right->at(i), path)) {
          if (path != nullptr)
            path->push_back(i);
          return false;
        }
      return true;
    }
    case INTEGER:
      return left->integer()->value == right->integer()->value;
    case FLOATING:
      return left->floating()->value == right->floating()->value;
    case STRING:
      return left->string()->value == right->string()->value;
    case BOOLEAN:
      return left->boolean()->value == right->boolean()->value;
    default:
      return true;
  }
}




path_t* 
diff(json_ast_t left, json_ast_t right, bool ignore_metadata) 
{
  path_t* path = new path_t();
  bool b = true;
  if (ignore_metadata) {
      auto itl = left->object()->begin();
      auto endl = left->object()->end();
      auto itr = right->object()->begin();
      auto endr = right->object()->end();
      while (itl != endl && itr != endr) {
        if (itl->first == "metadata")
          itl++;
        if (itr->first == "metadata")
          itr++;
        if (itl->first != itr->first) {
          b = false;
          break;
        }
        if (!internal::equal(itl->second,itr->second,path)) {
          path->push_back(itl->first);
          b = false;
          break;
        }
        itl++;
        itr++;
      }
      if (!(itl == endl && itr == endr))
        b = false;
  }
  else
    b = internal::equal(&*left, &*right, path);
  if (b) {
    delete (path);
    return nullptr;
  }
  else {
    std::reverse(path->begin(), path->end());
    return path;
  }
}


bool 
equal(json_ast_t left, json_ast_t right, bool ignore_metadata) 
{
  path_t * path = diff(left,right,ignore_metadata);
  if (path == nullptr)
    return true;
  delete(path);
  return false;
}



}//end of namespace awali::json
}//end of namespace awali

#endif
