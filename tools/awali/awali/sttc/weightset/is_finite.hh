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

#ifndef AWALI_WEIGHTSETS_IS_FINITE_HH
#define AWALI_WEIGHTSETS_IS_FINITE_HH

#include <awali/common/priority.hh>

namespace awali {
  namespace sttc {
    namespace internal {
                                                                              template<typename WS, typename P>
      constexpr auto is_finite(const WS&, priority::TWO<P>) -> typename WS::finite_t {
        return WS::finite;
      }
                                                                   
      template<typename WS, typename P>
      constexpr bool is_finite(const WS&, priority::ONE<P>) {
        return false;
      }

      template<typename WS, typename P>
      constexpr auto is_locally_finite(const WS&, priority::TWO<P>) -> typename WS::locally_finite_t {
        return WS::locally_finite;
      }
                                                                   
      template<typename WS, typename P>
      constexpr bool is_locally_finite(const WS& ws, priority::ONE<P>) {
        return is_finite(ws);
      }
                                                                            }
  
    template<typename WS>
    constexpr bool is_finite(const WS& ws) {
      return internal::is_finite(ws,priority::value);
    }

    template<typename WS>
    constexpr bool is_locally_finite(const WS& ws) {
      return internal::is_locally_finite(ws,priority::value);
    }
    
  }  
}

#endif
