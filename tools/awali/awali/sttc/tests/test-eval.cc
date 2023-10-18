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
#include<awali/sttc/weightset/z.hh>
#include<awali/sttc/algos/eval.hh>
#include<awali/sttc/algos/enumerate.hh>

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

  *osc << "Load automaton b1" << std::endl;
  
  std::ifstream in(AWALI_AUTOMATA_DEPOSITORY "binary.json");
  auto b1 = load_automaton<z>(in);
  in.close();

  assert(eval(b1,"1101") == 13);
  auto map = shortest(b1, 4);
  assert (map.size() == 4);
  assert (map.find("11")->second == 3);
  for(auto & p : map)
    *osc << p.first << ',' << p.second << std::endl;
  map = enumerate(b1, 3);
  assert (map.size() == 11);

  *osc << "Load automaton a1" << std::endl;
  
  std::ifstream in2(AWALI_AUTOMATA_DEPOSITORY "a1.json");
  auto ab = load_automaton(in2);
  in2.close();

  auto map2 = shortest(ab, 6);
  assert (map2.size() == 6);
  std::string tab[]={"ab","aab","aba","abb","bab","aaab"};
  int g=0;
  for(auto & p : map2) {
    *osc << p.first << ',' << p.second << std::endl;
    assert(p.first == tab[g++]);
  }
  map2 = enumerate(ab, 4);
  assert (map2.size() == 16);



  return 0;
}
