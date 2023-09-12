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

#include <awali/dyn/modules/graph.hh>

#include <awali/dyn/loading/handler.hh>

namespace awali {
  namespace dyn {

    namespace internal {
      std::pair< std::unordered_map<state_t, unsigned int>,
          std::vector<std::vector<state_t>> >
          scc_recursive (automaton_t aut)
      {
        return loading::call1<std::pair< std::unordered_map<state_t, unsigned int>, std::vector<std::vector<state_t>> >>("scc_recursive",
               "graph", aut);
      }


      std::pair< std::unordered_map<state_t, unsigned int>,
      std::vector<std::vector<state_t>> >
                                     scc_iterative (automaton_t aut)
      {
        return loading::call1<  std::pair< std::unordered_map<state_t, unsigned int>, std::vector<std::vector<state_t>> >  >("scc_iterative",
               "graph", aut);
      }


      std::pair<  automaton_t,
      std::pair< std::unordered_map<state_t, unsigned int>,
      std::vector<std::vector<state_t>> >  >
                                     condensation (automaton_t aut)
      {
        return loading::call1< std::pair<  automaton_t, std::pair< std::unordered_map<state_t, unsigned int>, std::vector<std::vector<state_t>> >  >
               >("condensation", "graph", aut);
      }
    }


    scc_return_t strongly_connected_components(automaton_t aut)
    {
      auto res = internal::scc_iterative(aut);
      return {res.first, res.second};
    }

    automaton_t condensation (automaton_t aut)
    {
      return internal::condensation(aut).first;
    }

    /** Computes the strongly connected components of @pname{s}.
     * 
     * Note that if \pname{stt} cannot access itself with a path of positive
     * length, this still returns `{stt}`.
     *
     * @param aut
     * @param stt
     * @return The set of states that are reachable and may be reached by
     * @pname{stt}; if this set is empty, returns `{stt}`.
     * */
    std::vector<state_t>
    scc_of (automaton_t aut, state_t stt)
    {
      return loading::call1<std::vector<state_t>>("scc_of", "graph", aut, stt);
    }

  }
}//end of ns awali::dyn
