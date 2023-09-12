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

#ifndef CORA_EDIT_INPUT_CC
#define CORA_EDIT_INPUT_CC

#include <cora/edit/input.hh>
#include <map>

namespace awali{
namespace cora {
namespace editter {

const std::map<signed char, input_kind_t> input_t::kinds = 
  { {NORMAL,NORMAL}, {UP,UP}, {LEFT,LEFT}, {DOWN,DOWN}, {RIGHT,RIGHT},
    {BACKSPACE,BACKSPACE}, {DELETE,DELETE}, {BACK,BACK}, {NEXT,NEXT},
    {TAB,TAB}, {OTHER,OTHER} };


input_kind_t 
input_t::kind() 
const 
{ 
  if (_value>0) 
    return NORMAL; 
  else 
    return kinds.find(_value)->second; 
}


input_t::internal_t
input_t::raw_value() 
const
{ 
  return _value; 
}


char 
input_t::value() 
const { 
  if (_value<0) 
    return 0; 
  else 
    return _value; 
}


} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
