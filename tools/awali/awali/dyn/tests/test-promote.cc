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

using namespace awali::dyn;


int main() {
  automaton_t a = load("a1");
  context_t c = a->get_context();
  context::context_description cd = c->description();
  context_t law_z{context::wordset("ab"),context::weightset("Z")};
  context_t lal_b_abc{context::letterset("abc"),context::weightset("B")};
  context_t lal_b_a{context::letterset("a"),context::weightset("B")};
  context_t lal_b_ab{context::letterset("ab"),context::weightset("B")};
  context_t lal_z{context::letterset("ab"),context::weightset("Z")};
  context_t lan_z{context::nullableset(context::letterset("ab")),context::weightset("Z")};
  context_t lan_b{context::nullableset(context::letterset("ab")),context::weightset("B")};
  context_t law_b{context::wordset("ab"),context::weightset("B")};

  automaton_t b = promote_automaton(a,lan_b);
  std::cout << "Promotion lal_b -> lan_b worked" << std::endl;

  automaton_t b1 = promote_automaton(a,lal_z);
  std::cout << "Promotion lal_b -> lal_z worked" << std::endl;

  automaton_t b2 = promote_automaton(a,lan_z);
  std::cout << "Promotion lal_b -> lan_z worked" << std::endl;
  
  automaton_t b3 = promote_automaton(a,lal_b_abc);
  std::cout << "Promotion lal_b(ab) -> lal_b(abc) worked" << std::endl;

  try {
    automaton_t c = promote_automaton(a,lal_b_a);
    std::cout << "Promotion lal_b(ab) -> lal_b(a) should have failed" << std::endl;
    exit(1);
  }
  catch (std::runtime_error const& e) {}
  std::cout << "Promotion lal_b(ab) -> lal_b(a) failed as expected" << std::endl;


  try {
    automaton_t c = promote_automaton(b,lal_z);
    std::cout << "Promotion lan_b -> lal_z should have failed" << std::endl;
    exit(1);
  }
  catch (std::runtime_error const& e) {}
  std::cout << "Promotion lan_b -> lal_z failed as expected" << std::endl;

  try {
    automaton_t c = promote_automaton(b,lal_b_ab);
    std::cout << "Promotion lan_b -> lal_b should have failed" << std::endl;
    exit(1);
  }
  catch (std::runtime_error const& e) {}
  std::cout << "Promotion lan_b -> lal_b failed as expected" << std::endl;


}



