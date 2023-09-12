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
#include <awali/dyn/loading/locations.hh>
#include <fstream>
#include <cassert>

using namespace awali::dyn;
using namespace awali;

int main() {
//   std::ifstream file(loading::get_share_directory()+"/automata/a1.json");
  automaton_t aut = load("a1");
//   file.close();

  aut=sum(aut, aut);
  std::cout << aut << std::endl;
  std::ofstream out("tmp");
  put(aut, out, {IO_FORMAT=FADO});
  out.close();
  std::ifstream file;
  file.open("tmp");
  automaton_t fado_aut = parse_automaton(file, {IO_FORMAT=FADO});
  std::cout << fado_aut << std::endl;
  file.close();

  assert(are_isomorphic(aut,fado_aut));

  out.open("tmp");
  put(aut, out,{IO_FORMAT=GRAIL});
  out.close();
  file.open("tmp");
  automaton_t grail_aut = parse_automaton(file, {IO_FORMAT=GRAIL});
  file.close();

  assert(are_isomorphic(aut,grail_aut));

  out.open("tmp");
  put(aut, out, {IO_FORMAT=DOT});
  out.close();

  return 0;
}
