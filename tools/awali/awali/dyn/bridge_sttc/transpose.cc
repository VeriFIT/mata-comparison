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

#include <awali/sttc/algos/transpose.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>

#include<set-types.hh>

namespace awali {

  extern "C" dyn::automaton_t transpose(dyn::automaton_t aut, bool keep_history) {
      auto a= dyn::get_stc_automaton<context_t>(aut);
      return dyn::make_automaton(sttc::transpose(a, keep_history));
  }

  extern "C" void transpose_here(dyn::automaton_t aut) {
      auto a= dyn::get_stc_automaton<context_t>(aut);
      sttc::transpose_here(a);
  }

}

#include <awali/dyn/core/any.cc>
