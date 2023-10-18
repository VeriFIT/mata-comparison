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

#include <awali/common/priority.hh>
#include <stdexcept>
#include <iostream>

using namespace awali;

#define GENOK(x,y) template<typename P> void x (priority::y<P>) {std::cout << #x << "(priority::" << #y << "): OK" << std::endl;}
#define GENFAIL(x,y) template<typename P> void x (priority::y<P>) {throw std::runtime_error(std::string(#x)+"(priority::"+#y+"): FAIL");}

#define GEN2(x,y1,y2) GENFAIL(x,y1) GENOK(x,y2)
#define GEN3(x,y1,y2,y3) GENFAIL(x,y1) GENFAIL(x,y2) GENOK(x,y3)
#define GEN4(x,y1,y2,y3,y4) GENFAIL(x,y1) GENFAIL(x,y2) GENFAIL(x,y3) GENOK(x,y4)
#define GEN5(x,y1,y2,y3,y4,y5) GENFAIL(x,y1) GENFAIL(x,y2) GENFAIL(x,y3) GENFAIL(x,y4) GENOK(x,y5)

GEN2(f1, BOTTOM, LOW)
GEN2(f2, LOW,    MEDIUM)
GEN2(f3, MEDIUM, HIGH)
GEN2(f4, HIGH,   TOP)

GEN3(g1, LOW,    MEDIUM, HIGH)
GEN3(g2, BOTTOM, LOW,    MEDIUM)
GEN3(g3, BOTTOM, LOW,    TOP)
GEN3(g4, MEDIUM, HIGH,   TOP)

GEN4(h1, LOW,    MEDIUM, HIGH,   TOP)
GEN4(h2, BOTTOM, MEDIUM, HIGH,   TOP)
GEN4(h3, BOTTOM, LOW,    MEDIUM, HIGH)
GEN4(h4, BOTTOM, LOW,    HIGH,   TOP)

GEN5(i1, BOTTOM, LOW, MEDIUM, HIGH, TOP)

int main() {
  f1(priority::value);
  f2(priority::value);
  f3(priority::value);
  f4(priority::value);

  g1(priority::value);
  g2(priority::value);
  g3(priority::value);
  g4(priority::value);
  
  h1(priority::value);
  h2(priority::value);
  h3(priority::value);
  h4(priority::value);
  
  i1(priority::value);
}
