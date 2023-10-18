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

#include <awali/sttc/tests/null_stream.hxx>

#include<awali/sttc/automaton.hh>
#include<awali/sttc/misc/add_epsilon_trans.hh>

using namespace awali;
using namespace awali::sttc;


int main(int argc, char **argv) {
  // Context of 4-tape transducer
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Build a NFA with epsilons" << std::endl;

  auto a = make_automaton_with_epsilon({'a','b'});
  state_t s[5];
  for(int i=0; i<5; ++i)
        s[i]= a->add_state();
  for(int i=0; i<4; ++i)
    a->set_transition(s[i], s[i+1], 'a');
  for(int i=1; i<4; ++i)
    a->set_transition(s[i], s[i+1], 'b');
  *osc << "First epsilon transition" << std::endl;
  auto eps = get_epsilon();
  a->set_transition(s[0], s[1], eps);
  *osc << "Second epsilon transition" << std::endl;
  set_epsilon_trans(a, s[1], s[2]);
  a->set_transition(s[0], s[0], 'a');
  //a->new_transition(s[0], s[0], 'b');
  a->set_initial(s[0]);
  a->set_final(s[3]);
  js_print(a, *osc) << std::endl;

  return 0;
}
