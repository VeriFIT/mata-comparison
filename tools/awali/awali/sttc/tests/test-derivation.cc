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
#include<awali/sttc/algos/derivation.hh>

using namespace awali::sttc;


int main(int argc, char **argv) {
  // Context of 4-tape transducer
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Declare ratexpset" << std::endl;
  auto ratexpset = make_ratexpset();
  *osc << "Parse expression" << std::endl;
  auto e = make_ratexp(ratexpset, "\\e+(\\e+a)((b+aa)a)*(ab*+b+aa)");

  *osc << "Compute the derived term automaton" << std::endl;
  auto b = derived_term(ratexpset, e);
  *osc << "Result:" << std::endl;
  js_print(b, *osc) << std::endl;
  *osc << "------" << std::endl
       << "Check state number" << std::endl;
  assert(b -> num_states() == 7);

  *osc << "Compute the broken derived term automaton" << std::endl;
  b= derived_term(ratexpset, e,true);
  *osc << "Check state number" << std::endl;
  assert(b -> num_states() == 6);

  *osc << "Derivation w.r.t. 'a'" << std::endl;
  auto terms = derivation(ratexpset, e,'a');
  *osc << "Check number of terms" << std::endl;
  assert(terms.size() == 4);

  *osc << "Broken derivation w.r.t. 'a'" << std::endl;
  terms = derivation(ratexpset, e,'a', true);
  *osc << "Check number of broken terms" << std::endl;
  assert(terms.size() == 4);

  return 0;
}
