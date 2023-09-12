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

#include<awali/sttc/automaton.hh>
#include<awali/sttc/algos/eval_tdc.hh> // eval_words_in_tdc
#include <awali/sttc/weightset/zmin.hh>

/* Example of the construction of a Z-min tranducer that computes
   the Levenshtein distance between two words.
*/

using namespace awali::sttc;


int main() {
  std::set<char> alpha;
  for(char c='a'; c<='z'; c++)
    alpha.emplace(c);
  
  auto tdc=make_transducer<zmin>(alpha,alpha);
  unsigned s=tdc->add_state();
  auto eps1=tdc->labelset()->set<0>().one();
  auto eps2=tdc->labelset()->set<1>().one();
  tdc->set_initial(s);
  tdc->set_final(s);
  for(char c='a'; c<='z'; c++) {
    tdc->new_transition(s,s,{c,eps2},1);
    tdc->new_transition(s,s,{eps1,c},1);
  }
  for(char c='a'; c<='z'; c++)
    for(char d='a'; d<='z'; d++)
      tdc->new_transition(s,s,{c,d},c!=d);
  //    d  i  s  t  a     n  c  e
  // *        *     *  *     *  *   -> 6
  // e  d  i     t  i  o  n
  std::string w1("distance");
  std::string w2("edition");
  std::cout << "d(" << w1 << ',' << w2 << ")="
            << eval_words_in_tdc(tdc, w1, w2) << std::endl;
  return 0;
}
