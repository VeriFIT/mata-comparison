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

#include <awali/common/version.hh>
#include <stdexcept>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  if (argc != 5) {
    std::cerr << "./usage test-version <version_major> <version_minor> <version_patch> <version_suffix>";
    exit(1);
  }
  if (std::to_string(awali::version::major) != std::string(argv[1]))
    throw std::runtime_error("Major version does not coincide");
  if (std::to_string(awali::version::minor) != std::string(argv[2]))
    throw std::runtime_error("Minor version does not coincide");
  if (std::to_string(awali::version::patch) != std::string(argv[3]))
    throw std::runtime_error("Patch version does not coincide");
  if (awali::version::suffix != std::string(argv[4]))
    throw std::runtime_error("Suffix of version does not coincide");

  awali::version::check_fsmjson<1>();

  try {
    awali::version::check_fsmjson<0>();
    exit(1);
  } catch (std::runtime_error const&) {}
  try {
    awali::version::check_fsmjson<awali::version::fsm_json+1>();
    exit(1);
  } catch (std::runtime_error const&) {}
  
}
