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
#include <awali/sttc/algos/projection.hh>
#include <awali/sttc/ctx/lat.hh>
#include <awali/sttc/ctx/lan_char.hh>
#include <awali/sttc/ctx/lal_char.hh>
#include <awali/sttc/ctx/law_char.hh>
#include <awali/sttc/algos/compose.hh>
#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/algos/lift_tdc.hh>
#include <awali/sttc/factories/divkbaseb.hh>

#include <awali/sttc/tests/null_stream.hxx>

using namespace awali;
using namespace awali::sttc;


std::ostream * osc;

int main(int argc, char **argv) {
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  auto mc = make_transducer({'a','b'},{'x','y','z'});
  unsigned smc[2];
  smc[0]= mc->add_state();
  smc[1]= mc->add_state();
  auto eps = get_epsilon();
  mc->set_initial(smc[0]);
  mc->set_final(smc[0]);
  //set_tdc_transition(mc,smc[0],smc[0],'a','x');
  mc->set_transition(smc[0],smc[0],{'a','x'});
  mc->set_transition(smc[0],smc[0],{'b','y'});
  mc->set_transition(smc[0],smc[1],{'b','z'});
  mc->set_transition(smc[1],smc[0],{'b',eps});
  *osc << "--------------------------------" << std::endl;
  *osc << "        Transducer" << std::endl;
  *osc << "--------------------------------" << std::endl;
  js_print(mc, *osc) << std::endl;
  auto i = projections<1,0>(mc); // inverse the two tapes
  *osc << "--------------------------------" << std::endl;
  *osc << "        Inverse" << std::endl;
  *osc << "--------------------------------" << std::endl;
  js_print(i, *osc) << std::endl;

  using context_3t_t = context<ctx::lat<ctx::lan_char,ctx::lan_char,ctx::lan_char>,z>;
  
  auto threetapes =
    make_shared_ptr<mutable_automaton<context_3t_t>>
		    (context_3t_t(std::make_tuple(ctx::lan_char({'a','b'}),
						  ctx::lan_char({'c','d'}),
						  ctx::lan_char({'x','y'})),
				  z()));
  unsigned tts[2];
  tts[0]=threetapes->add_state();
  tts[1]=threetapes->add_state();
  threetapes->set_initial(tts[0]);
  threetapes->set_final(tts[1]);
  threetapes->set_transition(tts[0], tts[1],{'a','d','x'});
  threetapes->set_transition(tts[1], tts[0],{'b','d','y'},2);
  *osc << "--------------------------------" << std::endl;
  *osc << "        Transducer 3 tapes" << std::endl;
  *osc << "--------------------------------" << std::endl;
  js_print(threetapes,*osc) << std::endl;

  *osc << "--------------------------------" << std::endl;
  *osc << "        Last 2 tapes" << std::endl;
  *osc << "--------------------------------" << std::endl;
  auto lasttapes = images(threetapes);
  js_print(lasttapes,*osc) << std::endl;

  *osc << "--------------------------------" << std::endl;
  *osc << "        Last tape" << std::endl;
  *osc << "--------------------------------" << std::endl;
  auto lasttape = projection<2>(threetapes);
  js_print(lasttape,*osc) << std::endl;

  using context_tdc_t = context<ctx::lat<ctx::lal_char,ctx::law_char>,b>;
  auto rt =
    make_shared_ptr<mutable_automaton<context_tdc_t>>
		    (context_tdc_t(std::make_tuple(ctx::lal_char({'a','b'}),
						  ctx::law_char({'x','y'})),
				  b()));
  unsigned rts[2];
  rts[0]=rt->add_state();
  rts[1]=rt->add_state();
  rt->set_transition(rts[0],rts[1],{'a',"xyx"});
  *osc << "--------------------------------" << std::endl;
  *osc << "        Transducer lat/law" << std::endl;
  *osc << "--------------------------------" << std::endl;
  js_print(rt,*osc) << std::endl;
  /*
  std::cerr << mc->get_static_context() << std::endl;
  auto te = get_stc_automaton<::context_t>(mc);
  // sttc::outsplit_here<0>(te);

  auto ite = get_stc_automaton<::context_t>(i);
  compose(te, ite);
  */

  *osc << "--------------------------------" << std::endl;
  *osc << "        Composition" << std::endl;
  *osc << "--------------------------------" << std::endl;
  auto comp = compose(mc,i);
  js_print(comp, *osc) << std::endl;

  *osc << "--------------------------------" << std::endl;
  *osc << "        Evaluation" << std::endl;
  *osc << "--------------------------------" << std::endl;
  auto aut = divkbaseb(make_context({'0','1'}),3,2);
  auto aa = eval_tdc(aut,mc);
  js_print(aa, *osc) << std::endl;

  unsigned st[6];
  auto tdc = make_transducer({'a','b'},{'a','b'});
  for(unsigned i=0; i<6; ++i)
    st[i] = tdc->add_state();
  tdc->set_initial(st[0]);
  tdc->set_final(st[0]);
  tdc->set_final(st[5]);
  tdc->set_transition(st[0], st[0], {'b', 'b'});
  tdc->set_transition(st[0], st[1], {'a', eps});
  tdc->set_transition(st[1], st[1], {'a', 'a'});
  tdc->set_transition(st[1], st[2], {'b', eps});
  tdc->set_transition(st[2], st[3], {'b', 'b'});
  tdc->set_transition(st[3], st[1], {eps, 'a'});
  tdc->set_transition(st[2], st[4], {eps, 'a'});
  tdc->set_transition(st[4], st[1], {'a', 'b'});
  tdc->set_transition(st[1], st[5], {eps, 'a'});
  tdc->set_transition(st[4], st[5], {eps, 'b'});
  tdc->set_transition(st[5], st[5], {eps, 'a'});
  *osc << "--------------------------------" << std::endl;
  *osc << "        Transducer" << std::endl;
  *osc << "--------------------------------" << std::endl;
  js_print(tdc,*osc) << std::endl;

  *osc << "--------------------------------" << std::endl;
  *osc << "        Lift & proper" << std::endl;
  *osc << "--------------------------------" << std::endl;
  auto lmc=proper(lift_tdc(tdc));
  js_print(lmc,*osc) << std::endl;

 }
