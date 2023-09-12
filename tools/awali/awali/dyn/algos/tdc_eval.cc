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

#include <awali/dyn/algos/tdc_eval.hh>
#include <awali/dyn/modules/partial_id.hh>
#include <awali/dyn/modules/context.hh>

namespace awali {
  namespace dyn {

    automaton_t
    eval_tdc (automaton_t aut, transducer_t tdc) {
      return image(compose(partial_identity(aut),tdc));
    }

    ratexp_t eval_word(transducer_t tdc, const std::string& word) {
      automaton_t aut = automaton_t::from("");
      context_t ctx = aut->get_context();
      state_t s=aut->add_state();
      aut->set_initial(s);
      for(unsigned i=0; i<word.length(); ++i) {
        state_t t=aut->add_state();
        ctx->add_letter(word[i]);
        aut->set_transition(s, t, word[i]);
        s=t;
      }
      aut->set_final(s);
      automaton_t res = eval_tdc(aut, tdc);
//      return aut_to_exp(res);  mod.js 200316
      return aut_to_exp(res);
    }
  }
}
