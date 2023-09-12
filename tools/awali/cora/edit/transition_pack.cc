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


#ifndef CORA_EDIT_TRANSITION_PACK_CC
#define CORA_EDIT_TRANSITION_PACK_CC

#include <cora/edit/transition_pack.hh>

namespace awali {
namespace cora {
namespace editter {

const std::string transition_pack_t::DEFAULT = "@tr_pack_t::DEFAULT@";


transition_pack_t::transition_pack_t(dyn::automaton_t aut) 
: id(-1),
  src(DEFAULT),
  dst(DEFAULT),
  weight("")
{
  if (aut->is_transducer())
    labels = std::vector<std::string>(
      ((dyn::transducer_t) aut).num_tapes(),DEFAULT);
  else
    labels = {DEFAULT};
  dyn::context_t ctx = aut->get_context();
  weight = ctx->weight_to_string(ctx->weight_one());
}


transition_pack_t::transition_pack_t(dyn::automaton_t aut, 
                                     dyn::transition_t t, 
                                     const std::string& eps_repr)
:transition_pack_t(aut)
{
  id = t;
  src = aut->get_state_name(aut->src_of(t));
  dst = aut->get_state_name(aut->dst_of(t));
  weight = aut->get_context()->weight_to_string(aut->weight_of(t));
  if (aut->is_transducer()) {
    labels.clear();
    for(const std::string& l : ((dyn::transducer_t) aut).label_of(t)) {
      labels.push_back(l==""?eps_repr:l);
    }
  }
  else {
    labels[0] 
      =  aut->is_eps_transition(t) ? eps_repr
                                   : aut->get_context()->label_to_string(
                                                              aut->label_of(t));
  }
}



} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali


#endif
