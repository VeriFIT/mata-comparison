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

#include <awali/common/tuple.hh>
#include <awali/dyn.hh>

using namespace awali::dyn;

int main() {
  automaton_t a = automaton_t::from("ab",true);
  state_t s[5];
  for(int i=0; i<5; ++i)
        s[i]= a->add_state();
  for(int i=0; i<4; ++i)
    a->set_transition(s[i], s[i+1], 'a');
  for(int i=1; i<4; ++i)
    a->set_transition(s[i], s[i+1], 'b');
  a->set_eps_transition(s[0], s[1]);
  a->set_transition(s[0], s[0], 'a');
  //a->new_transition(s[0], s[0], 'b');
  a->set_initial(s[0]);
  a->set_final(s[3]);
  put(a,std::cout) << std::endl;

  return 0;
}

