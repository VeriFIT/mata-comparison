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

#include <awali/dyn.hh>

using namespace awali::dyn;

int main() {
  // Creates an empty automaton with an empty alphabet and that allows
  // epsilon-transitions.
  automaton_t aut= automaton_t::from("",true);
  aut->set_name("url-validator");
  aut->set_desc("Automaton with epsilon transition that validates url");


  state_t protocol=aut->add_state("protocol");  //Add a state named "protocol"
  state_t domain=aut->add_state("domain");      //      --          "domain"
  state_t first_level=aut->add_state("first level"); // etc.
  state_t port=aut->add_state("port");
  state_t path=aut->add_state("path");
  state_t name=aut->add_state("name");
  state_t variable=aut->add_state("variable");
  state_t key=aut->add_state("key");
  state_t value=aut->add_state("value");
  state_t kv=aut->add_state();

  aut->set_initial(protocol);

  // Adds the states and transitions required such that every words of
  // expression  "https?://"  labels a path from state `protocol` to state 
  // `domain`.
  // Since the last argument is false, the alphabet is not strict and
  // encountered letters that are not part of the alphabet of `aut` are added
  // to the alphabet automatically.
  add_path(aut, protocol, domain, "https?://", false);



  add_path(aut, domain, first_level, "[a-z]((-*+.)[a-z])*.", false);
  add_path(aut, first_level, port, "fr+uk+com+org+net+de+info+be+ca+it", false);
  add_path(aut, port, path, "(:[1-9][0-9]*)?", false);
  add_path(aut, path, name, 
           "(/(['-.0-9A-Z_a-z]{1,}/)*['-.0-9A-Z_a-z]*)?", 
           false);
  add_path(aut, name, variable, "(#['-.0-9A-Z_a-z]*)?", false);
  aut->set_final(variable);
  add_path(aut, variable, key, "[?]", false);
  aut->set_final(key);
  add_path(aut, key, kv, "['-.0-9A-Z_a-z]");
  aut->set_final(kv);
  add_path(aut, kv, kv,"['-.0-9A-Z_a-z]");
  add_path(aut, kv, value, "=", false);
  aut->set_final(value);
  add_path(aut, value, key, "&", false);
  add_path(aut, value, value, "['-.0-9A-Z_a-z]");

  for (state_t s : aut->states()) {
    if (!aut->has_explicit_name(s)) {
      aut->set_state_name(s,"");
    } 
  }

  // Writes the resulting automaton to the standard output (in json format)
  std::cout << aut << std::endl;

  return 0;
}

  
