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

#ifndef DYN_MODULES_GRAPH_HH
#define DYN_MODULES_GRAPH_HH

#include <unordered_map>
#include <vector>

#include <awali/dyn/core/automaton.hh>

namespace awali {
  namespace dyn {

    struct scc_return_t {
      /** Map of states to their strongly connected component. */
      std::unordered_map<state_t, unsigned int> scc_of;

      /** Partition of the states into strongly connected components. */
      std::vector<std::vector<state_t>> partition;
    };

    /**
     * Computes the strongly connected components of an automaton.
     * @param aut
     * @return A double map: states to scc and scc to states
     */
    scc_return_t strongly_connected_components(automaton_t aut);


    /** Computes the condensation of an automaton; it results from reducing each
     * strongly connected component of the original automaton to a single state
     *
     * @param aut
     * @return The condensation of \p aut
     */
    automaton_t condensation (automaton_t aut);

    /** Returns the strongly connected component of a state.
     *
     * @ param aut
     * @ param state The strongly connected component of which is computed
     * @ return The list of the states in the strongly connected component of \p
     * aut
     */
    std::vector<state_t> scc_of(automaton_t aut, state_t s);

  }
}//end of ns awali::dyn

#endif
