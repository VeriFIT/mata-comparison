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

#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/alphabets/char.hh>
#include <awali/sttc/factories/n_ultimate.hh>
#include <awali/sttc/factories/divkbaseb.hh>
#include <awali/sttc/factories/double_ring.hh>
#include <awali/sttc/factories/ladybird.hh>
#include <awali/sttc/factories/synchronizing_word.hh>
#include <awali/sttc/factories/witness.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>

#include<set-types.hh>

namespace awali {

  extern "C" dyn::automaton_t n_ultimate(dyn::context_t ctx, dyn::label_t a, unsigned n) {
    context_t context = dyn::get_stc_context<context_t>(ctx);
    return dyn::make_automaton(sttc::n_ultimate(context, (labelset_t::value_t)a, n));
  }

  extern "C" dyn::automaton_t divkbaseb(dyn::context_t ctx, unsigned k, unsigned b) {
    context_t context = dyn::get_stc_context<context_t>(ctx);
    return dyn::make_automaton(sttc::divkbaseb(context, k, b));
  }
  
  extern "C" dyn::automaton_t double_ring(dyn::context_t ctx, unsigned n, const std::vector<unsigned>& finals) {
    context_t context = dyn::get_stc_context<context_t>(ctx);
    return dyn::make_automaton(sttc::double_ring(context, n, finals));
  }

  extern "C" dyn::automaton_t ladybird(dyn::context_t ctx, unsigned n) {
    context_t context = dyn::get_stc_context<context_t>(ctx);
    return dyn::make_automaton(sttc::ladybird(context, n));
  }

  extern "C" dyn::automaton_t cerny(dyn::context_t ctx, unsigned n) {
    context_t context = dyn::get_stc_context<context_t>(ctx);
    return dyn::make_automaton(sttc::cerny(context, n));
  }

  extern "C" dyn::automaton_t witness(dyn::context_t ctx, unsigned n) {
    context_t context = dyn::get_stc_context<context_t>(ctx);
    return dyn::make_automaton(sttc::witness(context, n));
  }

}

#include <awali/dyn/core/any.cc>
