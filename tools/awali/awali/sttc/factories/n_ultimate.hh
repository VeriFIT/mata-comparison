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

#ifndef AWALI_ALGOS_N_ULTIMATE_HH
# define AWALI_ALGOS_N_ULTIMATE_HH

# include <iterator> // std::distance
# include <stdexcept>

#include <awali/sttc/alphabets/char.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc {

    /** \ingroup Factories
     *  @brief Returns an automaton which recognizes words with a specific n-ultimate letter
     *
     * The factory builds an automaton with \p n +1 states.    
     * The alphabet must contain at least two letters.
     * The automaton accepts every word where the n-ultimate letter
     * is \p a.
     *
     * The automaton is co-deterministic hence unambiguous;
     * if weighted,
     * the automaton realizes the characteristic series of the language described above.
     *
     * @tparam Context the type of the context
     * @param ctx the context of the automaton
     * @param a a label
     * @param n a positive integer: the position to last of the specified letter
     * @return an automaton with \p n +1 states
     * @throw runtime_error if \p n is not positive or if the alphabet has less than 2 letters
     **/
  template <typename Context>
  mutable_automaton<Context>
  n_ultimate(const Context& ctx, typename Context::labelset_t::value_t a, unsigned n)
  {
    const auto& gens = ctx.labelset()->genset();
    size_t sz = std::distance(std::begin(gens), std::end(gens));
    require(2 <= sz, "n_ultimate: the alphabet needs at least 2 letters");
    require(n > 0, "n_ultimate: automaton defined for n>0");
    require(gens.has(a),"n_ultimate: ", "letter not in alphabet"); 
    using context_t = Context;
    using automaton_t = mutable_automaton<context_t>;
    automaton_t res = make_shared_ptr<automaton_t>(ctx);

    auto init = res->add_state();
    res->set_initial(init);
    for (auto l: gens)
      res->new_transition(init, init, l);

    auto prev = res->add_state();
    res->new_transition(init, prev, a);

    while (--n)
      {
        auto next = res->add_state();
        for (auto l: gens)
          res->new_transition(prev, next, l);
        prev = next;
      }
    res->set_final(prev);
    return res;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_N_ULTIMATE_HH
