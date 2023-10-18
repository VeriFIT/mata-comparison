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

#include <set-types.hh>
#include <awali/dyn/bridge_sttc/explicit_context.cc>
#include <awali/sttc/ctx/context.hh>

namespace awali {

  extern "C" dyn::context_t join_ctx(dyn::context_t c1, dyn::context_t c2) {
    const context1_t& context1=dyn::get_stc_context<context1_t>(c1);    
    const context2_t& context2=dyn::get_stc_context<context2_t>(c2);
    auto cres=sttc::join(context1,context2);
    return std::make_shared<dyn::explicit_context_t<decltype(cres)>>(cres);
  }
  
}
