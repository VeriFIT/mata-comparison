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

#include <awali/dyn.hh>
#include <fstream>
using namespace awali::dyn;
using namespace awali;

int main() {
  
  for (auto a: loading::example_automata()) {
    std::cerr << "Automaton " << a.first << "...  ";
    std::string path = a.second.full();
    json_ast_t in = json_ast::from_file(path);
    automaton_t aut = parse_automaton(in);
    json_ast_t out = to_json_ast(aut);
    for (json_ast_t x : {in,out})
      if (x->has_path({"metadata","timestamp"}))
        x->at("metadata")->object()->erase("timestamp");
    if (!equal(in,out,false)) {
      if (awali::version::suffix == "")
        save(aut, path);
      std::cerr << "Updated." << std::endl;
      put(in,std::cerr) << std::endl;
      put(out,std::cerr) << std::endl;
      std::cerr << *diff(in,out,false) << std::endl;
    }
    else
      std::cerr << "Unchanged." << std::endl;
  }
}
