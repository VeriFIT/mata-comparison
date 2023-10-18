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

#ifndef AWALI_ALGOS_EXP_SUPPORT_HH
# define AWALI_ALGOS_EXP_SUPPORT_HH

#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/weightset/b.hh>

namespace awali { namespace sttc {

  namespace rat
  {
    /// \tparam Aut      relative the generated automaton
    /// \tparam Context  relative to the RatExp.
    template <typename RatExpSet>
    class expsupport_visitor
      : public RatExpSet::const_visitor
    {
    public:
      using in_ratexpset_t = RatExpSet;
      using in_context_t = context_t_of<in_ratexpset_t>;
      using labelset_t = labelset_t_of<in_context_t>;

      using out_context_t =  context<labelset_t, sttc::b>;
      using out_ratexpset_t =  ratexpset_of<out_context_t>;
      using ratexp_t = typename in_ratexpset_t::ratexp_t;
      using out_ratexp_t = typename out_ratexpset_t::ratexp_t;
       
      using super_type = typename RatExpSet::const_visitor;

      constexpr static const char* me() { return "exp support"; }

      expsupport_visitor(const in_ratexpset_t& ratexpset)
        : in_ratexpset_ (ratexpset)
      {}

      out_ratexp_t
      operator()(const ratexp_t& v)
      {
        v->accept(*this);
        return res_;
      }

      AWALI_RAT_VISIT(zero,)
      {
	res_ = out_ratexpset_.zero();
      }

      AWALI_RAT_VISIT(one,)
      {
	res_ = out_ratexpset_.one();
      }

      AWALI_RAT_VISIT(atom, e)
      {
	res_ = out_ratexpset_.atom(e.value());
      }

      AWALI_RAT_VISIT(sum, e)
      {
	auto r = out_ratexpset_.zero();	
        for (auto c: e)
          {
            c->accept(*this);
	    r = out_ratexpset_.add(r, res_);
          }
	res_ = r;
      }

      AWALI_RAT_UNSUPPORTED(complement)
      AWALI_RAT_UNSUPPORTED(conjunction)
      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(shuffle)
      AWALI_RAT_UNSUPPORTED(transposition)

      AWALI_RAT_VISIT(prod, e)
      {
	auto r = out_ratexpset_.one();	
        for (auto c: e)
          {
            c->accept(*this);
	    r = out_ratexpset_.mul(r, res_);
          }
        res_ = r;
      }

      AWALI_RAT_VISIT(star, e)
      {
        e.sub()->accept(*this);
	res_ = out_ratexpset_.star(res_);
      }

      AWALI_RAT_VISIT(maybe, e)
      {
        e.sub()->accept(*this);
	res_ = out_ratexpset_.maybe(res_);
      }

      AWALI_RAT_VISIT(plus, e)
      {
        e.sub()->accept(*this);
	res_ = out_ratexpset_.plus(res_);
      }

      AWALI_RAT_VISIT(lweight, e)
      {
        e.sub()->accept(*this);
      }

      AWALI_RAT_VISIT(rweight, e)
      {
        e.sub()->accept(*this);
     }

      out_ratexpset_t get_ratexpset() {
	return out_ratexpset_;
      }
      
    private:
      const in_ratexpset_t& in_ratexpset_;
      out_context_t out_context_{*in_ratexpset_.labelset(), b()};
      out_ratexpset_t out_ratexpset_{out_context_, in_ratexpset_.identities()};
      typename out_ratexpset_t::ratexp_t res_;
    };

  } // rat::

   /** @brief support of a rational expression
    *
    * @tparam RatExpSet  type of the ratexpset of the rational expression
    * @param exp          the rational expression
    * @param ratexpset    the ratexpset of the rational expression
    * @return a Boolean rational expression 
    *
    * The function produces a fresh Boolean rational expression, which is a copy of \p exp where all weights are removed.
    */
    template <typename RatExpSet>
  auto
    ratexp_support(const typename RatExpSet::ratexp_t& exp,
		const RatExpSet& ratexpset)
      -> typename rat::expsupport_visitor<RatExpSet>::out_ratexp_t
  {
    rat::expsupport_visitor<RatExpSet> copyer{ratexpset};
    return copyer(exp);
  }
    
  /** @brief Compute a derived ratexpset
   * 
   * @tparam RatExpSet  type of the ratexpset of the rational expression
   * @param ratexpset    the original ratexpset
   * @return a ratexpset with the same letterset and the same identities as the original \p ratexpset, with a Boolean weightset.
   */									        template <typename RatExpSet>
  auto
   support_ratexpset(const RatExpSet& ratexpset)
     -> typename rat::expsupport_visitor<RatExpSet>::out_ratexpset_t
  {
    rat::expsupport_visitor<RatExpSet> copyer{ratexpset};
    return copyer.get_ratexpset();
  }
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_EXP_COPY_HH
