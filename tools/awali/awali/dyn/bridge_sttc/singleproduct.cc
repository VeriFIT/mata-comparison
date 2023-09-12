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

#include <awali/sttc/algos/product.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/common/priority.hh>

#include<set-types.hh>

namespace awali {

  namespace internal {

    template <typename C, typename P>
    dyn::automaton_t power(dyn::automaton_t aut, unsigned n, priority::ONE<P>) {
      throw std::runtime_error("power only supported for free label-sets.");
    }


    template <typename C, typename P>
    auto power(dyn::automaton_t aut, unsigned n, priority::TWO<P>)
    -> typename std::enable_if< C::labelset_t::is_free(), dyn::automaton_t>::type
    {
      auto a = dyn::get_stc_automaton<C>(aut);
      return dyn::make_automaton(sttc::power(a,n));
    }
  }

  extern "C" dyn::automaton_t power(dyn::automaton_t aut, unsigned n) {
      return internal::power<context_t>(aut,n, priority::value);
  }

}

#include <awali/dyn/core/any.cc>
