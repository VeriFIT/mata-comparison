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
#include<cassert>

using namespace awali::dyn;
using namespace awali::dyn::factory;

int main() {
  automaton_t aut = n_ultimate(4,"01");
  assert(aut->num_states() == 5);
  aut = divkbaseb(5, 2,"01");
  assert(aut->num_states() == 5);
  aut = double_ring(6, {0,2,3},"ab");
  assert(aut->num_states() == 6);
  assert(aut->num_finals() == 3);
  aut = witness(5,"abc");
  assert(aut->num_states() == 5);
  aut = cerny(5,"ab");
  assert(aut->num_states() == 5);
  aut = ladybird(5,"abc");
  assert(aut->num_states() == 5);
}
