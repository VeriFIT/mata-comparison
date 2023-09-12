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

#ifndef CORA_EDIT_VALIDATOR_CC
#define CORA_EDIT_VALIDATOR_CC

#include<cora/edit/validator.hh>

namespace awali { 
namespace cora {
namespace editter {



bool 
simple_validator_t::validate(std::string const& str) 
{
  bool b = _function(str);
  if (!b)
    _error_message = sentence_t(_proto_error_message) /= str;
  return b;
}


sentence_t const& 
simple_validator_t::error_message() const
{
  return _error_message;
}



simple_validator_t::simple_validator_t( 
  std::function<bool(std::string const&)> is_valid, sentence_t prototype)
: _proto_error_message(prototype),
  _function(is_valid)
{}


/* ************************************************************************** *
 *                         combined_validator_t                               *
 * ************************************************************************** */
bool 
combined_validator_t::validate(std::string const& str)
{
  for (size_t i=0; i< _validators.size(); i++) {
    if (!_validators[i]->validate(str)) {
      _error_is_in = i;
      return false;
    }
  }
  return true;
}


sentence_t const& 
combined_validator_t::error_message()
const
{
  return _validators[_error_is_in]->error_message();
}


combined_validator_t::combined_validator_t(
    std::initializer_list<abstract_validator_t*> validators)
: _validators(validators)
{}


} // end of namespace awali::cora::editter
} // end of namespace awali::cora
} // end of namespace awali

#endif
