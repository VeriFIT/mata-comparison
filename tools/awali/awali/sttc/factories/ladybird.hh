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

#ifndef AWALI_ALGOS_LADYBIRD_HH
# define AWALI_ALGOS_LADYBIRD_HH

# include <vector>

#include <awali/sttc/alphabets/char.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc {

   /** Returns a "ladybird" automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z.    
     * For every state p,     
     * - the first letter of the alphabet labels transitions from p to p+1;    
     * - the second letter of the alphabet labels transitions from p to p;    
     * - the third letter of the alphabet labels transitions from p to p and from p to 0;   
     * -- If the alphabet has more than 3 letters, the other letters do not label any transition.
     * State 0 is both initial and final.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * The minimal automaton of the language accepted by ladybird \p n has 2^\p n states.    
     * The name ladybird comes from the shape of the 6-state automaton.
     *
     * @tparam Context the type of the context
     * @param ctx the context of the automaton
     * @param n a positive integer
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is not positive
     *                   or if the alphabet has less than 3 letters
     **/
  template <class Context>
  mutable_automaton<Context>
  ladybird(const Context& ctx, unsigned n)
  {
    using context_t = Context;
    const auto& gens = ctx.labelset()->genset();
    std::vector<typename context_t::labelset_t::letter_t> letters
      {std::begin(gens), std::end(gens)};
    require(3 <= letters.size(),
            "ladybird: the alphabet needs at least 3 letters");
    require(n>0,
            "ladybird: the automaton should have at least one state");
    auto a = letters[0];
    auto b = letters[1];
    auto c = letters[2];

    using automaton_t = mutable_automaton<Context>;
    automaton_t res = make_shared_ptr<automaton_t>(ctx);

    auto p = res->add_state();
    res->set_initial(p);
    res->set_final(p);
    auto x = p;
    for (unsigned i = 1; i < n; ++i)
      {
        auto y = res->add_state();
        res->new_transition(x, y, a);
        res->new_transition(y, y, b);
        res->new_transition(y, y, c);
        res->new_transition(y, p, c);
        x = y;
      }
    res->new_transition(x, p, a);
    res->set_name("ladybird-"+std::to_string(n));
    return res;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_LADYBIRD_HH
