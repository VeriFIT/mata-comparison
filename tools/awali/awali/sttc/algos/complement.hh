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

#ifndef AWALI_ALGOS_COMPLEMENT_HH
# define AWALI_ALGOS_COMPLEMENT_HH

# include <set>

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/is_complete.hh>
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/weightset/fwd.hh> // b

namespace awali { namespace sttc {


  /*------------------------.
  | complement(automaton).  |
  `------------------------*/

    /** Complementation of a deterministic complete automaton
     *
     * The function swaps the final status of every state.
     *
     * @tparam Aut the type of the automaton
     * @param aut a Boolean deterministic complete automaton
     * @return the automaton \a aut itself
     */
  template <typename Aut>
  Aut
  complement_here(Aut& aut)
  {
    static_assert(labelset_t_of<Aut>::is_free(),
                  "requires free labelset");
    static_assert(std::is_same<weightset_t_of<Aut>, b>::value,
                  "requires Boolean weights");

    // using automaton_t = Aut;

    require(is_deterministic(aut),
            "complement: requires a deterministic automaton");
    require(is_complete(aut),
            "complement: requires a complete automaton");

    // Complement.
    for (auto s: aut->states())
      if (!aut->is_final(s))
        aut->set_final(s);
      else
        aut->unset_final(s);
    return aut;
  }

    /** Complementation of a deterministic complete automaton
     *
     * Applies {@link complement_here} to a copy of \a aut
     *
     * @tparam Aut the type of the automaton
     * @param aut a Boolean deterministic complete automaton
     * @param keep_history if true, every state of the result is linked to a  state of \a aut
     *
     */
  template <typename Aut>
  auto
  complement(const Aut& aut, bool keep_history=true)
    -> decltype(copy(aut))
  {
    auto res = copy(aut, keep_history);
    complement_here(res);
    return res;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_COMPLEMENT_HH
