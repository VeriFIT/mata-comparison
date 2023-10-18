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

#ifndef AWALI_MISC_UNORDERED_MAP_HH
# define AWALI_MISC_UNORDERED_MAP_HH

# include <unordered_map>

namespace awali {
  namespace sttc {
    namespace internal {
    
      /// Sum of bucket sizes.
      template <typename Key, typename T,
                typename Hash, typename KeyEqual, typename Alloc>
      inline
      size_t
      collision_count(const std::unordered_map<Key, T, Hash, KeyEqual, Alloc>& m)
      {
        size_t res = 0;
        for (size_t i = 0; i < m.bucket_count(); ++i)
          res += m.bucket_size(i);
        return res;
      }

      /// Whether \a e is member of \a m.
      template <typename Key, typename T,
                typename Hash, typename KeyEqual, typename Alloc>
      inline
      bool
      has(const std::unordered_map<Key, T, Hash, KeyEqual, Alloc>& m,
          const Key& k)
      {
        return m.find(k) != std::end(m);
      }

    }
  }
}//end of ns awali::stc

#endif // !AWALI_MISC_UNORDERED_MAP_HH
