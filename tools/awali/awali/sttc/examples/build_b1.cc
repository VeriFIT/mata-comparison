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

#include<awali/sttc/automaton.hh>
#include<awali/sttc/weightset/z.hh>

using namespace awali;
using namespace awali::sttc;

int main() {
  auto aut=make_automaton<z>({'a','b'});
  state_t s[2];
  std::string name="p";
  for(int i=0; i<2; i++) {
    s[i] = aut->add_state();
    aut->set_state_name(s[i],name);
    ++name[0];
    aut->set_transition(s[i],s[i],'a');
    aut->set_transition(s[i],s[i],'b');
  }
  aut->set_transition(s[0],s[1],'b');
  aut->set_initial(s[0]);
  aut->set_final(s[1]);  

  aut->set_name("b1");
  aut->set_desc("Z-FA that counts the number of 'b' in words of {a,b}*");
  js_print(aut, std::cout) << std::endl;
}


  
