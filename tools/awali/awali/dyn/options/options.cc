#ifndef AWALI_DYN_OPTIONS_OPTIONS_CC
#define AWALI_DYN_OPTIONS_OPTIONS_CC

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

#include <algorithm>
#include <awali/dyn/options/options.hh>
#include <utility>

namespace awali { namespace dyn {

  options_t::options_t(
      std::initializer_list<internal::option_value_pair_t> list)
  {
    for (internal::option_value_pair_t const& pair : list)
      *this += pair;
  }


  options_t& options_t::operator+=(internal::option_value_pair_t const& pair)
  {
    map.emplace(pair.id, pair.value);
    return *this;
  }


  options_t& options_t::operator+=(options_t const& other)
  {
    map.insert(other.map.begin(), other.map.end());
    return *this;
  }


  options_t
  options_t::operator+(internal::option_value_pair_t const& pair) const
  {
    return (*this) + options_t(pair);
  }


  options_t options_t::operator+(options_t const& other) const
  {
    options_t opt = {};
    opt += (*this);
    opt += other;
    return opt;
  }


}} // namespace awali::dyn


#endif
