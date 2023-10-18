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

#include <awali/dyn/modules/quotient.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/modules/determinize.hh>
#include <awali/dyn/algos/aliases.hh>
#include <unordered_set>

namespace awali {
  namespace dyn {

    automaton_t min_quotient_det(automaton_t aut, options_t opts)
    {
      return loading::call1<automaton_t>("min_quotient_det", "quotient", aut,
                                         opts[QUOTIENT_ALGO]);
    }

    automaton_t min_quotient(automaton_t aut, options_t opts)
    {
      try {
        if(is_deterministic(aut))
          return min_quotient_det(aut, opts);
      }
      catch(...) {}
      return loading::call1<automaton_t>("min_quotient", "quotient", aut,
                                         opts[QUOTIENT_ALGO]);
    }

    bool is_quotient(automaton_t aut1, automaton_t aut2)
    {
      return loading::call1<bool>("is_quotient", "quotient", aut1, aut2);
    }

    static void normalize_equiv(automaton_t aut,
                                std::vector<std::vector<state_t>>& equiv)
    {
      std::unordered_set<state_t> m;
      unsigned count =0;
      for(auto v: equiv) {
        count += v.size();
        for(auto s: v) {
          if(!aut->has_state(s))
            throw std::runtime_error("Quotient: Wrong state in partition");
          if(m.find(s)!=m.end())
            throw std::runtime_error("Quotient: Duplicate state in partition");
          m.emplace(s);
        }
      }
      for(auto s: aut->states({PREPOST_PARADIGM=true}))
        if(m.find(s)==m.end()) {
          std::vector<state_t> vs{s};
          equiv.emplace_back(vs);
        }
    }

    automaton_t merge(automaton_t aut, std::vector<std::vector<state_t>>& equiv)
    {
      return loading::call1<automaton_t, automaton_t, std::vector<std::vector<state_t>>&>("merge",
             "quotient", aut, equiv);
    }

    static bool is_congruence_exact(automaton_t aut,
                                    std::vector<std::vector<state_t>>& equiv)
    {
      return loading::call1<bool, automaton_t, std::vector<std::vector<state_t>>&>("is_congruence",
             "quotient", aut, equiv);
    }

    automaton_t
    quotient(automaton_t aut, std::vector<std::vector<state_t>>& equiv,
             options_t opts)
    {
      normalize_equiv(aut, equiv);
      if (opts[SAFE])
        if (!(is_congruence_exact(aut, equiv)))
          throw std::runtime_error("Quotient: The equivalence is not a congruence");
      return merge(aut, equiv);
    }

    bool is_congruence(automaton_t aut, std::vector<std::vector<state_t>>& equiv)
    {
      normalize_equiv(aut, equiv);
      return is_congruence_exact(aut, equiv);
    }

    //Boolean automata only
    automaton_t minimal_automaton(automaton_t aut, options_t opts)
    {
      return loading::call1<automaton_t>("minimal_automaton", "quotient", aut,
                                         opts[MINIM_ALGO], opts[QUOTIENT_ALGO]);
    }

    automaton_t minimal_automaton(ratexp_t exp, options_t opts)
    {
      return minimal_automaton(exp_to_aut(exp, opts), opts);
    }


  }
}//end of ns awali::dyn and awali
