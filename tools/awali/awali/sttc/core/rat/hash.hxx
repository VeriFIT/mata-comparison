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

#ifndef AWALI_CORE_RAT_HASH_HXX
# define AWALI_CORE_RAT_HASH_HXX

#include <awali/utils/hash.hh>

namespace awali {
  namespace sttc {
    namespace rat {

# define DEFINE                                 \
      template <typename RatExpSet>             \
      inline                                    \
      auto                                      \
      hash<RatExpSet>

# define VISIT(Type)                            \
      DEFINE::visit(const Type ## _t& v)        \
      -> void


      VISIT(atom) {
        visit_nullary(v);
        std::hash_combine(res_, RatExpSet::labelset_t::hash(v.value()));
      }

      VISIT(lweight)
      {
        visit_weight_node(v);
      }

      VISIT(rweight)
      {
        visit_weight_node(v);
      }

      template <typename RatExpSet>
      inline
      void
      hash<RatExpSet>::combine_type(const node_t& node)
      {
        std::hash_combine(res_, int(node.type()));
      }

      template <typename RatExpSet>
      inline
      void
      hash<RatExpSet>::visit_nullary(const node_t &n)
      {
        combine_type(n);
      }

      template <typename RatExpSet>
      template <type_t Type>
      inline
      void
      hash<RatExpSet>::visit_unary(const unary_t<Type>& n)
      {
        combine_type(n);
        n.sub()->accept(*this);
      }

      template <typename RatExpSet>
      template <type_t Type>
      inline
      void
      hash<RatExpSet>::visit_weight_node(const weight_node_t<Type>& n)
      {
        combine_type(n);
        std::hash_combine(res_, RatExpSet::weightset_t::hash(n.weight()));
        n.sub()->accept(*this);
      }

      template <typename RatExpSet>
      template <type_t Type>
      inline
      void
      hash<RatExpSet>::visit_variadic(const variadic_t<Type>& n)
      {
        combine_type(n);
        for (auto child : n)
          child->accept(*this);
      }
# undef VISIT
# undef DEFINE

    } // namespace rat
  }
}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_HASH_HXX
