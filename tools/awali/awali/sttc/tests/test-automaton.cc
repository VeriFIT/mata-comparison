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

#include <awali/sttc/tests/null_stream.hxx>

#include<awali/sttc/ctx/lan_char.hh>
#include<awali/sttc/ctx/lal_char.hh>
#include<awali/sttc/weightset/tupleset.hh>

#include<awali/sttc/automaton.hh>
#include<awali/sttc/weightset/z.hh>
#include<awali/sttc/weightset/zmin.hh>
#include<awali/sttc/weightset/q.hh>
#include<awali/sttc/weightset/r.hh>
#include<awali/sttc/weightset/c.hh>
#include<awali/sttc/weightset/f2.hh>

#include<awali/sttc/algos/allow_words.hh>
#include<awali/sttc/algos/lal_lan_conversion.hh>
#include<awali/sttc/algos/letterize.hh>
#include<awali/sttc/algos/lift.hh>
#include<awali/sttc/algos/lift_tdc.hh>
#include<awali/sttc/algos/partial_identity.hh>
#include<awali/sttc/algos/projection.hh>
#include<awali/sttc/algos/accessible.hh>
#include<awali/sttc/algos/factor.hh>
#include<awali/sttc/algos/proper.hh>
#include<awali/sttc/algos/standard.hh>
#include<awali/sttc/algos/transpose.hh>

using namespace awali::sttc;

template<typename A1, typename A2>
void compare(A1 aut1, A2 aut2) {
  auto it1=aut1->states().begin();
  auto it2=aut2->states().begin();
  for(; it1!=aut1->states().end(); ++it1,++it2) {
    assert(*it1==*it2);
    assert(aut1->get_state_name(*it1) ==
	   aut2->get_state_name(*it2));
  }
}



int main(int argc, char **argv) {
  // Context of 4-tape transducer
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Build a transducer" << std::endl;
  
  using context_t = context<ctx::lat<ctx::lan_char,ctx::lal_char>,b>;
  context_t ctx(std::make_tuple(ctx::lan_char({'a','b'}),
			       ctx::lal_char({'x','y'})),
		b());
  auto tdc=make_shared_ptr<mutable_automaton<context_t>>(ctx);

  *osc << "Build a NFA" << std::endl;
  auto autb = make_automaton({'a','b'});
  *osc << "Build a Z-WFA" << std::endl;
  auto autz = make_automaton<z>({'a','b'});
  *osc << "Build a (Z,min,+)-WFA" << std::endl;
  auto autzmin = make_automaton<zmin>({'a','b'});
  *osc << "Build a Q-WFA" << std::endl;
  auto autq = make_automaton<q>({'a','b'});
  *osc << "Build a R-WFA" << std::endl;
  auto autr = make_automaton<r>({'a','b'});
  *osc << "Build a C-WFA" << std::endl;
  auto autc = make_automaton<c>({'a','b'});
  *osc << "Build a F2-WFA" << std::endl;
  auto autf2 = make_automaton<f2>({'a','b'});

  *osc << "Build a NFA with epsilon transitions" << std::endl;
  auto autbe = make_automaton_with_epsilon({'a','b'});
  *osc << "Build a Z-WFA with epsilon transitions" << std::endl;
  auto autze = make_automaton_with_epsilon<z>({'a','b'});
  *osc << "Build a (Z,min,+)-WFA with epsilon transitions" << std::endl;
  auto autzmine = make_automaton_with_epsilon<zmin>({'a','b'});
  *osc << "Build a Q-WFA with epsilon transitions" << std::endl;
  auto autqe = make_automaton_with_epsilon<q>({'a','b'});
  *osc << "Build a R-WFA with epsilon transitions" << std::endl;
  auto autre = make_automaton_with_epsilon<r>({'a','b'});
  *osc << "Build a C-WFA with epsilon transitions" << std::endl;
  auto autce = make_automaton_with_epsilon<c>({'a','b'});
  *osc << "Build a F2-WFA with epsilon transitions" << std::endl;
  auto autf2e = make_automaton_with_epsilon<f2>({'a','b'});

  *osc << "Add a state with index 3 to the NFA" << std::endl;
  autb->add_state(3);
  for(awali::state_t q : autb->all_states())
    *osc << "state " << q << std::endl;
  assert(!autb->has_state(2));
  *osc << "state 2 ? " << std::boolalpha << autb->has_state(2) << std::endl;
  *osc << "Add a state to the NFA" << std::endl;
  autb->add_state();
  assert(autb->has_state(2));
  *osc << "state 2 ? " << std::boolalpha << autb->has_state(2) << std::endl;
  for(awali::state_t q : autb->all_states())
    *osc << "state " << q << std::endl;
  *osc << "Add a state with index 5 to the NFA" << std::endl;
  autb->add_state(5);
  assert(autb->has_state(5));
  assert(!autb->has_state(4));
  for(awali::state_t q : autb->all_states())
    *osc << "state " << q << std::endl;

  autb->set_state_name(2,"a");
  autb->set_initial(3);
  autb->new_transition(3,5,'a');
  autb->new_transition(5,2,'a');
  autb->set_final(2);
  
  //copy
  *osc << "copy" << std::endl;
  compare(autb, copy(autb,false, false, true));
  *osc << "allow_words" << std::endl;
  auto allw = allow_words(autb);
  compare(autb, allw);
  *osc << "to_lan" << std::endl;
  auto autlan = to_lan(autb);
  compare(autb, autlan);
  *osc << "to_lal" << std::endl;
  compare(autb, to_lal(autlan));
  *osc << "letterize" << std::endl;
  compare(autb, letterize(allw));
  *osc << "unnormalized lift" << std::endl;
  compare(autb, unnormalized_lift(autb));
  *osc << "partial_identity" << std::endl;
  auto auttdc =  partial_identity<2>(autb);
  compare(autb, auttdc);
  *osc << "lift_tdc" << std::endl;
  compare(autb, lift_tdc(auttdc));
  *osc << "projection" << std::endl;
  compare(autb, projection<0>(auttdc));
  *osc << "projections" << std::endl;
  compare(autb, projections<1,0>(auttdc));
  *osc << "images" << std::endl;
  compare(autb, images(auttdc));
  *osc << "accessible" << std::endl;
  compare(autb, accessible(autb));
  *osc << "coaccessible" << std::endl;
  compare(autb, coaccessible(autb));
  *osc << "trim" << std::endl;
  compare(autb, trim(autb));
  *osc << "prefix" << std::endl;
  compare(autb, prefix(autb));
  *osc << "suffix" << std::endl;
  compare(autb, suffix(autb));
  *osc << "factor" << std::endl;
  compare(autb, factor(autb));
  *osc << "proper" << std::endl;
  compare(autb, proper(autb));
  *osc << "proper 2" << std::endl;
  compare(autb, proper(autb, awali::direction_t::FORWARD));
  *osc << "standard" << std::endl;
  compare(autb, standard(autb));
  *osc << "transpose" << std::endl;
  compare(autb, transpose(autb));
  
  return 0;
}
