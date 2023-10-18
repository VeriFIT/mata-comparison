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

#include<awali/dyn/core/abstract_context.hh>
#include<awali/dyn/core/ratexp.hh>
#include<awali/dyn/core/automaton.hh>

namespace awali { namespace dyn {

ratexp_t 
abstract_context_t::make_ratexp(std::string str, bool fixed_alphabet) 
{ 
  return ratexp_t::from_context(
             str,
             std::shared_ptr<abstract_context_t>(this->copy()), 
             fixed_alphabet); 
}

automaton_t 
abstract_context_t::make_automaton () 
{ 
  return automaton_t::from_context(std::shared_ptr<abstract_context_t>(this->copy())); 
}

}}
