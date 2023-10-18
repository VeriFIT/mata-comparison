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

#ifndef AWALI_MISC_SET_HH
# define AWALI_MISC_SET_HH

# include <algorithm> // set_intersection
# include <map>
# include <set>

#include <awali/sttc/misc/attributes.hh>
#include <awali/utils/hash.hh>

namespace std
{

  /*---------------.
  | hash(set<T>).  |
  `---------------*/

  template <typename T>
  struct hash<set<T>>
  {
    size_t operator()(const set<T>& ss) const
    {
      size_t res = 0;
      for (const auto& s: ss)
        hash_combine(res, s);
      return res;
    }
  };
}

namespace awali {
  namespace sttc {
    namespace internal {
      
      /// Whether \a e is member of \a s.
      template <typename T, typename Compare, typename Alloc>
      bool
      has(const std::set<T, Compare, Alloc>& s, const T& e)
        ATTRIBUTE_PURE;

      /// The set of values of a map.
      template <typename Key, typename Value, typename Comp, typename Alloc>
      std::set<typename std::map<Key, Value, Comp, Alloc>::mapped_type>
      image(const std::map<Key, Value, Comp, Alloc>& m);

      /// The intersection of two sets.
      template <typename T, typename Compare, typename Alloc>
      std::set<T, Compare, Alloc>
      intersection(const std::set<T, Compare, Alloc>& set1,
                   const std::set<T, Compare, Alloc>& set2);

      /// The set of all the intersections of the sets in \a pset.
      template <typename T, typename Compare, typename Alloc>
      std::set<std::set<T, Compare, Alloc>>
        intersection_closure(std::set<std::set<T, Compare, Alloc>> pset);

      /// The union of two sets.
      template <typename T, typename Compare, typename Alloc>
      std::set<T, Compare, Alloc>
      get_union(const std::set<T, Compare, Alloc>& set1,
                const std::set<T, Compare, Alloc>& set2);

      /// Print with a separator.  Meant to help debugging.
      template <typename T, typename Compare, typename Alloc>
      std::ostream&
      print(const std::set<T, Compare, Alloc>& set, std::ostream& o);

      /// Whether \a set1 ⊆ \a set2.
      template <typename Container1, typename Container2>
      bool subset(const Container1& set1, const Container2& set2)
        ATTRIBUTE_PURE;
    }
  }
}//end of ns awali::stc

#include <awali/sttc/misc/set.hxx>

#endif // !AWALI_MISC_SET_HH
