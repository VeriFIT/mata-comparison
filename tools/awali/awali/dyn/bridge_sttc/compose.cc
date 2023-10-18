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

#include <awali/sttc/algos/compose.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>

#include<set-types.hh>
namespace awali {

extern "C" dyn::automaton_t compose(dyn::automaton_t tdc1, dyn::automaton_t tdc2) {
  auto t1 = dyn::get_stc_automaton<context1_t>(tdc1);
  auto t2 = dyn::get_stc_automaton<context2_t>(tdc2);
  return dyn::make_automaton(sttc::compose(t1,t2));
}

}

#include <awali/dyn/core/any.cc>
