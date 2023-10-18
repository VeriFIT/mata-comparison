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

#include <awali/dyn.hh>
#include<fstream>

using namespace awali::dyn;

int main() {

  automaton_t mc = make_transducer({"ab","xyz"});
  unsigned smc[2];
  smc[0]= mc->add_state();
  smc[1]= mc->add_state();
  auto eps = get_epsilon(mc);
  mc->set_initial(smc[0]);
  mc->set_final(smc[0]);
  //set_tdc_transition(mc,smc[0],smc[0],'a','x');
  set_tdc_transition(mc,smc[0],smc[0],{'a','x'});
  set_tdc_transition(mc,smc[0],smc[0],{'b','y'});
  set_tdc_transition(mc,smc[0],smc[1],{'b','z'});
  set_tdc_transition(mc,smc[1],smc[0],{'b',eps});
  /*
  std::ofstream out("tmp.json");
  mc->json(out);
  out.close();
  std::ifstream fic("tmp.json");
  dyn::automaton_t aut = dyn::parse_automaton(fic);
  fic.close();
  dyn::dot(aut, std::cout) << std::endl;

  automaton_t d = domain(mc);
  d->json(std::cout) << std::endl;
  */
  //mc->json(std::cout) << std::endl;
  automaton_t i = inverse(mc);
  i->json(std::cout) << std::endl;
  automaton_t threetapes = make_transducer({"ab","cd","xy"},"Z");
  unsigned tts[2];
  tts[0]=threetapes->add_state();
  tts[1]=threetapes->add_state();
  threetapes->set_initial(tts[0]);
  threetapes->set_final(tts[1]);
  set_tdc_transition(threetapes, tts[0], tts[1],{'a','d','x'});
  set_tdc_transition(threetapes, tts[1], tts[0],{'b','d','y'},2);
  threetapes->json(std::cout) << std::endl;

  automaton_t lasttapes = images(threetapes);
  lasttapes->json(std::cout) << std::endl;

  lasttapes = projection(threetapes,2);
  lasttapes->json(std::cout) << std::endl;

  automaton_t rt = make_automaton_from_context(tupleset({letterset("ab"), wordset("xy")}),weightset("B"));
  unsigned rts[2];
  rts[0]=rt->add_state();
  rts[1]=rt->add_state();
  //set_tdc_transition(rt,rts[0],rts[1],{'a',"xyx"}); //do not work...
  //rt->set_transition(rts[0],rts[1],std::make_tuple('a',"xyx"));
  //auto fe = get_stc_automaton<::lw_context_t>(rt);
  //fe->set_transition(rts[0],rts[1],std::make_tuple('a',"xyx"));
  rt->json(std::cout) << std::endl;
  /*
  std::cerr << mc->get_static_context() << std::endl;
  auto te = get_stc_automaton<::context_t>(mc);
  // sttc::outsplit_here<0>(te);

  auto ite = get_stc_automaton<::context_t>(i);
  compose(te, ite);
  */

  automaton_t comp = compose(mc,i);
  //comp->json(std::cout) << std::endl;
  //dot(comp, std::cout) << std::endl;

  automaton_t aut = divkbaseb(3,2);
  auto aa = eval(aut,mc);
  //aut->json(std::cout) << std::endl;
  dot(aa, std::cout) << std::endl;


  unsigned st[6];
  automaton_t tdc = make_transducer({"ab","ab"});
  for(unsigned i=0; i<6; ++i)
    st[i] = tdc->add_state();
  tdc->set_initial(st[0]);
  tdc->set_final(st[0]);
  tdc->set_final(st[5]);
  //auto eps = get_epsilon(tdc);
  set_tdc_transition(tdc, st[0], st[0], {'b', 'b'});
  set_tdc_transition(tdc, st[0], st[1], {'a', eps});
  set_tdc_transition(tdc, st[1], st[1], {'a', 'a'});
  set_tdc_transition(tdc, st[1], st[2], {'b', eps});
  set_tdc_transition(tdc, st[2], st[3], {'b', 'b'});
  set_tdc_transition(tdc, st[3], st[1], {eps, 'a'});
  set_tdc_transition(tdc, st[2], st[4], {eps, 'a'});
  set_tdc_transition(tdc, st[4], st[1], {'a', 'b'});
  set_tdc_transition(tdc, st[1], st[5], {eps, 'a'});
  set_tdc_transition(tdc, st[4], st[5], {eps, 'b'});
  tdc->json(std::cout) << std::endl;

  automaton_t lmc=proper(lift_tdc(tdc));
  lmc->json(std::cout) << std::endl;

}

