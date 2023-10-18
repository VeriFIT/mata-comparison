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
#include<awali/sttc/algos/accessible.hh>
#include<awali/sttc/misc/raise.hh>

using namespace awali::sttc;

template<typename Aut>
void test(Aut aut, bool acc, bool coacc, bool trim, bool useless, std::string name) {
  require(is_trim(aut) == trim , name, ": wrong trim status");
  require(is_accessible(aut) == acc , name, ": wrong accessible status");
  require(is_coaccessible(aut) == coacc , name, ": wrong coaccessible status");
  require(is_useless(aut) == useless , name, ": wrong useless status");
}



int main(int argc, char **argv) {
  // Context of 4-tape transducer
  std::ostream * osc;
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;

  *osc << "Build a transducer" << std::endl;
  
  *osc << "Build a NFA" << std::endl;
  auto a = make_automaton({'a'});
  unsigned st[5];
  for(unsigned i=0; i<5; ++i)
    st[i] = a -> add_state();
  for(unsigned i=0; i<4; ++i)
    a -> set_transition(st[i], st[i+1], 'a');
  a -> set_transition(st[3], st[1], 'a');
  a -> set_initial(st[1]);
  a -> set_initial(st[4]);
  a -> set_final(st[0]);
  a -> set_final(st[3]);
 
  *osc << "Compute accessible states" << std::endl;
  auto acc_set = accessible_states(a);
  *osc << "Compute coaccessible states" << std::endl;
  auto coacc_set = coaccessible_states(a);
  *osc << "Compute useful states" << std::endl;
  auto useful_set = useful_states(a);

  require(acc_set.size() == 4, "Wrong number of accessible states");
  require(coacc_set.size() == 4, "Wrong number of coaccessible states");
  require(useful_set.size() == 3, "Wrong number of useful states");
  unsigned n_acc = num_accessible_states(a);
  require(n_acc == 4, "problem with num_accessible_states");

  unsigned n_coacc = num_coaccessible_states(a);
  require(n_coacc == 4, "problem with num_coaccessible_states");

  auto acc = accessible(a);
  auto coacc = coaccessible(a);
  auto tra = trim(a);

  require(acc -> num_states() == 4, "Size of the accessible automaton");
  require(coacc -> num_states() == 4, "Size of the coaccessible automaton");
  require(tra -> num_states() == 3, "Size of the trim automaton");

  test(a, false, false, false, false, "aut"); 
  test(acc, true, false, false, false, "acc"); 
  test(coacc, false, true, false, false, "coacc"); 
  test(tra, true, true, true, false, "trimaut"); 

  require(!is_empty(tra),"trimaut should not be empty");

  *osc << "Compute copies" << std::endl;
  auto b = copy(a);
  auto c = copy(a);
  *osc << "Compute accessible in place" << std::endl;
  accessible_here(a);
  require(a -> num_states() == 4, "Size of the accessible automaton");
  *osc << "Compute coaccessible in place" << std::endl;
  coaccessible_here(b);
  require(b -> num_states() == 4, "Size of the coaccessible automaton");
  *osc << "Compute trim in place" << std::endl;
  trim_here(c);
  require(c -> num_states() == 3, "Size of the trim automaton");
 
  return 0;
}
