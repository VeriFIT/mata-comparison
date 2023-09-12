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

#include <vector>
#include <cassert>

#include <awali/dyn.hh>

using namespace awali::dyn;

int main() {
  automaton_t a = factory::ladybird(12,"abc");
  automaton_t d = determinize(a);
  automaton_t m = min_quotient(d);
  assert(d->num_states() == (1<<12)-1);
  assert(m->num_states() == (1<<12)-1);
  m = min_coquotient(d);
  assert(m->num_states() == (1<<12)-1);
  m = minimal_automaton(a);
  assert(m->num_states() == (1<<12));

  a = factory::divkbaseb(6,2);
  std::vector<std::vector<unsigned int>> eq_class= {{3,6},{4,7}};
  m = quotient(a, eq_class);
  assert(m->num_states() == 4);
  //quotient is also tested with reduce in test-determinize
}
