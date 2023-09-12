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

#ifndef DYN_MODULES_AUTOMATON_CC
#define DYN_MODULES_AUTOMATON_CC

#include <iostream>
#include <unordered_map>
#include <set>


#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/core/context_description.hh>

#include <awali/dyn/modules/automaton.hh>

namespace awali {
  namespace dyn {

    automaton_t copy(automaton_t aut, options_t opts)
    {
      return loading::call1<automaton_t>("copy", "automaton", aut,
                                         opts[KEEP_HISTORY]);
    }

    automaton_t support(automaton_t aut, options_t opts)
    {
      return loading::call1<automaton_t>("support", "automaton", aut,
                                         opts[KEEP_HISTORY]);
    }

    automaton_t
    characteristic(automaton_t aut, std::string semiring, options_t opts)
    {
      std::string stat_ctx
        = aut->get_context()->labelset_name() + "_"
          + context::tostring(context::weightset(semiring), false);
      return loading::call0<automaton_t>("characteristic", "automaton",
                                         stat_ctx, aut, opts[KEEP_HISTORY]);
    }

    bool are_isomorphic(automaton_t aut1, automaton_t aut2)
    {
      return loading::call1<bool>("are_isomorphic", "automaton", aut1, aut2);
    }

    void change_alphabet(automaton_t aut,  const std::string& alphabet) {
      std::set<char> v(alphabet.begin(), alphabet.end());
      loading::call1<void>("change_alphabet", "automaton", aut, v);
    }

    void change_int_alphabet(automaton_t aut, int a, int b) {
      std::set<int> v;
      for(int i=a; i<=b; i++)
        v.emplace(i);
      loading::call1<void>("change_int_alphabet", "automaton", aut, v);
    }

    automaton_t left_mult(automaton_t aut, weight_t w, options_t opts)
    {
      automaton_t res = opts[IN_PLACE] ? aut : copy(aut, opts);
      loading::call1<void>("left_mult_here", "automaton", res, w);
      return res;
    }

    automaton_t right_mult(automaton_t aut, weight_t w, options_t opts)
    {
      automaton_t res = opts[IN_PLACE] ? aut : copy(aut, opts);
      loading::call1<void>("right_mult_here", "automaton", res, w);
      return res;
    }

    void add_path(automaton_t aut, state_t p, state_t q, ratexp_t exp) {
      loading::call1<void>("add_path_exp", "automaton", aut, p, q, exp);
    }
    
    void add_path(automaton_t aut, state_t p, state_t q, const std::string& s, bool strict_alphabet) {
      loading::call1<void>("add_path_string", "automaton", aut, p, q, s, strict_alphabet);
    }


  }
}//end of ns awali::dyn

#endif
