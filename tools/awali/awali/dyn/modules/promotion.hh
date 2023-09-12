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

#ifndef DYN_MODULES_PROMOTION_HH
#define DYN_MODULES_PROMOTION_HH

#include<string>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
  namespace dyn {

    /** Computes a context with the same labelset as \p ctx and the weightset \p ws
     *
     * \p ws is the public name of a weightset like "B", "Z/5Z", etc.
     *
     * @param ctx  A dynamic context
     * @param ws the public name of a weightset
     *
     * @return a new dynamic context
     **/
    context_t make_context_with_another_semiring(context_t ctx, const std::string& ws);

    /** Tests whether a promotion is possible from one weightset to the other one
     *
     * \p src_ws and \p dest_ws are the public names of weightsets like "B", "Z/5Z", etc.
     *
     * @param src_ws the weightset to promote
     * @param dest_ws the target of the promotion
     *
     * @return true if the promotion is possible.
     **/
    bool is_promotable(const std::string& src_ws, const std::string& dest_ws);

   /** Computes a new automaton with the specified weightset
     *
     * If \p aut is a Boolean automaton this function is equivalent to characteristic.
     * If \p ws is the weightset of \p aut, the function returns a copy of the automaton.
     *
     * \p ws is the public name of a weightset like "B", "Z/5Z", etc.
     *
     * The following option may be given: {@link KEEP_HISTORY}.
     * (Other options are ignored.)
     *
     * @param aut a dynamic automaton
     * @param ws the public name of a weightset
     * @param opts some options
     *
     * @return a new dynamic automaton
     **/
     automaton_t promote_automaton(automaton_t aut, const std::string& ws, options_t opts = {});

  }
}//end of ns awali::dyn

#endif
