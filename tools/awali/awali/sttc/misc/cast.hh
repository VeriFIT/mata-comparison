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

#ifndef AWALI_MISC_CAST_HH
# define AWALI_MISC_CAST_HH

# include <iostream>

# ifdef NDEBUG
#  define down_cast         static_cast
#  define down_pointer_cast std::static_pointer_cast
# else

// We want to support the "down_cast<FOO>(BAR)" syntax (with a pair of
// <> and of ()).  So expand "down_cast" into a templated function.
// Actually, make it a functor that can be given the current location.
#define down_cast                               \
  ::awali::sttc::internal::down_caster(__FILE__, __LINE__).cast
#define down_pointer_cast down_cast

namespace awali { namespace sttc {

  namespace internal
  {
    /// A functor that captures the current location (to report
    /// errors), and provides "cast".
    struct down_caster
    {
      down_caster(const char* f, int l)
        : file(f), line(l)
      {}
      const char* file;
      int line;

      template <typename T, typename U>
      void
      error(const char* msg)
      {
        std::cerr
          << file << ':' << line << ": " << msg
          << " from " << typeid(U).name() << " to " << typeid(T).name()
          << std::endl;
        abort();
      }

      template <typename T, typename U>
      inline
      T
      cast(U t)
      {
        if (!t)
          error<T, U>("down_casting nullptr");
        T res = dynamic_cast<const T>(t);
        if (!res)
          error<T, U>("failed down_cast");
        return res;
      }

      /// Special case for shared_ptr.
      template <typename T, typename U>
      inline
      std::shared_ptr<T>
      cast(std::shared_ptr<U> t)
      {
        if (!t)
          error<std::shared_ptr<T>, std::shared_ptr<U>>("down_casting nullptr");
        std::shared_ptr<T> res = std::dynamic_pointer_cast<T>(t);
        if (!res)
          error<std::shared_ptr<T>, std::shared_ptr<U>>("failed down_cast");
        return res;
      }
    };
  }

}}//end of ns awali::stc
#endif

#endif // !AWALI_MISC_CAST_HH
