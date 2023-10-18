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

#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/ctx/lal_char.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/algos/lift.hh>
#include <awali/sttc/algos/aut_to_exp.hh>

using namespace awali::sttc;
template<typename t>
struct aff{};

int main() {

  using context_t = context<ctx::lal_char,b>;
  context_t cont{ctx::lal_char{'a','b'},b{}};
  auto context=cont;
  std::cout << cont.vname() << std::endl;

  auto contl = get_letterset_context(cont);
  std::cout << contl.vname() << std::endl;
  auto contw = get_wordset_context(cont);
  std::cout << contw.vname() << std::endl;
  auto contn = get_nullable_context(cont);
  std::cout << contn.vname() << std::endl;

  auto contwl = get_letterset_context(contw);
  std::cout << contwl.vname() << std::endl;
  auto contww = get_wordset_context(contw);
  std::cout << contww.vname() << std::endl;
  auto contwn = get_nullable_context(contw);
  std::cout << contwn.vname() << std::endl;
  auto contwr = get_rat_context(contw);
  std::cout << contwr.vname() << std::endl;

  auto contnl = get_letterset_context(contn);
  std::cout << contnl.vname() << std::endl;
  auto contnw = get_wordset_context(contn);
  std::cout << contnw.vname() << std::endl;
  auto contnn = get_nullable_context(contn);
  std::cout << contnn.vname() << std::endl;
  auto contnr = get_rat_context(contn);
  std::cout << contnr.vname() << std::endl;

  auto a=make_mutable_automaton(contn);
  std::cout << a->vname() << std::endl;
    using ratset_t = ratexpset_of<context_t>;
    auto ratset = std::make_shared<ratset_t>(get_rat_context(context), ratset_t::identities_t::trivial);
    auto e = aut_to_exp(a);
  auto b=lift(a);
  std::cout << b->vname() << std::endl;
}
