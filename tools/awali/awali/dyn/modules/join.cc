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

#ifndef DYN_MODULES_JOIN_CC
#define DYN_MODULES_JOIN_CC

#include <algorithm>

#include <awali/dyn/modules/join.hh>
#include <awali/dyn/loading/handler.hh>

namespace awali {
  namespace dyn {

context_t join_context(context_t c1, context_t c2) {
  std::string stat_ctx1 = c1->sname();
  std::string stat_ctx2 = c2->sname();
  
  try {
    typedef context_t (*bridge_t)(context_t,context_t);
    bridge_t bridge;
    if (stat_ctx1 <= stat_ctx2) {
      bridge = (bridge_t) loading::get_handler("join_ctx", "join", stat_ctx1, stat_ctx2);
      return bridge(c1,c2);      
    }
    else {
      bridge = (bridge_t) loading::get_handler("join_ctx", "join", stat_ctx2, stat_ctx1);
      return bridge(c2,c1);      
    }
  } 
  catch (std::runtime_error const& e) {
    throw std::runtime_error(std::string("It is not possible to join contexts "+c1->sname()+" and "+c2->sname()+"."));
  }
  
}


}} // end of namespace awali::dyn


#endif
