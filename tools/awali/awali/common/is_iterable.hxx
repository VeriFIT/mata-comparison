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

#ifndef AWALI_IS_ITERABLE_HXX
#define AWALI_IS_ITERABLE_HXX

#include<type_traits>
#include<awali/common/priority.hh>
#include<iterator>

namespace awali {


namespace internal {

  template <typename T, typename P>
  auto is_iterable_aux1(priority::TOP<P>)
  -> decltype (
      /* test begin and and := */
      std::begin(std::declval<T&>()) != std::end(std::declval<T&>()),
      void(), // Handle evil operator ,
      ++std::declval<decltype(std::begin(std::declval<T&>()))&>(), // operator ++
      void(*std::begin(std::declval<T&>())), // operator*
      std::true_type{}
    );

  template <typename T, typename P>
  std::false_type is_iterable_aux1(priority::BOTTOM<P>);

}// end of namespace awali::internal

/** Trait to test whether type `T` can be iterated over. */
template <typename T>
using is_iterable = decltype(internal::is_iterable_aux1<T>(priority::value));


namespace internal {
  template <bool... T>
  struct std_all {};
  
  template <>
  struct std_all<> : std::true_type {};

  template <bool... Tail>
  struct std_all<false, Tail...> : std::false_type {};
  
  template <bool... Tail>
  struct std_all<true, Tail...> : std_all<Tail...> {};

// template <typename T, typename X, typename P>
// auto is_iterable_aux2(priority::HIGH<P>)
// -> typename std::enable_if<
//      std_all<is_iterable<T>::value,
//              std::is_assignable<X&,
//                                 decltype(*std::begin(std::declval<T&>()))
//                                >::value
//      >::value,
//      std::true_type
//    >::type;

template <typename T, typename X, typename P>
auto is_iterable_aux2(priority::TOP<P>)
-> typename std::enable_if<
     std_all<is_iterable<T>::value,
             std::is_convertible<
                                decltype(*std::begin(std::declval<T&>())), X
                               >::value
     >::value,
     std::true_type
   >::type;

template <typename T, typename X, typename P>
std::false_type is_iterable_aux2(priority::BOTTOM<P>);


}// end of namespace awali::internal


/** Trait to test whether type `T` can be iterated over and assign its values
 * to type X. 
 * This simply does not work...
 * */
template <typename T, typename X>
using is_iterable_with = decltype(internal::is_iterable_aux2<T,X>(priority::value));


}// end of namespace awali

#endif
