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

#ifndef DYN_MODULES_EVAL_HH
#define DYN_MODULES_EVAL_HH

#include <map>
#include <awali/dyn/core/automaton.hh>

//Only for lal

namespace awali {
  namespace dyn {

    /** Computes the weight associated with \p word by \p aut.
     * @param aut
     * @param word
     * @pre \p aut should not be a transducer or allow epsilon transitions.
     */
    weight_t eval(automaton_t aut, any_t word);


    /** Gives the weight associated with each word shorter than \p max by \p aut.
     *
     * @param aut
     * @param max
     * @pre \p aut should not be a transducer or allow epsilon transitions.
     */
    std::map<any_t, weight_t> enumerate(automaton_t aut, unsigned max);


    /** Gives the shortest , returns an empty map if no such word is shorter than
     * \p max.
     * @param aut
     * @param max
     * @pre \p aut should not be a transducer or allow epsilon transitions.
     */
    std::map<any_t, weight_t> shortest(automaton_t aut, unsigned max);

  }
}//end of ns awali::dyn

#endif
