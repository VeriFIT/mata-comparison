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

#ifndef AWALI_ALGOS_ADD_PATH_HH
# define AWALI_ALGOS_ADD_PATH_HH

#include <awali/sttc/algos/standard.hh>
#include <awali/sttc/algos/compact_thompson.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh>
#include <awali/sttc/core/rat/ratexpset.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/labelset/traits.hh> // ratexpset_of

namespace awali {
  namespace sttc {
   
    /*,---------------------------.
      | add_path(aut, p, q, exp). |
      `---------------------------'*/
    /** @brief adds a subautomaton realizing 
     * the series \p exp between states 
     * \p p and \p q of \p aut.
     *
     * @tparam Aut the type of the automaton
     * @param aut the automaton in which the subautomaton is inserted
     * @param p the source state
     * @param q the destination state
     * @param exp the inserted series
     * @throw std::runtime_error if \p aut does not allow 
     * epsilon-transitions and \p exp is not proper
     *
     * The behaviour of the function depends whether 
     * the automaton \p aut allows epsilon-transitions.
     *
     * -- If it does, a {@link compact_thompson} automaton 
     *   is built from
     *   \p exp and copied between states \p p and \p q.
     *
     * -- If it does not, a {@link standard} automaton is
     *    built from
     *    \p exp and copied between states \p p and \p q.
     *    In this case, the series \p exp must be proper.
    */
      template <typename Aut>
      auto add_path(Aut& aut, state_t p, state_t q, const typename context_t_of<Aut>::ratexp_t& exp)
        -> typename std::enable_if<!labelset_t_of<Aut>::has_one(),void>::type {
        const auto& ws = *aut->weightset();
        auto ste = standard<Aut>(aut->context(), exp);
        state_t ste_init = ste->dst_of(*(ste->initial_transitions().begin()));
        require(!ste->is_final(ste_init),__func__," : proper series required");
        std::unordered_map<state_t,state_t> map;
        for(auto s : ste->states())
          if(s!=ste_init)
            map[s]=aut->add_state();
          else
            map[s]=p;
        for(auto tr : ste->transitions())
          aut->new_transition(map[ste->src_of(tr)],
                              map[ste->dst_of(tr)],
                              ste->label_of(tr),
                              ste->weight_of(tr));
        for(auto ftr : ste->final_transitions()) {
          auto sq=ste->src_of(ftr);
          for(auto tr : ste->in(sq)) {
            auto sp=ste->src_of(tr);
            aut->add_transition(map[sp], q, ste->label_of(tr), ws.mul(ste->weight_of(tr),ste->weight_of(ftr)));
          }
        }
        for(auto ftr : ste->final_transitions()) {
          auto sq=ste->src_of(ftr);
          if(sq!=ste_init) {
            auto outit = ste->out(sq);
            if(outit.begin()==outit.end())
              aut->del_state(map[sq]);
          }
        }
      }
      
      template <typename Aut>
      auto add_path(Aut& aut, state_t p, state_t q, const typename context_t_of<Aut>::ratexp_t& exp)
        -> typename std::enable_if<labelset_t_of<Aut>::has_one(),void>::type
      {
        auto ste = compact_thompson<Aut>(aut->context(), exp);
        std::unordered_map<state_t,state_t> map;
        state_t ste_init = ste->dst_of(*(ste->initial_transitions().begin()));
        state_t ste_fin = ste->src_of(*(ste->final_transitions().begin()));
        for(auto s : ste->states())
          if(s==ste_init)
            map[s]=p;
          else if(s==ste_fin)
            map[s]=q;
          else
            map[s]=aut->add_state();
        for(auto tr : ste->transitions())
          aut->add_transition(map[ste->src_of(tr)],
                              map[ste->dst_of(tr)],
                              ste->label_of(tr),
                              ste->weight_of(tr));
      }                                                               
      
    /** @brief adds a subautomaton realizing 
     * the series \p s between states 
     * \p p and \p q of \p aut.
     *
     * @tparam Aut the type of the automaton
     * @param aut the automaton in which the subautomaton is inserted
     * @param p the source state
     * @param q the destination state
     * @param s the inserted series
     * @param strict_alphabet if `true`, the letters in the expression must belong 
     * to the alphabet of the automaton; otherwise, they can be added to the alphabet
     * @throw std::runtime_error if \p aut does not allow 
     * epsilon-transitions and \p s is not proper
     * @throw parse_exception if the expression is malformed
     * @throw domain_error if \p strict_alphabet is `true` and a letter does not belong to the alphabet
     *
     * The string \p s is parsed to obtain a rational expression;
     * the function {@link add_path} is then called
    */
    template <typename Aut>
    void add_path(Aut& aut, state_t p, state_t q, const std::string& s, bool strict_alphabet=true) {
      auto rs=ratexpset_of<context_t_of<Aut>>(get_rat_context(aut->context()),
                                                         rat::identities::associativity);
      add_path(aut, p, q, parse_exp(rs, s, true, strict_alphabet));
    }
  }
}

#endif // !AWALI_ALGOS_ADD_PATH_HH
