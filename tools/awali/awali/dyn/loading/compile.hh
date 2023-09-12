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

#ifndef AWALI_DYN_LOADING_COMPILE_HH
#define AWALI_DYN_LOADING_COMPILE_HH


#include <string>

namespace awali {
  namespace dyn {
    namespace loading {
      
      std::string libname(const std::string& ctx, const std::string& name);
      std::string libname(const std::string& ctx1, const std::string& ctx2,
                          const std::string& name);
      
      std::string
      make_library(const std::string &name, const std::string &static_context,
                   bool check_dependency = false);
      std::string
      make_library(const std::string &name, const std::string &static_context1,
                   const std::string &static_context2,
                   bool check_dependency = false);
    }
  }
}

#endif
