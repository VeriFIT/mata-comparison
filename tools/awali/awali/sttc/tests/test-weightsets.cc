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
#include<awali/sttc/weightset/zz.hh>
#include<awali/sttc/weightset/nn.hh>
#include<awali/sttc/weightset/noo.hh>
#include<awali/sttc/weightset/zmin.hh>
#include<awali/sttc/weightset/zmax.hh>
#include<awali/sttc/weightset/pmax.hh>
#include<awali/sttc/weightset/maxmin.hh>

#include<awali/sttc/misc/raise.hh>
#include <awali/sttc/tests/null_stream.hxx>

using namespace awali;
using namespace awali::sttc;

#ifndef AWALI_AUTOMATA_DEPOSITORY
#define AWALI_AUTOMATA_DEPOSITORY "../../automata/"
#endif

std::ostream * osc;

using namespace awali::sttc;

template<typename WS, typename T>
void print(WS ws, T x, T y, T z, char s) {
  ws.print(x,*osc) << ' ' << s << ' ';
  ws.print(y,*osc) << " = ";
  ws.print(z,*osc) << std::endl;
}

template<typename WS, typename T>
void printb(WS ws, T x, T y, bool b, char s) {
  ws.print(x,*osc) << ' ' << s << ' ';
  ws.print(y,*osc) << " -> " << std::boolalpha << b << std::noboolalpha << std::endl;
}

template<typename WS>
void test() {
  using value_t = typename WS::value_t;
  std::string s=WS::sname();
  *osc << "--- Test " << s << std::endl;
  WS ws;
  b bs;
  value_t z=ws.zero();
  *osc << "Zero "; ws.print(z,*osc) << std::endl;
  value_t o=ws.one();
  *osc << "One "; ws.print(o,*osc) << std::endl;
  require(ws.is_zero(z),s,": is_zero()");
  require(ws.is_one(o),s,": is_one()");
  require(!ws.is_zero(o),s,": is_zero(1)");
  require(!ws.is_one(z),s,": is_one(0)");
  value_t v=ws.add(z,z);
  print(ws, z, z, v, '+'); 
  require(ws.is_zero(v),s,": 0+0 != 0");
  v=ws.add(o,z);
  print(ws, o, z, v, '+'); 
  require(ws.is_one(v) ,s,": 1+0 != 1");
  v=ws.add(z,o);
  print(ws, z, o, v, '+'); 
  require(ws.is_one(v),s,": 0+1 != 1");
  v=ws.mul(z,z);
  print(ws, z, z, v, '.'); 
  require(ws.is_zero(v),s,": 0.0 != 0");
  v=ws.mul(o,z);
  print(ws, o, z, v, '.'); 
  require(ws.is_zero(v),s,": 1.0 != 0");
  print(ws, z, o, v, '.'); 
  require(ws.is_zero(v),s,": 0.1 != 0");
  v=ws.mul(o,o);
  print(ws, o, o, v, '.'); 
  require(ws.is_one(v),s,": 1.1 != 1");
  v=ws.star(z);
  ws.print(z,*osc) << "* = "; ws.print(v,*osc) << std::endl;
  require(ws.is_one(v),s,": 0* != 1");
  bool b=ws.equals(z,z);
  printb(ws, z, z, b, '=');
  require(b,s,": 0 != 0");
  b=ws.equals(o,o);
  printb(ws, o, o, b, '=');
  require(b,s,": 1 != 1");
  b=ws.equals(o,z);
  printb(ws, o, z, b, '=');
  require(!b,s,": 0 == 1");
  b=ws.less_than(z,o);
  printb(ws, z, o, b, '<');
  require(b,s,": !(0<1) ");
  b=ws.show_one();
  *osc << " show one -> " << b << std::endl;
  awali::star_status_t st=ws.star_status();
  value_t t=ws.transpose(o);
  size_t h = ws.hash(o);
  require(ws.is_one(ws.conv(ws,o)),s,": self conversion");
  awali::json::node_t* n = ws.value_to_json(o);
  delete n;
}

template<typename WS1, typename WS2, typename WSR=WS1>
void test_join() {
  WS1 ws1;
  WS2 ws2;
  WSR wr = join(ws1,ws2);
  std::string s1=WS1::sname();
  std::string s2=WS2::sname();
  *osc << "Test join " << s1 << ' ' << s2 << std::endl;
  wr = join(ws2,ws1);
  typename WS1::value_t o = ws1.conv(ws2, ws2.one());
  require(ws1.is_one(o),s1," conversion of one ",s2);
  typename WS1::value_t z = ws1.conv(ws2, ws2.zero());
  require(ws1.is_zero(z),s1," conversion of zero ",s2);
}






int main(int argc, char **argv) {
  if(argc==2)
    osc = &std::cout;
  else
    osc = &null_stream;
  
  test<b>();
  test<n>();
  test<z>();
  test<q>();
  test<r>();
  test<c>();
  test<f2>();
  test<zz<3>>();
  test<nn<3>>();
  test<noo>();
  test<zmin>();
  test<zmax>();
  test<pmax>();
  test<maxmin>();
  test_join<z,n>();
  test_join<q,n>();
  test_join<q,z>();
  test_join<r,n>();
  test_join<r,z>();
  test_join<r,q>();
  test_join<c,n>();
  test_join<c,z>();
  test_join<c,q>();
  test_join<c,r>();
  //test_join<f2,zz<2>>(); // add this test when join is implemented
  
  return 0;
}
