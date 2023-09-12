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

#ifndef CORA_EDIT_ENUMS_HH
#define CORA_EDIT_ENUMS_HH

namespace awali{
namespace cora {
namespace editter {

  enum input_kind_t { 
    NORMAL=0,                              // Char between 32 and 126
    UP=-128, LEFT, DOWN, RIGHT,            // Arrow keys
    BACKSPACE=-112, DELETE,                // Suppr previous/next char
    BACK=-96, NEXT,                        // ESCAPE/ENTER
    TAB=-80,                               // TAB
    OTHER=-1                               // Recognised char's that have no
                                           // effect (e.g. resize).
  };


  // editstate_t is the different states in which the program might be
  // waiting for characters.
  enum editstate_t {
    MAIN_STATE = 0,
    MAIN_TRANS = 1, 
    MAIN_META = 2, 
    ASK = 3,
    TRANSEDIT = 4,
  };

  const static editstate_t editstate_t_size = TRANSEDIT;


  enum answer_kind_t {
    ABORT = 0,  // The user aborted
    INPUT = 1,  // The user provided an answer
    DEFAULT = 2 // Default behaviour should be 
  };


} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
