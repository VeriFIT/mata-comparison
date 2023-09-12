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

#ifndef AWALI_ALGOS_DOUBLE_RING_HH
# define AWALI_ALGOS_DOUBLE_RING_HH

# include <map>

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/alphabets/char.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc {

    /** Returns a double ring automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z.      
     *
     * The \p context specifies both the alphabet and the weightset.
     *
     * For every state *p*,      
     * - the first letter of the \p alphabet labels transitions from *p* to *p+1*;    
     * - the second one labels transitions from *p* to *p-1*;
     * - if the \p alphabet has more than 2 letters, the other letters do not label any transition.
     *
     * State 0 is initial; \p finals is the list of the final states.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * @tparam Context the type of the context
     * @param ctx the context of the automaton
     * @param n a positive integer
     * @param finals a vector of states
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is not positive
     *                   or if \p alphabet has less than 2 letters
     **/
  template <class Context>
  mutable_automaton<Context>
  double_ring(const Context& ctx, unsigned n,
              const std::vector<unsigned>& finals)
  {
    using context_t = Context;
    const auto& gens = ctx.labelset()->genset();
    std::vector<typename context_t::labelset_t::letter_t> letters
      {std::begin(gens), std::end(gens)};
    require(2 <= letters.size(),
            "double_ring: the alphabet needs at least 2 letters");
    require(n>0,
            "double_ring: the automaton should have at least one state");
    auto a = letters[0];
    auto b = letters[1];

    using automaton_t = mutable_automaton<context_t>;
    automaton_t res = make_shared_ptr<automaton_t>(ctx);

    // Set initial.
    auto p = res->add_state();
    res->set_initial(p);
    // Have states start on base 0. No need for pre and post states here.
    std::map<unsigned, state_t> states;
    // We want first state to be 0 and not 2.
    states.emplace(0, p);
    // Set transitions.
    state_t x = p;
    for (unsigned i = 1; i < n; ++i)
      {
        state_t y = res->add_state();
        res->new_transition(x, y, a);
        res->new_transition(y, x, b);
        x = y;
        states.emplace(i, y);
      }
    res->new_transition(x, p, a);
    res->new_transition(p, x, b);

    // Add finals.
    for (auto f: finals)
    {
      require(f < n, "double_ring: invalid list of finals");
      res->set_final(states[f]);
    }
    res->set_name("double-ring-"+std::to_string(n));
    return res;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_DOUBLE_RING_HH
