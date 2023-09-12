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

#ifndef AWALI_ALGOS_UNIVERSAL_HH
# define AWALI_ALGOS_UNIVERSAL_HH

# include <map>

#include <awali/sttc/misc/set.hh>
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/algos/transpose.hh>
#include <awali/sttc/weightset/fwd.hh> // b

namespace awali { namespace sttc {

  namespace internal
  {
    /// Functor for universal.
    template <typename Aut>
    class universaler
    {
    public:
      static_assert(labelset_t_of<Aut>::is_free(),
                    "requires free labelset");
      static_assert(std::is_same<weightset_t_of<Aut>, b>::value,
                    "requires Boolean weights");

      using automaton_t = Aut;
      using state_set_t = std::set<state_t>;
      using pstate_t = std::set<state_set_t>;
      using map_t = std::map<state_t, state_set_t>;

      /// The universal automaton of \a automaton.
      automaton_t operator()(const Aut& automaton)
      {
        if (!is_deterministic(automaton))
          return work_(determinize(automaton,false));
        else if (!is_complete(automaton))
          return work_(complete(automaton));
        else
          return work_(automaton);
      }

    private:
      /// Work on \a aut, which is complete and deterministic.
      /// \pre is_complete(aut)
      /// \pre is_deterministic(aut)
      automaton_t work_(const automaton_t& aut)
      {
        // The initial state of automaton.
        state_t i = aut->dst_of(aut->initial_transitions().front());

        // compute the co-determinized of the minimal automaton
        // and retrieve the origin of each state.
        const auto transposed = transpose(aut);
        auto codet = determinize(transposed);
        auto h=codet->history();
        map_t origin = dynamic_cast<partition_history<automaton_t>*>(h.get())->origins();

        // the 'origin' is a map from co_det's state_t to
        // minimal's state_set_t.
        // let 'transp_states' be the image of 'origin'.
        pstate_t transp_states = image(origin);

        // the universal automaton's state set is its intersection closure.
        pstate_t univers_states(intersection_closure(transp_states));

        // The universal automaton.
        automaton_t res = make_shared_ptr<automaton_t>(aut->context());

        // The final states of aut.
        std::set<state_t> automaton_finals;
        for (auto t: aut->final_transitions())
          automaton_finals.insert(aut->src_of(t));

        // we have to save the state set associated to each automaton.
        map_t subset_label;

        // X = univers_states \ {}.
        for (const auto s: univers_states)
          if (!s.empty())
            {
              state_t new_s = res->add_state();
              subset_label[new_s] = s;
              // J = { X | i in X }
              if (has(s, i))
                res->set_initial(new_s);
              // U = { X | X \subset T }
              if (subset(s, automaton_finals))
                res->set_final(new_s);
            }

        // Finally, the transition set.
        for (const auto x: res->states())
          for (const auto y: res->states())
            for (const auto a: *res->labelset())
              {
                bool cont = false;
                state_set_t delta_ret;
                for (auto s: subset_label[x])
                  {
                    bool empty = true;
                    for (auto t: aut->out(s, a))
                      {
                        empty = false;
                        delta_ret.insert(aut->dst_of(t));
                      }
                    if (empty)
                      {
                        cont = true;
                        break;
                      }
                  }
                // case 1: \exists p \in X, p.a = {}
                if (cont)
                  continue;
                // case 2: X.a \subset Y?
                if (subset(delta_ret, subset_label[y]))
                  res->new_transition(x, y, a);
              }
        return res;
      }
    };
  }

  template <class Aut>
  inline
  Aut
  universal(const Aut& a)
  {
    internal::universaler<Aut> universal;
    return universal(a);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_UNIVERSAL_HH
