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

#ifndef AWALI_MISC_MEMORY_HH
# define AWALI_MISC_MEMORY_HH

# include <memory>
# include <utility> // std::forward

namespace awali { namespace sttc {

  /// Same as std::make_shared, but parameterized by the shared_ptr type,
  /// not the (pointed to) element_type.
  template <typename SharedPtr, typename... Args>
  SharedPtr
  make_shared_ptr(Args&&... args)
  {
    using type = typename SharedPtr::element_type;
    return std::make_shared<type>(std::forward<Args>(args)...);
  }
}}//end of ns awali::stc

#endif // !AWALI_MISC_MEMORY_HH
