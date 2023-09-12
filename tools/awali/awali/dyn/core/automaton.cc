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

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/modules/context.hh>

namespace awali { namespace dyn {

  automaton_t::automaton_t() 
    : std::shared_ptr<abstract_automaton_t>()
  {
  }

  automaton_t automaton_t::from_context(context_t ctx) {
    return internal::make_automaton_from_context(ctx);
  }

  automaton_t::automaton_t(context::labelset_description ld,
                           context::weightset_description wd)
  : automaton_t(internal::make_automaton_from_context(ld,wd))
  {}


  automaton_t automaton_t::from(std::string alphabet, std::string ws,
                                bool allow_eps_transitions)
  {
    context::labelset_description ld = context::letterset(alphabet);
    if (allow_eps_transitions)
      ld = context::nullableset(ld);
    return automaton_t(ld, context::weightset(ws));
  }

  automaton_t automaton_t::from(std::string alphabet, 
                                bool allow_eps_transitions,
                                std::string ws)
  {
    return from(alphabet, ws, allow_eps_transitions);
  }
  
  automaton_t automaton_t::from(std::string alphabet, char const* ws)
  {
    return from(alphabet, std::string(ws));
  }

  automaton_t 
  automaton_t::with_int_labels::from_range(int l, int u, 
                                           std::string weightset,
                                           bool allow_eps_transitions) 
  {
    context::labelset_description ld = context::intletterset(l,u);
    if (allow_eps_transitions)
      ld = context::nullableset(ld);
    return automaton_t(ld, context::weightset(weightset));
  }


  automaton_t 
  automaton_t::with_int_labels::from_range(int l, int u, 
                                           bool allow_eps_transitions) 
  {
    return with_int_labels::from_range(l,u,"B",allow_eps_transitions);
  }


  automaton_t
  automaton_t::with_int_labels::from_range(int l, int u, char const* ws) 
  {
    return with_int_labels::from_range(l,u,std::string(ws));
  }

  automaton_t 
  automaton_t::with_int_labels::from_size(unsigned n,
                                          std::string weightset,
                                          bool allow_eps_transitions) 
  {
    context::labelset_description ld = context::intletterset(n);
    if (allow_eps_transitions)
      ld = context::nullableset(ld);
    return automaton_t(ld, context::weightset(weightset));
  }


  automaton_t 
  automaton_t::with_int_labels::from_size(unsigned n,
                                          bool allow_eps_transitions) 
  {
    return with_int_labels::from_size(n,"B",allow_eps_transitions);
  }

  automaton_t
  automaton_t::with_int_labels::from_size(unsigned n, char const* ws)
  {
    return with_int_labels::from_size(n, std::string(ws));
  }

}}

