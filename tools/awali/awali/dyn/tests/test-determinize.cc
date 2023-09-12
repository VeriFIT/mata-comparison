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
#include<fstream>
#include<cassert>

using namespace awali::dyn;
using namespace awali;

int main() {
  automaton_t a = factory::ladybird(8,"abc");
  automaton_t d = determinize(a);
  assert(!is_deterministic(a));
  assert(is_deterministic(d));
  assert(are_equivalent(a, d));
  assert(!is_complete(d));
  automaton_t cp = complete(d);
  assert(is_complete(cp));
  automaton_t cpt = complement(cp);
  automaton_t inter = product(a,cpt);
  trim(inter,{IN_PLACE=true});
  assert(is_empty(inter));
  automaton_t trp = transpose(d);
  assert(!is_ambiguous(trp));
  complete(d,{IN_PLACE=true});
  assert(is_complete(d));
  complement(d,{IN_PLACE=true});
  inter = product(a,d);
  trim(inter,{IN_PLACE=true});
  assert(is_empty(inter));

//   std::ifstream

  automaton_t b1 = load("b1");
//   file.close();
  automaton_t b6 = power(b1, 6);
  automaton_t r6 = reduce(b6);
  assert(r6->num_states() == 7);
  r6 = min_quotient(b6);
  assert(r6->num_states() == 7);
  r6 = min_quotient(b6);
  return 0;
}
