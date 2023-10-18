
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

#ifndef CORA_EDITMESSAGES_HH
#define CORA_EDITMESSAGES_HH

#include <cora/edit/token.hh>

namespace awali {
namespace cora {

//   void print_message(sentence_t tokens);

namespace message {

  
  sentence_t const unknown_action("Key '%s' is bound to no action.");
  sentence_t const unknown_keycode("Key code '%s' is unknown.");

} // end of ns awali::cora::messages
} // end of ns awali::cora
} // end of ns awali



#endif
