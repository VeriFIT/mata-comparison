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
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/algos/eval.hh>
#include <awali/sttc/algos/enumerate.hh>
#include <awali/sttc/algos/standard.hh>
#include <awali/sttc/algos/thompson.hh>
#include <awali/sttc/weightset/r.hh>
#include <awali/sttc/algos/dot.hh>
#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/algos/reduce.hh>
#include <awali/sttc/algos/aut_to_exp.hh>

#include <awali/sttc/tests/null_stream.hxx>

using namespace awali;
using namespace awali::sttc;

std::ostream * osc;

template<typename Aut>
void print(Aut aut) {
  *osc << "--------------" << std::endl;
  *osc << "Automaton :" << std::endl;
  *osc << "Alphabet:" << std::endl;
  for(auto l : aut->context().labelset()->genset())
    *osc << l << ' ';
  *osc << std::endl;
  *osc << "Initial states:" << std::endl;
  for(auto t : aut->initial_transitions())
    aut->print_state(aut->dst_of(t),*osc) << std::endl;
  *osc << "Final states:" << std::endl;
  for(auto t : aut->final_transitions())
    aut->print_state(aut->src_of(t),*osc) << std::endl;
  *osc << "Transitions:" << std::endl;
  for(transition_t tr : aut->transitions()) {
    aut->print_state(aut->src_of(tr),*osc) << " --" << aut->label_of(tr) << "-> ";
    aut->print_state(aut->dst_of(tr),*osc) << std::endl;
  }
  *osc << "--------------" << std::endl;
}

int main(int argc, char **argv) {
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Create a NFA" << std::endl;
  auto a = make_automaton({'a','b'});
  state_t s[5];
  for(int i=0; i<5; ++i)
        s[i]= a->add_state();
  for(int i=0; i<4; ++i)
    a->set_transition(s[i], s[i+1], 'a');
  for(int i=1; i<4; ++i)
    a->set_transition(s[i], s[i+1], 'b');
  a->set_transition(s[0], s[0], 'a');
  //a->new_transition(s[0], s[0], 'b');
  a->set_initial(s[0]);
  a->set_final(s[3]);
  print(a);
  *osc << "Déterminisation" << std::endl;
  auto b = determinize(a);
  print(b);
  *osc << "Etats: " << b->num_states() <<  std::endl;
  *osc << "Etats co-accessibles: " << num_coaccessible_states(b) <<  std::endl;
  *osc << "Standard ? : " << is_standard(b) <<  std::endl;
  *osc << "Shortest word in b :" << std::endl;
  for(auto pair : shortest(b, 1))
        *osc << pair.first << std::endl;
  *osc << "Weight of 'aaa' in b :" << std::endl;
  *osc << eval(b,"aaa") << std::endl;
  *osc << "Words accepted by b up to length 10 :" << std::endl;
  for(auto p : enumerate(b,10))
    *osc << p.first << std::endl;
  *osc << "Export b in dot :" << std::endl;  
  dot(b,*osc);
  *osc << "Make expression :" << std::endl;  
  auto ratexpset = make_ratexpset<r>();
  auto exp = make_ratexp(ratexpset, "<5>a+(a*b?)<7>");
  print(ratexpset, exp, *osc) << std::endl;
  *osc << "Expression in json:" << std::endl;  
  js_print(ratexpset, exp, *osc) << std::endl;
  *osc << "-----------------" << std::endl;  
  auto rb = standard(ratexpset, exp);
  reduce(rb);
  dot(b,*osc);

  auto c = make_automaton_with_epsilon({'a','b'});
  unsigned p[5];
  for(int i=0;i<5;i++)
        p[i] = c->add_state();
  for(int i=0;i<4;i++) {
    c->set_transition(p[i],p[i+1],'a');
    c->set_transition(p[i],p[i+1], get_epsilon());
  }
  c->set_initial(p[0]);
  c->set_final(p[4]);
  auto exp3 = aut_to_exp(c);
  js_print(get_ratexpset(c), exp3, *osc) << std::endl;
  auto d = thompson(get_ratexpset(c), exp3);
  dot(d, *osc);
  *osc << std::boolalpha << "Valid: " << is_valid(d) << " Proper: " << is_proper(d) << std::endl;

  return 0;
}

