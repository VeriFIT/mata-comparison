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

/* @file accessible.hh
 *
 * This files contains static functions computing accessible states.
 */
#ifndef AWALI_ALGOS_RESTRICTION_HH
#define AWALI_ALGOS_RESTRICTION_HH

#include <set>

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/labelset/traits.hh>

namespace awali {
  namespace sttc {
    
    namespace internal {
      template<typename LS>
      struct make_labelset_from_letters {
        template<typename SET>
        static LS make(const SET& letters) {
          return LS(letters);
        }
      };

      template<typename LS>
      struct make_labelset_from_letters<nullableset<LS>> {
        template<typename SET>
        static nullableset<LS> make(const SET& letters) {
          return nullableset<LS>(LS(letters));
        }
      };
      
    }

    /** @brief Remove letters from the alphabet of the automaton
     *  
     * This function modifies the context of the automaton;
     * it removes the list of specified letters from the alphabet of the automaton
     *
     * @tparam Aut the type of the automaton
     * @tparam Letter the type of the letters
     * @param aut the automaton
     * @param letters a set of letters
     * @param del_unvalid_transitions if true, the transitions which carry
     *     a label that is not valid w.r.t the new alphabet are deleted
     */
    template<typename Aut>
    void remove_letters(Aut& aut, const std::set<typename labelset_t_of<Aut>::letter_t>& letters, bool del_unvalid_transitions =true) {
      std::set<typename labelset_t_of<Aut>::letter_t> new_alphabet;
      for(auto l: aut->labelset()->genset())
        new_alphabet.insert(l);
      for(auto l: letters)
        new_alphabet.erase(l);
        context_t_of<Aut> ctx(internal::make_labelset_from_letters<labelset_t_of<Aut>>::make(new_alphabet), *(aut->weightset()));
      aut->ctx_ = ctx;
      if(del_unvalid_transitions) {
        auto labelset = aut->labelset();
        std::list<transition_t> to_erase;
        for(auto tr: aut->transitions())
          if(!labelset->is_valid(aut->label_of(tr)))
            to_erase.emplace_back(tr);
        for(auto tr: to_erase)
          aut->del_transition(tr);
      }
    }
    
        
    /** @brief Change letters in the alphabet of the automaton
     *  
     * This function replaces the context of the automaton by a new context
     * where the alphabet is given by the specified letters
     *
     * @tparam Aut the type of the automaton
     * @tparam Letter the type of the letters
     * @param aut the automaton
     * @param letters a set of letters
     * @param del_unvalid_transitions if true, the transitions which carry
     *     a label that is not valid w.r.t the new alphabet are deleted
     */
    template<typename Aut>
    void change_alphabet(Aut& aut, const std::set<typename labelset_t_of<Aut>::letter_t>& letters, bool del_unvalid_transitions =true) {
      context_t_of<Aut> ctx(internal::make_labelset_from_letters<labelset_t_of<Aut>>::make(letters), *(aut->weightset()));
      aut->ctx_ = ctx;
      if(del_unvalid_transitions) {
        auto labelset = aut->labelset();
        std::list<transition_t> to_erase;
        for(auto tr: aut->transitions())
          if(!labelset->is_valid(aut->label_of(tr)))
            to_erase.emplace_back(tr);
        for(auto tr: to_erase)
          aut->del_transition(tr);
      }
    }
 
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_RESTRICTION_HH
