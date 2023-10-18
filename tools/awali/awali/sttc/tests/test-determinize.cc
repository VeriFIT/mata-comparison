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
#include<awali/sttc/factories/ladybird.hh>
#include<awali/sttc/ctx/lal_char.hh>
#include<awali/sttc/weightset/b.hh>
#include<awali/sttc/algos/determinize.hh>
#include<awali/sttc/algos/complete.hh>
#include<awali/sttc/algos/complement.hh>
#include<awali/sttc/algos/transpose.hh>
#include<awali/sttc/algos/product.hh>
#include<awali/sttc/algos/is_ambiguous.hh>
#include<awali/sttc/algos/are_equivalent.hh>

#include<awali/sttc/misc/raise.hh>

using namespace awali::sttc;

int main(int argc, char **argv) {
  // Context of 4-tape transducer
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Build Ladybird automaton" << std::endl;
  auto a = ladybird(make_context({'a','b','c'}), 8);
  *osc << "Determinize" << std::endl;
  auto d = determinize(a);
  require(!is_deterministic(a),"a should not be deterministic");
  require(is_deterministic(d),"d should be deterministic");
  require(are_equivalent(a, d),"a and d should be equivalent");
  require(!is_complete(d),"d should not be complete");
  *osc << "Complete" << std::endl;
  auto cp = complete(d);
  require(is_complete(cp),"cp should be complete");
  *osc << "Complement" << std::endl;
  auto cpt = complement(cp);
  *osc << "Product with complement" << std::endl;
  auto inter = product(a,cpt);
  *osc << "Trim the result" << std::endl;
  trim_here(inter);
  require(is_empty(inter),"inter should be empty");
  *osc << "Transpose the det" << std::endl;
  auto trp = transpose(d);
  require(!is_ambiguous(trp),"trp should not be ambiguous");
  *osc << "Complete the det in place" << std::endl;
  complete_here(d);
  require(is_complete(d),"d should be complete");
  *osc << "Complement the det in place" << std::endl;
  complement_here(d);
  *osc << "Product with the automaton" << std::endl;
  inter = product(a,d);
  *osc << "Trim the result" << std::endl;
  trim_here(inter);
  require(is_empty(inter),"inter should be empty");

  return 0;
}
