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

#include <awali/sttc/weightset/z.hh>
#include <awali/sttc/ctx/law_char.hh>
#include <awali/sttc/algos/js_print.hh>
#include <awali/sttc/algos/allow_words.hh>
#include <awali/sttc/algos/letterize.hh>
#include <awali/sttc/core/mutable_automaton.hh>

//Static features are defined in the awali namespace
using namespace awali::sttc;

int main() {
  /* The NFA factory requires a list of char letters and returns an empty
     mutable automaton.
  */
  auto aut=make_shared_ptr<mutable_automaton<context<ctx::law_char,z>>>
    (context<ctx::law_char,z>(ctx::law_char({'a','b'}),z()));


  unsigned s[3];
  for(int i=0; i<3; i++)
    s[i]=aut->add_state();
  /* a mutable_automaton is actually a (shared) pointer;
     to call the method, one have to use ->
  */
  aut->set_initial(s[0],3);
  aut->set_final(s[2],5);
  aut->set_transition(s[0], s[0], "",7);
  aut->set_transition(s[0], s[1], "a",9);
  aut->set_transition(s[1], s[2], "ba",2);
  //print a json description of the automaton:
  js_print(aut, std::cout) << std::endl;
  auto lan=letterize(aut);
  //print a json description of the determinization:
  std::cout << "Cut up:" << std::endl;
  js_print(lan, std::cout) << std::endl;

  auto law=compact(allow_words(lan));
  js_print(law, std::cout) << std::endl;

}
