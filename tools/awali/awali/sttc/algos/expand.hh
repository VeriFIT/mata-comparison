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

#ifndef AWALI_ALGOS_EXPAND_HH
# define AWALI_ALGOS_EXPAND_HH

#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/core/rat/visitor.hh>

#include <awali/sttc/algos/derivation.hh> // ratexp_polynomialset_t.

namespace awali { namespace sttc {


  namespace rat
  {

    /*-----------------.
    | expand(ratexp).  |
    `-----------------*/

    /// \tparam RatExpSet  relative to the RatExp.
    template <typename RatExpSet>
    class expand_visitor
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using ratexp_t = typename ratexpset_t::value_t;
      using context_t = context_t_of<ratexpset_t>;
      using weightset_t = weightset_t_of<ratexpset_t>;
      using weight_t = typename weightset_t::value_t;

      using polynomialset_t = ratexp_polynomialset_t<ratexpset_t>;
      using polynomial_t = typename polynomialset_t::value_t;

      using super_type = typename RatExpSet::const_visitor;

      constexpr static const char* me() { return "expand"; }

      expand_visitor(const ratexpset_t& rs)
        : rs_(rs)
      {}

      ratexp_t
      operator()(const ratexp_t& v)
      {
        v->accept(*this);
        return ratexp(res_);
      }

      ratexp_t
      ratexp(const polynomial_t p)
      {
        ratexp_t res = rs_.zero();
        for (const auto& m: p)
          res = rs_.add(res, rs_.lmul(m.second, m.first));
         return res;
      }

      /// Syntactic sugar: recursive call to this visitor.
      polynomial_t expand(const ratexp_t& e)
      {
        e->accept(*this);
        return res_;
      }

      AWALI_RAT_VISIT(zero,)
      {
        res_ = ps_.zero();
      }

      AWALI_RAT_VISIT(one,)
      {
        res_ = polynomial_t{{rs_.one(), ws_.one()}};
      }

      AWALI_RAT_VISIT(atom, v)
      {
        res_ = polynomial_t{{rs_.atom(v.value()), ws_.one()}};
      }

      AWALI_RAT_VISIT(sum, v)
      {
        polynomial_t res = ps_.zero();
        for (auto c: v)
          res = ps_.add(res, expand(c));
        res_ = std::move(res);
      }

      AWALI_RAT_VISIT(conjunction, v)
      {
        auto res = expand(v.head());
        for (auto c: v.tail())
          {
            polynomial_t sum = ps_.zero();
            for (const auto& l: res)
              for (const auto& r: expand(c))
                ps_.add_here(sum,
                             rs_.conjunction(l.first, r.first),
                             ws_.mul(l.second, r.second));
            res = sum;
          }
        res_ = std::move(res);
      }

      AWALI_RAT_UNSUPPORTED(complement)
      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(shuffle)
      AWALI_RAT_UNSUPPORTED(transposition)

      AWALI_RAT_VISIT(prod, v)
      {
        polynomial_t res = ps_.one();
        for (auto c: v)
          res = ps_.mul(res, expand(c));
        res_ = std::move(res);
      }

      AWALI_RAT_VISIT(star, v)
      {
        // Recurse, but make it a star.
        v.sub()->accept(*this);
        res_ = polynomial_t{{rs_.star(ratexp(res_)), ws_.one()}};
      }

      AWALI_RAT_VISIT(maybe, v)
      {
        v.sub()->accept(*this);
        res_ = ps_.add(res_, ps_.one());
      }

      AWALI_RAT_VISIT(plus, v)
      {
        // Recurse, but make it a star.
        v.sub()->accept(*this);
        res_ = polynomial_t{{rs_.plus(ratexp(res_)), ws_.one()}};
      }

      AWALI_RAT_VISIT(lweight, v)
      {
        v.sub()->accept(*this);
        res_ = ps_.lmul(v.weight(), std::move(res_));
      }

      AWALI_RAT_VISIT(rweight, v)
      {
        v.sub()->accept(*this);
        res_ = ps_.rmul(std::move(res_), v.weight());
      }

    private:
      ratexpset_t rs_;
      /// Shorthand to the weightset.
      weightset_t ws_ = *rs_.weightset();
      /// Polynomialset of ratexps.
      polynomialset_t ps_ = make_ratexp_polynomialset(rs_);
      /// The result.
      polynomial_t res_;
    };

  } // rat::

  /// Expanding a typed ratexp shared_ptr.
  template <typename RatExpSet>
  typename RatExpSet::value_t
  expand(const RatExpSet& rs, const typename RatExpSet::value_t& e)
  {
    rat::expand_visitor<RatExpSet> expand{rs};
    return expand(e);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_EXPAND_HH
