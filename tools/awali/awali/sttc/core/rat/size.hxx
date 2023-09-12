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

#ifndef AWALI_CORE_RAT_SIZE_HXX
# define AWALI_CORE_RAT_SIZE_HXX

namespace awali { namespace sttc
{
  namespace rat
  {

# define DEFINE                                 \
    template <typename RatExpSet>               \
    inline                                      \
    auto                                        \
    size<RatExpSet>

# define VISIT(Type)                          \
    DEFINE::visit(const Type ## _t& v)        \
      -> void

    VISIT(zero)
    {
      (void) v;
      ++ size_;
    }

    VISIT(one)
    {
      (void) v;
      ++ size_;
    }

    VISIT(atom)
    {
      (void) v;
      // FIXME: use the label size instead of 1.  Labels don't have a size yet.
      ++ size_;
    }

    VISIT(lweight)
    {
      ++ size_;
      v.sub()->accept(*this);
    }

    VISIT(rweight)
    {
      ++ size_;
      v.sub()->accept(*this);
    }

    template <typename RatExpSet>
    template <type_t Type>
    inline
    void
    size<RatExpSet>::visit_unary(const unary_t<Type>& v)
    {
      ++ size_;
      v.sub()->accept(*this);
    }

    template <typename RatExpSet>
    template <type_t Type>
    inline
    void
    size<RatExpSet>::visit_variadic(const variadic_t<Type>& n)
    {
      /* An n-ary node contributes n-1 unit (plus the sum of its
         children sizes) to the ratexp size. */
      -- size_;
      for (auto child : n)
        {
          ++ size_;
          child->accept(*this);
        }
    }


# undef VISIT
# undef DEFINE

  } // namespace rat
}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_SIZE_HXX
