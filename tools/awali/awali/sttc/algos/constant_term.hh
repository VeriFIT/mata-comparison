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

#ifndef AWALI_ALGOS_CONSTANT_TERM_HH
# define AWALI_ALGOS_CONSTANT_TERM_HH

# include <set>

#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/core/rat/visitor.hh>

namespace awali { namespace sttc {


  namespace rat
  {

    /*------------------------.
    | constant_term(ratexp).  |
    `------------------------*/

    /// \tparam RatExpSet  the ratexp set type.
    template <typename RatExpSet>
    class constant_term_visitor
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using ratexp_t = typename ratexpset_t::ratexp_t;
      using weight_t = weight_t_of<ratexpset_t>;
      using weightset_t = weightset_t_of<ratexpset_t>;

      using super_type = typename ratexpset_t::const_visitor;

      constexpr static const char* me() { return "constant_term"; }

      constant_term_visitor(const ratexpset_t& rs)
        : ws_(*rs.weightset())
      {}

      weight_t
      operator()(const ratexp_t& v)
      {
        v->accept(*this);
        return std::move(res_);
      }

      /// Easy recursion.
      weight_t constant_term(const ratexp_t& v)
      {
        v->accept(*this);
        return std::move(res_);
      }

      AWALI_RAT_VISIT(zero,)
      {
        res_ = ws_.zero();
      }

      AWALI_RAT_VISIT(one,)
      {
        res_ = ws_.one();
      }

      AWALI_RAT_VISIT(atom,)
      {
        res_ = ws_.zero();
      }

      AWALI_RAT_VISIT(sum, v)
      {
        weight_t res = ws_.zero();
        for (auto c: v)
          res = ws_.add(res, constant_term(c));
        res_ = std::move(res);
      }

      AWALI_RAT_VISIT(prod, v)
      {
        weight_t res = ws_.one();
        for (auto c: v)
          res = ws_.mul(res, constant_term(c));
        res_ = std::move(res);
      }

      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(transposition)

      AWALI_RAT_VISIT(conjunction, v)
      {
        // FIXME: Code duplication with prod_t.
        weight_t res = ws_.one();
        for (auto c: v)
          res = ws_.mul(res, constant_term(c));
        res_ = std::move(res);
      }

      AWALI_RAT_VISIT(shuffle, v)
      {
        // FIXME: Code duplication with prod_t.
        weight_t res = ws_.one();
        for (auto c: v)
          res = ws_.mul(res, constant_term(c));
        res_ = std::move(res);
      }

      AWALI_RAT_VISIT(star, v)
      {
        res_ = ws_.star(constant_term(v.sub()));
      }

      AWALI_RAT_VISIT(lweight, v)
      {
        res_ = ws_.mul(v.weight(), constant_term(v.sub()));
      }

      AWALI_RAT_VISIT(rweight, v)
      {
        res_ = ws_.mul(constant_term(v.sub()), v.weight());
      }

      AWALI_RAT_VISIT(complement, v)
      {
        res_
          = ws_.is_zero(constant_term(v.sub()))
          ? ws_.one()
          : ws_.zero();
      }

    private:
      //ratexpset_t ws_;
      weightset_t ws_;
      weight_t res_;
    };

  } // rat::

  template <typename RatExpSet>
  weight_t_of<RatExpSet>
  constant_term(const RatExpSet& rs, const typename RatExpSet::ratexp_t& e)
  {
    rat::constant_term_visitor<RatExpSet> constant_term{rs};
    return constant_term(e);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_CONSTANT_TERM_HH
