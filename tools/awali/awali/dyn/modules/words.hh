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

#ifndef DYN_MODULES_WORDS_HH
#define DYN_MODULES_WORDS_HH
#include <awali/dyn/core/automaton.hh>

namespace awali {
  namespace dyn {

    /** Returns a copy of \p aut in which transitions may bear words.
     *
     * @param aut
     * @return a new automaton, copy of \p aut in which transitions may bear
     * words.
     */
    automaton_t allow_words(automaton_t aut);


    /* From an automaton that allows words as label, build a new equivalent
     * automaton that has letters or epsilon as label.
     *  @param aut
     */
    automaton_t letterize(automaton_t aut);


    /** In a copy of an automaton \p aut which allows words as label, compacts
     * each non-branching path into one transition.
     * @param aut
     */
    automaton_t compact(automaton_t aut);

    context_t context_words(automaton_t aut);
  }
}//end of ns awali::dyn

#endif
