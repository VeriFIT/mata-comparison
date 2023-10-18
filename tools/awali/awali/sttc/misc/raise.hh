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

#ifndef AWALI_MISC_RAISE_HH
# define AWALI_MISC_RAISE_HH

# include <stdexcept>
# include <sstream>
# include <utility>

#include <awali/sttc/misc/attributes.hh>

namespace awali { namespace sttc {

  namespace internal
  {
    /// Ignore its arguments.
    ///
    /// Useful auxiliary to handle variadic arguments.
    struct pass
    {
      template<typename ...T> pass(T...) {}
    };
  }

  /// Raise a runtime_error with the concatenation of \a args as message.
  template <typename... Args>
  ATTRIBUTE_NORETURN
  inline void raise(Args&&... args)
  {
    std::ostringstream o;
    using swallow = int[];
    (void) swallow
      {
        (o << args, 0)...
      };
    throw std::runtime_error{o.str()};
  }

  /// If \a b is not verified, raise an error with \a args as message.
  template <typename... Args>
  inline void require(bool b, Args&&... args)
  {
    if (!b)
      raise(std::forward<Args>(args)...);
  }


}}//end of ns awali::stc

#endif // !AWALI_MISC_RAISE_HH
