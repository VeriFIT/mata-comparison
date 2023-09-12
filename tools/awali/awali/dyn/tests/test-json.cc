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

#include <awali/dyn.hh>

using namespace awali::dyn;
using namespace awali;

int main() {
  automaton_t a = factory::ladybird(4,"abc");
  //a->json(std::cout) << std::endl;
  //std::cout << "------------------------------------" << std::endl;
  ratexp_t e = aut_to_exp(a);
  awali::dyn::internal::json(e,std::cout) << std::endl;
  automaton_t t=exp_to_aut(e,{EXP_TO_AUT_ALGO=THOMPSON});
  put(t,std::cout) << std::endl;

  ratexp_t ef = aut_to_exp(t);
   awali::dyn::internal::json(ef,std::cout) << std::endl;
  /*
  automaton_t s=standard(e);
  s->json(std::cout) << std::endl;
  */
//  std::cout << "------------------------------------" << std::endl;
  automaton_t b = lift::lift(a);
  put(b,std::cout) << std::endl;
// dot(b,std::cout);
//  std::cout << "------------------------------------" << std::endl;
  automaton_t c = automaton_t::from("ab",true);
  c->set_eps_transition(c->add_state(), c->add_state());
  put(c,std::cout) << std::endl;

  std::cout << "------------------------------------" << std::endl;
  automaton_t d = automaton_t::from("ab","Z");
  std::cout << d->get_context()->weightset_name() << std::endl;
  d->set_initial(d->add_state(),-5);
  d->set_transition(d->add_state(), d->add_state(),'a',3);
  put(d,std::cout) << std::endl;

  std::cout << "------------------------------------" << std::endl;
  automaton_t q = automaton_t::from("ab","Q");
  state_t sq[2];
  q->set_transition(sq[0]=q->add_state(), sq[1]=q->add_state(),'a',q_fraction_t(3,4));
  q->set_initial(sq[0],q_fraction_t(-5));
  q->set_final(sq[1]);
  put(q, std::cout) << std::endl;
  ratexp_t eq = aut_to_exp(q);
  dyn::internal::json(eq,std::cout) << std::endl;



  std::cout << "------------------------------------" << std::endl;
  {
    std::map<std::string,std::string> map = { {"prenom",   "Victor"}, 
                                              {"nom",      "Marsault"}, 
                                              {"timestamp","zkzkzk"} };
    json_ast_t string_map_ast = json_ast::from_string_map(map);
    std::cout << string_map_ast;
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
   
    std::cout << std::endl;

  }


  std::cout << "-----------------load_json_ast:ratexp-----------------" << std::endl;
  {
    json_ast_t ast = dyn::internal::load_json_ast("div3b2-exp");
    std::cout << ast;
    std::cout << std::endl;
  }

  std::cout << "-----------------load_json_ast:automaton-----------------" << std::endl;
  {
    json_ast_t ast = dyn::internal::load_json_ast("b1");
    std::cout << ast;
    std::cout << std::endl;
  }

  return 0;
}

