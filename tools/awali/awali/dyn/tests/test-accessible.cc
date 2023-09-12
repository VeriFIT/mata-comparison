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
  automaton_t a = automaton_t::from("a");
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

  auto acc_set = accessible_states(a);
  auto coacc_set = coaccessible_states(a);
  auto useful_set = useful_states(a);

  assert(acc_set.size() == 4);
  assert(coacc_set.size() == 4);
  assert(useful_set.size() == 3);

  unsigned n_acc = num_accessible_states(a);
  assert(n_acc == 4);

  unsigned n_coacc = num_coaccessible_states(a);
  assert(n_coacc == 4);

  automaton_t acc = accessible(a);
  automaton_t coacc = coaccessible(a);
  automaton_t tra = trim(a);

  assert(acc -> num_states() == 4);
  assert(coacc -> num_states() == 4);
  assert(tra -> num_states() == 3);

  assert(!is_trim(a));
  assert(!is_accessible(a));
  assert(!is_coaccessible(a));
  assert(!is_useless(a));

  assert(is_accessible(acc));
  assert(is_coaccessible(coacc));
  assert(is_trim(tra));
  assert(!is_empty(tra));

  accessible(a,{IN_PLACE=true});
  assert(a -> num_states() == 4);
  coaccessible(a,{IN_PLACE=true});
  assert(a -> num_states() == 3);
  trim(a,{IN_PLACE=true});
  assert(a -> num_states() == 3);

  return 0;
}
