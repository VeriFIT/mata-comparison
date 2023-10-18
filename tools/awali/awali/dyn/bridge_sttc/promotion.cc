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

/**

  This module is only supposed to be compiled with a context2_t that is 
  more general than context2_t !

  If it's not the case, the module may not compile at all.

*/

#include<set-types.hh>
#include <awali/dyn/bridge_sttc/explicit_context.cc>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/dyn/bridge_sttc/explicit_ratexp.cc>
#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/exp_copy.hh>
#include <awali/sttc/ctx/context.hh>

namespace awali {


//   extern "C" dyn::context_t promote_ctx(dyn::context_t ctx) {
//     context1_t context1=dyn::get_stc_context<context1_t>(ctx);
//     weightset2_t ws;
//     context2_t context2(*context1.labelset(), ws);
//     return std::make_shared<dyn::explicit_context_t<context2_t>>(context2);  
//   }

  extern "C" dyn::automaton_t promote_aut(dyn::automaton_t aut, bool keep_history) {
    context1_t context1=dyn::get_stc_context<context1_t>(aut->get_context());
    weightset2_t ws;
    context2_t context2(*context1.labelset(), ws);
    auto aut1 =dyn::get_stc_automaton<context1_t>(aut);
    auto aut2 =sttc::make_mutable_automaton(context2);
    sttc::copy_into(aut1, aut2, keep_history);
    return dyn::make_automaton(aut2);  
  }

  extern "C" dyn::automaton_t promote_aut_with_ctx(dyn::automaton_t aut, dyn::context_t ctx, bool keep_history) {
    context1_t context1=dyn::get_stc_context<context1_t>(aut->get_context());
    context2_t context2=dyn::get_stc_context<context2_t>(ctx);
    auto aut1 =dyn::get_stc_automaton<context1_t>(aut);
    auto aut2 =sttc::make_mutable_automaton(context2);
    sttc::copy_into(aut1, aut2, keep_history);
    return dyn::make_automaton(aut2);  
  }

  extern "C" dyn::ratexp_t promote_exp(dyn::ratexp_t exp) {
    using non_nullable_context1_t = sttc::not_nullable_of<context1_t>;
    auto e= get_stc_ratexp<non_nullable_context1_t>(exp);
    auto rs1= get_stc_ratexpset<non_nullable_context1_t>(exp);
    auto e2 = sttc::ratexp_copy<weightset2_t>(e, rs1);
    auto rs2 = sttc::promote_ratexpset<weightset2_t>(rs1);
    auto srs = std::make_shared<decltype(rs2)>(rs2);
    return make_ratexp(e2, srs);
  }

//  template<typename T>
//  struct aff {};
// 
//   extern "C" dyn::context_t join_ctx(dyn::context_t c1, dyn::context_t c2) {
//     const context1_t& context1=dyn::get_stc_context<context1_t>(c1);    
//     const context2_t& context2=dyn::get_stc_context<context2_t>(c2);
//     auto cres=sttc::join(context1,context2);
// //     aff<decltype(cres)>::print(); // Commented by VM
//     return std::make_shared<dyn::explicit_context_t<decltype(cres)>>(cres);      
//   }
  
}
