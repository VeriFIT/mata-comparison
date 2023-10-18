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

#ifndef AWALI_WEIGHTSET_WEIGHTSET_HH
# define AWALI_WEIGHTSET_WEIGHTSET_HH

# include <iostream>

// It is much simpler and saner in C++ to put types and functions on
// these types in the same namespace.  Since "using q =
// internal::variadic_mul_mixin<q_impl>" would just create an alias of
// q, its original namespace, internal::, would still be the namespace
// used in ADL.
//
// This is really troublesome to implement free-functions such as join.
//
// Therefore, although this wrapper should be hidden as a internal::, it
// will remain in sttc::, where join and the like will find it.

namespace awali { namespace sttc {

  /// Provide a variadic mul on top of a binary mul(), and one().
  template <typename WeightSet>
  struct variadic_mul_mixin : WeightSet
  {
    using super = WeightSet;
    using typename super::value_t;

    // Inherit the constructors.
    using super::super;

    // Provide a variadic mul.
    using super::mul;

    template <typename... Ts>
    value_t mul(const Ts&... ts) const
    {
      value_t res = this->one();
      // FIXME: Remove once GCC is fixed.
      using swallow = int[];
      (void) swallow
        {
          ((res = super::mul(res, ts)), 0)...
        };
      return res;
    }
  };

}}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_WEIGHTSET_HH
