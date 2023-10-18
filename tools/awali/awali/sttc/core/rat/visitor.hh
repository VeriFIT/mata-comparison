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

#ifndef AWALI_CORE_RAT_VISITOR_HH
# define AWALI_CORE_RAT_VISITOR_HH

#include <awali/sttc/core/rat/fwd.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc
{
  namespace rat
  {

    template <typename Label, typename Weight>
    class const_visitor
    {
    public:
      using label_t = Label;
      using weight_t = Weight;
      using node_t = node<label_t, weight_t>;
      using inner_t = inner<label_t, weight_t>;
      using leaf_t = leaf<label_t, weight_t>;
      template <type_t Type>
      using unary_t = rat::unary<Type, label_t, weight_t>;
      template <type_t Type>
      using variadic_t = rat::variadic<Type, label_t, weight_t>;
      template <type_t Type>
      using weight_node_t = rat::weight_node<Type, label_t, weight_t>;

      virtual ~const_visitor() = default;

# define DEFINE(Type)                                   \
      using Type ## _t = Type<label_t, weight_t>;       \
      virtual void visit(const Type ## _t& v)

      DEFINE(atom);
      DEFINE(complement);
      DEFINE(conjunction);
      DEFINE(ldiv);
      DEFINE(lweight);
      DEFINE(one);
      DEFINE(prod);
      DEFINE(rweight);
      DEFINE(shuffle);
      DEFINE(star);
      DEFINE(maybe);
      DEFINE(plus);
      DEFINE(sum);
      DEFINE(transposition);
      DEFINE(zero);
# undef DEFINE
    };

  } // namespace rat
}}//end of ns awali::stc

# define AWALI_RAT_VISIT(Type, Val)                      \
  using Type ## _t = typename super_type::Type ## _t;   \
  virtual void visit(const Type ## _t& Val) override

# define AWALI_RAT_UNSUPPORTED(Type)                     \
  AWALI_RAT_VISIT(Type,)                                 \
  {                                                     \
    awali::sttc::raise(me(), ": " #Type " is not supported");        \
  }

#include <awali/sttc/core/rat/visitor.hxx>

#endif // !AWALI_CORE_RAT_VISITOR_HH
