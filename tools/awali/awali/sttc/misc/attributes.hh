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

#ifndef AWALI_MISC_ATTRIBUTES_HH
# define AWALI_MISC_ATTRIBUTES_HH

# if defined __clang__

#  define ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#  define ATTRIBUTE_CONST         __attribute__((const))
#  define ATTRIBUTE_DLLEXPORT
#  define ATTRIBUTE_DLLIMPORT
#  define ATTRIBUTE_HOT           __attribute__((hot))
#  define ATTRIBUTE_NORETURN      [[noreturn]]
#  define ATTRIBUTE_PURE          __attribute__((pure))
#  define ATTRIBUTE_HIDDEN

# elif defined __GNUC__

#  define ATTRIBUTE_ALWAYS_INLINE __attribute__((always_inline))
#  define ATTRIBUTE_CONST         __attribute__((const))
#  define ATTRIBUTE_DLLEXPORT     __attribute__((visibility("default")))
#  define ATTRIBUTE_DLLIMPORT     ATTRIBUTE_DLLEXPORT
#  define ATTRIBUTE_HOT           __attribute__((hot))
#  define ATTRIBUTE_NORETURN      __attribute__((noreturn))
#  define ATTRIBUTE_PURE          __attribute__((pure))
#  define ATTRIBUTE_HIDDEN        __attribute__ ((visibility ("hidden")))

# elif defined _MSC_VER

#  define ATTRIBUTE_ALWAYS_INLINE
#  define ATTRIBUTE_CONST         __declspec(const)
#  define ATTRIBUTE_DLLEXPORT     __declspec(dllexport)
#  define ATTRIBUTE_DLLIMPORT     __declspec(dllimport)
#  define ATTRIBUTE_HOT
#  define ATTRIBUTE_NORETURN      [[noreturn]]
#  define ATTRIBUTE_PURE
#  define ATTRIBUTE_HIDDEN

# endif


#endif // !AWALI_MISC_ATTRIBUTES_HH
