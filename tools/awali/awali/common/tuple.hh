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

#ifndef AWALI_TUPLE_HH
# define AWALI_TUPLE_HH

# include <iostream>
# include <tuple>

# include <awali/utils/hash.hh>

namespace awali
{

  // These definitions come in handy every time we define variadic tuples.
  namespace internal
  {

    /*-----------------.
    | index_sequence.  |
    `-----------------*/

    // See "Pretty-print std::tuple"
    // <http://stackoverflow.com/questions/6245735>.

    // See O(log N) implementation of integer sequence
    // <http://stackoverflow.com/questions/17424477>

    template<std::size_t...> struct index_sequence
    { using type = index_sequence; };

    template<class S1, class S2> struct concat;

    template<std::size_t... I1, std::size_t... I2>
    struct concat<index_sequence<I1...>, index_sequence<I2...>>
      : index_sequence<I1..., (sizeof...(I1)+I2)...>{};

    template<class S1, class S2>
    using Concat = typename concat<S1, S2>::type;

    template<std::size_t N> struct make_index_sequence;
    template<std::size_t N> using GenSeq =
      typename make_index_sequence<N>::type;

    template<std::size_t N>
    struct make_index_sequence : Concat<GenSeq<N/2>, GenSeq<N - N/2>>{};

    template<> struct make_index_sequence<0> : index_sequence<>{};
    template<> struct make_index_sequence<1> : index_sequence<0>{};

    template<class S1, class S2> struct seqconcat;

    template<std::size_t... I1, std::size_t... I2>
    struct seqconcat<index_sequence<I1...>, index_sequence<I2...>>
      : index_sequence<I1..., I2...>{};

    template<typename S> struct reverseseq;
    template<size_t I1, size_t... I2>
    struct reverseseq<index_sequence<I1, I2...>> {
      using type = typename seqconcat<typename reverseseq<index_sequence<I2...>>::type,
                             index_sequence<I1>>::type;
    };
    template<size_t I1>
    struct reverseseq<index_sequence<I1>> {
      using type = index_sequence<I1>;
    };
    
    template<std::size_t off, class S2> struct int_range;

    template<std::size_t off, std::size_t... I>
    struct int_range<off, index_sequence<I...>>
      : index_sequence<I + off...>{};

    template<std::size_t S, std::size_t L>
    struct make_index_range
      : int_range<S, typename make_index_sequence<L>::type>{};

    template<std::size_t S>
    struct make_index_range<S, 0> : index_sequence<>{};
    template<std::size_t S>
    struct make_index_range<S, -1U> : index_sequence<>{};

    template<typename S1, typename S2>
    struct concat_index_sequence;

    template<std::size_t... I1, std::size_t... I2>
    struct concat_index_sequence<index_sequence<I1...>, index_sequence<I2...>>
      : index_sequence<I1..., I2...>{};

    template <typename S1, typename S2>
    using concat_sequence = typename concat_index_sequence<S1, S2>::type;

    // There is a bug in clang making this one useless...
    // The index sequence generated is always <0>
    // Bug report:
    // http://llvm.org/bugs/show_bug.cgi?id=14858
    //template<class... T>
    //using index_sequence_for = make_index_sequence<sizeof...(T)>;


    template <typename Fun, typename... Ts>
    inline void
    for_(const std::tuple<Ts...>& ts, Fun f)
    {
      for_(f, ts, make_index_sequence<sizeof...(Ts)>());
    }

    template <typename Fun, typename... Ts, size_t... I>
    inline void
    for_(Fun f,
         const std::tuple<Ts...>& ts,
         index_sequence<I...>)
    {
      using swallow = int[];
      (void) swallow{ (f(std::get<I>(ts)), 0)... };
    }

    /// Map a function on a tuple, return tuple of the results.
    template <typename Fun, typename... Ts>
    inline auto
    map(const std::tuple<Ts...>& ts, Fun f)
      -> decltype(map_tuple_(f, ts, make_index_sequence<sizeof...(Ts)>()))
    {
      return map_tuple_(f, ts, make_index_sequence<sizeof...(Ts)>());
    }

    template <typename Fun, typename... Ts, size_t... I>
    inline auto
    map_tuple_(Fun f,
               const std::tuple<Ts...>& ts,
               index_sequence<I...>)
      -> decltype(map_variadic_(f, std::get<I>(ts)...))
    {
      return map_variadic_(f, std::get<I>(ts)...);
    }

    template<typename Fun>
    inline auto
    map_variadic_(Fun)
      -> decltype(std::make_tuple())
    {
      return std::make_tuple();
    }

    template <typename Fun, typename T, typename... Ts>
    inline auto
    map_variadic_(Fun f, T t, Ts&&... ts)
      -> decltype(std::tuple_cat(std::make_tuple(f(t)), map_variadic_(f, ts...)))
    {
      // Enforce evaluation order from left to right.
      auto r = f(t);
      return std::tuple_cat(std::make_tuple(r), map_variadic_(f, ts...));
    }



    //#if 0

    /*-----------------.
    | make_inv_tuple.  |
    `-----------------*/

    // This does not work, I don't understand why.  If you know, please
    // let me (AD) know.
    inline auto
    make_inv_tuple()
      -> std::tuple<>
    {
      return {};
    }

    template <typename T, typename... Ts>
    inline auto
    make_inv_tuple(T t, Ts&&... ts)
      -> decltype(std::tuple_cat(make_inv_tuple(std::forward<Ts>(ts)...), std::make_tuple(t)));

    template <typename T, typename... Ts>
    inline auto
    make_inv_tuple(T t, Ts&&... ts)
      -> decltype(std::tuple_cat(make_inv_tuple(std::forward<Ts>(ts)...), std::make_tuple(t)))
    {
      return std::tuple_cat(make_inv_tuple(std::forward<Ts>(ts)...), std::make_tuple(t));
    }

    //#endif


    /*----------------.
    | reverse_tuple.  |
    `----------------*/

    template <typename... Ts>
    inline auto
    reverse_tuple(const std::tuple<Ts...>& t)
      -> decltype(reverse_tuple(t, make_index_sequence<sizeof...(Ts)>()))
    {
      return reverse_tuple(t, make_index_sequence<sizeof...(Ts)>());
    }

    template <typename... Ts, std::size_t... I>
    inline auto
    reverse_tuple(const std::tuple<Ts...>& t, index_sequence<I...>)
      -> decltype(std::make_tuple(std::get<sizeof...(Ts) - 1 - I>(t)...))
    {
      return std::make_tuple(std::get<sizeof...(Ts) - 1 - I>(t)...);
    }

    template <typename... Ts>
    inline auto
    make_inv_tuple(Ts&&... ts)
      -> decltype(reverse_tuple(std::make_tuple(std::forward<Ts>(ts)...)))
    {
      return reverse_tuple(std::make_tuple(std::forward<Ts>(ts)...));
    }


    /*------------------------.
    | print(tuple, ostream).  |
    `------------------------*/

    template<class Tuple, std::size_t N>
    struct tuple_printer
    {
      static void print(const Tuple& t, std::ostream& o)
      {
        tuple_printer<Tuple, N-1>::print(t, o);
        o << ", " << std::get<N-1>(t);
      }
    };

    template<class Tuple>
    struct tuple_printer<Tuple, 1>
    {
      static void print(const Tuple& t, std::ostream& o)
      {
        o << std::get<0>(t);
      }
    };

    template <typename... Args>
    std::ostream& print(const std::tuple<Args...>& args, std::ostream& o)
    {
      o << '(';
      tuple_printer<decltype(args), sizeof...(Args)>::print(args, o);
      return o << ')';
    }

    // Compile-time logic
    // See:
    // http://stillmoreperfect.blogspot.fr/2010/03/template-metaprogramming-compile-time.html

    // Test if (c) then T1 else T2
    template<bool c, class T1, class T2>
    struct if_c { typedef T1 type; };

    template<class T1, class T2>
    struct if_c<false, T1, T2> { typedef T2 type; };

    template<class C, class T1, class T2>
    struct if_ : if_c<C::value, T1, T2> {};

    // Test if (c) then F1 else F2 and get the value
    template<bool c, class F1, class F2>
    struct eval_if_c : if_c<c, F1, F2>::type {};

    template<class C, class F1, class F2>
    struct eval_if : if_<C, F1, F2>::type {};

    // And condition on several classes
    template<class... F>
    struct and_;

    template<class F1, class... F>
    struct and_<F1, F...> : eval_if<F1, and_<F...>, std::false_type>::type {};

    template<class F1>
    struct and_<F1> : eval_if<F1, std::true_type, std::false_type>::type {};

    template<>
    struct and_<> : std::true_type::type {};

    // Or condition on several classes
    template<class... F>
    struct or_;

    template<class F1, class... F>
    struct or_<F1, F...> : eval_if<F1, std::true_type, or_<F...>>::type { };

    template<class F1>
    struct or_<F1> : eval_if<F1, std::true_type, std::false_type>::type { };

    template<>
    struct or_<> : std::true_type::type {};
  }

  /// Static evaluation of the 'or' of the template parameters
  template<bool... B>
  constexpr bool any_()
  {
    return internal::or_<std::integral_constant<bool, B>...>::value;
  }

  // Static evaluation of the 'and' of the template parameters
  template<bool... B>
  constexpr bool all_()
  {
    return internal::and_<std::integral_constant<bool, B>...>::value;
  }


}

namespace std
{

  /*-------------------------.
  | std::hash(tuple<T...>).  |
  `-------------------------*/

  template <typename... Elements>
  struct hash<std::tuple<Elements...>>
  {
    using value_t = std::tuple<Elements...>;
    using indices_t = awali::internal::make_index_sequence<sizeof...(Elements)>;

    std::size_t operator()(const value_t& v) const
    {
      return hash_(v, indices_t{});
    }

  private:
    template <std::size_t... I>
    static std::size_t
    hash_(const value_t& v, awali::internal::index_sequence<I...>)
    {
      std::size_t res = 0;
      using swallow = int[];
      (void) swallow
        {
          (std::hash_combine(res, std::get<I>(v)), 0)...
        };
      return res;
    }
  };

  template<typename T1, typename T2> struct cons_tuple;
  template<typename T1, typename... T2>
  struct cons_tuple<T1,std::tuple<T2...>> {
    using type = std::tuple<T1,T2...>;
  };

  template<typename T1, typename T2> struct concat_tuple;
  template<typename... T1, typename... T2>
  struct concat_tuple<std::tuple<T1...>,std::tuple<T2...>> {
    using type = std::tuple<T1...,T2...>;
  };

  template<typename T, unsigned N>
  struct cst_tuple {
    using type = typename cons_tuple<T, typename cst_tuple<T, N-1>::type>::type;
  };

  template<typename T>
  struct cst_tuple<T,1u> {
    using type = std::tuple<T>;
  };

  
}

namespace std {
  template <typename... Args>
  std::ostream& operator<<(std::ostream& o, const std::tuple<Args...>& args)
  {
    return awali::internal::print(args, o);
  }
}

#endif // !AWALI_MISC_TUPLE_HH
