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

#ifndef CORA_EDIT_TRANSITION_PACK_HH
#define CORA_EDIT_TRANSITION_PACK_HH

#include <string>
#include <vector>
#include <awali/dyn.hh>

namespace awali {
namespace cora {
namespace editter {

  struct transition_pack_t {
    int id; // id of the transition if it exists, -1 otherwise
    std::string src;
    std::string dst;
    std::string weight;
    std::vector<std::string> labels;
    const static std::string DEFAULT;
    
    transition_pack_t(dyn::automaton_t aut);
    transition_pack_t(dyn::automaton_t aut, dyn::transition_t t, 
                      const std::string& eps_repr);
  };


} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
