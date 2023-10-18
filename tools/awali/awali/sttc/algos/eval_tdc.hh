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

#ifndef AWALI_ALGOS_EVAL_TDC_HH
#define AWALI_ALGOS_EVAL_TDC_HH

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/algos/compose.hh>
#include <awali/sttc/algos/eval.hh>
#include <awali/sttc/algos/lal_lan_conversion.hh>

namespace awali {
  namespace sttc {

    /* @brief evaluation of a pair of words in a transducer
     *
     * The transducer should have at least two tapes.
     * The words are evaluated on the first two tapes.
     * @tparam Tdc the type of the transducer
     * @param tdc the transducer
     * @param w1 the first word
     * @param w2 the second word
     * @return the evaluation of the pair (\p w1,\p w2) in \p tdc
     */
    template <typename Tdc>
    inline
    auto
    eval_words_in_tdc(const Tdc& tdc,
             const typename internal::select<labelset_t_of<Tdc>,0>::labelset_t::word_t& w1,
             const typename internal::select<labelset_t_of<Tdc>,1>::labelset_t::word_t& w2)
      -> weight_t_of<Tdc>
    {
      using tdc_context_t = context_t_of<Tdc>;
      using tdc_labelset_t = labelset_t_of<tdc_context_t>;
      using weightset_t = weightset_t_of<tdc_context_t>;
      using labelset1_t = typename internal::select<tdc_labelset_t,0>::labelset_t;
      using aut_context1_t = context<labelset1_t, weightset_t>;
      auto tdc_labelset = tdc->labelset();
      auto weightset = tdc->weightset();
      auto labelset1 = internal::select<tdc_labelset_t,0>::get(*tdc_labelset);
      aut_context1_t aut_context1{labelset1, *weightset};
      auto aut1 = make_mutable_automaton(aut_context1);
      state_t p = aut1->add_state();
      aut1->set_initial(p);
      for(auto l : w1) {
        state_t q = aut1->add_state();
        aut1->new_transition(p,q,l);
        p=q;
      }
      aut1->set_final(p);
      auto aut2 = to_lal(eval_tdc(aut1, tdc));
      auto w = eval(aut2, w2);
      return w;
    }
    
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_EVAL_TDC_HH
