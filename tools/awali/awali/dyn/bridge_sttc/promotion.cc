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

#include<set-types.hh>
#include <awali/dyn/bridge_sttc/explicit_context.cc>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/sttc/algos/copy.hh>

namespace awali {

  extern "C" dyn::context_t promote_ctx(dyn::context_t ctx) {
    context1_t context1=dyn::get_stc_context<context1_t>(ctx);
    weightset2_t ws;
    context2_t context2(*context1.labelset(), ws);
    return std::make_shared<dyn::explicit_context_t<context2_t>>(context2);  
  }

  extern "C" dyn::automaton_t promote_aut(dyn::automaton_t aut, bool keep_history) {
    context1_t context1=dyn::get_stc_context<context1_t>(aut->get_context());
    weightset2_t ws;
    context2_t context2(*context1.labelset(), ws);
    auto aut1 =dyn::get_stc_automaton<context1_t>(aut);
    auto aut2 =sttc::make_mutable_automaton(context2);
    sttc::copy_into(aut1, aut2, keep_history);
    return dyn::make_automaton(aut2);  
  }
}
