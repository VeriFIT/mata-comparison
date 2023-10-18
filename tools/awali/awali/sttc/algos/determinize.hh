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

#ifndef AWALI_ALGOS_DETERMINIZE_HH
# define AWALI_ALGOS_DETERMINIZE_HH

#include <awali/sttc/algos/determinize/determinize.hxx>
#include <awali/sttc/algos/determinize/is_deterministic.hxx>
#include <awali/common/priority.hh>

namespace awali {
  namespace sttc {
    /** Determinization of the automaton
     *
     * The determinization is computed with the subset construction.
     * If the number of states of \a a is small (not larger than twice
     * the number of digits of \p size_t, every subset is represented by
     * a bitset, otherwise, it is represented by a \p std::set. 
     *
     * @tparam Aut the type of the automaton
     * @param a the input automaton
     * @param keep_history if true, every state of the result is linked to a set of states of \a a
     * return a deterministic automaton
     */
    template <typename Aut>
      inline
      auto
      determinize(const Aut& a, bool keep_history = true)
      -> mutable_automaton<context_t_of<Aut>>
    {
      // We use state numbers as indexes, so we need to know the last
      // state number.  If states were removed, it is not the same as
      // the number of states.
      unsigned state_size = a->max_state();
      static const unsigned lim = std::numeric_limits<size_t>::digits;
      if(state_size<lim)
        {
          internal::determinization_bitset_impl<Aut,lim> algo(a);
          auto res=algo();
          if(keep_history)
            algo.set_history();
          return res;
        }
      if(state_size<2*lim)
        {
          internal::determinization_bitset_impl<Aut,2*lim> algo(a);
          auto res=algo();
          if(keep_history)
            algo.set_history();
          return res;
        }
      internal::determinization_set_impl<Aut> algo(a);
      auto res=algo();
      if(keep_history)
        algo.set_history();
      return res;
    }

    /** Co-determinization of the automaton
     *
     * Computes the transpostion of the determinization of the transposition.
     * The determinization is actually applied to a transpose_view in order to avoid a copy.
     *
     * @tparam Aut the type of the automaton
     * @param aut the input automaton
     * @param keep_history if true, every state of the result is linked to a set of states of \p aut
     * return a co-deterministic automaton
     */
    template <typename Aut>
    inline
    auto
    codeterminize(const Aut& aut, bool keep_history = true)
      -> mutable_automaton<context_t_of<Aut>>
    {
      return transpose(determinize(transpose_view(aut, keep_history)), keep_history);
    }

    /** Weighted determinization of the automaton
     *
     * This function only applies to true finite state machine.
     * The result is a deterministic automaton where the weight of the
     * word is given by the final function.
     * If the semiring is not locally finite, the termination is not assured.   
     * @tparam Aut the type of the automaton
     * @param aut the input automaton
     * return a deterministic automaton
     */
    template <typename Aut>
    inline
    auto
    weighted_determinize(const Aut& aut)
      -> mutable_automaton<context_t_of<Aut>>
    {
      internal::detweighted_algo_impl<Aut> algo(aut);
      auto res = algo();
      return res;
    }

    /** Exploration of the automaton up to a given depth
     *
     * The result is a deterministic automaton where the weight of the
     * word is given by the final function.
     * The result is a restriction of the determinization to the states
     * at distance at most \p depth of the initial state.
     * @tparam Aut the type of the automaton
     * @param aut the input automaton
     * @param depth the depth of the exploration
     * return a deterministic automaton
     */
    template <typename Aut>
    inline
    auto
    explore_by_length(const Aut& aut, unsigned depth)
      -> mutable_automaton<context_t_of<Aut>>
    {
      internal::detweighted_algo_impl<Aut> algo(aut, depth);
      auto res = algo();
      return res;
    }

    /** Exploration of the automaton up to a given bound
     *
     * The result is a deterministic automaton where the weight of the
     * word is given by the final function.
     * The result is a restriction of the determinization to the states
     * corresponding to multisets of states where no weight is larger
     *than \p bound.
     * The comparison is made through the less_than method of the weightset
     * applied to the square of the component and the square of the bound.
     *
     * Caveat : in zmin, the ordering of the weightset is opposite to the
     * natural way, such that zero (=oo) is smaller than one (=0).
     *
     * @tparam Aut the type of the automaton
     * @param aut the input automaton
     * @param bound the bound for the exploration
     * return a deterministic automaton
     */
    template <typename Aut>
    inline
    auto
    explore_with_bound(const Aut& aut, typename weightset_t_of<Aut>::value_t bound)
      -> mutable_automaton<context_t_of<Aut>>
    {
      internal::detweighted_algo_impl<Aut> algo(aut);
      auto ws = *aut->weightset();
      auto bb=ws.mul(bound,bound);
      auto res = algo([=](typename internal::detweighted_algo_impl<Aut>::state_name_t& st){
	  for (const auto& p : st)
	    if(ws.less_than(bb,ws.mul(p.second,p.second)))
	      return false;
	  return true;
	});
      return res;
    }

    /** tests whether the automaton is deterministic
     *
     * This function does not consider weights; 
     * weighted automata are supported yet.
     *
     * @tparam Aut the type of the automaton
     * @param aut the input automaton
     * return true if the automaton has at most one initial state,
     * and for every state *s* and every label *l*, *l* labels at most one
     * transoition outgoing from *s* 
     */
    template <class Aut>
    inline bool
    is_sequential(const Aut& aut)
    {
      static_assert(labelset_t_of<Aut>::is_free(),
                    "requires free labelset");

      if (1 < aut->num_initials())
        return false;

      for (auto s: aut->states())
	if (!internal::is_sequential(aut, s))
	  return false;
      return true;
    }

    /** tests whether the automaton is deterministic
     *
     * This function does not consider weights; 
     * weighted automata are supported yet.
     *
     * @tparam Aut the type of the automaton
     * @param aut the input automaton
     * return true if the automaton has at most one initial state,
     * and for every state *s* and every label *l*, *l* labels at most one
     * transoition outgoing from *s* 
     */
    template <class Aut>
    inline bool
    is_deterministic(const Aut& aut)
    {
      static_assert(labelset_t_of<Aut>::is_free(),
		    "requires free labelset");
    
      if (1 < aut->num_initials())
	return false;
      for (auto t : aut->initial_transitions())
	if(!aut->weightset()->is_one(aut->weight_of(t)))
	  return false;
      for (auto s: aut->states())
	if (!internal::is_deterministic(aut, s))
	  return false;
      return true;
    }
  }
}
#endif //AWALI_ALGOS_DETERMINIZE_HH
