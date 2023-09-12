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

#ifndef AWALI_ALGOS_COMPLETE_HH
# define AWALI_ALGOS_COMPLETE_HH

# include <queue>
# include <unordered_map>

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/misc/unordered_set.hh>

namespace awali { namespace sttc {

    /** Completion of a deterministic automaton
     *
     * The function adds if needed a sink state.
     *
     * @tparam Aut the type of the automaton
     * @param aut a Boolean deterministic complete automaton
     * @return the automaton \a aut itself
     */
  template <typename Aut>
  Aut&
  complete_here(Aut& aut)
  {
    static_assert(labelset_t_of<Aut>::is_free(),
                  "requires free labelset");

    using automaton_t = Aut;
    using letter_t = typename labelset_t_of<automaton_t>::letter_t;

    // A sink state, to allocate if needed.
    state_t sink = aut->null_state();
    const auto& ls = *aut->labelset();

    if (aut->num_initials() == 0) {
      sink = aut->add_state();
      aut->set_initial(sink);
    }

    // The outgoing labels of a state.
    std::unordered_set<letter_t> labels_met;
    for (auto st : aut->states())
      if (st != sink) {
        for (auto tr : aut->out(st))
          labels_met.insert(aut->label_of(tr));

        for (auto letter : ls.genset())
          if (labels_met.find(letter)==labels_met.end()) {
            if (sink == aut->null_state())
              sink = aut->add_state();
            aut->new_transition(st, sink, letter);
          }

        labels_met.clear();
      }

    if (sink != aut->null_state())
      for (auto letter : ls.genset())
        aut->new_transition(sink, sink, letter);

    return aut;
  }

    /** Completion of a deterministic automaton
     *
     * Applies {@link complete_here} to a copy of \a aut
     *
     * @tparam Aut the type of the automaton
     * @param aut a Boolean deterministic automaton
     * @param keep_history if true, every state of the result is linked to a  state of \a aut
     *
     */
  template <typename Aut>
  auto
  complete(const Aut& aut, bool keep_history=true)
    -> decltype(copy(aut, keep_history))
  {
    auto res = copy(aut, keep_history);
    complete_here(res);
    return res;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_COMPLETE_HH
