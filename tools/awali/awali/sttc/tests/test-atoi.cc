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

#include<awali/common/ato.cc>
#include<iostream>
#include<awali/sttc/misc/raise.hh>

using namespace awali;
using namespace awali::sttc;

int main() {
  bool b;
  try {
    b=false;
    strict_atoi("30945094509475094570");
  } catch(std::out_of_range& e) {
    b=true;
  }
  require(b, "Large int value not detected");
  require(strict_atoi("-237")==-237, "Wrong conversion of 237");
  try {
    b=false;
    strict_atoi("305.");
  } catch(parse_exception& e) {
    b=true;
  }
  require(b, "Partial parsing not detected");
  require(strict_atou("237")==237u, "Wrong conversion of 237u");
  try {
    b=false;
    strict_atou("-305");
  } catch(std::out_of_range& e) {
    b=true;
  }
  require(b, "Negative unsigned not detected");
  std::stringstream ss;
  ss << std::numeric_limits<unsigned>::max() << '0' << std::endl;
  try {
    b=false;
    strict_atou(ss.str());
  } catch(std::out_of_range& e) {
    b=true;
  }
  require(b, "Large unsigned not detected");    
  return 0;
}
