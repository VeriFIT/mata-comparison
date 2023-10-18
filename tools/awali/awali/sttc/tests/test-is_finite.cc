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
#include <awali/sttc/tests/null_stream.hxx>

using namespace awali;
using namespace awali::sttc;

std::ostream * osc;

template<typename WS>
void test(const WS& ws, star_status_t star_status, bool show_one=true, bool locally_finite=false, bool finite=false) {
  *osc << "------------" << std::endl
       << "Semiring " << WS::sname() << std::endl
       << "------------" << std::endl;
  require((is_finite(ws) == finite), "Semiring ",WS::sname(),": wrong finite status");
  *osc <<  "Finite status \t -> OK" << std::endl;
  require((is_locally_finite(ws) == locally_finite), "Semiring ",WS::sname(),": wrong locally finite status");
  *osc << "Locally finite status \t -> OK" << std::endl;
  require((ws.is_commutative_semiring() == true), "Semiring ",WS::sname(),": wrong commutativity status");
  *osc << "Commutativity status \t -> OK" << std::endl;
  require((ws.show_one() == show_one), "Semiring ",WS::sname(),": wrong 'show one' status");
  *osc << "'Show one' status \t -> OK" << std::endl;
  require((ws.star_status() == star_status), "Semiring ",WS::sname(),": wrong star status");  
  *osc << "Star status \t -> OK" << std::endl;
}


int main(int argc, char **argv) {
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;
  
  test(b(),star_status_t::STARRABLE,false,true,true);
  test(n(),star_status_t::NON_STARRABLE,false);
  test(z(),star_status_t::NON_STARRABLE,false);
  test(q(),star_status_t::ABSVAL,false);
  test(r(),star_status_t::ABSVAL,false);
  test(c(),star_status_t::ABSVAL,false);
  test(f2(),star_status_t::NON_STARRABLE,false, true, true);
  test(nn<3>(),star_status_t::STARRABLE,false, true, true);
  test(noo(),star_status_t::STARRABLE,false);
  test(zz<5>(),star_status_t::NON_STARRABLE,false, true, true);
  test(pmax(),star_status_t::TOPS,false);
  test(zmax(),star_status_t::TOPS);
  test(zmin(),star_status_t::TOPS);
  test(maxmin(),star_status_t::STARRABLE,true , true);
  return 0;
}
