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

#ifndef DYN_MODULES_WORDS_CC
#define DYN_MODULES_WORDS_CC

#include <unordered_map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/loading/handler.hh>

#include <awali/dyn/modules/words.hh>

namespace awali {
  namespace dyn {

    automaton_t allow_words(automaton_t aut)
    {
      return loading::call1<automaton_t>("allow_words", "words", aut);
    }

    automaton_t letterize(automaton_t aut)
    {
      return loading::call1<automaton_t>("letterize", "words", aut);
    }

    automaton_t compact(automaton_t aut)
    {
      return loading::call1<automaton_t>("compact", "words", aut);
    }

    context_t context_words(automaton_t aut)
    {
      return loading::call1<context_t>("context_words", "words", aut);
    }
  }
}//end of ns awali::dyn

#endif
