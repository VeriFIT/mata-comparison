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

#include<awali/sttc/automaton.hh>
#include<awali/sttc/algos/add_path.hh>

using namespace awali;  // state_t
using namespace awali::sttc;


int main(int argc, char **argv) {
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Declare ratexpset" << std::endl;
  auto ratexpset = make_ratexpset();
  *osc << "Parse expression" << std::endl;
  auto e = make_ratexp(ratexpset, "((b+aa)a)*(ab*+b+aa)");

  auto a1 = make_automaton({'a','b'});

  state_t p = a1->add_state();
  state_t q = a1->add_state();
  a1->set_initial(p);
  a1->set_final(q);
  add_path(a1, p, q, e);
  
  *osc << "Sans epsilon :" << std::endl;
  js_print(a1, *osc) << std::endl;
  *osc << "Avec epsilon :" << std::endl;
  *osc << "-----" << std::endl;

  auto b1 = make_automaton_with_epsilon({'a','b'});
  p = b1->add_state();
  q = b1->add_state();
  b1->set_initial(p);
  b1->set_final(q);
  add_path(b1, p, q, e);
  js_print(b1, *osc) << std::endl;
  
  return 0;
}
