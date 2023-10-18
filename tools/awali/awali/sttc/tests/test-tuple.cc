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
#include<awali/sttc/algos/js_print.hh>
//#include<awali/sttc/algos/is-sequential.hh>
#include<awali/sttc/algos/min_quotient.hh>
#include<awali/sttc/algos/proper.hh>
#include<awali/sttc/algos/is_of_finite_image.hh>
#include<awali/sttc/ctx/law_char.hh>
#include<awali/sttc/ctx/lan_int.hh>
#include<awali/sttc/algos/letterize_tape.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon
#include <awali/sttc/algos/aut_to_exp.hh> // is_epsilon
#include <awali/sttc/tests/null_stream.hxx>

using namespace awali::sttc;


int main(int argc, char **argv) {
  // Context of 4-tape transducer
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

      
  using context_t = context<ctx::lat<ctx::lan_char,ctx::lan_char,ctx::lan_char,ctx::lan_char>,b>;
  context_t ctx(std::make_tuple(ctx::lan_char({'a','b'}),
                          ctx::lan_char({'x','y'}),
                          ctx::lan_char({'u','v'}),
                                ctx::lan_char({'i','j'})),b());
  auto tdc=make_shared_ptr<mutable_automaton<context_t>>(ctx);
  unsigned b=tdc->add_state();
  unsigned f=tdc->add_state();
  unsigned i=tdc->add_state();
  unsigned a=tdc->add_state();
  tdc->set_initial(i);
  tdc->set_final(f);
  tdc->new_transition(i,a,{'a','y','u','i'});
  tdc->new_transition(a,i,{'a','y','u','j'});
  tdc->new_transition(i,f,{'a','y','v','i'});
  tdc->new_transition(a,b,{'a','y','v','j'});
  tdc->new_transition(b,f,tdc->labelset()->one());
  tdc->new_transition(f,b,tdc->labelset()->one());
  auto lb=(*tdc->labelset()).set<0>();  // epsilon for the first tape
  tdc->new_transition(a,a,{lb.one(),'x','u','i'});
  *osc << "***** A 4-tape transducer *****" << std::endl << std::endl;
  js_print(tdc, *osc,true) << std::endl << std::endl;

  *osc << "Is the image of every word finite ? " << std::endl;
  *osc << is_of_finite_image<0>(tdc) << std::endl << std::endl;
  require( !is_of_finite_image<0>(tdc), "The image should not be finite");
  
  *osc << "Remove epsilon transitions" << std::endl;
  auto pdc = proper(tdc);
  js_print(pdc, *osc,true) << std::endl << std::endl;
  require( is_proper(pdc), "The transducer is not proper");

  *osc << "Transducer --> Expression" << std::endl;
  using ratset_t = ratexpset_of<context_t>; 
  ratset_t ratset(get_rat_context(ctx), ratset_t::identities_t::trivial);
  auto exp = aut_to_exp(tdc);
  *osc << "Expression en Json:" << std::endl;
  js_print(ratset, exp, *osc) << std::endl << std::endl;
  *osc << "Expression:" << std::endl;
  print(ratset,exp,*osc) << std::endl;
  *osc << "On parse et affiche l'expression:" << std::endl;
  std::ostringstream os;
  print(ratset,exp,os);
  auto exp2 = parse_exp(ratset, os.str(),true,false);
  print(ratset,exp2,*osc) << std::endl << std::endl << std::endl;
  
 


  auto tdclw = make_shared_ptr<mutable_automaton<context<ctx::lat<ctx::lal_char,ctx::law_char>,awali::sttc::b>>>
    (context<ctx::lat<ctx::lal_char,ctx::law_char>,awali::sttc::b>(std::make_tuple(ctx::lal_char({'a','b'}), ctx::law_char({'x','y'})),awali::sttc::b()));

  unsigned s1=tdclw->add_state();
  unsigned s2=tdclw->add_state();
  unsigned s3=tdclw->add_state();
  unsigned s4=tdclw->add_state();
  tdclw->set_initial(s1);
  tdclw->set_final(s4);
  tdclw->new_transition(s1,s2,{'a',""});
  tdclw->new_transition(s2,s3,{'b',"x"});
  tdclw->new_transition(s3,s4,{'a',"yx"});
  js_print(tdclw, *osc,true) << std::endl;


  auto tdcsub = letterize_tape<1>(tdclw);
  js_print(tdcsub, *osc,true) << std::endl;
  

  
  //AFF<decltype(tdc->context().labelset()->set<1>())::print();
  
  //*osc << is_func(tdc) << std::endl;

  //print_aff(tdc->labelset());
  
  /*
  sequential_tester<decltype(tdc)> st(tdc);
  st.compute_scc();
  for(auto p: tdc->states())
    *osc << p << " in " << st.scc_of[p] << std::endl;
  for(unsigned i=0; i< st.strongly_con_comp.size(); ++i) {
    *osc << i << " : ";
    for(auto p: st.strongly_con_comp[i])
      *osc << p << ' ';
    *osc << std::endl;
  }
  */
  //test_zero_comp(tdc);
  /*
  std::vector<std::list<unsigned> > equiv;
  hopcroft_quotient(tdc, equiv);
  tdc = merge(tdc, equiv);
  */
  /*
"[a,13,itworks]" as a value of lat<lal_char(ab),lan<lal_int(121314)>,law_char(!?giknorstw)>.
  */
  *osc << "***** A 3-tape transducer *****" << std::endl;
  *osc << "Tape 0:letters without epsilon" << std::endl;
  *osc << "Tape 1:int with epsilon" << std::endl;
  *osc << "Tape 2:words" << std::endl << std::endl;

  using context2_t = context<ctx::lat<ctx::lal_char,ctx::lan_int,ctx::law_char>,awali::sttc::b>;
  context2_t ctx2(std::make_tuple(ctx::lal_char({'a','b'}),
                                  ctx::lan_int({12,13,14}),
                                  ctx::law_char({'i','t','w','o','r','k','s'})
                                  ),awali::sttc::b());
  
  std::string sc = "[a,13,itworks]";
  size_t p = sc.size();
  auto r = ctx2.labelset()->parse(sc,p);
  ctx2.labelset()->print(r,*osc) << std::endl;
  auto tdc3=make_shared_ptr<mutable_automaton<context2_t>>(ctx2);
  unsigned p3= tdc3->add_state();
  unsigned q3= tdc3->add_state();
  tdc3->set_initial(p3);
  tdc3->set_final(q3);
  tdc3->new_transition(p3,q3,r);
  js_print(tdc3,*osc) << std::endl;
  auto tdc3b = letterize_tape<2>(tdc3);
  js_print(tdc3b,*osc) << std::endl;
  return 0;
}
