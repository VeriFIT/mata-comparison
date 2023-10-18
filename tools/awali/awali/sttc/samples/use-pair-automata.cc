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
#include <awali/sttc/alphabets/int.hh>
#include <awali/sttc/alphabets/pair.hh>
#include <awali/sttc/algos/eval.hh>
#include <awali/sttc/algos/enumerate.hh>
#include <awali/sttc/algos/allow_words.hh>
#include <awali/sttc/algos/aut_to_exp.hh>
#include <awali/sttc/algos/derivation.hh>

//Static features are defined in the awali namespace
using namespace awali::sttc;

int main() {
  using letterset_t=letterset<set_alphabet<pair_letters<char_letters,int_letters>>>;
  using context_t=context<letterset_t,b>;
  context_t ctx{letterset_t{{'a',0},{'a',1},{'b',0}}, b()};
  auto aut=make_mutable_automaton(ctx);  
  unsigned s[3];
  for(int i=0; i<3; i++)
    s[i]=aut->add_state();
  /* a mutable_automaton is actually a (shared) pointer;
     to call the method, one have to use ->
  */
  aut->set_initial(s[0]);
  aut->set_final(s[2]);
  aut->set_transition(s[0], s[0], {'a',0});
  aut->set_transition(s[0], s[0], {'b',0});
  aut->set_transition(s[0], s[1], {'a',1});
  aut->set_transition(s[1], s[2], {'b',0});
  aut->set_transition(s[2], s[2], {'a',0});
  aut->set_transition(s[2], s[2], {'b',0});

  js_print(aut, std::cout) << std::endl;
  auto ratexpset=ratexpset_of<context_t>(get_rat_context(ctx),rat::identities::trivial);
  auto exp=aut_to_exp(aut);
  std::ostringstream oss;
  ratexpset.print(exp,oss);
  std::cout << oss.str() << std::endl;
  auto autb=derived_term(ratexpset, exp);
  js_print(autb, std::cout) << std::endl;
  
  auto expb = parse_exp(ratexpset, oss.str());
  ratexpset.print(expb, std::cout) << std::endl;

  auto wlbs = get_wordset(*aut->labelset());
  std::cout << "Evaluation of two words:" << std::endl;
  std::cout << "[a,0][b,0][a,1] :" << eval(aut,wlbs.parse("[a,0][b,0][a,1]")) << std::endl;
  std::cout << "[a,1][b,0][a,0] :" << eval(aut,wlbs.parse("[a,1][b,0][a,0]")) << std::endl;
  std::cout << "Enumeration up to length 3:" << std::endl;
  for(auto p : enumerate(aut, 3))
    wlbs.print(p.first,std::cout) << std::endl;

  
  return 0;

}
