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
#include <awali/sttc/ctx/lat.hh>
#include <awali/sttc/ctx/lan_char.hh>
#include <awali/sttc/automaton.hh>
#include <awali/sttc/weightset/z.hh>
#include <awali/sttc/algos/js_print.hh>
#include <awali/sttc/algos/js_parser.hh>
#include <awali/sttc/algos/aut_to_exp.hh>
#include<fstream>
//Static features are defined in the awali namespace
using namespace awali::sttc;

int main() {
  /* The NFA factory requires a list of char letters and returns an empty
     mutable automaton.
  */
  auto aut=make_automaton<z>({'a','b'});

  unsigned s[3];
  for(int i=0; i<3; i++)
    s[i]=aut->add_state();
  /* a mutable_automaton is actually a (shared) pointer;
     to call the method, one have to use ->
  */
  aut->set_initial(s[0],5);
  aut->set_final(s[2],4);
  aut->set_transition(s[0], s[0], 'a',4);
  aut->set_transition(s[0], s[0], 'b');
  aut->set_transition(s[0], s[1], 'a',3);
  aut->set_transition(s[1], s[2], 'b');
  aut->set_transition(s[2], s[2], 'a');
  aut->set_transition(s[2], s[2], 'b');
  aut->set_name("Test");
  aut->set_desc("Description test");
  //print a json description of the automaton:
  js_print(aut, std::cout) << std::endl;
  std::ofstream ofs("tmp.json");
  js_print(aut, ofs) << std::endl;
  ofs.close();
  std::ifstream ifs1("tmp.json");
  auto aut2 = load_automaton<z>(ifs1);
  ifs1.close();
  js_print(aut2, std::cout) << std::endl;
    
  /*
  awali::json::object_t* jo= awali::json::object_t::parse(ifs);
  auto autb=js_parse_aut_content(aut->context(), jo->object("data"));
  */
  std::ifstream ifs("../../automata/c1.json");
  auto autb = load_automaton<z>(ifs);
  ifs.close();
  js_print(autb, std::cout) << std::endl;

  auto exp=aut_to_exp_heuristic(aut);
  auto rs= get_ratexpset(aut);
  js_print(rs,exp,std::cout);

  //using tdcctx_t = context<ctx::lat<ctx::lal_char,ctx::lal_char>,b>;
  //tdcctx_t tdcctx(std::make_tuple(ctx::lal_char({'a','b'}),ctx::lal_char({'c','d'})));
  auto tdc = make_transducer({'a','b'},{'c','d'});
  js_print(tdc, std::cout) << std::endl;  
}
