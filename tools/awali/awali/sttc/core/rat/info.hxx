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

#ifndef AWALI_CORE_RAT_INFO_HXX
# define AWALI_CORE_RAT_INFO_HXX

namespace awali { namespace sttc
{
  namespace rat
  {

# define DEFINE                                 \
    template <typename RatExpSet>               \
    inline                                      \
    auto                                        \
    info<RatExpSet>

    DEFINE::operator()(const node_t& v)
      -> void
    {
      atom = 0;
      complement = 0;
      conjunction = 0;
      lweight = 0;
      ldiv = 0;
      one = 0;
      prod = 0;
      rweight = 0;
      shuffle = 0;
      star = 0;
      maybe = 0;
      plus = 0;
      sum = 0;
      transposition = 0;
      zero = 0;
      v.accept(*this);
    }

# define VISIT(Type)                           \
    DEFINE::visit(const Type ## _t& v)         \
    -> void

    VISIT(sum)
    {
      ++sum;
      for (const auto& c: v)
        c->accept(*this);
    }

    VISIT(prod)
    {
      ++prod;
      for (const auto& c: v)
        c->accept(*this);
    }

    VISIT(conjunction)
    {
      ++conjunction;
      for (const auto& c: v)
        c->accept(*this);
    }

    VISIT(shuffle)
    {
      ++shuffle;
      for (const auto& c: v)
        c->accept(*this);
    }


    VISIT(ldiv)
    {
      ++ldiv;
      for (const auto& c: v)
        c->accept(*this);
    }

    VISIT(star)
    {
      ++star;
      v.sub()->accept(*this);
    }

    VISIT(maybe)
    {
      ++maybe;
      v.sub()->accept(*this);
    }

    VISIT(plus)
    {
      ++plus;
      v.sub()->accept(*this);
    }

    VISIT(complement)
    {
      ++complement;
      v.sub()->accept(*this);
    }

    VISIT(transposition)
    {
      ++transposition;
      v.sub()->accept(*this);
    }

    VISIT(zero)
    {
      (void) v;
      ++zero;
    }

    VISIT(one)
    {
      (void) v;
      ++one;
    }

    VISIT(atom)
    {
      (void) v;
      ++atom;
    }

    VISIT(lweight)
    {
      ++lweight;
      v.sub()->accept(*this);
    }

    VISIT(rweight)
    {
      ++rweight;
      v.sub()->accept(*this);
    }

# undef VISIT
# undef DEFINE

  } // namespace rat
}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_INFO_HXX
