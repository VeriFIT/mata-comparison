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

#ifndef AWALI_CORE_RAT_HASH_HH
# define AWALI_CORE_RAT_HASH_HH

#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/misc/cast.hh>

namespace awali {
  namespace sttc {
    namespace rat {
      template <typename RatExpSet>
      class hash : public RatExpSet::const_visitor {
      public:
        using ratexpset_t = RatExpSet;
        using context_t = context_t_of<ratexpset_t>;
        using weight_t = typename context_t::weightset_t::value_t;
        using super_type = typename ratexpset_t::const_visitor;
        using node_t = typename super_type::node_t;
        using inner_t = typename super_type::inner_t;
        template <type_t Type>
        using variadic_t = typename super_type::template variadic_t<Type>;
        template <type_t Type>
        using unary_t = typename super_type::template unary_t<Type>;
        template <type_t Type>
        using weight_node_t = typename super_type::template weight_node_t<Type>;
        using leaf_t = typename super_type::leaf_t;


        /// Entry point: return the hash of \a v.
        size_t operator()(const node_t& v) {
          res_ = 0;
          v.accept(*this);
          return res_;
        }

        /// Entry point: return the hash of \a v.
        size_t operator()(const std::shared_ptr<const node_t>& v) {
          return operator()(*v);
        }

      private:

# define DEFINE(Type)                                           \
        using Type ## _t = typename super_type::Type ## _t;     \
        virtual void visit(const Type ## _t& v)

        DEFINE(atom);
        DEFINE(complement)   { visit_unary(v); }
        DEFINE(conjunction)  { visit_variadic(v); }
        DEFINE(ldiv)         { visit_variadic(v); }
        DEFINE(lweight);
        DEFINE(one)          { visit_nullary(v); }
        DEFINE(prod)         { visit_variadic(v); }
        DEFINE(rweight);
        DEFINE(shuffle)      { visit_variadic(v); }
        DEFINE(star)         { visit_unary(v); }
        DEFINE(sum)          { visit_variadic(v); }
        DEFINE(transposition){ visit_unary(v); }
        DEFINE(zero)         { visit_nullary(v); }

# undef DEFINE

        /// Update res_ by hashing the node type; this is needed for any node.
        void combine_type(const node_t& node);

        /// Traverse a nullary node (atom, `\z`, `\e`).
        void visit_nullary(const node_t& v);

        /// Traverse a unary node (`*`, `{c}`).
        template <rat::exp::type_t Type>
        void visit_unary(const unary_t<Type>& v);

        /// Traverse an n-ary node (`+`, concatenation, `&`, `:`).
        template <rat::exp::type_t Type>
        void visit_variadic(const variadic_t<Type>& v);

        /// Traverse a weight node (lweight, rweight).
        template <rat::exp::type_t Type>
        void visit_weight_node(const weight_node_t<Type>& v);

        size_t res_;
      };
    } // namespace rat
  }
}//end of ns awali::stc

#include <awali/sttc/core/rat/hash.hxx>

#endif // !AWALI_CORE_RAT_HASH_HH
