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

#ifndef CORA_EDIT_TIP_CC
#define CORA_EDIT_TIP_CC

#include <cora/edit/tip.hh>

namespace awali{
namespace cora {
namespace editter {
    
unsigned tip_t::id_counter = 0;


tip_t::tip_t(std::string k, std::vector<std::string> h, 
             bool (*a)(dyn::automaton_t))
: key(k), hints(h), available(a) 
{}


tip_t::tip_t(std::string k, std::string hint, bool (*a)(dyn::automaton_t)) 
: tip_t(k, {hint}, a) 
{}


tip_t::tip_t(std::string k, std::initializer_list<std::string> l, 
             bool (*a)(dyn::automaton_t)) 
: tip_t(k, std::vector<std::string>(l), a) 
{}


bool
tip_t::available_for(dyn::automaton_t aut) 
const 
{ 
  if (available == nullptr)
    return true;
  else
    return available(aut); 
}


std::string const& 
tip_t::hint(size_t length) 
const 
{
  for (std::string const& s: hints) {
    if (length >= s.size())
      return s;
  }
  return hints.back(); // FIXME: Could improve this.
}



static bool (*if_eps_disallowed)(dyn::automaton_t)
= [](dyn::automaton_t aut) {
  return !aut->get_context()->is_eps_allowed();
};


static bool (*if_not_transducer)(dyn::automaton_t)
= [](dyn::automaton_t aut) {
  return !aut->is_transducer();
};


static bool (*has_available_promotion)(dyn::automaton_t)
= [](dyn::automaton_t aut) {
  dyn::context::weightset_description wd
    = dyn::context::weightset(aut->get_context()->weightset_name());
  std::vector<std::string> const& proms
    = dyn::context::weightset_promotion_public_static_names(wd);
  return !proms.empty();
};


tip_handler_t::tip_handler_t(dyn::automaton_t aut) 
: tip_lists(10),
  blank(             "",  ""),
  change_mode(    "<->", {"change mode", "chg mode"}),
  display(          "w", {"display"} ),
  quit(             "q", {"quit"} ),
  save(             "s", {"save automaton", "save"}),
  accept(         "RET", {"accept"}),
  select(         "RET", {"select"}),
  cancel(          "^X", {"cancel"}),
  navigate(       "<->", {"navigate"}),
  add_state(        "a", {"add new state", "add state", "add stt."}),
  rename_state (    "n", {"rename state", "rename stt."}),
  delete_state(     "d", {"rename state", "del. state", "del. stt."}),
  boolean_init(     "i", {"toggle initial status", "toggle init. status",
                          "toggle initial",        "toggle init.", 
                          "tggl init"                                     }),
  weighted_init(    "i", {"set initial weight", "set init. weight",
                          "init. weight",      "init weight"        }),
  boolean_final(    "f", {"toggle final status", "toggle final", 
                          "tggl final"                           }),
  weighted_final(   "f", {"set final weight", "final weight" }),
  add_transition(   "a", {"add new transition", "add transition", 
                          "add trans.",         "add tr."         }),
  edit_transition(  "e", {"edit transition", "edit trans.", "edit tr."}),
  delete_transition("d", {"delete transition", "del. transition",
                          "del. trans.",       "del. tr."        }),
  add_letter(       "l", {"add letter"}, if_not_transducer ),
  allow_eps_trans(  "e", {"allow epsilon-transitions", 
                          "allow epsilon-trans.", 
                          "allow epsilon-tr.",         
                          "allow eps."                 },
                    if_eps_disallowed                     ),
  promote_weightset("p", {"promote weightset", "promote ws."},
                    has_available_promotion                    ),
  rename_automaton( "n", {"rename automaton", "rename aut."}),
  edit_caption(     "c", {"edit caption", "caption"})
{
  tip_lists[MAIN_STATE]
    = {&add_state, &rename_state, &boolean_init, &boolean_final, &delete_state,
       &blank, &change_mode, &display, &save, &quit};
  
  tip_lists[MAIN_TRANS]
  = {&add_transition, &edit_transition, &delete_transition,
     &blank, &change_mode, &display, &save, &quit};

  tip_lists[MAIN_META]
  = {&add_letter, &allow_eps_trans, &promote_weightset, &rename_automaton,
     &edit_caption, 
     &blank, &change_mode, &display, &save, &quit};

  tip_lists[ASK] = {&accept, &cancel};
  tip_lists[TRANSEDIT] = {&navigate, &select, &cancel};


  update_alternatives(aut);
}


bool 
tip_handler_t::update_alternatives(dyn::automaton_t aut)
{
  bool modified = false;
  if (aut->get_context()->weightset_name() == "B") {
    if (tip_lists[MAIN_STATE][2]->id != boolean_init.id) {
      tip_lists[MAIN_STATE][2] = &boolean_init;
      modified = true;
    }
    if (tip_lists[MAIN_STATE][3]->id != boolean_final.id) {
      tip_lists[MAIN_STATE][3] = &boolean_final;
      modified = true;
    }
  } 
  else {
    if (tip_lists[MAIN_STATE][2]->id != weighted_init.id) {
      tip_lists[MAIN_STATE][2] = &weighted_init;
      modified = true;
    }
    if (tip_lists[MAIN_STATE][3]->id != weighted_final.id) {
      tip_lists[MAIN_STATE][3] = &weighted_final;
      modified = true;
    }
  }
  return modified;
}


tip_list_t const& 
tip_handler_t::for_state(editstate_t state)
{
  return tip_lists[state];
}

} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
