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

#ifndef PRIORITY_HH
#define PRIORITY_HH

#include<utility>

namespace awali{
namespace priority {


/* Concept: priority::value is of any of the type priority::ONE<T>,
 * priority::TWO<T>, priority::THREE<T>, etc.. for some type T (different each
 * time).
 * Since it is more specific, static linkage will preferentially use a function
 * using FIVE<T>, over FOUR<T>, over THREE<T>, etc...
 * Example: assuming existence of functions:
 *  - template<typename P> f(..., ONE<P>) and  
 *  - template<typename P> f(..., TWO<P>)
 * Then a call to f(..., priority::value) will always call the second function.
 * This is useful when multiple algorithms that are doing the same thing are
 * conditional (eg. works only for some weightsets) and we don't wan't to
 * enforce that only one function f(...) exists at the same time.
 * (Cf algos/are-equivalent.hh for a use case)
 *
 * On order to allow usage to be more transparent give types:
 * - priority::TOP<T>
 * - priority::HIGH<T>
 * - priority::MEDIUM<T>
 * - priority::LOW<T>
 * - priority::BOTTOM<T>
 * Hence  if both functions
 * - template<typename P> f(..., MEDIUM<P>) and  
 * - template<typename P> f(..., HIGH<P>)
 * exists, a call to f(...,priority::value) will choose the second one.
 */
  template<typename T>
  struct  _p {
  };

  template<typename T>
  using ONE = _p<T>;


  template<typename T>
  using TWO = _p<_p<T>>;

  template<typename T>
  using THREE = _p<_p<_p<T>>>;

  template<typename T>
  using FOUR = _p<_p<_p<_p<T>>>>;

  template<typename T>
  using FIVE = _p<_p<_p<_p<_p<T>>>>>;

  // These values are for dynamical fallback between priorities
  static constexpr FIVE<void> five;
  static constexpr FOUR<void> four;
  static constexpr THREE<void> three;
  static constexpr TWO<void> two;
  static constexpr ONE<void> one;
  
  template<typename T>
  using TOP = FIVE<T>;

  template<typename T>
  using HIGH = FOUR<T>;

  template<typename T>
  using MEDIUM = THREE<T>;

  template<typename T>
  using LOW = TWO<T>;

  template<typename T>
  using BOTTOM = ONE<T>;

  static constexpr TOP<void> value, top;
  static constexpr HIGH<void> high;
  static constexpr MEDIUM<void> medium;
  static constexpr LOW<void> low;
  static constexpr BOTTOM<void> bottom;

} //end of ns priority
} //end of ns awali


#endif /* PRIORITY_HH */
