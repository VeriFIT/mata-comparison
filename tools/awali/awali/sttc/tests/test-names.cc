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

#include <awali/sttc/automaton.hh>
#include <awali/sttc/weightset/z.hh>
#include <awali/sttc/weightset/zmin.hh>
#include <awali/sttc/ctx/lal_int.hh>
#include <awali/sttc/ctx/lan_int.hh>
#include<cassert>
#include<iostream>

using namespace awali::sttc;
using namespace awali;

template<typename T, typename T2>
void myassert(T x, T2 y) {
  std::cout << x << " == " << y << std::endl;
  assert(x == y);
}

void sep(std::string str) {
  std::cout << "==================== " << str
            << " ====================" << std::endl;
}

// This tests that automaton are correctly created with the correct context.
int main() {
  
  sep("aut1");
  auto aut1 = make_automaton({'a','b'});
  myassert (aut1->context().sname(),"lal_char_b");
  myassert (aut1->context().vname(),"lal_char(ab)_b");
  std::ostringstream os;
  aut1->context().weightset()->print_set(os);
  myassert (os.str(),"B");

  sep("aut2");
  auto aut2 = make_automaton_with_epsilon({'a','b','c'});
  myassert (aut2->context().sname(),"lan<lal_char>_b");
  myassert (aut2->context().vname(),"lan<lal_char(abc)>_b");
  std::ostringstream os2;
  aut2->context().weightset()->print_set(os2);
  myassert (os2.str(),"B");

  sep("aut3");
  auto aut3 = make_automaton_with_epsilon<z>({'a','b','c','d'});
  myassert (aut3->context().sname(),"lan<lal_char>_z");
  myassert (aut3->context().vname(),"lan<lal_char(abcd)>_z");
  std::ostringstream os3;
  aut3->context().weightset()->print_set(os3);
  myassert (os3.str(),"Z");

  sep("aut4");
  auto aut4 = make_automaton<zmin>({'a','b','c','d','e'});
  myassert (aut4->context().sname(),"lal_char_zmin");
  myassert (aut4->context().vname(),"lal_char(abcde)_zmin");
  std::ostringstream os4;
  aut4->context().weightset()->print_set(os4);
  myassert (os4.str(),"Z-min-plus");

  sep("int_aut1");
  using ctxi1_t = context<ctx::lal_int,b>;
  ctxi1_t ctxi1(ctx::lal_int({2,3,4}),b());
  auto int_aut1 = make_shared_ptr<mutable_automaton<ctxi1_t>>(ctxi1);
  myassert (int_aut1->context().sname(),"lal_int_b");
  myassert (int_aut1->context().vname(),"lal_int(234)_b");
  std::ostringstream osi1;
  int_aut1->context().weightset()->print_set(osi1);
  myassert (osi1.str(),"B");

  sep("int_aut2");
  using ctxi2_t = context<ctx::lan_int,z>;
  ctxi2_t ctxi2(ctx::lan_int(ctx::lal_int({0,1})),z());
  auto int_aut2 = make_shared_ptr<mutable_automaton<ctxi2_t>>(ctxi2);
  myassert (int_aut2->context().sname(),"lan<lal_int>_z");
  myassert (int_aut2->context().vname(),"lan<lal_int(01)>_z");
  std::ostringstream osi2;
  int_aut2->context().weightset()->print_set(osi2);
  myassert (osi2.str(),"Z");

  sep("tdc1");
  auto tdc1 = make_transducer({'a','b'},{'b','c'});
  myassert (tdc1->context().sname(),"lat<lan<lal_char>,lan<lal_char>>_b");
  myassert (tdc1->context().vname(),"lat<lan<lal_char(ab)>,lan<lal_char(bc)>>_b");
  std::ostringstream ost1;
  tdc1->context().weightset()->print_set(ost1);
  myassert (ost1.str(),"B");

  sep("tdc2");
  auto tdc2 = make_transducer<z>({'a','b'},{'b','c'});
  myassert (tdc2->context().sname(),"lat<lan<lal_char>,lan<lal_char>>_z");
  myassert (tdc2->context().vname(),"lat<lan<lal_char(ab)>,lan<lal_char(bc)>>_z");
  std::ostringstream ost2;
  tdc2->context().weightset()->print_set(ost2);
  myassert (ost2.str(),"Z");
}

