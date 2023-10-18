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

#include <awali/sttc/algos/js_print.hh>
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/algos/enumerate.hh>
#include <awali/sttc/algos/dot.hh>
#include<fstream>
//We work with NFAs (thus the "weight" is Boolean"):
//We work with NFAs (thus the "weight" is Boolean"):
#include <awali/sttc/weightset/b.hh>
//The labels are letters implemented by int:
#include <awali/sttc/ctx/lal_int.hh>

//Static features are defined in the awali namespace
using namespace awali::sttc;

using labelset_t = ctx::lal_int;
using weightset_t = b;
using context_t = context<labelset_t, weightset_t>;

int main() {
  /* To instanciate an automaton, it is required to define an objet that describe the weightset (semiring) and the labelset)
   */
  weightset_t b;
  labelset_t alph{0,1,10};
  //This information is stored into a "context" object.
  context_t ctx(alph,b);
  /* The type of an automaton is mutable_automaton<context_t>
     It must be built through a factor make_mutable_automaton
  */
  auto aut=make_mutable_automaton(ctx);

  unsigned s[3];
  for(int i=0; i<3; i++)
    s[i]=aut->add_state();
  /* a mutable_automaton is actually a (shared) pointer;
     to call the method, one have to use ->
  */
  aut->set_initial(s[0]);
  aut->set_final(s[2]);
  aut->set_transition(s[0], s[0], 0);
  aut->set_transition(s[0], s[0], 1);
  aut->set_transition(s[0], s[1], 0);
  aut->set_transition(s[1], s[2], 10);
  aut->set_transition(s[2], s[2], 0);
  aut->set_transition(s[2], s[2], 1);
  //print a json description of the automaton:
  js_print(aut, std::cout) << std::endl;

  auto det=determinize(aut);
  //print a json description of the determinization:
  std::cout << "Determinization:" << std::endl;
  js_print(det, std::cout) << std::endl;
  
  auto pol = shortest(aut, 5);
  for(auto p : pol)
    std::cout << alph.genset().format(p.first) << "  " << p.second << std::endl;

  std::cout << "Affichage avec dotty" << std::endl;
  std::string filename= "/tmp/dot.gv";
  std::ofstream o(filename);
  dot(aut, o, false, true, true) << std::endl;
  o.close();
  system("dotty /tmp/dot.gv");

  std::string filenam= "/tmp/ddot.gv";
  std::ofstream od(filenam);
  dot(det, od, false, true, true) << std::endl;
  od.close();
  system("dotty /tmp/ddot.gv");
}
