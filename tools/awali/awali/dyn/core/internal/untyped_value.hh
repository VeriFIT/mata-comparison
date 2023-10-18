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

#ifndef DYN_UNTYPED_VALUE_HH
#define DYN_UNTYPED_VALUE_HH

#include<iostream>

namespace awali { namespace dyn {
  struct any_t;

  namespace internal {
    struct untyped_value{
      untyped_value() {}
      untyped_value(const untyped_value&) {}
      virtual ~untyped_value(){}
      virtual untyped_value* clone() const = 0;
      virtual std::ostream& output(std::ostream& o) const = 0;
      virtual bool less(const untyped_value& uv) const = 0;
      virtual bool equal(const untyped_value& uv) const = 0;
      virtual std::ostream& real_type_name(std::ostream& o) const = 0;
      virtual std::ostream& real_type_id(std::ostream& o) const = 0;
    };
  }//end of ns internal
}}//end of ns awali::dyn

#endif //DYN_UNTYPED_VALUE_HH
