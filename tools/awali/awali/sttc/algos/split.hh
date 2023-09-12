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

#ifndef AWALI_ALGOS_SPLIT_HH
# define AWALI_ALGOS_SPLIT_HH

#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/weightset/polynomialset.hh>

namespace awali { namespace sttc {


  namespace rat
  {
    // FIXME: this is a general feature which is useful elsewhere.
    // E.g., expand.

    /// Type of PolynomialSet of RatExps from the RatExpSet type.
    template <typename RatExpSet>
    using ratexp_polynomialset_t
      = polynomialset<context<RatExpSet,
                              weightset_t_of<RatExpSet>>>;

    /// Type of polynomials of ratexps from the RatExpSet type.
    template <typename RatExpSet>
    using ratexp_polynomial_t
      = typename ratexp_polynomialset_t<RatExpSet>::value_t;

    /// From a RatExpSet to its polynomialset.
    template <typename RatExpSet>
    inline
    ratexp_polynomialset_t<RatExpSet>
    make_ratexp_polynomialset(const RatExpSet& rs)
    {
      using context_t = context<RatExpSet,
                                weightset_t_of<RatExpSet>>;
      return context_t{rs, *rs.weightset()};
    }
  }


  /*----------------.
  | split(ratexp).  |
  `----------------*/

  namespace rat
  {
    template <typename RatExpSet>
    class split_visitor
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using labelset_t = labelset_t_of<context_t>;
      using label_t = label_t_of<context_t>;
      using ratexp_t = typename ratexpset_t::value_t;
      using weightset_t = weightset_t_of<ratexpset_t>;
      using weight_t = typename weightset_t::value_t;

      using polynomialset_t = ratexp_polynomialset_t<ratexpset_t>;
      using polynomial_t = typename polynomialset_t::value_t;

      using super_type = typename ratexpset_t::const_visitor;

      constexpr static const char* me() { return "split"; }

      split_visitor(const ratexpset_t& rs)
        : rs_(rs)
      {}

      /// Break a ratexp into a polynomial.
      polynomial_t operator()(const ratexp_t& v)
      {
        return split(v);
      }

      /// Easy recursion.
      polynomial_t split(const ratexp_t& v)
      {
        v->accept(*this);
        return std::move(res_);
      }

      AWALI_RAT_VISIT(zero,)
      {
        res_ = ps_.zero();
      }

      AWALI_RAT_VISIT(one,)
      {
        res_ = polynomial_t{{rs_.one(), ws_.one()}};
      }

      AWALI_RAT_VISIT(atom, e)
      {
        res_ = polynomial_t{{rs_.atom(e.value()), ws_.one()}};
      }

      AWALI_RAT_VISIT(sum, e)
      {
        polynomial_t res = ps_.zero();
        for (const auto& v: e)
          {
            v->accept(*this);
            ps_.add_here(res, res_);
          }
        res_ = std::move(res);
      }

      /// The split-product of \a l with \a r.
      ///
      /// Returns split(l) x split(r).
      /// FIXME: This is inefficient, we split the lhs way too often.
      polynomial_t product(const ratexp_t& l, const ratexp_t& r)
      {
        // B(l).
        polynomial_t l_split = split(l);
        // constant-term(B(l)).
        weight_t l_split_const = ps_.get_weight(l_split, rs_.one());
        // proper(B(l)).
        ps_.del_weight(l_split, rs_.one());

        // res = proper(B(l)).r + constant-term(B(l))B(r).
        return ps_.add(ps_.rmul_letter(l_split, r),
                       ps_.lmul(l_split_const, split(r)));
      }

      /// The split-product of \a l with \a r.
      ///
      /// Returns l x split(r).
      polynomial_t product(const polynomial_t& l, const ratexp_t& r)
      {
        polynomial_t res;
        for (const auto& m: l)
          ps_.add_here(res, ps_.lmul(m.second, product(m.first, r)));
        return res;
      }

      /// Handle an n-ary product.
      AWALI_RAT_VISIT(prod, e)
      {
        auto res = product(e[0], e[1]);
        for (unsigned i = 2, n = e.size(); i < n; ++i)
          res = product(res, e[i]);
        res_ = std::move(res);
      }

      /// The split-product of \a l with \a r.
      ///
      /// Returns split(l) x split(r).
      /// FIXME: This is inefficient, we split the lhs way too often.
      polynomial_t conjunction(const ratexp_t& l, const ratexp_t& r)
      {
        // B(l).
        polynomial_t l_split = split(l);
        // constant-term(B(l)).
        weight_t l_split_const = ps_.get_weight(l_split, rs_.one());
        // proper(B(l)).
        ps_.del_weight(l_split, rs_.one());

        // res = proper(B(l))&r.
        polynomial_t res;
        for (const auto& e: l_split)
          ps_.add_here(res, rs_.conjunction(e.first, r), e.second);
        // res += constant-term(B(l))B(r)
        ps_.add_here(res,
                       ps_.lmul(l_split_const, split(r)));
        return res;
      }

      /// The split-product of \a l with \a r.
      ///
      /// Returns l x split(r).
      polynomial_t conjunction(const polynomial_t& l, const ratexp_t& r)
      {
        polynomial_t res;
        for (const auto& m: l)
          ps_.add_here(res, ps_.lmul(m.second, conjunction(m.first, r)));
        return res;
      }

      /// Handle an n-ary conjunction.
      AWALI_RAT_VISIT(conjunction, e)
      {
        auto res = conjunction(e[0], e[1]);
        for (unsigned i = 2, n = e.size(); i < n; ++i)
          res = conjunction(res, e[i]);
        res_ = std::move(res);
      }

      AWALI_RAT_UNSUPPORTED(complement)
      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(shuffle)
      AWALI_RAT_UNSUPPORTED(transposition)

      AWALI_RAT_VISIT(star, e)
      {
        res_ = polynomial_t{{e.shared_from_this(), ws_.one()}};
      }

      AWALI_RAT_VISIT(lweight, e)
      {
        e.sub()->accept(*this);
        res_ = ps_.lmul(e.weight(), res_);
      }

      AWALI_RAT_VISIT(rweight, e)
      {
        e.sub()->accept(*this);
        res_ = ps_.rmul(res_, e.weight());
      }

    private:
      ratexpset_t rs_;
      /// Shorthand to the weightset.
      weightset_t ws_ = *rs_.weightset();
      polynomialset_t ps_ = make_ratexp_polynomialset(rs_);
      /// The result.
      polynomial_t res_;
    };
  }//end of ns rat

  /// Split a ratexp.
  template <typename RatExpSet>
  inline
  rat::ratexp_polynomial_t<RatExpSet>
  split(const RatExpSet& rs, const typename RatExpSet::ratexp_t& e)
  {
    rat::split_visitor<RatExpSet> split{rs};
    return split(e);
  }

  /// Split a polynomial of ratexps.
  template <typename RatExpSet>
  inline
  rat::ratexp_polynomial_t<RatExpSet>
  split(const RatExpSet& rs, const rat::ratexp_polynomial_t<RatExpSet>& p)
  {
    auto ps = rat::make_ratexp_polynomialset(rs);
    using polynomial_t = rat::ratexp_polynomial_t<RatExpSet>;
    rat::split_visitor<RatExpSet> split{rs};
    polynomial_t res;
    for (const auto& m: p)
      res = ps.add(res, ps.lmul(m.second, split(m.first)));
    return res;
  }


}}//end of ns awali::stc

#endif // !AWALI_ALGOS_SPLIT_HH
