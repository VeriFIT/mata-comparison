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

#include<awali/sttc/factories/divkbaseb.hh>
#include<awali/sttc/factories/ladybird.hh>
#include<awali/sttc/factories/double_ring.hh>
#include<awali/sttc/factories/n_ultimate.hh>
#include<awali/sttc/factories/witness.hh>
#include<awali/sttc/factories/synchronizing_word.hh>
#include<awali/sttc/ctx/lal_char.hh>
#include<awali/sttc/weightset/b.hh>

#include<awali/sttc/misc/raise.hh>

using namespace awali::sttc;

int main(int argc, char **argv) {
  // Context of 4-tape transducer
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Declare context" << std::endl;
  context<ctx::lal_char,b> ctx2(ctx::lal_char({'a','b'}),
			       b());
  context<ctx::lal_char,b> ctx3(ctx::lal_char({'a','b','c'}),
			       b());
  *osc << "Build n-ultimate automaton" << std::endl;
  auto aut = n_ultimate(ctx2, 'a', 4);
  assert(aut->num_states() == 5);
  *osc << "Build div-k-base-b automaton" << std::endl;
  aut = divkbaseb(ctx2, 5, 2);
  assert(aut->num_states() == 5);
  *osc << "Build double-ring automaton" << std::endl;
  aut = double_ring(ctx2, 6, {0,2,3});
  assert(aut->num_states() == 6);
  assert(aut->num_finals() == 3);
  *osc << "Build witness automaton" << std::endl;
  aut = witness(ctx3, 5);
  assert(aut->num_states() == 5);
  *osc << "Build Cerny automaton" << std::endl;
  aut = cerny(ctx2, 5);
  assert(aut->num_states() == 5);
  *osc << "Build Ladybird automaton" << std::endl;
  aut = ladybird(ctx3, 5);
}
