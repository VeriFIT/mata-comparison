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

#include <awali/dyn/algos/random.hh>

/* This is an implementation of

  A. Carayol and C. Nicaud.
  Distribution of the number of accessible states in a random deterministic automaton,
  STACS 2012, volume 14 of LIPIcs, 194-205, 2012.

 */

namespace awali { namespace dyn {

  namespace internal {
    //Usual exponentiation
    long pow(long i, unsigned k) {
      long j=i, r=1l;
      while(k) {
        if(k&1u)
          r *= j;
        j*=j;
        k/=2;
      }
      return r;
    }

    double w0fct(unsigned k,unsigned size=1000) {
      double err=.1/size;
      double x=-std::exp(-(double)k)*k;
      double xi=1;
      double fact=1;
      double r=0;
      for(long i=1;true;++i) {
        xi*=x; fact*=i;
        double t=pow(-i,i-1)/fact*xi;
        r+=t;
        if(t<err && -t<err)
          return r;
      }
      return 0;
    }
  }//details

  automaton_t factory::randomDFA(unsigned size, std::string alphabet) {
    unsigned alpha_size=alphabet.length();
    double v=1+internal::w0fct(alpha_size,size)/alpha_size;
    unsigned n=size/v;
    //we randomly generate an automaton with at most n states
    std::default_random_engine generator;
    generator.seed(std::time(nullptr));
    std::uniform_int_distribution<unsigned> distribution(0,n-1);
    auto dice = std::bind ( distribution, generator );
    //dice returns uniform independant integers in [0;n-1]
    // is state p accessible :
    std::vector<bool> accessible;
    // successor of state p by letter l is succ[p+n*l]:
    std::vector<unsigned> succ(alpha_size*n);
    // list of accessible states:
    std::vector<unsigned> visited;
    for(unsigned j=0;j<50;) {
      //We start with (initial) state 0 and explore the accessible part;
      //The list of accessible states which are not yet explored is in todo:
      std::queue<unsigned> todo;
      accessible.assign(n,false);
      accessible[0]=true;
      visited.emplace_back(0);
      todo.emplace(0);
      while(!todo.empty()) {
        unsigned i=todo.front();
        todo.pop();
        for(unsigned l=0; l<alpha_size; ++l) {
          //for each state and each letter, a successor is chosen:
          unsigned j=dice();
          succ[l*n+i]=j;
          if(!accessible[j]) {
            accessible[j]=true;
            visited.emplace_back(j);
            todo.emplace(j);
          }
        }
      }
      //end of the exploration; if the size of the automaton is m; we are done.
      if(visited.size()==size)
        break;
      succ.clear();
      visited.clear();
    }
    //creation of the DFA
    automaton_t aut=automaton_t::from(alphabet);
    std::unordered_map<unsigned,unsigned> states;
    for(auto i: visited) {
      states[i] =aut->add_state();
    }
    aut->set_initial(states[0]);
    std::uniform_int_distribution<unsigned> distribution2(0,1);
    auto dice2 = std::bind ( distribution2, generator );
    //Every state is final with probability 0.5
    for(auto i: visited) {
      for(unsigned l=0; l<alpha_size;++l)
        aut->set_transition(states[i], states[succ[l*n+i]], alphabet[l]);
      if(dice2())
        aut->set_final(states[i]);
    }
    return aut;
  }

}}//end of ns awali::dyn


