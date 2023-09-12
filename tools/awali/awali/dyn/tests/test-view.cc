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


#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/ctx/lal_char.hh>
#include <awali/sttc/weightset/b.hh>

using labelset_t = awali::sttc::ctx::lal_char;
using weightset_t = awali::sttc::b;
using context_t = awali::sttc::context<labelset_t, weightset_t>;

#include <awali/sttc/algos/minimize_brzozowski.hh>
#include <awali/sttc/algos/dot.hh>


using namespace awali::sttc;


int main() {
  context_t ctx{labelset_t{'a','b'},b{}};
  auto a = make_mutable_automaton<context_t>(ctx);
  unsigned s[3];
  for(int i=0;i<3;++i)
    s[i] = a->add_state();
  a->set_initial(s[0]);
  a->set_final(s[2]);
  a->new_transition(s[0],s[0],'a');
  a->new_transition(s[0],s[0],'b');
  a->new_transition(s[0],s[1],'a');
  a->new_transition(s[1],s[2],'b');
  a->new_transition(s[2],s[2],'a');
  a->new_transition(s[2],s[2],'b');
  auto d=determinize(transpose_view(a));
  //  dot(d,std::cout) << std::endl;
  for(auto t : d->all_transitions()) {
    std::cout << d->src_of(t) << " --";
    std::cout << d->label_of(t) << "-> ";
    std::cout << d->dst_of(t) << std::endl;
  }
  return 0;
}


