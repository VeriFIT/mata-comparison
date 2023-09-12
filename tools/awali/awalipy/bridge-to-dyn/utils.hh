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

#ifndef PY_UTILS_HH
#define PY_UTILS_HH

#include<sstream>

namespace awali { namespace py {


    static 
    std::string weight_to_string(dyn::any_t const& w, dyn::context_t const ctx) 
    {
      return ctx->weight_to_string(w);
    }
    static 
    std::string label_to_string(dyn::any_t const& w, dyn::context_t const ctx) 
    {
      return ctx->label_to_string(w);
    }

// Do not use.
//     static std::string any_to_string(dyn::any_t const& a) {
//       std::ostringstream o;
//       o << a;
//       return o.str();
//     }


}}// end of namespace awali::py

#endif
