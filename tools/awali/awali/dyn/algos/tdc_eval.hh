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

#ifndef DYN_TDC_EVAL_EVAL_HH
#define DYN_TDC_EVAL_EVAL_HH

#include <awali/dyn/modules/transducer.hh>
#include <awali/dyn/modules/ratexp.hh>
#include<string>


namespace awali {
  namespace dyn {

    /** \ingroup Products
     */
    automaton_t eval_tdc(automaton_t aut, transducer_t tdc);
    
    ratexp_t
    eval_exp (ratexp_t exp, transducer_t tdc);

    ratexp_t eval_word(transducer_t tdc, const std::string& word);
  }
}

#endif
