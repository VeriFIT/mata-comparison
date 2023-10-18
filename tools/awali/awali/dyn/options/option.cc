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

#ifndef AWALI_DYN_OPTIONS_OPTION_CC
#define AWALI_DYN_OPTIONS_OPTION_CC

#include <algorithm>
#include <awali/dyn/options/option.hh>
#include <cctype>
#include <iterator>
#include <sstream>
#include <stdexcept>


namespace awali { namespace dyn { namespace internal {
  size_t next_id = 0;

  std::unordered_map<std::string, stov_t> registered_stov;

  void register_stov(std::string option_name, stov_t function)
  {
    if (registered_stov.count(option_name) == 0)
      registered_stov.emplace(std::move(option_name), std::move(function));
  }

  stov_t get_stov(std::string const& option_name)
  {
    std::string s;
    s.reserve(option_name.size());
    std::transform(option_name.begin(), option_name.end(),
                   std::back_insert_iterator<std::string>(s),
                   [](unsigned char c) -> unsigned char
                   {
                     if (c == '-')
                       return '_';
                     else
                       return std::toupper(c);
                   });
    auto it = registered_stov.find(s);
    if (it == registered_stov.cend()) {
      std::stringstream ss;
      ss << "No option is named \"" << option_name
         << "\" is registered. Admissible values (case insensitive): ";
      bool notfirst = false;
      for (auto const& pair : registered_stov) {
        if (notfirst)
          ss << ", ";
        else
          notfirst = true;
        ss << '"' << pair.first << '"';
      }
      ss << ".";
      throw std::out_of_range{ss.str()};
    }
    else
      return it->second;
  }
}}} // namespace awali::dyn::internal

#define INSTANTIATE_OPTIONS
#include <awali/dyn/options/declaration.incl.hh>
#undef INSTANTIATE_OPTIONS


#endif
