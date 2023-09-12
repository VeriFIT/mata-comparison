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

#ifndef DYN_MODULES_SINGLEPRODUCT_CC
#define DYN_MODULES_SINGLEPRODUCT_CC

#include <unordered_map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/loading/handler.hh>

#include <awali/dyn/modules/singleproduct.hh>

namespace awali {
  namespace dyn {

    automaton_t power(automaton_t aut, unsigned n)
    {
      return loading::call1<automaton_t>("power", "singleproduct", aut, n);
    }

  }
}//end of ns awali::dyn

#endif
