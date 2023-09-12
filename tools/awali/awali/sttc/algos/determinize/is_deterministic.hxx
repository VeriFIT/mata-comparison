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

#ifndef AWALI_ALGOS_IS_DETERMINISTIC_HXX
# define AWALI_ALGOS_IS_DETERMINISTIC_HXX

# include <queue>
# include <unordered_set>

#include <awali/sttc/ctx/traits.hh>

namespace awali {
  namespace sttc {
    namespace internal {

      /// Whether state \a s is sequential in \a aut.
      template <typename Aut>
      inline bool
      is_sequential(const Aut& aut, state_t s)
      {
	using automaton_t = Aut;
	static_assert(labelset_t_of<automaton_t>::is_free(),
		      "requires free labelset");
	using label_t = label_t_of<automaton_t>;
	std::unordered_set<label_t> seen;
	for (auto t : aut->out(s))
	  if (!seen.insert(aut->label_of(t)).second)
	    return false;
	return true;
      }

      /// Whether state \a s is deterministic in \a aut.
      template <typename Aut>
      inline bool
      is_deterministic(const Aut& aut, state_t s)
      {
	using automaton_t = Aut;
	static_assert(labelset_t_of<automaton_t>::is_free(),
		      "requires free labelset");
	using label_t = label_t_of<automaton_t>;
	std::unordered_set<label_t> seen;
	for (auto t : aut->out(s)) {
	  if (!aut->weightset()->is_one(aut->weight_of(t)))
	    return false;
	  if (!seen.insert(aut->label_of(t)).second)
	    return false;
	}
	return true;
      }

      /// Number of non-deterministic states.
      template <class Aut>
      inline size_t
      num_deterministic_states(const Aut& aut)
      {
        static_assert(labelset_t_of<Aut>::is_free(),
                      "requires free labelset");

        size_t res = 0;
        for (auto s: aut->states())
          res += is_deterministic(aut, s);
        return res;
      }

    }
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_IS_DETERMINISTIC_HXX
