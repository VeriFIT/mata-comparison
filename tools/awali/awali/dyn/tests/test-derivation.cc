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
#include<cassert>

using namespace awali::dyn;
using namespace awali;

int main() {
  ratexp_t e=ratexp_t::from("\\e+(\\e+a)((b+aa)a)*(ab*+b+aa)");
  automaton_t b= exp_to_aut(e,{EXP_TO_AUT_ALGO=DERIVED_TERM});
  put(b, std::cout);
  assert(b -> num_states() == 7);
  b= exp_to_aut(e, {EXP_TO_AUT_ALGO=BREAKING});
  assert(b -> num_states() == 6);
  auto terms = dyn::internal::derivation(e,'a');
  assert(terms.size() == 4);
  terms = dyn::internal::derivation(e,'a',true);
  assert(terms.size() == 4);
}
