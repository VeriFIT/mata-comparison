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

#include <awali/sttc/algos/are_equivalent.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>

#include<set-types.hh>

namespace awali {


  extern "C" bool are_equivalent (dyn::automaton_t aut, dyn::automaton_t aut2)
  {
    auto a= dyn::get_stc_automaton<context1_t>(aut);
    auto a2= dyn::get_stc_automaton<context2_t>(aut2);
    return sttc::are_equivalent(a, a2);
  }

}//end of ns awali

#include <awali/dyn/core/any.cc>
