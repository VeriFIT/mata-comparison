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


#include <awali/dyn/options/option.hh>
#include <awali/dyn/options/value.hh>

namespace awali { namespace dyn { namespace internal {

  option_value_pair_t::option_value_pair_t(std::string const& option_name,
                                           std::string const& option_value)
      : option_value_pair_t(get_stov(option_name)(option_value))
  {}

}}} // namespace awali::dyn::internal
