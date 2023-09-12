
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

#include<cmath>
#include<random>
#include<ctime>
#include<queue>
#include <awali/dyn/modules/automaton.hh>
#include <awali/dyn/modules/context.hh>

#ifndef AWALI_DYN_RANDOM_HH
#define AWALI_DYN_RANDOM_HH

namespace awali { namespace dyn {
namespace factory {
  /** \ingroup Factories  
   * Generates a random deterministic automaton over alphabet \p alph with \p
   * size states.
   * @param size The number of states in the generates
   * @param alph Alphabet of the returned automaton
   * @return A new automaton
   */
  automaton_t randomDFA(unsigned size, std::string alph);
}}}
#endif
  
