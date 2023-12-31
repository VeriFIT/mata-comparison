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

#ifndef UTILS_ARITH_HH
#define UTILS_ARITH_HH

#include <awali/sttc/misc/attributes.hh>

namespace awali { namespace utils {
  // Greatest common divisor.
  
  ATTRIBUTE_CONST
  inline
  unsigned int gcd(unsigned int a, unsigned int b)
  {
    while (b)
    {
      unsigned int t = a;
      a = b;
      b = t % b;
    }
    return a;
  }

  // Lowest common multiple
  ATTRIBUTE_CONST
  inline
  unsigned int lcm(unsigned int a, unsigned int b)
  {
    return a / gcd(a, b) * b;
  }

  ATTRIBUTE_CONST
  inline
  int min(int a, int b) {
    return (a<b?a:b);
  }

  ATTRIBUTE_CONST
  inline
  int max(int a, int b) {
    return (a>b?a:b);
  }

}}//end of ns awali::utils



#endif
