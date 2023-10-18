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
#include<awali/sttc/algos/allow_words.hh>

using namespace awali;
using namespace awali::sttc;

#ifndef AWALI_AUTOMATA_DEPOSITORY
#define AWALI_AUTOMATA_DEPOSITORY "../../automata/"
#endif

int main(int argc, char **argv) {
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Load automaton url-validator" << std::endl;
  
  std::ifstream in(AWALI_AUTOMATA_DEPOSITORY "url-validator.json");
  auto url = load_automaton_with_epsilon<b>(in);
  in.close();

  *osc << "#states : " << url->num_states() << std::endl;
  *osc << "Compact automaton" << std::endl;
  auto cpt=compact(url);
  *osc << "#states : " << cpt->num_states() << std::endl;
  return 0;
}
