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

#ifndef AWALI_CORE_RAT_COPY_HH
# define AWALI_CORE_RAT_COPY_HH

# include <iostream>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/misc/cast.hh>

namespace awali { namespace sttc
{
  namespace rat
  {

    template <typename InRatExpSet, typename OutRatExpSet = InRatExpSet>
    class copier
      : public InRatExpSet::const_visitor
    {
    public:
      using in_ratexpset_t = InRatExpSet;
      using out_ratexpset_t = OutRatExpSet;
      using in_value_t = typename in_ratexpset_t::value_t;
      using out_value_t = typename out_ratexpset_t::value_t;
      using super_type = typename in_ratexpset_t::const_visitor;
      using node_t = typename super_type::node_t;
      using inner_t = typename super_type::inner_t;
      template <type_t Type>
      using unary_t = typename super_type::template unary_t<Type>;
      template <type_t Type>
      using variadic_t = typename super_type::template variadic_t<Type>;
      using leaf_t = typename super_type::leaf_t;

      copier(const in_ratexpset_t& in_rs, const out_ratexpset_t& out_rs)
        : in_rs_(in_rs)
        , out_rs_(out_rs)
      {}

      /// Entry point: print \a v.
      out_value_t
      operator()(const in_value_t& v)
      {
        return copy(v);
      }

    private:
      /// Factor the copy of n-ary operations.
      template <exp::type_t Type>
      void
      copy_unary(const unary_t<Type>& v)
      {
        using out_unary_t = typename out_ratexpset_t::template unary_t<Type>;
        res_ = std::make_shared<out_unary_t>(copy(v.sub()));
      }

      /// Factor the copy of n-ary operations.
      template <exp::type_t Type>
      void
      copy_variadic(const variadic_t<Type>& v)
      {
        using out_variadic_t = typename out_ratexpset_t::template variadic_t<Type>;
        typename out_ratexpset_t::ratexps_t sub;
        for (auto s: v)
          sub.emplace_back(copy(s));
        res_ = std::make_shared<out_variadic_t>(sub);
      }

      out_value_t
      copy(const in_value_t& v)
      {
        v->accept(*this);
        return res_;
      }

# define DEFINE(Type)                                           \
      using Type ## _t = typename super_type::Type ## _t;       \
      virtual void visit(const Type ## _t& v)

      DEFINE(conjunction)  { copy_variadic(v); }
      DEFINE(ldiv)         { copy_variadic(v); }
      DEFINE(prod)         { copy_variadic(v); }
      DEFINE(shuffle)      { copy_variadic(v); }
      DEFINE(sum)          { copy_variadic(v); }

      DEFINE(complement)   { copy_unary(v); }
      DEFINE(star)         { copy_unary(v); }
      DEFINE(maybe)        { copy_unary(v); }
      DEFINE(plus)         { copy_unary(v); }
      DEFINE(transposition){ copy_unary(v); }

      DEFINE(one)
      {
        (void) v;
        res_ = out_rs_.one();
      }

      DEFINE(zero)
      {
        (void) v;
        res_ = out_rs_.zero();
      }

      DEFINE(atom)
      {
        res_ = out_rs_.atom(out_rs_.labelset()->conv(*in_rs_.labelset(),
                                                     v.value()));
      }

      DEFINE(lweight)
      {
        res_ = out_rs_.lmul(out_rs_.weightset()->conv(*in_rs_.weightset(),
                                                      v.weight()),
                            copy(v.sub()));
      }

      DEFINE(rweight)
      {
        res_ = out_rs_.rmul(copy(v.sub()),
                            out_rs_.weightset()->conv(*in_rs_.weightset(),
                                                      v.weight()));
      }


# undef DEFINE
      /// ratexpset to decode the input value.
      const in_ratexpset_t& in_rs_;
      /// ratexpset to build the output value.
      const out_ratexpset_t& out_rs_;
      /// Output value, under construction.
      out_value_t res_;
    };

    template <typename InRatExpSet, typename OutRatExpSet = InRatExpSet>
    typename OutRatExpSet::value_t
    copy(const InRatExpSet& in_rs, const OutRatExpSet& out_rs,
         const typename InRatExpSet::value_t& v)
    {
      copier<InRatExpSet, OutRatExpSet> copy(in_rs, out_rs);
      return copy(v);
    }

  } // namespace rat
}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_COPY_HH
