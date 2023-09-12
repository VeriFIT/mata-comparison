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

#ifndef AWALI_MISC_UNORDERED_SET_HH
# define AWALI_MISC_UNORDERED_SET_HH

# include <unordered_set>

#include <awali/utils/hash.hh>

namespace std
{

  /*-------------------------.
  | hash(unordered_set<T>).  |
  `-------------------------*/

  template <typename T>
  struct hash<unordered_set<T>>
  {
    size_t operator()(const unordered_set<T>& ss) const
    {
      std::hash<T> hasher;
      size_t res = 0;
      for (auto s: ss)
        res += hasher(s);
      return res;
    }
  };
}

namespace awali {
  namespace sttc {
    namespace internal {

      /// Whether \a e is member of \a s.
      template <typename Key, typename Hash, typename KeyEqual, typename Alloc>
      bool
      has(const std::unordered_set<Key, Hash, KeyEqual, Alloc>& s, const Key& k)
      {
        return s.find(k) != std::end(s);
      }
    }
  }
}//end of ns awali::stc

#endif // !AWALI_MISC_UNORDERED_SET_HH
