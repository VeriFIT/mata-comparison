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

#ifndef AWALI_ALGOS_FACTOR_HH
#define AWALI_ALGOS_FACTOR_HH

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/accessible.hh>

namespace awali { namespace sttc {


  /*-------.
  | Prefix |
  `-------*/

  /// Make all coaccessible states final.
  template <typename Aut>
  void
  prefix_here(Aut& a)
  {
    for(auto s : coaccessible_states(a))
      a->set_final(s);
  }

  template <typename Aut>
  typename Aut::element_type::automaton_nocv_t
  prefix(const Aut& a, bool keep_history=true)
  {
    auto r = copy(a, keep_history, false, true);
    prefix_here(r);
    return r;
  }

  /*-------.
  | Suffix |
  `-------*/

  /// Make all accessible states initial.
  template <typename Aut>
  void
  suffix_here(Aut& a)
  {
    for(auto s : accessible_states(a))
      a->set_initial(s);
  }

  template <typename Aut>
  typename Aut::element_type::automaton_nocv_t
  suffix(const Aut& a, bool keep_history=true)
  {
    auto r = copy(a, keep_history, false, true);
    suffix_here(r);
    return r;
  }

  /*-------.
  | Factor |
  `-------*/

  /// Make each useful state both initial and final.
  template <typename Aut>
  void
  factor_here(Aut& a)
  {
    for(auto s : useful_states(a)) {
      a->set_initial(s);
      a->set_final(s);
    }
  }

  template <typename Aut>
  typename Aut::element_type::automaton_nocv_t
  factor(const Aut& a, bool keep_history=true)
  {
    auto r = copy(a, keep_history, false, true);
    factor_here(r);
    return r;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_FACTOR_HH
