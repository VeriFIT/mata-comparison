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
#include <awali/sttc/factories/ladybird.hh>
#include <awali/sttc/algos/aut_to_exp.hh>
#include <awali/sttc/algos/thompson.hh>
#include <awali/sttc/weightset/z.hh>
#include <awali/sttc/weightset/q.hh>


#include <awali/sttc/tests/null_stream.hxx>

using namespace awali;
using namespace awali::sttc;

#ifndef AWALI_AUTOMATA_DEPOSITORY
#define AWALI_AUTOMATA_DEPOSITORY "../../automata/"
#endif

std::ostream * osc;

int main(int argc, char **argv) {
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

 
  *osc << "------------------------------------" << std::endl;
  *osc << "      Ladybird" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto a = ladybird(make_context({'a','b','c'}),4);
  js_print(a, *osc) << std::endl;
  *osc << "------------------------------------" << std::endl;
  *osc << "      Ladybird expression" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto e = aut_to_exp(a);
  auto re = get_ratexpset(a);
  js_print(re, e, *osc) << std::endl;
  *osc << "------------------------------------" << std::endl;
  *osc << "      Thompson" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto t=thompson(re, e);
  js_print(t, *osc) << std::endl;

  *osc << "------------------------------------" << std::endl;
  *osc << "      Thompson expression" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto ef = aut_to_exp(t);
  auto ref = get_ratexpset(t);
  js_print(ref, ef, *osc) << std::endl;
  *osc << "------------------------------------" << std::endl;
  *osc << "      Lift" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto b = lift(a);
  js_print(b, *osc) << std::endl;


  *osc << "------------------------------------" << std::endl;
  *osc << "      Automaton with epsilon" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto c = make_automaton_with_epsilon({'a','b'});
  c->set_transition(c->add_state(), c->add_state(), get_epsilon());
  js_print(c, *osc) << std::endl;

  *osc << "------------------------------------" << std::endl;
  *osc << "      Z-Automaton" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto d = make_automaton<z>({'a','b'});
  d->set_initial(d->add_state(),-5);
  d->set_transition(d->add_state(), d->add_state(),'a',3);
  js_print(d, *osc) << std::endl;

  *osc << "------------------------------------" << std::endl;
  *osc << "      Q-Automaton" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto aq = make_automaton<q>({'a','b'});
  state_t sq[2];
  aq->set_transition(sq[0]=aq->add_state(), sq[1]=aq->add_state(),'a',q_fraction_t(3,4));
  aq->set_initial(sq[0],q_fraction_t(-5));
  aq->set_final(sq[1]);
  js_print(aq, *osc) << std::endl;
  *osc << "------------------------------------" << std::endl;
  *osc << "      Q-Automaton expression" << std::endl;
  *osc << "------------------------------------" << std::endl;
  auto eq = aut_to_exp(aq);
  js_print(get_ratexpset(aq), eq, *osc) << std::endl;



  *osc << "------------------------------------" << std::endl;
  {
    std::map<std::string,std::string> map = { {"prenom",   "Victor"}, 
                                              {"nom",      "Marsault"}, 
                                              {"timestamp","zkzkzk"} };
    json_ast_t string_map_ast = json_ast::from_string_map(map);
    *osc << string_map_ast;
    for (auto p : string_map_ast->fields) {
      std::string key = p.first;
      std::string value = ((json::string_t*) (p.second))->value ;
      for (auto p2 : map)
        if (!key.compare(p2.first) && value.compare(p2.second))
          throw std::runtime_error(
            "Wrong mapping for key: \"" + key + "\".  Expected: \""
                                      + p2.second + "\".  Got: \""
                                      + value + "\".");
    }
    if (string_map_ast->fields.size() != 2)
      throw std::runtime_error("Did not dismiss reserved key");
   
    *osc << std::endl;

  }

  *osc << "Load automaton b1" << std::endl;
  
  std::ifstream in(AWALI_AUTOMATA_DEPOSITORY "b1.json");
  auto b1 = load_automaton<z>(in);
  in.close();

  return 0;
}
