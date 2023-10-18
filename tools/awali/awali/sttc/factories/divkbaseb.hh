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

#ifndef AWALI_ALGOS_DIVKBASEB_HH
# define AWALI_ALGOS_DIVKBASEB_HH

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/alphabets/char.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc {


    /** Returns an automaton which recognizes numbers in the given \p base which are multiple of \p divisor
     *
     * The factory builds a deterministic automaton with \p divisor states.
     * If the alphabet has more than \p base letters, the larger letters do not label any transition.    
     *
     * Every transition is created with weight one; thus, if weighted,
     * the automaton realizes the characteristic series of the language described above.
     *
     * @tparam Context the type of the context
     * @param ctx the context of the automaton
     * @param divisor a positive integer
     * @param base a positive integer
     * @return an automaton with \p divisor states
     * @throw runtime_error if \p divisor is not positive,
     *                   or if \p base is smaller than 2,
     *                   or if the alphabet has less than \p base letters
     **/
  template <typename Context>
  mutable_automaton<Context>
  divkbaseb(const Context& ctx, unsigned divisor, unsigned base)
  {
    using context_t = Context;
    using automaton_t = mutable_automaton<context_t>;
    const auto& gens = ctx.labelset()->genset();
    std::vector<typename context_t::labelset_t::letter_t> letters
      {std::begin(gens), std::end(gens)};

    require(divisor,
            "divkbaseb: divisor cannot be 0");
    require(2 <= base,
            "divkbaseb: base (" + std::to_string(base)
            + ") must be at least 2");
    require(base <= letters.size(),
            "divkbaseb: base (" + std::to_string(base)
            + ") must be less than or equal to the alphabet size ("
            + std::to_string(letters.size()) + ")");

    automaton_t res = make_shared_ptr<automaton_t>(ctx);

    // Add one state for each possible remainder. The last state encountered
    // during the evaluation will be n % k. If the last state is the state 0,
    // it means that the residue is 0, ie the word will be accepted, ie the
    // number is a multiple of k.
    std::vector<state_t> states;
    for (unsigned i = 0; i < divisor; ++i)
      states.emplace_back(res->add_state());

    res->set_initial(states[0]);
    res->set_final(states[0]);

    for (unsigned i = 0; i < divisor; ++i)
      {
        int e = i * base;
        for (unsigned l = 0; l < base; ++l)
          {
            int d = (e + l) % divisor;
            res->new_transition(states[i], states[d], letters[l]);
          }
      }
    res->set_name("div-"+std::to_string(divisor)+"-base-"+std::to_string(base));
    return res;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_DIVKBASEB_HH
