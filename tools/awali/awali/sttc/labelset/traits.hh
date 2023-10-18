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

#ifndef AWALI_LABELSET_TRAITS_HH
#define AWALI_LABELSET_TRAITS_HH

#include <awali/sttc/labelset/letterset.hh>
#include <awali/sttc/labelset/wordset.hh>
#include <awali/sttc/labelset/nullableset.hh>
#include <awali/sttc/labelset/oneset.hh>
#include <awali/sttc/labelset/tupleset.hh>
#include <awali/sttc/ctx/context.hh>


namespace awali {
  namespace sttc {

    /** @brief trait that computes the related types of a labelset
     */
    template<typename L>
    struct labelset_trait {
      using nullable_t = L;
      using not_nullable_t = L;
      using letterset_t = L;
      using wordset_t = L;
      using ratlabelset_t = L;

      static const letterset_t& get_letterset(const L& ls) {
        return ls;
      }

      static const wordset_t& get_wordset(const L& ls) {
        return ls;
      }

      static const nullable_t& get_nullableset(const L& ls) {
        return ls;
      }

      static const not_nullable_t& get_not_nullableset(const L& ls) {
        return ls;
      }

      static const ratlabelset_t& get_ratlabelset(const L& ls) {
        return ls;
      }
    };
    
    ///specialisation of labelset_trait for letterset
    template<typename T>
    struct labelset_trait<letterset<T>> {
      using nullable_t = nullableset<letterset<T>>;
      using not_nullable_t = letterset<T>;
      using letterset_t = letterset<T>;
      using wordset_t = wordset<T>;
      using ratlabelset_t = letterset<T>;

      static const letterset_t& get_letterset(const letterset<T>& ls) {
        return ls;
      }

      static wordset_t get_wordset(const letterset<T>& ls) {
        return {ls.genset()};
      }

      static nullable_t get_nullableset(const letterset<T>& ls) {
        return nullable_t(ls);
      }

      static const not_nullable_t& get_not_nullableset(const letterset<T>& ls) {
        return ls;
      }

      static const ratlabelset_t& get_ratlabelset(const letterset<T>& ls) {
        return ls;
      }

    };

    ///specialisation of labelset_trait for wordset
    template<typename T>
    struct labelset_trait<wordset<T>> {
      using nullable_t = wordset<T>;
      using not_nullable_t = wordset<T>;
      using letterset_t = letterset<T>;
      using wordset_t = wordset<T>;
      using ratlabelset_t = wordset<T>;

      static letterset_t get_letterset(const wordset<T>& ls) {
        return {ls.genset()};
      }

      static const wordset_t& get_wordset(const wordset<T>& ls) {
        return ls;
      }

      static const nullable_t& get_nullableset(const wordset<T>& ls) {
        return ls;
      }

      static const not_nullable_t& get_not_nullableset(const wordset<T>& ls) {
        return ls;
      }

      static const ratlabelset_t& get_ratlabelset(const wordset<T>& ls) {
        return ls;
      }

    };

    ///specialisation of labelset_trait for nullableset
    template<typename T>
    struct labelset_trait<nullableset<T>> {
      using nullable_t = typename labelset_trait<T>::nullable_t;
      using not_nullable_t = typename labelset_trait<T>::not_nullable_t;
      using letterset_t = typename labelset_trait<T>::letterset_t;
      using wordset_t = typename labelset_trait<T>::wordset_t;
      using ratlabelset_t = typename labelset_trait<T>::ratlabelset_t;

      static letterset_t get_letterset(const nullableset<T>& ls) {
        auto ls_=*ls.labelset();
        return labelset_trait<decltype(ls_)>::get_letterset(ls_);
      }

      static wordset_t get_wordset(const nullableset<T>& ls) {
        auto ls_=*ls.labelset();
        return labelset_trait<decltype(ls_)>::get_wordset(ls_);
      }

      static nullable_t get_nullableset(const nullableset<T>& ls) {
        auto ls_=*ls.labelset();
        return labelset_trait<decltype(ls_)>::get_nullableset(ls_);
      }

      static not_nullable_t get_not_nullableset(const nullableset<T>& ls) {
        auto ls_=*ls.labelset();
        return labelset_trait<decltype(ls_)>::get_not_nullableset(ls_);
      }

      static ratlabelset_t get_ratlabelset(const nullableset<T>& ls) {
        auto ls_=*ls.labelset();
        return labelset_trait<decltype(ls_)>::get_ratlabelset(ls_);
      }

    };
    
    ///specialisation of labelset_trait for tupleset
    template<typename... Ts>
    struct labelset_trait<tupleset<Ts...>> {
      using self_t = tupleset<Ts...>;
      using not_nullable_t = self_t;
      using nullable_t 
        = tupleset<typename labelset_trait<Ts>::nullable_t...>;
      using wordset_t
	= tupleset<typename labelset_trait<Ts>::wordset_t...>;
      using ratlabelset_t = self_t;

      static nullable_t get_nullableset(const self_t& ls) {
        return get_nullableset(ls, ls.indices);
      }
 
     template<std::size_t... I>
     static nullable_t get_nullableset(const self_t& ls,
                                       awali::internal::index_sequence<I...>)
     {
        return nullable_t{(labelset_trait<typename std::tuple_element<I, std::tuple<Ts...>>::type>::get_nullableset(ls.template set<I>()))...};
     }
      
      static wordset_t get_wordset(const self_t& ls) {
        return get_wordset(ls, ls.indices);
      }

      template<std::size_t... I>
      static wordset_t get_wordset(const self_t& ls,
                                       awali::internal::index_sequence<I...>)
     {
        return nullable_t{(labelset_trait<typename std::tuple_element<I, std::tuple<Ts...>>::type>::get_wordset(ls.template set<I>()))...};
     }

      static const ratlabelset_t& get_ratlabelset(const self_t& ls) {
        return ls;
      }

      static not_nullable_t const& get_not_nullableset(const self_t& ls)
      {
        return ls;
      }


    };

    ///specialisation of labelset_trait for oneset
    template<>
    struct labelset_trait<oneset> {
      using self_t = oneset;
      using nullable_t = self_t;
      using not_nullable_t = self_t;
      using ratlabelset_t = self_t;
      
      static ratlabelset_t get_ratlabelset(const oneset& os) {
        return os;
      }
      static not_nullable_t const& get_not_nullableset(const self_t& ls)
      {
        return ls;
      }
      
    };
    
    template<typename L>
    auto
    get_letterset(const L& labelset) -> typename labelset_trait<L>::letterset_t
    {
      return labelset_trait<L>::get_letterset(labelset);
    }

    template<typename L>
    auto
    get_wordset(const L& labelset) -> typename labelset_trait<L>::wordset_t
    {
      return labelset_trait<L>::get_wordset(labelset);
    }
                                                                 
    template<typename L>
    auto
    get_ratlabelset(const L& labelset) -> typename labelset_trait<L>::ratlabelset_t
    {
      return labelset_trait<L>::get_ratlabelset(labelset);
    }
                                                                     
    template<typename L>
    auto
    get_nullableset(const L& labelset) -> typename labelset_trait<L>::nullable_t
    {
      return labelset_trait<L>::get_nullableset(labelset);
    }

    template<typename L>
    auto
    get_not_nullableset(const L& labelset) -> typename labelset_trait<L>::not_nullable_t
    {
      return labelset_trait<L>::get_not_nullableset(labelset);
    }
                                                                         
    template<typename LS, typename WS>
    auto
    get_letterset_context(const context<LS,WS>& ctx) -> context<typename labelset_trait<LS>::letterset_t, WS>
    {
      return {get_letterset(*ctx.labelset()), *ctx.weightset()};
    }
    
    template<typename LS, typename WS>
    auto
    get_wordset_context(const context<LS,WS>& ctx) -> context<typename labelset_trait<LS>::wordset_t, WS>
    {
      return {get_wordset(*ctx.labelset()), *ctx.weightset()};
    }

  

    template<typename Context>
    using ratexp_context_of 
      = context< typename labelset_trait<typename Context::labelset_t>::ratlabelset_t,
                 typename Context::weightset_t >;


    template<typename Context>
    using ratexpset_of = ratexpset<ratexp_context_of<Context>>;

    template<typename Context>
    using nullable_of =
      context<typename labelset_trait<typename Context::labelset_t>::nullable_t,
              typename Context::weightset_t>;

    template<typename Context>
    using not_nullable_of =
      context<typename labelset_trait<typename Context::labelset_t>::not_nullable_t,
              typename Context::weightset_t>;

    template<typename Context>
    ratexp_context_of<Context>
    get_rat_context(const Context& ctx)
    {
      return {get_ratlabelset(*ctx.labelset()), *ctx.weightset()};
    }

    template<typename Context>
    auto
    get_nullable_context(const Context& ctx) -> nullable_of<Context>
    {
      return {get_nullableset(*ctx.labelset()), *ctx.weightset()};
    }

    template<typename Context>
    auto
    get_not_nullable_context(const Context& ctx) -> not_nullable_of<Context>
    {
      return {get_not_nullableset(*ctx.labelset()), *ctx.weightset()};
    }

  }
}//end of ns awali::stc

#endif //!AWALI_LABELSET_TRAITS_HH
