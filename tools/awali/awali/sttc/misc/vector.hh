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

#ifndef AWALI_MISC_VECTOR_HH
# define AWALI_MISC_VECTOR_HH

# include <algorithm>
# include <vector>

#include <awali/sttc/misc/attributes.hh>
#include <awali/common/tuple.hh> // make_index_sequence.

namespace awali { namespace sttc {


  namespace internal
  {

    /// Variadic Cartesian product of vectors.
    ///
    /// http://stackoverflow.com/questions/13813007/
    template <typename Fun>
    inline void
    cross(Fun f)
    {
      f();
    }

    template<typename Fun, typename H, typename... Ts>
    inline void
    cross(Fun f,
          std::vector<H> const& h,
          std::vector<Ts> const&... ts)
    {
      for (H const& he: h)
        cross([&](Ts const&... ts) { f(he, ts...); }, ts...);
    }

    template<typename Fun, typename... Ts>
    inline void
    cross_tuple(Fun f,
                const std::tuple<Ts...>& ts)
    {
      cross_tuple_(f, ts, awali::internal::make_index_sequence<sizeof...(Ts)>());
    }

    template<typename Fun, typename... Ts, size_t... I>
    inline void
    cross_tuple_(Fun f,
                 const std::tuple<Ts...>& ts,
                 awali::internal::index_sequence<I...>)
    {
      cross(f, std::get<I>(ts)...);
    }

    /// Return the content of \a cont as a vector.
    template <typename Cont>
    std::vector<typename Cont::value_type>
    to_vector(const Cont& cont)
    {
      return {cont.begin(), cont.end()};
    }

  /// Convenience wrapper around std::find.
  template <typename T, typename Alloc>
  ATTRIBUTE_PURE
  auto
  find(const std::vector<T, Alloc>& s, const T& e)
    -> typename std::vector<T, Alloc>::const_iterator
  {
    return std::find(std::begin(s), std::end(s), e);
  }

  /// Whether \a e is member of \a s.
  template <typename T, typename Alloc>
  ATTRIBUTE_PURE
  bool
  has(const std::vector<T, Alloc>& s, const T& e)
  {
    return find(s, e) != std::end(s);
  }
  }
}}//end of ns awali::stc

#endif // !AWALI_MISC_VECTOR_HH
