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
#include<awali/sttc/weightset/z.hh>
#include<awali/sttc/algos/determinize.hh>
#include<awali/sttc/algos/allow_words.hh>
#include<awali/sttc/algos/reduce.hh>

using namespace awali;
using namespace awali::sttc;

int main() {
  auto aut=make_automaton<z>({'a','b'});
  state_t i=aut->add_state();
  state_t f=aut->add_state();
  aut->set_transition(i,i,'a');
  aut->set_transition(i,f,'a');
  aut->set_transition(f,i,'a',-1);
  aut->set_initial(i);
  aut->set_final(f);

  std::cout << "-------" << std::endl;
  std::cout << "Z-automaton:" << std::endl;
  js_print(aut, std::cout) << std::endl;

  std::cout << "-------" << std::endl;
  std::cout << "Weighted determinization:" << std::endl;
  auto det = weighted_determinize(aut);
  js_print(det, std::cout) << std::endl;

  std::cout << "-------" << std::endl;
  std::cout << "Reduce the  determinization:" << std::endl;
  auto red = reduce(det);
  js_print(red, std::cout) << std::endl;

  state_t x=aut->add_state();
  aut->set_transition(i,x,'a');
  aut->set_transition(x,f,'b');
  std::cout << "-------" << std::endl;
  std::cout << "Add a path with length 2:" << std::endl;
  js_print(aut, std::cout) << std::endl;

  std::cout << "-------" << std::endl;
  std::cout << "Compact paths:" << std::endl;
  auto law = allow_words(aut);
  law=compact(law);
  js_print(law, std::cout) << std::endl;
}


  
