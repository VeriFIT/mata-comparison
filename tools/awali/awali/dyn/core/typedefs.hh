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


#ifndef DYN_CORE_TYPEDEFS_HH
#define DYN_CORE_TYPEDEFS_HH

#include<awali/dyn/core/any.hh>

namespace awali { namespace dyn {

  /** Type representing automata states; currently simply identifiers of type
   * unsigned, but this might change in the future.
   */
  using state_t = unsigned;

  /** Type representing automata transitions; currently simply identifiers of
   * type unsigned, but this might change in the future.
   */
  using transition_t = unsigned;

  /** 
  Type for (transition) labels; it is an alias to {@link any_t} since its precise
  type depends on the weightset of manipulated {@link automaton_t} or {@link ratexp_t}.

  @example (Getting back a label knowing its type)
  \code
    automaton_t aut = load("a1");
    // In a1, labels are chars so we may cast it directly:
    char label = (char) aut->label_of(aut->transitions()[0]) ; 
    // NB: the previous line takes the label of the first transition in an
    // arbitrary order; its meaning is dubious.
  \endcode
  */
  using label_t = any_t;
  
  /** 
  Type for (transition) weights; it is an alias to {@link any_t} since the
  its precise type depends on the weightset of 
  manipulated {@link automaton_t} or {@link ratexp_t}.
  @example (Getting back a weight knowing its type)
  \code
    automaton_t aut = load("binary");
    // In `binary`, weights are integers so we may cast it as such:
    int weight = (int) eval(aut, "101010");
  \endcode
  */
  using weight_t = any_t;
  
  /**
  Type for words; it is an alias to {@link any_t} since the precise type
  depends on the context (most of the time, it is a std::string).
  */
  using word_t = any_t;
}}
#endif
