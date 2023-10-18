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

#include <iostream>

#include <awali/dyn.hh>

using namespace awali::dyn;

void test(automaton_t aut) {
  std::cout << "Testing with automaton " 
            << aut->get_name() << std::endl;
  automaton_t aut1 = minimal_automaton(aut);
  automaton_t aut2 = min_quotient(complete(determinize(proper(aut))));

  if (!are_isomorphic(aut1,aut2)) {
    std::cout << "Automata are not isomorphic." << std::endl;
    std::cout << aut1 << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << aut2 << std::endl;
    exit(1);
  }
}

int main() {
  std::vector<automaton_t> automata;
  for(auto const& a: loading::example_automata()) { 
    automaton_t aut = load(a.second.full());
    if (aut->get_context()->weightset_name() == "B" && !aut->is_transducer())
      test(aut);
    automata.push_back(aut);
  }
  
  return 0;
}
  
