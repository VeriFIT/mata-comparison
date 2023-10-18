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

#ifndef AWALI_ALGOS_READ_HH
# define AWALI_ALGOS_READ_HH

#include <awali/sttc/weightset/polynomialset.hh>

namespace awali { namespace sttc {


  /*-------------.
  | read_label.  |
  `-------------*/

  template <typename Context>
  inline
  auto
  read_label(std::istream& is, const Context& ctx)
    -> label_t_of<Context>
  {
    return ctx.labelset()->conv(is);
  }

  /*------------------.
  | read_polynomial.  |
  `------------------*/

  template <typename Context>
  inline
  auto
  read_polynomial(const Context& ctx, std::istream& is)
    -> typename polynomialset<Context>::value_t
  {
    return polynomialset<Context>(ctx).conv(is);
  }

  /*--------------.
  | read_weight.  |
  `--------------*/

  template <typename Context>
  inline
  auto
  read_weight(const Context& ctx, std::istream& is)
    -> weight_t_of<Context>
  {
    return ctx.weightset()->conv(is);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_READ_HH
