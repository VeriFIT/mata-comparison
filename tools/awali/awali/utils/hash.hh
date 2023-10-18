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

#ifndef AWALI_MISC_HASH_HH
# define AWALI_MISC_HASH_HH
# include <utility>
# include <functional> // std::equal_to

namespace std
{

  // http://stackoverflow.com/questions/2590677
  template <class T>
  inline void hash_combine(std::size_t& seed, const T& v)
  {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  }

}

namespace awali { namespace utils {

  /// This is useful to make hashes with labels or weights as keys
  /// without using non-default constructors; to be used along with
  /// utils::hash.
  template <typename ValueSet>
  class equal_to : public std::equal_to<typename ValueSet::value_t>
  {
  public:
    using valueset_t = ValueSet;
    using value_t = typename valueset_t::value_t;

    bool operator()(const value_t& v1, const value_t& v2) const
    {
      return valueset_t::equals(v1, v2);
    }
  };

  /// This is useful to make hashes with labels or weights as keys
  /// without using non-default constructors; to be used along with
  /// utils::equal_to.
  template <typename ValueSet>
  class hash
  {
  public:
    using valueset_t = ValueSet;
    using value_t = typename valueset_t::value_t;

    std::size_t operator()(const value_t& v) const
    {
      return valueset_t::hash(v);
    }

    // Not used, but needed to satisfy the specification.  See for example
    // http://www.cplusplus.com/reference/functional/hash/ .
    using result_type = std::size_t;
    using argument_type = value_t;
  };

  // Following the naming convention of Boost.
  template <class T>
  inline std::size_t hash_value(const T& v)
  {
    std::hash<T> hasher;
    return hasher(v);
  }

  // Following the naming convention of Boost.
  template<typename F, typename S>
  inline std::size_t hash_value(const std::pair<F,S>& p)
  {
    std::size_t res = 0;
    std::hash_combine(res, hash_value(p.first));
    std::hash_combine(res, hash_value(p.second));
    return res;
  }
}}//end of ns awali::utils 

#endif // !AWALI_MISC_HASH_HH
