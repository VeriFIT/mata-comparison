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

#include <string>

#ifndef CORA_COLOR_HH
#define CORA_COLOR_HH

namespace awali { 
namespace cora {
	
const std::string red("\033[0;31m");
const std::string redb("\033[1;91m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;96m");
const std::string blueb("\033[0;94m");
const std::string magenta("\033[0;35m");
const std::string magentab("\033[0;95m");
const std::string reset_clr("\033[0m");

const std::string usage_clr = magentab;
const std::string error_clr = redb;
const std::string warning_clr = green;
const std::string arg_clr = magenta;
const std::string title_clr = blueb; //cyan


}}// end of namespaces awali::cora and awali


#endif // CORA_COLOR_HH
