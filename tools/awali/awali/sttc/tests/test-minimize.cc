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
#include<awali/sttc/factories/ladybird.hh>
#include<awali/sttc/factories/divkbaseb.hh>
#include<awali/sttc/algos/determinize.hh>
#include<awali/sttc/algos/min_quotient.hh>

#include<awali/sttc/misc/raise.hh>

using namespace awali;
using namespace awali::sttc;

const int naut = 8; 
const int size = (1<<naut)-1; 

void test_equal(const std::string& message, unsigned long test, unsigned long expect) {
  require((test == expect), message, " : expected ", std::to_string(expect)," got ",std::to_string(test));
}


int main(int argc, char **argv) {
  // Context of 4-tape transducer
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Build Ladybird automaton" << std::endl;
  auto a = ladybird(make_context({'a','b','c'}), naut);
  *osc << "Determinize" << std::endl;
  auto d = determinize(a);
  *osc << "Minimize with Moore" << std::endl;
  auto m = minimize(d);
  *osc << "Minimize with Hopcroft" << std::endl;
  auto dh = minimize(d, HOPCROFT);
  test_equal("Det",d->num_states(),size);
  test_equal("Moore",m->num_states(),size);
  test_equal("Hopcroft",dh->num_states(),size);
  m = min_quotient(transpose(d));
  test_equal("Co quotient",m->num_states(),size);

  /*
  m = minimal_automaton(a);
  assert(m->num_states() == (1<<12));
  */
  
  a = divkbaseb(make_context({'0','1'}),6,2);
  std::vector<std::vector<unsigned int>> eq_class= {{2},{5},{3,6},{4,7}};
  m = merge(a, eq_class);
  test_equal("Merge", m->num_states(), 4);

  return 0;
}
