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

#ifndef AWALI_ALGOS_STAR_HH
# define AWALI_ALGOS_STAR_HH

#include <awali/common/priority.hh>
#include <awali/sttc/labelset/nullableset.hh>
#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/standard.hh> // is_standard
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/raise.hh> // require

namespace awali {
  namespace sttc {

    /*,------.
      | star |
      `------'*/

    namespace internal {
      
    /// In place star of an automaton.
      
       template<typename Aut, typename P>
       Aut&
       star_here(Aut& res, priority::ONE<P>)
       {           
         const auto& ws=*res->context().weightset();
         // To compute the star of a non standard automaton,
         // a state has to be added (at least to deal with the empty word).
         // Hence, the simplest way is to standardize.
         standard_here(res);
         
         state_t initial = res->dst_of(*(res->initial_transitions().begin()));
         // The "final weight of the initial state", starred.
         auto w = ws.star(res->get_final_weight(initial));
         // Branch all the final states (but initial) to the successors
         // of initial.
         for (auto ti: res->out(initial))
           {
             res->lmul_weight(ti, w);
             for (auto tf: res->final_transitions())
               if (res->src_of(tf) != initial)
                 // The weight of ti has already been multiplied, on the
                 // left, by w.
                 res->add_transition
                   (res->src_of(tf),
                    res->dst_of(ti),
                    res->label_of(ti),
                    ws.mul(res->weight_of(tf), res->weight_of(ti)));
           }
         for (auto tf: res->final_transitions())
           res->rmul_weight(tf, w);
         res->set_final(initial, w);
         return res;
       }
      
       template<typename Aut, typename P>
       auto
       star_here(Aut& res, priority::TWO<P>) -> typename std::enable_if<labelset_t_of<Aut>::has_one(),Aut&>::type
       {
         const auto& ws=*res->weightset();
         auto one=res->labelset()->one();
         for (auto ft: res->final_transitions())
           for (auto it: res->initial_transitions())
             res->add_transition(res->src_of(ft),
                                 res->dst_of(it),
                                 one,
                                 ws.mul(res->weight_of(ft), res->weight_of(it)));
         state_t init=res->add_state();
         res->set_initial(init);
         res->set_final(init);
         return res;
       }
    }

    template<typename Aut>
    Aut&
    star_here(Aut& res) {
      return internal::star_here(res, priority::value);
    }
    
    /// Star of an automaton.
    template <typename Aut>
    typename Aut::element_type::automaton_nocv_t
    star(const Aut& aut)
    {
      auto res = copy(aut);
      star_here(res);
      return res;
    }
  }
}//end of ns awali::sttc

#endif // !AWALI_ALGOS_STAR_HH
