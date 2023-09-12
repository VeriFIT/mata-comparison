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

#ifndef AWALI_ALGOS_EXP_STATS_HH
# define AWALI_ALGOS_EXP_STATS_HH

# include <set>

#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/core/rat/visitor.hh>

namespace awali {
  namespace sttc {

    /** @brief gathers informations on some rational expression
     *
     * This structure is returned by the function {@link exp_stats}
     */
    struct exp_stats_t {
      unsigned size;
      unsigned length;
      unsigned height;
      unsigned star_height;
    };
    
    namespace rat
    {

    /*------------------------.
    | exp_stats(ratexp).  |
    `------------------------*/

    /// \tparam RatExpSet  the ratexp set type.
    template <typename RatExpSet>
    class exp_stats_visitor
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using ratexp_t = typename ratexpset_t::ratexp_t;

      using super_type = typename ratexpset_t::const_visitor;

      constexpr static const char* me() { return "exp_stats"; }

      exp_stats_visitor(const ratexpset_t& rs) {}

      void
      operator()(const ratexp_t& v) {
        v->accept(*this);
      }
      
      void exp_stats(const ratexp_t& v) {
        v->accept(*this);
      }
      
      AWALI_RAT_VISIT(zero,) {
        size_ = 1;
        length_ = 0;
        sh_ = 0;
        height_ = 1;
      }

      AWALI_RAT_VISIT(one,) {
        size_ = 1;
        length_ = 0;
        height_ = 1;
        sh_ = 0;
      }

      AWALI_RAT_VISIT(atom,) {
        size_ = 1;
        length_ = 1;
        sh_ = 0;
        height_ = 1;
       }

      AWALI_RAT_VISIT(sum, v)
      {

        unsigned s=1, l=0, sh=0, height=0;
        for (auto c: v) {
          exp_stats(c);
          s+=size_;
          l+=length_;
          sh=(sh_>sh)?sh_:sh;
          height = (height_>height)?height_:height;
        }
        size_=s;
        length_=l;
        sh_=sh;
        height_=height+1;
      }

      AWALI_RAT_VISIT(prod, v)
      {
        unsigned s=1, l=0, sh=0, height=0;
        for (auto c: v) {
          exp_stats(c);
          s+=size_;
          l+=length_;
          sh=(sh_>sh)?sh_:sh;
          height = (height_>height)?height_:height;
        }
        size_=s;
        length_=l;
        sh_=sh;
        height_=height+1;
      }

      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(transposition)
      AWALI_RAT_UNSUPPORTED(conjunction)
      AWALI_RAT_UNSUPPORTED(shuffle)
      AWALI_RAT_UNSUPPORTED(complement)

      AWALI_RAT_VISIT(star, v)
      {
        exp_stats(v.sub());
        ++sh_;
        ++size_;
        ++height_;
      }

      AWALI_RAT_VISIT(lweight, v)
      {
        exp_stats(v.sub());
      }

      AWALI_RAT_VISIT(rweight, v)
      {
        exp_stats(v.sub());
      }

      unsigned size() {
        return size_;
      }

      unsigned length() {
        return length_;
      }

      unsigned star_height() {
        return sh_;
      }
      
      unsigned height() {
        return height_;
      }

    private:
      //ratexpset_t ws_;
      unsigned size_;
      unsigned length_;
      unsigned sh_;
      unsigned height_;
    };

  } // rat::

    /** @brief computes some statistics on a rational expression
     *
     * @tparam RatExpSet type of the context of the rational expression
     * @param rs the context of the rational expression
     * @param exp the rational expression
     * @return a exp_stats_t structure that contains the statistics
     *
     * The function computes    
     *     - the size: the number of nodes in \p exp,
     *     - the length : the number of leaves (different from Zero or One),   
     *     - the height of the expression \p exp as a tree,    
     *     - the star height of \p exp: the maximum number of nested stars.   
     * The context \p rs is used to produce the visitor that computes 
     * these statistics.
     */

  template <typename RatExpSet>
  exp_stats_t
  exp_stats(const RatExpSet& rs, const typename RatExpSet::ratexp_t& exp)
  {
    rat::exp_stats_visitor<RatExpSet> exp_stats{rs};
    exp_stats(exp);
    return {exp_stats.size(),exp_stats.length(),exp_stats.height(),exp_stats.star_height()};
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_EXP_STATS_HH
