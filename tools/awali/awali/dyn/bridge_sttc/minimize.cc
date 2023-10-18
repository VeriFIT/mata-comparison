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

#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/algos/min_quotient.hh>
#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/algos/complete.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include<stdexcept>

#include<set-types.hh>

namespace awali {

  template<typename Context,
           typename LS=typename Context::labelset_t,
           typename WS=typename Context::weightset_t>
  struct dispatch { //default
    static dyn::automaton_t minimal_automaton(dyn::automaton_t,
                                              minim_algo_t, 
                                              quotient_algo_t) 
    { 
      throw std::runtime_error("minimal_automaton only supported for Boolean automata");
    }
  };

  template<typename Context, typename T>
  struct dispatch<Context,sttc::letterset<T>, sttc::b> {
    static dyn::automaton_t minimal_automaton(dyn::automaton_t aut,
      minim_algo_t malgo, quotient_algo_t qalgo) 
    {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      switch(malgo) {
        case BRZOZOWSKI:{
          a=sttc::determinize(sttc::transpose_view(a));
          a=determinize(sttc::transpose_view(a));
	  sttc::complete_here(a);
          break;
        }
        case DET_QUOTIENT: {
          if(!sttc::is_deterministic(a))
            a=determinize(a);
	  sttc::complete_here(a);
          switch (qalgo) {
            case MOORE:
              a=sttc::min_quotient(a,MOORE);
              break;
            case HOPCROFT:
              a=sttc::min_quotient(a,HOPCROFT);
              break;
          }
        break;
        }
        default:
          throw std::runtime_error("Unreachable statement");
      }
      return dyn::make_automaton(a);
    }
  };

  template<typename Context, typename T>
  struct dispatch<Context, sttc::nullableset<sttc::letterset<T>>, sttc::b> {
    static dyn::automaton_t minimal_automaton(
      dyn::automaton_t aut, 
      minim_algo_t malgo, 
      quotient_algo_t qalgo) 
  {
      auto res=proper(dyn::get_stc_automaton<context_t>(aut));
      auto a = sttc::make_mutable_automaton<sttc::not_nullable_of<context_t>>(sttc::get_not_nullable_context(res->context()));
      sttc::copy_into(res,a);
      switch(malgo) {
        case BRZOZOWSKI:{
          a=sttc::determinize(sttc::transpose_view(a));
          a=determinize(sttc::transpose_view(a));
	  sttc::complete_here(a);
          break;
        }
        case DET_QUOTIENT: {
          if(!sttc::is_deterministic(a))
            a=determinize(a);
	  sttc::complete_here(a);
          switch (qalgo) {
            case MOORE:
              a=sttc::min_quotient(a,MOORE);
              break;
            case HOPCROFT:
              a=sttc::min_quotient(a,HOPCROFT);
              break;
          }
        break;
        }
        default:
          throw std::runtime_error("Unreachable statement");
      }
      return dyn::make_automaton(a);
    }
  };


  extern "C" dyn::automaton_t minimal_automaton(dyn::automaton_t aut, 
    minim_algo_t malgo, quotient_algo_t qalgo) {
    return dispatch<context_t>::minimal_automaton(aut, malgo, qalgo);
  }
}
