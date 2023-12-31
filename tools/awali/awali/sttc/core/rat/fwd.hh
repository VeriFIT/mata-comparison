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

#ifndef AWALI_CORE_RAT_FWD_HH
# define AWALI_CORE_RAT_FWD_HH

# include <memory>

#include <awali/sttc/core/rat/identities.hh>
#include <awali/sttc/weightset/weightset.hh>

namespace awali { namespace sttc
{
  namespace rat
  {

    /// Base class for the AST.
    class exp;
    using exp_t = std::shared_ptr<const exp>;

    // info.hh
    template <typename Context>
    class info;

    // printer.hh.
    template <typename Context>
    class printer;

    /// The possible types of ratexps.
    ///
    /// They also code the order in which they are sorted by
    /// less_than, so that, for instance, polynomials always display
    /// `\e + a`, and never `a + \e`.
    enum class type_t
    {
      zero,
      one,
      atom,
      sum,
      prod,
      ldiv,
      conjunction,
      shuffle,
      star,
      maybe,
      plus,
      transposition,
      lweight,
      rweight,
      complement,
    };

    /// Whether is a constant (`\z` or `\e`).
    inline constexpr bool is_constant(type_t t)
    {
      return (t == type_t::one
              || t == type_t::zero);
    }

    /// Whether star, complement.
    inline constexpr bool is_unary(type_t t)
    {
      return (t == type_t::complement
              || t == type_t::star
              || t == type_t::maybe
              || t == type_t::plus
              || t == type_t::transposition);
    }

    /// Whether one of the variadic types.
    inline constexpr bool is_variadic(type_t t)
    {
      return (t == type_t::conjunction
              || t == type_t::ldiv
              || t == type_t::prod
              || t == type_t::shuffle
              || t == type_t::sum);
    }

    std::ostream&
    operator<<(std::ostream& o, type_t t);

# define DEFINE(Node)                           \
    template <typename Label, typename Weight>  \
    class Node

    DEFINE(node);
    DEFINE(leaf);
    DEFINE(atom);
    DEFINE(inner);

    DEFINE(const_visitor);

# undef DEFINE

    /*-----------.
    | constant.  |
    `-----------*/

    template <type_t Type, typename Label, typename Weight>
    class constant;

    template <typename Label, typename Weight>
    using zero = constant<type_t::zero, Label, Weight>;

    template <typename Label, typename Weight>
    using one = constant<type_t::one, Label, Weight>;

    /*--------.
    | unary.  |
    `--------*/

    template <type_t Type, typename Label, typename Weight>
    class unary;

    template <typename Label, typename Weight>
    using complement = unary<type_t::complement, Label, Weight>;

    template <typename Label, typename Weight>
    using star = unary<type_t::star, Label, Weight>;

    template <typename Label, typename Weight>
    using maybe = unary<type_t::maybe, Label, Weight>;

    template <typename Label, typename Weight>
    using plus = unary<type_t::plus, Label, Weight>;

    template <typename Label, typename Weight>
    using transposition = unary<type_t::transposition, Label, Weight>;

    /*-----------.
    | variadic.  |
    `-----------*/

    template <type_t Type, typename Label, typename Weight>
    class variadic;

    template <typename Label, typename Weight>
    using prod = variadic<type_t::prod, Label, Weight>;

    template <typename Label, typename Weight>
    using conjunction = variadic<type_t::conjunction, Label, Weight>;

    template <typename Label, typename Weight>
    using ldiv = variadic<type_t::ldiv, Label, Weight>;

    template <typename Label, typename Weight>
    using shuffle = variadic<type_t::shuffle, Label, Weight>;

    template <typename Label, typename Weight>
    using sum = variadic<type_t::sum, Label, Weight>;

    /*--------------.
    | weight_node.  |
    `--------------*/

    template <type_t Type, typename Label, typename Weight>
    class weight_node;

    template <typename Label, typename Weight>
    using lweight = weight_node<type_t::lweight, Label, Weight>;

    template <typename Label, typename Weight>
    using rweight = weight_node<type_t::rweight, Label, Weight>;



    template <typename Label, typename Weight>
    using ratexp = std::shared_ptr<const node<Label, Weight>>;


    // ratexpset.hh.
    template <typename Context>
    class ratexpset_impl;

  } // namespace rat

  template <typename Context>
  using ratexpset = variadic_mul_mixin<rat::ratexpset_impl<Context>>;

}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_FWD_HH
