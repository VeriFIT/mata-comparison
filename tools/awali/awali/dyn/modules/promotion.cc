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

#ifndef DYN_MODULES_PROMOTION_CC
#define DYN_MODULES_PROMOTION_CC

#include <set>

#include <awali/dyn/modules/promotion.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/modules/automaton.hh>
#include <awali/dyn/core/context_description.hh>


namespace awali {
  namespace dyn {

    bool is_promotable(const std::string& src_semiring, const std::string& semiring) {
        return context::is_promotable(context::weightset(src_semiring),
                                      context::weightset(semiring));
    }
    
    context_t make_context_with_another_semiring(context_t ctx, const std::string& semiring) {
      std::string src_semiring = ctx->weightset_name();
      std::string stat_ctx1 = ctx->sname();
      std::string stat_ctx2 = ctx->labelset_name()+"_"+context::tostring(context::weightset(semiring),false);
      typedef context_t (*bridge_t)(context_t);
      bridge_t bridge 
      = (bridge_t) loading::get_handler("promote_ctx", "promotion", stat_ctx1, stat_ctx2);
      return bridge(ctx);
    }
    
    automaton_t promote_automaton(automaton_t aut, const std::string& semiring, options_t opts) {
      bool keep_history = opts[KEEP_HISTORY];
      context_t ctx=aut->get_context();
      std::string src_semiring = ctx->weightset_name();
      if(src_semiring=="B")
        return characteristic(aut, semiring, opts);
      if(src_semiring==semiring)
        return copy(aut);
      if(!is_promotable(src_semiring, semiring))
        throw std::runtime_error("no promotion from "+src_semiring+" to "+semiring);
      std::string stat_ctx1 = ctx->sname();
      std::string stat_ctx2 = ctx->labelset_name()+"_"+context::tostring(context::weightset(semiring),false);
      typedef automaton_t (*bridge_t)(automaton_t,bool);
      bridge_t bridge 
      = (bridge_t) loading::get_handler("promote_aut", "promotion", stat_ctx1, stat_ctx2);
      return bridge(aut, keep_history);
    }

  }
}//end of ns awali::dyn

#endif
