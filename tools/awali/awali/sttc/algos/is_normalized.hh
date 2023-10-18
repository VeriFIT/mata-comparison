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

#ifndef AWALI_ALGOS_IS_NORMALIZED_HH
# define AWALI_ALGOS_IS_NORMALIZED_HH

#include <awali/sttc/core/mutable_automaton.hh>

namespace awali { namespace sttc {

  template <typename Aut>
  bool
  is_normalized(const Aut& a)
  {
    return
      a->num_initials() == 1
      && a->weightset()->is_one(a->weight_of(a->initial_transitions().front()))
      && a->num_finals() == 1
      && a->weightset()->is_one(a->weight_of(a->final_transitions().front()))
      // No arrival on the initial state.
      && a->in(a->dst_of(a->initial_transitions().front())).empty()
      // No departure from the final state.
      && a->out(a->src_of(a->final_transitions().front())).empty();
  }
}}//end of ns awali::stc

#endif // !AWALI_ALGOS_IS_NORMALIZED_HH
