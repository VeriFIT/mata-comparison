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

#ifndef DYN_MODULES_PARTIALID_HH
#define DYN_MODULES_PARTIALID_HH


#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/transducer.hh>

namespace awali {
  namespace dyn {

    transducer_t partial_identity(automaton_t aut);

  }
}//end of ns awali::dyn

#endif
