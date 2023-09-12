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

#ifndef AWALI_CORE_KIND_HH
# define AWALI_CORE_KIND_HH

# include <istream>
# include <stdexcept>
# include <type_traits>

#include <awali/sttc/misc/escape.hh>

namespace awali { namespace sttc {


  /// Define the kinds, and auxiliary tools.
  ///
  /// is_ABBREV<Kinded>: Whether Kinded has a specific kind_t
  ///
  /// if_ABBREV<Kinded, R = Kinded>:
  /// Provide "overloading" on parameter types, for instance to require
  /// letter_t when labels_are_letters, and word_t when
  /// labels_are_words.  See ratexpset::atom and ratexpset::atom_ for
  /// an example.
  ///
  /// It is very tempting to turns these guys into members of
  /// context, but then, instead of "(if_lal<Ctx, letter_t> v)",
  /// one must write "(typename Cxx::template if_lal<letter_t> v)".

# define DEFINE(Abbrev, Name)                                           \
  struct labels_are_ ## Name                                            \
  {                                                                     \
    static std::string name()                                           \
    {                                                                   \
      return "labels_are_" #Name;                                       \
    }                                                                   \
    static std::string sname()                                          \
    {                                                                   \
      return #Abbrev;                                                   \
    }                                                                   \
                                                                        \
    static void make(std::istream& is)                                  \
    {                                                                   \
      /*                                                                \
       * name: lal_char(abc)_ratexpset<law_char(xyz)_b>.                \
       *       ^^^ ^^^^ ^^^  ^^^^^^^^^^^^^^^^^^^^^^^^^                  \
       *        |   |    |        weightset                             \
       *        |   |    +-- gens                                       \
       *        |   +-- letter_type                                     \
       *        +-- kind                                                \
       *                                                                \
       * name: lao_ratexpset<law_char(xyz)_b>                           \
       *       ^^^ ^^^^^^^^^^^^^^^^^^^^^^^^^^                           \
       *       kind         weightset                                   \
       */                                                               \
      char kind[4];                                                     \
      is.get(kind, sizeof kind);                                        \
      if (sname() != kind)                                              \
        throw std::runtime_error("kind::make: unexpected: "             \
                                 + str_escape(kind)                     \
                                 + ", expected: " + sname());           \
    }                                                                   \
                                                                        \
  };                                                                    \

    /*                                                                  \
  template <typename Kinded>                                            \
  struct is_ ## Abbrev                                                  \
    : std::is_same<typename Kinded::kind_t, labels_are_ ## Name>::type  \
  {};                                                                   \
                                                                        \
    */
    /// marker type for labelsets where labels are letters
  DEFINE(lal, letters);
    /// marker type for labelsets where labels are nullable
  DEFINE(lan, nullable);
    /// marker type for labelsets where labels are one
  DEFINE(lao, one);
    /// marker type for labelsets where labels are rational expressions
  DEFINE(lar, ratexps);
    /// marker type for labelsets where labels are tuples
  DEFINE(lat, tuples);
    /// marker type for labelsets where labels are words
  DEFINE(law, words);

# undef DEFINE
}}//end of ns awali::stc

#endif // !AWALI_CORE_KIND_HH
