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

#ifndef AWALI_WEIGHTSET_FWD_HH
# define AWALI_WEIGHTSET_FWD_HH

namespace awali {
  namespace sttc {

  // polynomialset.hh.
  template <class Context>
  class polynomialset;

}}//end of ns awali::stc

#define AWALI_WEIGHTS_BINARY(Lhs, Rhs, Res)     \
  /** The join of two weightsets. */            \
  inline                                        \
  Res join(const Lhs&, const Rhs&)              \
  {                                             \
    return {};                                  \
  }

#define AWALI_WEIGHTS_BINARY_TEMPL(Tmplt, Lhs, Rhs, Res)      \
  /** The join of two templated weightsets. */               \
  template Tmplt                                             \
  inline                                                     \
  Res join(const Lhs&, const Rhs&)                           \
  {                                                          \
  return {};                                                 \
  }

#endif // !AWALI_WEIGHTSET_FWD_HH
