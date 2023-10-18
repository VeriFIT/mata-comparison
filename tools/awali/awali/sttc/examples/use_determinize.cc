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

#include <awali/sttc/automaton.hh>
#include <awali/sttc/algos/js_print.hh>
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/algos/add_path.hh>
#include <awali/sttc/algos/lal_lan_conversion.hh>

//Static features are defined in the awali namespace
using namespace awali::sttc;

int main() {
  /* The NFA factory requires a list of char letters and returns an empty
     mutable automaton.
  */
  auto aut=make_automaton({'a','b'});

  unsigned s[3];
  for(int i=0; i<3; i++)
    s[i]=aut->add_state();
  /* a mutable_automaton is actually a (shared) pointer;
     to call the method, one have to use ->
  */
  aut->set_initial(s[0]);
  aut->set_final(s[2]);
  aut->set_transition(s[0], s[0], 'a');
  aut->set_transition(s[0], s[0], 'b');
  aut->set_transition(s[0], s[1], 'a');
  aut->set_transition(s[1], s[2], 'b');
  aut->set_transition(s[2], s[2], 'a');
  aut->set_transition(s[2], s[2], 'b');
  //print a json description of the automaton:
  js_print(aut, std::cout) << std::endl;

  auto det=determinize(aut);
  //print a json description of the determinization:
  std::cout << "Determinization:" << std::endl;
  js_print(det, std::cout) << std::endl;

}
