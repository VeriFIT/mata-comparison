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

#ifndef AWALI_DEMANGLE_CC
#define AWALI_DEMANGLE_CC

#include <string>


#ifdef __GNUG__

#include <cxxabi.h>


namespace awali { namespace internal {

  inline std::string demangle(char const* name)
  {
    int status = -4;
    char* res = abi::__cxa_demangle(name, NULL, NULL, &status);
    std::string str_res = (status == 0) ? res : name;
    if (res != nullptr)
      free(res);
    return str_res;
  }

}} // namespace awali::internal

#else

namespace awali { namespace internal {

  inline std::string demangle(const char* name)
  {
    return name;
  }

}} // namespace awali::internal

#endif

#endif
