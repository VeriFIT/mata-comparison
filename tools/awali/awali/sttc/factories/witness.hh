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

#ifndef AWALI_ALGOS_WITNESS_HH
# define AWALI_ALGOS_WITNESS_HH

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/alphabets/char.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc {

    /** Returns an automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z.    
     * For every state p,     
     * - the first letter of the \p alphabet labels transitions from p to p+1;    
     * - the second letter of the \p alphabet labels transitions from p to    
     *        - 1 if p=0,
     *        - 0 if p=1,
     *        - p otherwise;
     * - the third letter  of the \p alphabet labels transitions from p to    
     *        - 0 if p=\p n -1,
     *        - p otherwise;
     * - the other letters do not label any transition.
     *      
     * State 0 is initial, state n-1 is final.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * This automaton is the "universal witness" described in     
     * Janusz A. Brzozowski and David Liu,
     * *Universal Witnesses for State Complexity of Basic Operations Combined with Reversal*,     
     *  CIAA 2013,
     * Lecture Notes in Computer Science, vol. 7982, pp. 72-83.   
     * doi : 10.1007/978-3-642-39274-0_8
     *
     * @tparam Context the type of the context
     * @param ctx the context of the automaton
     * @param n an integer larger than 1
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is lesser than 2
     *                   or if the alphabet has less than 3 letters
     **/
  template <typename Context>
  mutable_automaton<Context>
  witness(const Context& ctx, unsigned n)
  {
    using context_t = Context;
    using automaton_t = mutable_automaton<context_t>;
    require(2 <= n, "u: n must be at least 3");
    const auto& gens = ctx.labelset()->genset();
    std::vector<typename context_t::labelset_t::letter_t> letters
      {std::begin(gens), std::end(gens)};
    require(3 <= letters.size(), "u: the alphabet needs at least 3 letters");
    automaton_t res = make_shared_ptr<automaton_t>(ctx);

    // The states.
    std::vector<state_t> states;
    for (unsigned i = 0; i < n; ++i)
      states.push_back(res->add_state());
    res->set_initial(states[0]);
    res->set_final(states[n-1]);

    // The 'a' transitions.
    auto a = letters[0];
    for (unsigned i = 0; i < n; ++i)
      res->new_transition(states[i], states[(i+1) % n], a);

    // The 'b' transitions.
    auto b = letters[1];
    res->new_transition(states[0], states[1], b);
    res->new_transition(states[1], states[0], b);
    for (unsigned i = 2; i < n; ++i)
      res->new_transition(states[i], states[i], b);

    // The 'c' transitions.
    auto c = letters[2];
    for (unsigned i = 0; i < n - 1; ++i)
      res->new_transition(states[i], states[i], c);
    res->new_transition(states[n - 1], states[0], c);

    return res;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_U_HH
