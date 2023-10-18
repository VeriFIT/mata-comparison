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
#include <awali/sttc/algos/complement.hh>
#include <awali/sttc/algos/complete.hh>
#include <awali/sttc/algos/reduce.hh>
#include <awali/sttc/algos/is_complete.hh>
#include <awali/sttc/algos/is_ambiguous.hh>
#include <awali/sttc/algos/real_time.hh>
#include <awali/sttc/weightset/z.hh>
#include <awali/sttc/weightset/q.hh>
#include <awali/sttc/weightset/r.hh>
#include <awali/sttc/weightset/c.hh>
#include <awali/sttc/weightset/f2.hh>
#include <awali/sttc/weightset/zz.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include<stdexcept>

#include<set-types.hh>

namespace awali {

  //Default behaviour for functions dedicated to Boolean

  template<typename Context,
           typename LS=typename Context::labelset_t,
           typename WS=typename Context::weightset_t>
  struct dispatch_B {
    static dyn::automaton_t determinize(dyn::automaton_t, bool) {
      throw std::runtime_error("determinize only supported for automata with finite weightset and free labelset with no epsilon-transitions allowed.");
    }

    static dyn::automaton_t complement(dyn::automaton_t) {
      throw std::runtime_error("complement only supported for NFA");
    }

    static void complement_here(dyn::automaton_t) {
      throw std::runtime_error("complement_here only supported for DFA");
    }
  };

  template<typename T>
  struct dispatch_B<context_t, sttc::letterset<T>, sttc::b> { //default
    static dyn::automaton_t determinize(dyn::automaton_t aut, bool history) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return dyn::make_automaton(sttc::determinize(a, history));
    }

    static dyn::automaton_t complement(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return dyn::make_automaton(sttc::complement(a));
    }

    static void complement_here(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      sttc::complement_here(a);
    }
  };

  //default behaviour for functions dedicated to LAL

  template<typename Context,
           typename LS=typename Context::labelset_t,
           typename WS=typename Context::weightset_t>
  struct dispatch_LAL {
    static dyn::automaton_t complete(dyn::automaton_t) {
      throw std::runtime_error("complete only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }

    static void complete_here(dyn::automaton_t) {
      throw std::runtime_error("complete_here only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }

    static bool is_deterministic(dyn::automaton_t) {
      throw std::runtime_error("is-deterministic only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }

    static bool is_sequential(dyn::automaton_t) {
      throw std::runtime_error("is-sequential only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }

    static bool is_complete(dyn::automaton_t) {
      throw std::runtime_error("is-complete only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
    static bool is_ambiguous(dyn::automaton_t){
      throw std::runtime_error("is-ambiguous only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
    static dyn::automaton_t weighted_determinize(dyn::automaton_t) {
      throw std::runtime_error("weighted determinization only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
    static dyn::automaton_t explore_by_length(dyn::automaton_t, unsigned) {
      throw std::runtime_error("exploration only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
    
    static dyn::automaton_t explore_with_bound(dyn::automaton_t, dyn::any_t /*bound*/) {
      throw std::runtime_error("exploration only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
  };

  template<typename ...T>
  struct dispatch_LAL<context_t, sttc::tupleset<T...>, weightset_t> {
    static dyn::automaton_t complete(dyn::automaton_t) {
      throw std::runtime_error("complete only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }

    static void complete_here(dyn::automaton_t) {
      throw std::runtime_error("complete_here only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }

    static bool is_deterministic(dyn::automaton_t) {
      throw std::runtime_error("is_deterministic only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }

    static bool is_sequential(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return sttc::is_sequential_tdc(a);
    }

    static bool is_complete(dyn::automaton_t) {
      throw std::runtime_error("is-complete only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
    static bool is_ambiguous(dyn::automaton_t) {
      throw std::runtime_error("is-ambiguous only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
    static dyn::automaton_t weighted_determinize(dyn::automaton_t)  {
      throw std::runtime_error("weighted determinization only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
    static dyn::automaton_t explore_by_length(dyn::automaton_t, unsigned /*depth*/) {
      throw std::runtime_error("exploration only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
    static dyn::automaton_t explore_with_bound(dyn::automaton_t, dyn::any_t /*bound*/) {
      throw std::runtime_error("exploration only supported for automata labelled with letters and no epsilon-transitions allowed.");
    }
  };
  ////  Specific behaviour

  template<typename T>
  struct dispatch_LAL<context_t, sttc::letterset<T>, weightset_t> { //default
    static dyn::automaton_t complete(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return dyn::make_automaton(sttc::complete(a));
    }

    static void complete_here(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      sttc::complete_here(a);
    }

    static bool is_deterministic(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return sttc::is_deterministic(a);
    }

    static bool is_sequential(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return sttc::is_sequential(a);
    }

    static bool is_complete(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return sttc::is_complete(a);
    }

    static bool is_ambiguous(dyn::automaton_t aut){
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return sttc::is_ambiguous(a);
    }

    static dyn::automaton_t weighted_determinize(dyn::automaton_t aut) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return dyn::make_automaton(sttc::weighted_determinize(a));
    }

    static dyn::automaton_t explore_by_length(dyn::automaton_t aut, unsigned depth) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      return dyn::make_automaton(sttc::explore_by_length(a, depth));
    }

    static dyn::automaton_t explore_with_bound(dyn::automaton_t aut, dyn::any_t bound) {
      auto a=dyn::get_stc_automaton<context_t>(aut);
      auto b=dyn::internal::extract_value(bound,*(a->weightset()));
      return dyn::make_automaton(sttc::explore_with_bound(a, b));
    }
  };

  //default behaviour for reduction

  template<typename Context,
           typename LS=typename Context::labelset_t,
           typename WS=typename Context::weightset_t>
  struct dispatch_reduction {
    static dyn::automaton_t reduce(dyn::automaton_t) {
      throw std::runtime_error("reduce algorithm only supported for automata with weightset equal to Z or a field and with free labelset with no epsilon-transitions allowed.");
    }

    static dyn::automaton_t left_reduce(dyn::automaton_t) {
      throw std::runtime_error("reduce algorithm only supported for automata with weightset equal to Z or a field and with free labelset with no epsilon-transitions allowed.");
    }
  };

#define defreduce(SR)          \
  template<typename T>  \
  struct dispatch_reduction<context_t, sttc::letterset<T>, sttc::SR> {\
    static dyn::automaton_t reduce(dyn::automaton_t aut) {\
      auto a=dyn::get_stc_automaton<context_t>(aut);\
      return dyn::make_automaton(sttc::reduce(a));\
    }\
    \
    static dyn::automaton_t left_reduce(dyn::automaton_t aut) {\
      auto a=dyn::get_stc_automaton<context_t>(aut);\
      return dyn::make_automaton(sttc::left_reduce(a));\
    }\
  };

  defreduce(z)
  defreduce(q)
  defreduce(r)
  defreduce(c)
  defreduce(f2)
  defreduce(zz<2>)
  defreduce(zz<3>)
  defreduce(zz<5>)
  defreduce(zz<7>)
  defreduce(zz<11>)

 #undef defreduce

  //////////////////////////////////////////////


  extern "C" dyn::automaton_t determinize(dyn::automaton_t aut, bool history) {
    return dispatch_B<context_t>::determinize(aut, history);
  }

  extern "C" dyn::automaton_t complement(dyn::automaton_t aut) {
    return dispatch_B<context_t>::complement(aut);
  }

  extern "C" void complement_here(dyn::automaton_t aut) {
    dispatch_B<context_t>::complement_here(aut);
  }

  extern "C" dyn::automaton_t complete(dyn::automaton_t aut) {
    return dispatch_LAL<context_t>::complete(aut);
  }

  extern "C" void complete_here(dyn::automaton_t aut) {
    dispatch_LAL<context_t>::complete_here(aut);
  }

  extern "C" dyn::automaton_t reduce(dyn::automaton_t aut) {
    return dispatch_reduction<context_t>::reduce(aut);
  }

  extern "C" dyn::automaton_t left_reduce(dyn::automaton_t aut) {
    return dispatch_reduction<context_t>::left_reduce(aut);
  }

  extern "C" bool is_deterministic(dyn::automaton_t aut) {
    return dispatch_LAL<context_t>::is_deterministic(aut);
  }

  extern "C" bool is_sequential(dyn::automaton_t aut) {
    return dispatch_LAL<context_t>::is_sequential(aut);
  }

  extern "C" bool is_complete(dyn::automaton_t aut) {
    return dispatch_LAL<context_t>::is_complete(aut);
  }

  extern "C" bool is_ambiguous(dyn::automaton_t aut) {
    return dispatch_LAL<context_t>::is_ambiguous(aut);
  }

  extern "C" dyn::automaton_t weighted_determinize(dyn::automaton_t aut) {
    return dispatch_LAL<context_t>::weighted_determinize(aut);
  }

  extern "C" dyn::automaton_t explore_by_length(dyn::automaton_t aut, unsigned depth) {
    return dispatch_LAL<context_t>::explore_by_length(aut, depth);
  }

  extern "C" dyn::automaton_t explore_with_bound(dyn::automaton_t aut, dyn::any_t bound) {
    return dispatch_LAL<context_t>::explore_with_bound(aut, bound);
  }
}

#include <awali/dyn/core/any.cc>
