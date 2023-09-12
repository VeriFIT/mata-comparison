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

#include<awali/sttc/weightset/b.hh>
#include<awali/sttc/weightset/n.hh>
#include<awali/sttc/weightset/z.hh>
#include<awali/sttc/weightset/q.hh>
#include<awali/sttc/weightset/r.hh>
#include<awali/sttc/weightset/c.hh>
#include<awali/sttc/weightset/f2.hh>
#include<awali/sttc/weightset/nn.hh>
#include<awali/sttc/weightset/noo.hh>
#include<awali/sttc/weightset/zz.hh>
#include<awali/sttc/weightset/pmax.hh>
#include<awali/sttc/weightset/zmax.hh>
#include<awali/sttc/weightset/zmin.hh>
#include<awali/sttc/weightset/maxmin.hh>
#include<awali/sttc/weightset/is_finite.hh>
#include<awali/sttc/misc/raise.hh>

using namespace awali::sttc;

template<typename WS>
void test(const WS& ws, bool locally_finite=false, bool finite=false) {
  require((is_finite(ws) == finite), "Semiring ",WS::sname(),": wrong finite status");
  require((is_locally_finite(ws) == locally_finite), "Semiring ",WS::sname(),": wrong locally finite status");
}


int main() {
  test(b(),true,true);
  test(n());
  test(z());
  test(q());
  test(r());
  test(c());
  test(f2(), true, true);
  test(nn<3>(), true, true);
  test(noo());
  test(zz<5>(), true, true);
  test(pmax());
  test(zmax());
  test(zmin());
  test(maxmin(), true);
  return 0;
}
