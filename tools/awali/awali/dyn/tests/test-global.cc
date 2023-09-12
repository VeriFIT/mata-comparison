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

#include <awali/dyn/awalidyn.cc>

using namespace awali::dyn;

void print(automaton_t aut) {

  std::cout << "Alphabet:" << std::endl;
  for(label_t l : aut->alphabet())
    std::cout << l << ' ';
  std::cout << std::endl;
  std::cout << "Initial states:" << std::endl;
  for(state_t s : aut->initial_states())
    aut->print_state(s,std::cout) << std::endl;
  std::cout << "Final states:" << std::endl;
  for(state_t s : aut->final_states())
    aut->print_state(s,std::cout) << std::endl;
  std::cout << "Transitions:" << std::endl;
  for(transition_t tr : aut->transitions()) {
    aut->print_state(aut->src_of(tr),std::cout) << " --" << aut->label_of(tr) << "-> ";
    aut->print_state(aut->dst_of(tr),std::cout) << std::endl;
  }
}

int main() {
  automaton_t a = automaton_t::from("ab");
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
  //std::cout << "Déterminisation" << std::endl;
  automaton_t b = determinize(a);
  /*print(b);
  std::cout << "Etats: " << b->num_states() <<  std::endl;
  std::cout << "Etats co-accessibles: " << num_coaccessible_states(b) <<  std::endl;
  std::cout << "Standard ? : " << is_standard(b) <<  std::endl;
  for(auto pair : shortest(b, 1))
        std::cout << pair.first << std::endl;
  */
  std::cout << eval(b,"aaa") << std::endl;
  for(auto p : enumerate(b,10))
    std::cout << p.first << std::endl;
  //dot(b,std::cout);
  ratexp_t exp = make_ratexp("<5>a+(a*b?)<7>","R");
  b = standard(exp);
  reduce(b);
  //dot(b,std::cout);
  exp->json(std::cout) << std::endl;
  //exp->print(std::cout) << std::endl;
  /*
  ratexp_t exp2 = aut_to_exp(a);
  //exp2->print(std::cout) << std::endl;
  */
  automaton_t c = make_automaton_with_eps("ab");
  unsigned p[5];
  for(int i=0;i<5;i++)
        p[i] = c->add_state();
  for(int i=0;i<4;i++) {
    c->set_transition(p[i],p[i+1],'a');
    c->set_eps_transition(p[i],p[i+1]);
  }
  c->set_initial(p[0]);
  c->set_final(p[4]);
  ratexp_t exp3 = aut_to_exp(c);
  //exp3->json(std::cout) << std::endl;
  automaton_t d = thompson(exp3);
  dot(d, std::cout);
  std::cout << std::boolalpha << "Valid: " << is_valid(d) << " Proper: " << is_proper(d) << std::endl;

  auto cd = c_desc(tupleset({nullableset(letterset("ab")),nullableset(letterset("cd"))}),weightset("Z"));
  std::cout << tostring(cd,true) << std::endl;

  return 0;
}

