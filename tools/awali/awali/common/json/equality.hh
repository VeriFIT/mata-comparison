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

#ifndef COMMON_JSON_EQUALITY_HH
#define COMMON_JSON_EQUALITY_HH

#include <cstddef>

#include <awali/common/json/node.hh>
#include <awali/common/json_ast.hh>

namespace awali {
namespace json {

namespace internal {

  bool equal(node_t const* left, node_t const* right,  path_t* path = nullptr);

}//end of namespace awali::json::internal



/** Return the path where occurs the smallest difference between @pname{left}
 * and @pname{right}, or `nullptr` if the two trees are equal.
 * */
path_t* diff(json_ast_t left, json_ast_t right, bool ignore_metadata=true);

bool equal(json_ast_t left, json_ast_t right, bool ignore_metadata = true);



}//end of namespace awali::json
}//end of namespace awali


#endif
