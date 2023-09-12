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

#include<awali/sttc/automaton.hh>
#include<awali/sttc/algos/js_print.hh>
//#include<awali/sttc/algos/is-sequential.hh>
#include<awali/sttc/algos/min_quotient.hh>
#include<awali/sttc/algos/proper.hh>
#include<awali/sttc/algos/is_of_finite_image.hh>
#include<awali/sttc/ctx/law_char.hh>
#include<awali/sttc/algos/letterize_tape.hh>
#include<awali/sttc/algos/synchronize.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon
#include <fstream> // is_epsilon

using namespace awali::sttc;

int main() {
  std::cout << "First create a simple transducer" << std::endl;
  auto tdc=make_transducer<b>({'a','b'},{'x','y'});
  unsigned b=tdc->add_state();
  unsigned f=tdc->add_state();
  unsigned i=tdc->add_state();
  unsigned a=tdc->add_state();
  tdc->set_initial(i);
  tdc->set_final(f);
  tdc->new_transition(i,a,{'a','y'});
  tdc->new_transition(a,i,{'a','y'});
  tdc->new_transition(i,f,{'a','y'});
  tdc->new_transition(a,b,{'a','y'});
  tdc->new_transition(b,f,tdc->labelset()->one());
  tdc->new_transition(f,b,tdc->labelset()->one());
  using labelset_t =  labelset_t_of<decltype(*tdc)>::valueset_t<1>;
  labelset_t lb((*tdc->labelset()).set<1>());
  tdc->new_transition(a,a,{lb.one(),'x'});
  js_print(tdc, std::cout,true) << std::endl;

  std::cout << "Test wether every word has a finite image:" << std::endl;
  std::cout << is_of_finite_image<0>(tdc) << std::endl;

  /*
  auto pdc = proper(tdc);
  js_print(pdc, std::cout,true) << std::endl;
  */

  std::cout << "----------" << std::endl;
  std::cout << "Create a lal/law transducer" << std::endl;
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
  js_print(tdclw, std::cout,true) << std::endl;


  std::cout << "----------" << std::endl;
  std::cout << "Make the second tape letter-to-letter" << std::endl;
  auto tdcsub = letterize_tape<1>(tdclw);
  js_print(tdcsub, std::cout,true) << std::endl;

  std::cout << "----------" << std::endl;
  std::cout << "Load a tranducer" << std::endl;
  auto gr=make_transducer<awali::sttc::b>({'0','1'},{'0','1'});
  std::ifstream file("../../automata/gray.json");
  awali::json::object_t* p=awali::json::object_t::parse(file);
  file.close();
  gr = js_parse_aut_content(gr->context(), p->object("data")); 
  js_print(gr, std::cout,true) << std::endl;
  std::cout << "----------" << std::endl;
  std::cout << "... and synchronize it" << std::endl;
  gr=synchronize(gr);
  js_print(gr, std::cout,true) << std::endl;

  return 0;
}
