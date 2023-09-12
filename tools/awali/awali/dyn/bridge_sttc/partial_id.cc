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

#include<string>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/algos/partial_identity.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/sttc/algos/copy.hh>

#include<set-types.hh>

/*This function should not be called with transducers */

namespace awali {

  extern "C" dyn::automaton_t partial_identity(dyn::automaton_t aut) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    using new_context_t = sttc::nullable_of<context_t>;
    auto tmp = sttc::make_mutable_automaton<new_context_t>(sttc::get_nullable_context(a->context()));
    sttc::copy_into(a,tmp);
    return dyn::make_automaton(sttc::partial_identity<2>(tmp));
  }

}

#include <awali/dyn/core/any.cc>
