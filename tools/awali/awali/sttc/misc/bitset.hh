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

#ifndef AWALI_MISC_BITSET_HH
#define AWALI_MISC_BITSET_HH

#include<bitset>

namespace awali {
  namespace sttc {
    namespace internal {
      template<size_t N>
      struct bitset_iterator {
      private:
        const std::bitset<N>& bs_;
        size_t current_ = 0u;
      public:
        bitset_iterator<N>(const std::bitset<N>& bs) : bs_(bs) {}

        bool has_next() {
          while(current_ < N && !bs_.test(current_))
            ++current_;
          return (current_ < N);
        }

        size_t next() {
          while(current_ < N && !bs_.test(current_))
            ++current_;
          return current_++;
        }

      };

      template<size_t N>
      auto get_iterator(const std::bitset<N>& bs) -> bitset_iterator<N> {
        return {bs};
      }

    }
  }
}//end of ns awali::stc
#endif
