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
#include<iostream>

using namespace awali::dyn;
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
  automaton_t aut1 = automaton_t::from("ab");
  myassert (aut1->get_context()->sname(),"lal_char_b");
  myassert (aut1->get_context()->vname(),"lal_char(ab)_b");
  myassert (aut1->get_context()->weightset_name(),"B");
  
  sep("aut2");
  automaton_t aut2 = automaton_t::from("abc", true);
  myassert (aut2->get_context()->sname(),"lan<lal_char>_b");
  myassert (aut2->get_context()->vname(),"lan<lal_char(abc)>_b");
  myassert (aut2->get_context()->weightset_name(),"B");

  sep("aut3");
  automaton_t aut3 = automaton_t::from("abcd", true, "Z");
  myassert (aut3->get_context()->sname(),"lan<lal_char>_z");
  myassert (aut3->get_context()->vname(),"lan<lal_char(abcd)>_z");
  myassert (aut3->get_context()->weightset_name(),"Z");

  sep("aut4");
  automaton_t aut4 = automaton_t::from("abcde", "Z-min-plus");
  myassert (aut4->get_context()->sname(),"lal_char_zmin");
  myassert (aut4->get_context()->vname(),"lal_char(abcde)_zmin");
  myassert (aut4->get_context()->weightset_name(),"Z-min-plus");


  sep("int_aut1");
  automaton_t int_aut1 = automaton_t::with_int_labels::from_range(2,4);
  myassert (int_aut1->get_context()->sname(),"lal_int_b");
  myassert (int_aut1->get_context()->vname(),"lal_int(234)_b");
  myassert (int_aut1->get_context()->weightset_name(),"B");
  
  sep("int_aut2");
  automaton_t int_aut2 = automaton_t::with_int_labels::from_size(2, "Z", true);
  myassert (int_aut2->get_context()->sname(),"lan<lal_int>_z");
  myassert (int_aut2->get_context()->vname(),"lan<lal_int(01)>_z");
  myassert (int_aut2->get_context()->weightset_name(),"Z");


  sep("tdc1");
  automaton_t tdc1 = transducer_t::from({"ab","bc"});
  myassert (tdc1->get_context()->sname(),"lat<lan<lal_char>,lan<lal_char>>_b");
  myassert (tdc1->get_context()->vname(),"lat<lan<lal_char(ab)>,lan<lal_char(bc)>>_b");
  myassert (tdc1->get_context()->weightset_name(),"B");

  sep("tdc2");
  automaton_t tdc2 = transducer_t::from({"ab","bc"}, "Z");
  myassert (tdc2->get_context()->sname(),"lat<lan<lal_char>,lan<lal_char>>_z");
  myassert (tdc2->get_context()->vname(),"lat<lan<lal_char(ab)>,lan<lal_char(bc)>>_z");
  myassert (tdc2->get_context()->weightset_name(),"Z");

}

