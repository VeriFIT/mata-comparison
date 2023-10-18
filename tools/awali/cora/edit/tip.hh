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

#ifndef CORA_EDIT_TIP_HH
#define CORA_EDIT_TIP_HH

#include<awali/dyn/core/automaton.hh>
#include<cora/edit/enums.hh>
#include<vector>

namespace awali{
namespace cora {
namespace editter {

  // Shortcut hints
  class tip_t {
    static unsigned id_counter;
  public:
    const unsigned id = id_counter++;
    std::string key;
    std::vector<std::string> hints;
    bool (*available)(dyn::automaton_t);
      
    
    tip_t(std::string k, std::vector<std::string> h, 
          bool (*a)(dyn::automaton_t) = nullptr);
    
    /* Case with one string defers to list */
    tip_t(std::string k, 
          std::string hint, 
          bool (*a)(dyn::automaton_t) = nullptr);

    /* Desambiguate the case where second arg is an initializer list. */
    tip_t(std::string k, 
          std::initializer_list<std::string> l, 
          bool (*a)(dyn::automaton_t) = nullptr);

    bool available_for(dyn::automaton_t aut) const;
    std::string const& hint(size_t length) const;
  };


  typedef std::vector<tip_t const *> tip_list_t;

  class tip_handler_t {
   
   private:
    /** indexed by editstate_t */
    std::vector<tip_list_t>  tip_lists;

   public:
    const tip_t blank, 
                change_mode,
                display,
                quit,
                save, 
                accept,
                select,
                cancel,
                navigate,
                add_state, 
                rename_state, 
                delete_state, 
                boolean_init, weighted_init,
                boolean_final, weighted_final,
                add_transition,
                edit_transition,
                delete_transition,
                add_letter,
                allow_eps_trans,
                promote_weightset,
                rename_automaton,
                edit_caption
                ;
    tip_list_t const& for_state(editstate_t state);
    
    /* Changes the tips depending of the given automaton. */
    bool update_alternatives(dyn::automaton_t aut);

    tip_handler_t(dyn::automaton_t aut);
  };



} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
