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

#include<awali/dyn.hh>
#include<cassert>

using namespace awali;
using namespace awali::dyn;

int main() {
  for (auto a : loading::example_ratexps()) {
    std::cout << std::endl
              << "========================================"
              << "========================================" << std::endl
              << "> Treating example ratexp: " << a.first << std::endl
              << "========================================"
              << "========================================" << std::endl;

    ratexp_t exp1 = load_exp(a.second.full());
    json_ast_t ast1 = to_json_ast(exp1);
    json_ast_t ast2 = json_ast::from_file(a.second.full());
    put(ast1, std::cerr);
    std::cerr << std::endl;
    put(ast2, std::cerr);
    json::path_t* path_ptr = diff(ast1,ast2);
    if (path_ptr)
      std::cerr << std::endl << std::endl << "diffpath:" << *path_ptr << std::endl;
  }
  for (auto a : loading::example_automata()) {
    std::cout << std::endl
              << "========================================"
              << "========================================" << std::endl
              << "> Treating example automaton: " << a.first << std::endl
              << "========================================"
              << "========================================" << std::endl;
    json_ast_t ast1 = json_ast::from_file(a.second.full());
    automaton_t aut1 = awali::dyn::internal::parse_automaton(ast1);
    put(ast1, std::cerr) << std::endl;

    json_ast_t ast2 = to_json_ast(aut1);
    automaton_t aut2 = awali::dyn::internal::parse_automaton(ast2);
    put(ast2, std::cerr) << std::endl;

    json::path_t* path_ptr = diff(ast1,ast2);
    if (path_ptr) {
      std::cerr << std::endl << "diffpath:" << *path_ptr << std::endl;
    }
    bool b1 = are_isomorphic(aut1,aut2);
    if (!b1)
      throw std::runtime_error(std::string("Problem with automaton ")+a.first);
  } 
}



