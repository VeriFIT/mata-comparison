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

#ifndef AWALI_CORE_RAT_TRANSPOSE_HH
# define AWALI_CORE_RAT_TRANSPOSE_HH

#include <awali/sttc/core/rat/fwd.hh>

namespace awali { namespace sttc
{

  namespace internal
  {

    /*--------------------.
    | transpose(ratexp).  |
    `--------------------*/

    /// \tparam RatExpSet  the ratexp set.
    template <class RatExpSet>
    class transposer
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using weightset_t = weightset_t_of<context_t>;
      using weight_t = weight_t_of<context_t>;
      using ratexp_t = typename context_t::ratexp_t;
      using super_type = typename ratexpset_t::const_visitor;

      transposer(const ratexpset_t& rs)
        : rs_{rs}
        , res_{}
      {}

      ratexp_t
      operator()(const ratexp_t& e)
      {
        e->accept(*this);
        return std::move(res_);
      }

      ratexp_t
      transpose(const ratexp_t& e)
      {
        ratexp_t res;
        std::swap(res_, res);
        e->accept(*this);
        std::swap(res_, res);
        return res;
      }

      AWALI_RAT_VISIT(zero,)
      {
        res_ = rs_.zero();
      }

      AWALI_RAT_VISIT(one,)
      {
        res_ = rs_.one();
      }

      AWALI_RAT_VISIT(atom, e)
      {
        res_ = rs_.atom(rs_.labelset()->transpose(e.value()));
      }

      AWALI_RAT_VISIT(sum, e)
      {
        res_ = rs_.zero();
        for (auto v: e)
          res_ = rs_.add(res_, transpose(v));
      }

      AWALI_RAT_VISIT(conjunction, e)
      {
        res_ = transpose(e.head());
        for (auto v: e.tail())
          res_ = rs_.conjunction(res_, transpose(v));
      }

      AWALI_RAT_VISIT(shuffle, e)
      {
        // FIXME: that should be easy to factor.
        res_= transpose(e.head());
        for (auto v: e.tail())
          res_ = rs_.shuffle(res_, transpose(v));
      }

      AWALI_RAT_VISIT(prod, e)
      {
        res_ = rs_.one();
        for (auto v: e)
          res_ = rs_.mul(transpose(v), res_);
      }

      AWALI_RAT_VISIT(star, e)
      {
        res_ = rs_.star(transpose(e.sub()));
      }

      AWALI_RAT_VISIT(complement, e)
      {
        res_ = rs_.complement(transpose(e.sub()));
      }

      AWALI_RAT_VISIT(transposition, e)
      {
        // Don't stack indefinitly transpositions on top of
        // transitions.  Not only is this useless, it would also break
        // the involution as r.transpose().transpose() would not be r,
        // but "r{T}{T}".  On the other hand, if "(abc){T}".tranpose()
        // return "abc", we also lose the involution.
        //
        // So rather, don't stack more that two transpositions:
        //
        // (abc){T}.transpose() => (abc){T}{T}
        // (abc){T}{T}.transpose() => (abc){T}
        //
        // Do the same with ldiv, for the same reasons: involution.
        //
        // (E\F).transpose() => (E\F){T}
        // (E\F){T}.transpose() => (E\F)
        if (e.sub()->kind == rat::type_t::transposition
            || e.sub()->kind == rat::type_t::ldiv)
          res_ = e.sub();
        else
          res_ = rs_.transposition(e.shared_from_this());
      }

      AWALI_RAT_VISIT(ldiv, e)
      {
        // There is nothing we can do here but leaving an explicit
        // transposition.
        res_ = rs_.transposition(e.shared_from_this());
      }

      AWALI_RAT_VISIT(lweight, e)
      {
        res_ = rs_.rmul(transpose(e.sub()),
                        rs_.weightset()->transpose(e.weight()));
      }

      AWALI_RAT_VISIT(rweight, e)
      {
        res_ = rs_.lmul(rs_.weightset()->transpose(e.weight()),
                        transpose(e.sub()));
      }


    private:
      ratexpset_t rs_;
      ratexp_t res_;
    };
  }

  template <class RatExpSet>
  typename RatExpSet::ratexp_t
  transpose(const RatExpSet& rs, const typename RatExpSet::ratexp_t& v)
  {
    return rs.transpose(v);
  }

}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_TRANSPOSE_HH
