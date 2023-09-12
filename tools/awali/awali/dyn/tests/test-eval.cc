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

int main() {
//   std::ifstream file(loading::get_share_directory()+"/automata/binary.json");
  automaton_t b1 = load("binary");
//   file.close();
  assert((int)eval(b1,"1101") == 13);
  auto map = shortest(b1, 4);
  assert (map.size() == 4);
  assert ((int)(map.find("11")->second) == 3);
  map = enumerate(b1, 3);
  assert (map.size() == 11);
  return 0;
}
