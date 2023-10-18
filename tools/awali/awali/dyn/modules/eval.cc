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

#ifndef DYN_MODULES_EVAL_CC
#define DYN_MODULES_EVAL_CC

#include <map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/loading/handler.hh>

#include <awali/dyn/modules/eval.hh>
//Only for lal

namespace awali {
  namespace dyn {

    weight_t eval(automaton_t aut, any_t word)
    {
      return loading::call1<weight_t>("eval", "eval", aut, word);
    }

    std::map<any_t, weight_t> enumerate(automaton_t aut, unsigned max)
    {
      return loading::call1<std::map<any_t, weight_t>>("enumerate", "eval", aut, max);
    }

    std::map<any_t, weight_t> shortest(automaton_t aut, unsigned max)
    {
      return loading::call1<std::map<any_t, weight_t>>("shortest", "eval", aut, max);
    }
  }
}//end of ns awali::dyn

#endif
