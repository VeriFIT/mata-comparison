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

#ifndef AWALI_JSON_AST_HH
#define AWALI_JSON_AST_HH

#include<awali/common/json/node.hh>
#include<awali/common/json/parser.hh>

#include<map>

namespace awali {

using json_ast_t = std::shared_ptr<json::object_t>;


namespace json_ast {
  
  /** Builds an empty {@link json_ast_t}. */
  inline json_ast_t empty() {return std::make_shared<json::object_t>();}

  /** Builds a `json_ast_t` from an input stream. */
  json_ast_t from(std::istream& i);
  
  /** This function serves to build metadata extra informations. */
  json_ast_t from_string_map(std::map<std::string, std::string> const& map);

  /** Reads json_tree from given file.
   *  
   *  This is a low-level function, and expects a valid path (relative or *
   *  absolute) without verifications. In particular, it does not look to
   *  example automata and expression.  
   */
  json_ast_t from_file(std::string const& filename);

}

std::ostream& put(json_ast_t tree, std::ostream& out);

}// end of namespace awali




#endif
