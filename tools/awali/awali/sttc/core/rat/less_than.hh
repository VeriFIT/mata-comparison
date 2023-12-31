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

#ifndef AWALI_CORE_RAT_LESS_THAN_HH
# define AWALI_CORE_RAT_LESS_THAN_HH

#include <awali/sttc/misc/cast.hh>

#include <awali/sttc/core/rat/fwd.hh>
#include <awali/sttc/core/rat/size.hh>

namespace awali { namespace sttc
{

  namespace rat
  {

    template <class RatExpSet>
    class less_than
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using labelset_t = labelset_t_of<context_t>;
      using weightset_t = weightset_t_of<context_t>;
      using weight_t = weight_t_of<context_t>;
      using ratexp_t = typename context_t::ratexp_t;
      using super_type = typename ratexpset_t::const_visitor;
      using node_t = typename super_type::node_t;
      using inner_t = typename super_type::inner_t;
      template <rat::exp::type_t Type>
      using unary_t = typename super_type::template unary_t<Type>;
      template <rat::exp::type_t Type>
      using variadic_t = typename super_type::template variadic_t<Type>;
      template <rat::exp::type_t Type>
      using weight_node_t = typename super_type::template weight_node_t<Type>;

      /// Whether \a lhs < \a rhs.
      bool
      operator()(ratexp_t lhs, ratexp_t rhs)
      {
        rat::size<RatExpSet> sizer;
        size_t lhss = sizer(lhs);
        size_t rhss = sizer(rhs);

        if (lhss < rhss)
          return true;
        else if (lhss > rhss)
          return false;
        else if (lhs->type() < rhs->type())
          return true;
        else if (lhs->type() > rhs->type())
          return false;
        else
          {
            rhs_ = rhs;
            lhs->accept(*this);
            return res_;
          }
      }

      /*-----------------------------------------------------------.
      | Unary visit functions than bounces to their binary peers.  |
      `-----------------------------------------------------------*/

#define VISIT(Type)                                                     \
      using Type ## _t = typename super_type::Type ## _t;               \
      virtual void                                                      \
      visit(const Type ## _t& lhs) override                             \
      {                                                                 \
        res_ = less_than_(lhs, *down_pointer_cast<const Type ## _t>(rhs_)); \
      }

      VISIT(atom);
      VISIT(complement);
      VISIT(conjunction);
      VISIT(ldiv);
      VISIT(lweight);
      VISIT(one);
      VISIT(prod);
      VISIT(rweight);
      VISIT(shuffle);
      VISIT(star);
      VISIT(maybe);
      VISIT(plus);
      VISIT(sum);
      VISIT(transposition);
      VISIT(zero);
#undef VISIT

      /*-------------------------------------------------------.
      | Binary functions that compare two nodes of same type.  |
      `-------------------------------------------------------*/

      bool less_than_(const zero_t&, const zero_t&)
      {
        return false;
      }

      bool less_than_(const one_t&, const one_t&)
      {
        return false;
      }

      bool less_than_(const atom_t& lhs, const atom_t& rhs)
      {
        return labelset_t::less_than(lhs.value(), rhs.value());
      }

      template <rat::exp::type_t Type>
      bool less_than_(const variadic_t<Type>& lhs, const variadic_t<Type>& rhs)
      {
        auto ls = lhs.size();
        auto rs = rhs.size();
        if (ls < rs)
          return true;
        else if (rs < ls)
          return false;
        else
          for (size_t i = 0; i < ls; ++i)
            if (ratexpset_t::less_than(lhs[i], rhs[i]))
              return true;
            else if (ratexpset_t::less_than(rhs[i], lhs[i]))
              return false;
        return false;
      }

      template <rat::exp::type_t Type>
      bool less_than_(const unary_t<Type>& lhs, const unary_t<Type>& rhs)
      {
        return ratexpset_t::less_than(lhs.sub(), rhs.sub());
      }

      template <rat::exp::type_t Type>
      bool less_than_(const weight_node_t<Type>& lhs, const weight_node_t<Type>& rhs)
      {
        // Lexicographic comparison on sub-expression, and then weight.
        if (ratexpset_t::less_than(lhs.sub(), rhs.sub()))
          return true;
        else if (ratexpset_t::less_than(rhs.sub(), lhs.sub()))
          return false;
        else
          return weightset_t::less_than(lhs.weight(), rhs.weight());
      }

   private:
      ratexp_t rhs_;
      bool res_;
    };
  }

  template <class RatExpSet>
  typename RatExpSet::ratexp_t
  less_than(const RatExpSet& rs, const typename RatExpSet::ratexp_t& v)
  {
    return rs.less_than(v);
  }

}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_LESS_THAN_HH
