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

#include <cassert>

#include <awali/dyn.hh>

using namespace awali::dyn;

int main() {
  automaton_t a = factory::ladybird(4,"abc");
  auto l = lift::lift(a);
  assert(l->num_states() == a->num_states()+2);
  state_t i = l->initial_states()[0];
  state_t t = l->final_states()[0];
  for(state_t s : l->states()) {
    if(s != i && s != t) {
      lift::eliminate_state(l, s);
    }
  }
  assert(l->num_states() == 2);
  assert(l->num_transitions() == 1);
  put(l, std::cout) << std::endl;
  return 0;
}
