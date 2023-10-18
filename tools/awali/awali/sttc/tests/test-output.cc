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
#include <awali/sttc/algos/sum.hh>
#include <awali/sttc/algos/grail.hh>
#include <awali/sttc/algos/dot.hh>


#include <awali/sttc/tests/null_stream.hxx>

using namespace awali;
using namespace awali::sttc;

#ifndef AWALI_AUTOMATA_DEPOSITORY
#define AWALI_AUTOMATA_DEPOSITORY "../../automata/"
#endif

std::ostream * osc;

int main(int argc, char **argv) {
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  std::ifstream in(AWALI_AUTOMATA_DEPOSITORY "a1.json");
  auto aut = load_automaton(in);
  in.close();

  aut=sum(aut, aut);
  *osc << "---- Print in Json ------" << std::endl;
  js_print(aut, *osc) << std::endl;
  *osc << "---- Print in Fado ------" << std::endl;
  fado(aut, *osc) << std::endl;
  *osc << "---- Print in Grail ------" << std::endl;
  grail(aut, *osc) << std::endl;
  *osc << "---- Print in Dot ------" << std::endl;
  dot(aut, *osc) << std::endl;

  return 0;
}
