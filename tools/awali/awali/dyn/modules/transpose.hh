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

#ifndef DYN_MODULES_TRANSPOSE_HH
#define DYN_MODULES_TRANSPOSE_HH

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
  namespace dyn {
    /** Transposes \p aut or returns a transposed copy of \p aut.
     *
     * The following options may be given: {@link KEEP_HISTORY}, {@link IN_PLACE}.
     * (Other options are ignored.)    
     * {@link KEEP_HISTORY} is meaningful only if {@link IN_PLACE} is false.
     *
     * @param aut Automaton to transpose
     * @param opts A set of {@link Options options}
     * @return a new automaton, or the input if {@link IN_PLACE} is `true`.
     */
    automaton_t transpose(automaton_t aut, options_t opts = {});
  }
}//end of ns awali::dyn

#endif
