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

#ifndef AWALI_MISC_LINKEDHASHMAP_HH
#define AWALI_MISC_LINKEDHASHMAP_HH

#include<unordered_map>
#include<vector>

namespace awali { 
  namespace sttc {
  namespace internal {
    /* This structure provides the minimal amount of services which is required by hopcroft algorithm:
     * - random access in (amortized) constant time
     * - iteration over keys in linear time
     */
    template<typename K, typename V>
    struct linked_hash_map {
    private:
      std::unordered_map<K,V> map_;
      std::vector<K> list_;
    public:
      const std::vector<K>& domain() const {
        return list_;
      }

      V& operator[](const K& k) {
        if(map_.count(k)==0)
          list_.emplace_back(k);
        return map_[k];
      }

      unsigned count(const K& k) const {
        return map_.count(k);
      }
      
      void clear() {
        map_.clear();
        list_.clear();
      }
    };

    template<typename K, typename V>
    struct lhm_values_const_iterator {
    private:
      const linked_hash_map<K,V>& map_;
      typename std::vector<K>::const_iterator current_;
    public:
      lhm_values_const_iterator(linked_hash_map<K,V>& map) :
        map_(map),
        current_(map.domain().begin())
      {}

      lhm_values_const_iterator(linked_hash_map<K,V>& map, typename std::vector<K>::const_iterator& current) :
        map_(map),
        current_(current)
      {}

      const V& operator*() const {
        return map_[*current_];
      }

      lhm_values_const_iterator& operator++() {
        ++current_;
      }
      
      lhm_values_const_iterator& operator==(const lhm_values_const_iterator& it) const {
        return current_==it.current_;
      }
      
      lhm_values_const_iterator& operator!=(const lhm_values_const_iterator& it) const {
        return current_!=it.current_;
      }
    };

    template<typename K, typename V>
    struct lhm_values {
    private:
      const linked_hash_map<K,V>& map_;
      using iterator = lhm_values_const_iterator<K,V>;
    public:
      lhm_values(linked_hash_map<K,V>& map) :
        map_(map)
      {}

      iterator begin() const {
        return iterator(map_);
      }

      iterator end() const {
        return iterator(map_, map_.domain().end());
      }
    };
  }
  }
}
#endif
