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
#ifndef AWALI_JSON_AST_CC
#define AWALI_JSON_AST_CC

#include<set>
#include<fstream>

#include<awali/common/json_ast.hh>
#include<awali/common/json/smart_printer.hh>
#include<awali/common/no_such_file_exception.hh>

namespace awali {


json_ast_t
json_ast::from(std::istream& i) 
{
  /* Do not use std::make_shared */
  /* The created object now owns the pointers !*/
  return std::shared_ptr<json::object_t>(json::parse(i)->object());
}

json_ast_t internal::metadata_ast(std::istream& i) {
  return std::shared_ptr<json::object_t>(json::parse(i, true)->object());
}

/** Helper function to be used with metadata */
json_ast_t
json_ast::from_string_map(const std::map<std::string, std::string>& map) 
{
  std::set<std::string> static reserved_keys = {"timestamp"};

  json_ast_t ast = std::make_shared<json::object_t>();
  for (auto p : map)  {
    if (reserved_keys.find(p.first) == reserved_keys.end()) {
    /* silently ignore reserved_keys. */
      ast->push_back(p.first, new json::string_t(p.second));
    }
  }

  return ast;

}
 
json_ast_t
json_ast::from_file(std::string const& filename) {
    std::ifstream fic(filename);
    if (fic.good())
      return from(fic);
    else 
      throw no_such_file_exception("Could not open file: "+filename);
}



// std::ostream& 
// operator<<(std::ostream& o, json::node_t*  ast) 
// {
//   json::smart_printer_t printer(o);
//   printer.run(ast);
//   return o;
// }
// 
// 
// std::ostream& 
// operator<<(std::ostream& o, const json_ast_t& ast) 
// {
//   return o << &(*ast);
// }

std::ostream& put(json_ast_t tree, std::ostream& out) 
{return json::put(out,&*tree);}


}//end of namespace awali

#endif
