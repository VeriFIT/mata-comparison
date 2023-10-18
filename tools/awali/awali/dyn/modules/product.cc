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

#ifndef DYN_MODULES_PRODUCT_CC
#define DYN_MODULES_PRODUCT_CC

#include <unordered_map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/loading/handler.hh>

#include <awali/dyn/modules/join.hh>
#include <awali/dyn/modules/promotion.hh>
#include <awali/dyn/modules/product.hh>

namespace awali {
  namespace dyn {

    static void check_weightsets(automaton_t aut1, automaton_t aut2) {
      std::string semiring1 = aut1->get_context()->weightset_name();
      std::string semiring2 = aut2->get_context()->weightset_name();
      if(semiring1 != semiring2)
        throw std::runtime_error("both automata should have the same weightset");
    }

    /*
    static void join_automata(automaton_t& aut1, automaton_t& aut2) {
      std::string semiring1 = aut1->get_context()->weightset_name();
      std::string semiring2 = aut2->get_context()->weightset_name();
      if(semiring1 != semiring2) {
        if(is_promotable(semiring1,semiring2))
          aut1 = promote_automaton(aut1, semiring2);
        else if(is_promotable(semiring2,semiring1))
          aut2 = promote_automaton(aut2, semiring1);
        else    
          throw std::runtime_error("no promotion between weightsets of both automata");
      
      }
    }
    */    
    static void join_automata(automaton_t& aut1, automaton_t& aut2) {
      context_t ctx1 = aut1->get_context();
      context_t ctx2 = aut2->get_context();
      if (*ctx1 == *ctx2)
        return;
      context_t join_ctx = join_context(ctx1, ctx2);
      if (*ctx1 != *join_ctx)
        aut1 = promote_automaton(aut1, join_ctx, {KEEP_HISTORY=true, SAFE=false});
      if (*ctx2 != *join_ctx)
        aut2 = promote_automaton(aut2, join_ctx, {KEEP_HISTORY=true, SAFE=false});
    }

    automaton_t product_strict(automaton_t aut1, automaton_t aut2, options_t opts)
    {
      check_weightsets(aut1, aut2);
      return loading::call2<automaton_t>("product", "product", aut1, aut2,
                                         opts[KEEP_HISTORY]);
    }

    automaton_t product(automaton_t aut1, automaton_t aut2, options_t opts)
    {
      join_automata(aut1, aut2);
      return loading::call2<automaton_t>("product", "product", aut1, aut2,
                                         opts[KEEP_HISTORY]);
    }

    automaton_t shuffle(automaton_t aut1, automaton_t aut2, options_t opts)
    {
      join_automata(aut1, aut2);
      return loading::call2<automaton_t>("shuffle", "product", aut1, aut2,
                                         opts[KEEP_HISTORY]);
    }

    automaton_t infiltration(automaton_t aut1, automaton_t aut2, options_t opts)
    {
      join_automata(aut1, aut2);
      return loading::call2<automaton_t>("infiltration", "product", aut1, aut2,
                                         opts[KEEP_HISTORY]);
    }

    automaton_t sum(automaton_t aut1, automaton_t aut2, options_t opts)
    {
      if (opts[IN_PLACE]) {
        loading::call1<void>("sum_here","automaton",aut1,aut2);
        return aut1;
      }
      else {
        join_automata(aut1, aut2);
        return loading::call2<automaton_t>("sum", "product", aut1, aut2);
      }
    }

    automaton_t concatenate(automaton_t aut1, automaton_t aut2, options_t opts)
    {
      if (opts[IN_PLACE]) {
        loading::call1<void>("concatenate_here", "automaton", aut1, aut2);
        return aut1;
      }
      else {
        join_automata(aut1, aut2);
        return loading::call2<automaton_t>("concatenate", "product", aut1, 
                                           aut2);
      }
    }
  }
}//end of ns awali::dyn

#endif
