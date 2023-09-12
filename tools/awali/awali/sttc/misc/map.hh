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

#ifndef AWALI_MISC_MAP_HH
# define AWALI_MISC_MAP_HH

# include <map>
#include <awali/utils/hash.hh>

namespace std
{

  /*------------------------.
  | hash(map<Key, Value>).  |
  `------------------------*/

  template <typename Key, typename Value, typename Compare, typename Alloc>
  struct hash<map<Key, Value, Compare, Alloc>>
  {
    size_t operator()(const map<Key, Value, Compare, Alloc>& m) const
    {
      size_t res = 0;
      for (const auto& kv: m)
        {
          hash_combine(res, kv.first);
          hash_combine(res, kv.second);
        }
      return res;
    }
  };
}

namespace awali {
  namespace sttc {
    namespace internal {

      template <typename Key, typename Value, typename Compare, typename Alloc>
      inline
      bool
      has(const std::map<Key, Value, Compare, Alloc>& s, const Key& e)
      {
        return s.find(e) != std::end(s);
      }

      ///adapter of std::less to wrap less_than method of valuesets
      template <typename ValueSet>
      class less : public std::less<typename ValueSet::value_t>
      {
      public:
        using valueset_t = ValueSet;
        using value_t = typename valueset_t::value_t;

        bool operator()(const value_t& lhs, const value_t& rhs) const
        {
          return valueset_t::less_than(lhs, rhs);
        }
      };
    }
  }
}//end of ns awali::stc

#endif // !AWALI_MISC_MAP_HH
