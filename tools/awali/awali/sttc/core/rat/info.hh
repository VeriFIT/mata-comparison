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

#ifndef AWALI_CORE_RAT_INFO_HH
# define AWALI_CORE_RAT_INFO_HH

# include <iostream>
#include <awali/sttc/core/rat/visitor.hh>

namespace awali { namespace sttc
{
  namespace rat
  {

    template <typename RatExpSet>
    class info
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using node_t = typename ratexpset_t::node_t;
      using super_type = typename ratexpset_t::const_visitor;

      /// For each node type, count its number of occurrences.
      void operator()(const node_t& v);

# define DEFINE(Type)                                           \
    public:                                                     \
      using Type ## _t = typename super_type::Type ## _t;       \
      size_t Type = 0;                                          \
                                                                \
    private:                                                    \
      void visit(const Type ## _t& v);

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

#include <awali/sttc/core/rat/info.hxx>

#endif // !AWALI_CORE_RAT_INFO_HH
