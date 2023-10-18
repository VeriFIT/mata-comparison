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

#include <awali/dyn.hh>
#include <cassert>

using namespace awali::dyn;

int main()
{
  for (auto a : loading::example_automata()) {
    std::cerr << "Automaton " << a.first << "...  ";
    automaton_t aut = load(a.first);
    automaton_t shuffled_aut = internal::shuffle_states(aut);
    assert(are_isomorphic(aut, shuffled_aut));
    std::cerr << "Done." << std::endl;
  }

  return 0;
}
