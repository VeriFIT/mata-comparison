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

#include<awali/sttc/automaton.hh>

using namespace awali;
using namespace awali::sttc;

/* 
    | a |     | b |
   /     \   /     \
  /   /   \ /   /   \
 /   / 1   |   / 2   \
|    __    |    __    |
|   /  \  ___  /  \   |
|   \__/   3   \__/   |
 \         |         / 
  \__   __/ \__   __/
    | c |     | d |

Switchs 1 and 2 are in position Left or Right
Switch 3 is in position Horizontal or Vertical

When a ball enters by gate 'a', it first meets the switch '1';
  depending on the position of the switch, the ball goes
  -- either to the left and leaves by gate 'c'
  -- or meets the switch '3' and 
      if '3' is vertical, the ball stays to the left part of the device
         (and leaves it by gate 'c')
      if '3' is horizontal, the ball  goes to the other part of the device
         (and leaves it by gate 'd')
When a ball enters by gate 'b', it first meets the switch '3';
  depending on the position of the switch, the ball goes
  -- either to the right and leaves by gate 'd'
  -- or meets the switch '3' and 
      if '3' is vertical, the ball stays to the right part of the device
         (and leaves it by gate 'd')
      if '3' is horizontal, the ball goes to the other part of the device
         (and leaves it by gate 'c')
Every time a ball meets a switch, the position of the switch is flipped.
 */



int main() {
  auto aut=make_transducer({'a','b'},{'c','d'});
  state_t s[8];
  static const int right1=1;
  static const int left2=2;
  static const int vertical3=4;
  std::string name="LLH";
  for(int i=0; i<8; i++) {
    s[i] = aut->add_state();
    aut->set_final(s[i]);  
  }
  aut->set_initial(s[left2]);
  for(int i=0; i<8; i++) {
    name[0]=(i & right1)?'R':'L';
    name[1]=(i & left2)?'L':'R';
    name[2]=(i & vertical3)?'V':'H';
    aut->set_state_name(s[i],name);
    char output;
    // when a ball enter by gate 'a'
    int next_state=i&left2; //the status of switch 2 does not change
    if(i&right1) // 1:R  -> next_state LX.
      if(i&vertical3) // 3:V  -> next_stat LXH
        output='c';
      else {   // 3:H -> next_state LXV
        next_state|=vertical3;
        output='d';
      }
    else { // 1:L -> next_state RXX
      next_state= i|right1;
      output='c';
    }
    aut->set_transition(s[i],s[next_state],{'a',output});
    // when a ball enter by gate 'b'
    next_state=i&right1; //the status of switch 1 does not change
    if(i&left2) // 2:L -> next_state XRX
      if(i&vertical3) // 3:V  -> next_stat XRH
        output='d';
      else {   // 3:H -> next_state XRV
        next_state|=vertical3;
        output='c';
      }
    else { // 2:R -> next_state XLX
      next_state= i|left2;
      output='d';
    }
    aut->set_transition(s[i],s[next_state],{'b',output});    
  }

  aut->set_name("flipper");
  aut->set_desc("Transducer that mimics a flipper game");
  js_print(aut, std::cout) << std::endl;
  return 0;
}


  
