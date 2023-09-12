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


#ifndef COMMON_VERSION_HH
#define COMMON_VERSION_HH

#include<string>

namespace awali {
namespace version {

  /** Current major version of Awali. */
  static constexpr unsigned major = 2;

  /** Current minor version of Awali. */
  static constexpr unsigned minor = 0;

  /** Current minor version of Awali. */
  static constexpr unsigned patch = 7;

  /** Suffix of the version of Awali, used mostly by developers.*/
  static const std::string suffix = "";

  /** Completely version of Awali as a std::string. */
  static const std::string full 
    = std::to_string(major) + "." + std::to_string(minor)
      + "." + std::to_string(patch) + (suffix=="" ? "" : ("-"+suffix));


  /** Main version of the format FSM json used in the library.
   *  Versions lesser than {@link fsm_json} are usually supported fully for
   *  input but not necessarily for output.
   *  The behavior is undefined when using versions greater than 
   *  {@link fsm_json} is undefined; usually {@link fsm_json} is used instead. 
   * */
  static constexpr unsigned fsm_json = 1;

}// end of namespace awali::version
}// end of namespace awali

#endif
