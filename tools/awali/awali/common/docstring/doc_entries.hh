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


#ifndef AWALI_COMMON_DOCTSRING_DOCSTRINGS_HH
#define AWALI_COMMON_DOCTSRING_DOCSTRINGS_HH

#include <vector>
#include <string>

// Do not forget to include the entry files, or the compiler
// will cry and you wont understand why 
#include <awali/common/docstring/entry.hh>
#include <awali/common/docstring/eps_removal.hh>
// #include <awali/common/docstring/exp_to_aut.hh>
#include <awali/common/docstring/json_format.hh>
#include <awali/common/docstring/ratexp.hh>
#include <awali/common/docstring/reduction.hh>

namespace awali { namespace docstring {

  static std::string toescape = "\\<>[]\"";

  static std::vector<entry_t> entries = 
  {
	eps_removal,
// 	exp_to_aut,
    json_format,
    ratexp,
	reduction
  };

}}




#endif
