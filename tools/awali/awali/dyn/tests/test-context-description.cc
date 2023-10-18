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

#include <awali/dyn.hh>
#include <fstream>
#include <cassert>

using namespace awali::dyn::context;
using namespace awali::dyn;
using namespace awali;

using ls_t=labelset_description;
using ws_t=weightset_description;
using ctx_t=context_description;

int main() {
  
  std::string separator = std::string(20,' ')+std::string(40,'-')+std::string(20,' ');
  ls_t lal_ab      = letterset({'a','b'}),
       lan_ab      = nullableset(lal_ab),
       law_ab      = wordset({'a','b'}),
       lal_09      = intletterset(0,9),
       lan_09      = nullableset(lal_09),
       lat_lan     = ltupleset({lan_ab,lan_ab}),
       lat_lan_lal = ltupleset({lan_ab,lal_ab}),
       lat_lal_lan_law = ltupleset({lal_ab,lan_09,law_ab})
  ;

  std::vector<ls_t> labelsets = { lal_ab, lan_ab, law_ab, lal_09, lan_09, 
                                  lat_lan, lat_lan_lal, lat_lal_lan_law };
  
  std::cout << tostring(lal_ab,true) << std::endl;

  assert(!lal_ab->is_lao());
  assert( lal_ab->is_lal());
  assert(!lal_ab->is_lan());
  assert(!lal_ab->is_law());
  assert(!lal_ab->is_lat());
  

  std::cout << separator << std::endl
            << tostring(lan_ab,true) << std::endl;

  assert(!lan_ab->is_lao());
  assert(!lan_ab->is_lal());
  assert( lan_ab->is_lan());
  assert(!lan_ab->is_law());
  assert(!lan_ab->is_lat());


  std::cout << separator << std::endl
            << tostring(law_ab,true) << std::endl;

  assert(!law_ab->is_lao());
  assert(!law_ab->is_lal());
  assert(!law_ab->is_lan());
  assert( law_ab->is_law());
  assert(!law_ab->is_lat());

  
  std::cout << separator << std::endl
            << tostring(lal_09,true) << std::endl;

  assert(!lal_09->is_lao());
  assert( lal_09->is_lal());
  assert(!lal_09->is_lan());
  assert(!lal_09->is_law());
  assert(!lal_09->is_lat());


  std::cout << separator << std::endl
            << tostring(lan_09,true) << std::endl;

  assert(!lan_09->is_lao());
  assert(!lan_09->is_lal());
  assert( lan_09->is_lan());
  assert(!lan_09->is_law());
  assert(!lan_09->is_lat());


  std::cout << separator << std::endl
            << tostring(lat_lan,true) << std::endl;

  assert(!lat_lan->is_lao());
  assert(!lat_lan->is_lal());
  assert(!lat_lan->is_lan());
  assert(!lat_lan->is_law());
  assert( lat_lan->is_lat());


  std::cout << separator << std::endl
            << tostring(lat_lal_lan_law,true) << std::endl;

  assert(!lat_lal_lan_law->is_lao());
  assert(!lat_lal_lan_law->is_lal());
  assert(!lat_lal_lan_law->is_lan());
  assert(!lat_lal_lan_law->is_law());
  assert( lat_lal_lan_law->is_lat());


  ws_t B     = weightset("B"),
       Z     = weightset("N"),
       Q     = weightset("Q"),
       R     = weightset("R"),
       Zmin  = weightset("Z-min-plus"),
       ZZ3   = weightset("Z/3Z"),
       NN7   = weightset("N7")
  ;
  
  std::vector<ws_t> weightsets = {B, Z, Q, R, Zmin, ZZ3, NN7};



  for (ls_t const& ls : labelsets) {
    for (ws_t const& ws : weightsets){
      std::cout << std::string(80,'=') << std::endl;
      std::cout << "labelset: " << tostring(ls,true) << std::endl;
      std::cout << "weightset: " << tostring(ws, true) << std::endl;

      std::cout << separator << std::endl
                << "Building automaton from context_description" << std::endl;

      automaton_t a(ls,ws);
      std::cout << a << std::endl;

      std::cout << separator << std::endl
                << "Computing context_description back from automaton" 
                << std::endl;
      ctx_t ctx = a->get_context()->description();

      std::cout << "Labelset got: " << tostring(ctx->labelset(), true) 
                << std::endl;
      if (!(*ctx->labelset() == *ls))
        throw std::runtime_error("Labelsets are not equal");

      std::cout << "weightset got: " << tostring(ctx->weightset(), true) 
                << std::endl;
      if (!(*ctx->weightset() == *ws))
        throw std::runtime_error("Weightsets are not equal");



    }
  }

  return 0;
}
