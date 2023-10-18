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
#include<awali/sttc/factories/ladybird.hh>
#include<awali/sttc/algos/restriction.hh>
#include <awali/sttc/tests/null_stream.hxx>

using namespace awali;
using namespace awali::sttc;

template<typename Aut>
void test(Aut aut, unsigned alph_size, unsigned num_trans) {
  require(aut->context().labelset()->genset().size()==alph_size, "Wrong number of letters");
  require(aut->num_transitions()==num_trans, "Wrong number of transitions");
}

int main(int argc, char **argv) {
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;
  
  *osc << "Use ladybird 3 for the test" << std::endl;
  auto aut=make_automaton({'a','b','c'});
  auto lady = ladybird(aut->context(), 3);
  test(lady,3,9);
  *osc << "It has 3 letters and 9 transitions" << std::endl;
  change_alphabet(lady, {'a','b'});
  *osc << "Restrict to alphabet {a,b}" << std::endl;
  test(lady,2,5);
  *osc << "It has 2 letters and 5 transitions" << std::endl;
  //js_print(lady, std::cout) << std::endl;
  lady = ladybird(aut->context(), 3);
  remove_letters(lady, {'c'});
  *osc << "Restrict to alphabet {c}" << std::endl;
  test(lady,2,5);
  *osc << "It has 2 letters and 5 transitions" << std::endl;
  *osc << "Print the restriction" << std::endl;
  js_print(lady, *osc) << std::endl;
}

