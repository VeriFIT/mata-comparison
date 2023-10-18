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

#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <cassert>

namespace awali { namespace sttc
{
  namespace rat
  {
#define VISIT(Type, Name)                                               \
    template <typename Label, typename Weight>                          \
    inline                                                              \
    void                                                                \
    const_visitor<Label, Weight>::visit(const Type ## _t& Name)

    VISIT(prod, v)
    {
      for (auto t : v)
        t->accept(*this);
    }

    VISIT(conjunction, v)
    {
      for (auto t : v)
        t->accept(*this);
    }

    VISIT(shuffle, v)
    {
      for (auto t : v)
        t->accept(*this);
    }

    VISIT(ldiv, v)
    {
      for (auto t : v)
        t->accept(*this);
    }

    VISIT(sum, v)
    {
      for (auto t : v)
        t->accept(*this);
    }

    VISIT(star, v)
    {
      v.sub()->accept(*this);
    }

    VISIT(maybe, v)
    {
      v.sub()->accept(*this);
    }

    VISIT(plus, v)
    {
      v.sub()->accept(*this);
    }

    VISIT(lweight, v)
    {
      v.sub()->accept(*this);
    }

    VISIT(rweight, v)
    {
      v.sub()->accept(*this);
    }

    VISIT(complement, v)
    {
      v.sub()->accept(*this);
    }

    VISIT(transposition, v)
    {
      v.sub()->accept(*this);
    }

    VISIT(one, )
    {}

    VISIT(zero, )
    {}

    VISIT(atom, )
    {}

#undef VISIT

  } // namespace rat
}}//end of ns awali::stc
