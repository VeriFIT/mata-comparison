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

#ifndef AWALI_MISC_SUB_TUPLE_HH
#define AWALI_MISC_SUB_TUPLE_HH

#include <awali/common/tuple.hh>

namespace awali {
  namespace sttc {
    namespace internal {
      /*  sub_tuple<I...>::get(t)
       *  builds the tuple formed with the components at indices I...
       *  of t
       *
       *  The type is obtained from the type T of the tuple by
       *  sub_tuple<I...>::template type<T>
       */

      template<size_t Head, size_t... Tail>
      struct sub_tuple {

        template<typename Tuple>
        using type = typename std::cons_tuple<typename std::tuple_element<Head,Tuple>::type, typename sub_tuple<Tail...>::template type<Tuple>>::type;
        template<typename Tuple>
        static
        auto get(const Tuple& t) -> type<Tuple> {
          return std::tuple_cat(std::make_tuple(std::get<Head>(t)), sub_tuple<Tail...>::get(t));
        }
      };

      template<size_t I>
      struct sub_tuple<I> {

        template<typename Tuple>
        using type = std::tuple<typename std::tuple_element<I,Tuple>::type>;
        template<typename Tuple>
        static
        auto
        get(const Tuple& t) -> type<Tuple> {
          return std::make_tuple(std::get<I>(t));
        }
      };

      /*  tuple_tail (t)
       *  returns the tail of the tuple t
       *
       *  The type is obtained from the type T of the tuple by
       *  tail_tuple<T>::type
       */

      template<typename T> struct tail_tuple;
      template<typename Head, typename... Tail>
      struct tail_tuple<std::tuple<Head, Tail...>> {
        using type = std::tuple<Tail...>;
      };

      template<size_t I, size_t N>
      struct comp_tail_tuple {
        template<typename Tuple, typename... Values>
        static
        auto
        get(const Tuple& t, Values & ... values)
          -> typename tail_tuple<Tuple>::type
        {
          return comp_tail_tuple<I+1, N>::get(t, values..., std::get<I>(t));
        }
                                        };

      template<size_t N>
      struct comp_tail_tuple<N, N> {
        template<typename Tuple, typename... Values>
        static
        auto
        get(const Tuple&, Values & ... values)
          -> typename tail_tuple<Tuple>::type
        {
          return std::make_tuple(values...);
        }
                                        };

      template<typename Tuple>
      auto tuple_tail(const Tuple& t )
        -> typename tail_tuple<Tuple>::type
      {
        return comp_tail_tuple<1,std::tuple_size<Tuple>::value>::get(t);
      }

      /*  rem_in_tuple<I>::get(t)
       *  builds the tuple formed with the components all components
       *  of t except component at index I
       *
       *  The type is obtained from the type T of the tuple by
       *  rem_in_tuple<I>::template type<T>
       */
                                      
      template<size_t N>
      struct rem_in_tuple{
        template<typename Tuple>
        using type = typename std::cons_tuple<
          typename std::tuple_element<0,Tuple>::type,
          typename rem_in_tuple<N-1>::template type<typename tail_tuple<Tuple>::type>>::type;
        
        template<typename Tuple>
        static
        auto get(const Tuple& t) -> type<Tuple>
        {
          return std::tuple_cat(std::make_tuple(std::get<0>(t)), rem_in_tuple<N-1>::get(tuple_tail(t)));
        }
      };
      
      template<>
      struct rem_in_tuple<0u>{
        template<typename Tuple>
        using type = typename tail_tuple<Tuple>::type;
        
        template<typename Tuple>
        static
        auto get(const Tuple& t) -> type<Tuple> {
          return tuple_tail(t);
        }
      };

      /*  replace_in_tuple<I,T>::get(t,x)
       *  builds the tuple formed with the components
       *  of t except, with the component at index I replace by x of type T
       *
       */

      template<size_t N, typename T>
      struct replace_in_tuple{
        template<typename Tuple>
        using type = typename std::cons_tuple<
          typename std::tuple_element<0,Tuple>::type,
          typename replace_in_tuple<N-1,T>::template type<typename tail_tuple<Tuple>::type>>::type;

        template<typename Tuple>
        static
        auto get(const Tuple& t, const T& x) -> type<Tuple>
        {
          return std::tuple_cat(std::make_tuple(std::get<0>(t)), replace_in_tuple<N-1,T>::get(tuple_tail(t),x));
        }
      };

      template<typename T>
      struct replace_in_tuple<0u,T>{
        template<typename Tuple>
        using type = typename std::cons_tuple<T, typename tail_tuple<Tuple>::type>::type;

        template<typename Tuple>
        static
        auto get(const Tuple& t, const T& x) -> type<Tuple> {
          return std::tuple_cat(std::make_tuple(x), tuple_tail(t));
        }
      };


    }
  }
}//end of ns awali::stc

#endif
