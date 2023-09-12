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

#include <awali/sttc/automaton.hh>
#include <awali/sttc/algos/js_print.hh>
#include <awali/sttc/factories/ladybird.hh>
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/algos/min_quotient.hh>
#include <awali/sttc/algos/product.hh>
#include <awali/sttc/weightset/z.hh>
#include <sys/resource.h>

     #define   RUSAGE_SELF     0
     #define   RUSAGE_CHILDREN     -1


/*  Benchmark of quotient algorithms
 */
int getrusage(int who, struct rusage *r_usage);
//Static features are defined in the awali namespace
using namespace awali::sttc;

int main() {
  /* The NFA factory requires a list of char letters and returns an empty
     mutable automaton.
  */

  std::cout << "Benchmark of quotient algorithms" << std::endl
            << "Each algorithm is launched 10 times" << std::endl;


  auto aut=make_automaton<z>({'a','b'});

  unsigned s[2];
  for(int i=0; i<2; i++)
    s[i]=aut->add_state();
  /* a mutable_automaton is actually a (shared) pointer;
     to call the method, one have to use ->
  */
  aut->set_initial(s[0]);
  aut->set_final(s[1]);
  aut->set_transition(s[0], s[0], 'a');
  aut->set_transition(s[0], s[0], 'b');
  aut->set_transition(s[0], s[1], 'b');
  aut->set_transition(s[1], s[1], 'b', 2);
  aut->set_transition(s[1], s[1], 'a', 2);

  std::cout << "**************** First benchmark *************" << std::endl
            << "Power of the Z-automaton" << std::endl
            << "that computes the value of a number in basis 2" << std::endl
            << "**********************************************" << std::endl;

  for(unsigned round=6; round < 15 ; round++) {
    struct rusage r0,r1,r2;
    auto ap = power(aut, round);
    std::cout << "*** ROUND " << round << " : ";
    std::cout << ap->num_states() <<" states, "
              << ap->num_transitions() <<" transitions" << std::endl;
    std::vector<std::vector<awali::state_t> > equiv;
    std::vector<std::list<awali::state_t> > equivl;
    unsigned iterations;
    for(int alg=0; alg<3; ++alg) {
      double t=0;
        switch(alg) {
        case 0: std::cout << "Moore   "; break;
        case 1: std::cout << "Backward"; break;
        case 2: std::cout << "Hopcroft"; break;
        }
        //each algorithm is launched 10 times
    for(int i=0; i<10;i++) {
        std::cout << '.';
        std::cout << std::flush;
        getrusage(RUSAGE_SELF,&r1);
        switch(alg) {
        case 0: iterations=moore_quotient(ap,equiv); break;
        case 1: iterations=hopcroft_quotient(ap,equivl); break;
        case 2: iterations=hopcroft_quotient(ap,equivl,true); break;
        }
        getrusage(RUSAGE_SELF,&r2);
        t+=r2.ru_utime.tv_sec-r1.ru_utime.tv_sec +(r2.ru_utime.tv_usec-r1.ru_utime.tv_usec)/1000000.0;
        equiv.clear();
      }
    std::cout << std::setw(12) << t/10 << ",   " << iterations << " iterations" << std::endl;
      //js_print(minap, std::cout) << std::endl;
      //std::cout << minap->num_states() << std::endl;
    }
  }
  std::cout << "*************** Second benchmark *************" << std::endl
            << "Determinization of ladybird" << std::endl
            << "**********************************************" << std::endl;
  for(unsigned round=6; round < 15 ; round++) {
      struct rusage r0,r1,r2;
    unsigned iterations;
    std::vector<std::vector<awali::state_t> > equiv;
    std::vector<std::list<awali::state_t> > equivl;

    auto baut=make_automaton({'a','b','c'});
    auto p = baut->add_state();
    baut->set_initial(p);
    baut->set_final(p);
    auto x=p;
    for (unsigned i = 1; i <=round ; ++i)
      {
        auto y = baut->add_state();
        baut->new_transition(x, y, 'a');
        baut->new_transition(y, y, 'b');
        baut->new_transition(y, y, 'c');
        baut->new_transition(y, p, 'c');
        x = y;
      }
    baut->new_transition(x, p, 'a');
    
    auto daut=determinize(baut);
    
    std::cout << "*** ROUND " << round << " : ";
    std::cout << daut->num_states() <<" states, "
              << daut->num_transitions() <<" transitions" << std::endl;
    for(int alg=0; alg<3; ++alg) {
      double t=0;
        switch(alg) {
        case 0: std::cout << "Moore   "; break;
        case 1: std::cout << "Backward"; break;
        case 2: std::cout << "Hopcroft"; break;
        }
        for(int i=0; i<10;i++) {
          std::cout << '.';
          std::cout << std::flush;
          getrusage(RUSAGE_SELF,&r1);
          switch(alg) {
          case 0: iterations=moore_quotient(daut,equiv); break;
          case 1: iterations=hopcroft_quotient(daut,equivl); break;
          case 2: iterations=hopcroft_quotient(daut,equivl,true); break;
          }
          getrusage(RUSAGE_SELF,&r2);
          t+=r2.ru_utime.tv_sec-r1.ru_utime.tv_sec +(r2.ru_utime.tv_usec-r1.ru_utime.tv_usec)/1000000.0;
          equiv.clear();
        }
        std::cout << std::setw(12) << t/10 << ", " << iterations << " iterations" << std::endl;
    }
  }

  std::cout << "*************** Third benchmark **************" << std::endl
            << "Railroad automaton" << std::endl
            << ".... x -- a+2b --> xx   ....." << std::endl
            << ".... x --   a  --> yy   ....." << std::endl
           <<  ".... y --   b  --> xx   ....." << std::endl
           <<  ".... y -- 2a+b --> yy   ....." << std::endl
            << "**********************************************" << std::endl;
  for(unsigned round=6; round<12; round++) {
  auto zaut=make_automaton<z>({'a','b'});
  auto si = zaut->add_state();
  auto sj = zaut->add_state();
  zaut->set_initial(si);
  auto x=si, y=sj;
  for(unsigned r=0; r<(1<<round); ++r) {
    auto xx=zaut->add_state();
    auto yy=zaut->add_state();
    zaut->new_transition(x,xx,'a');
    zaut->new_transition(x,xx,'b',2);
    zaut->new_transition(x,yy,'a');
    zaut->new_transition(y,xx,'b');
    zaut->new_transition(y,yy,'a',2);
    zaut->new_transition(y,yy,'b');
    x=xx;
    y=yy;
  }  
  zaut->set_final(x);
  zaut->set_final(y);
    std::cout << "*** ROUND " << round << " : ";
    std::cout << zaut->num_states() <<" states, "
              << zaut->num_transitions() <<" transitions" << std::endl;
    struct rusage r0,r1,r2;
    unsigned iterations;
    std::vector<std::vector<awali::state_t> > equiv;
    std::vector<std::list<awali::state_t> > equivl;
     for(int alg=0; alg<3; ++alg) {
        switch(alg) {
        case 0: std::cout << "Moore   "; break;
        case 1: std::cout << "Backward"; break;
        case 2: std::cout << "Hopcroft"; break;
        }
        double t=0;
        for(int i=0; i<10;i++) {
          std::cout << '.';
          std::cout << std::flush;
          getrusage(RUSAGE_SELF,&r1);
          switch(alg) {
          case 0: iterations=moore_quotient(zaut,equiv); break;
          case 1: iterations=hopcroft_quotient(zaut,equivl); break;
          case 2: iterations=hopcroft_quotient(zaut,equivl,true); break;
          }
          getrusage(RUSAGE_SELF,&r2);
          t+=r2.ru_utime.tv_sec-r1.ru_utime.tv_sec +(r2.ru_utime.tv_usec-r1.ru_utime.tv_usec)/1000000.0;
          equiv.clear();
        }
        std::cout << t/10 << ',' << iterations << " iterations" << std::endl;
    }
  }
}
