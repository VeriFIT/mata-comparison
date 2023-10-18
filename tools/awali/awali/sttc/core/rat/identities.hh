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

#ifndef AWALI_CORE_RAT_IDENTITIES_HH
# define AWALI_CORE_RAT_IDENTITIES_HH

# include <iostream>
# include <string>
//#include <awali/sttc/misc/export.hh>

namespace awali { namespace sttc //LIBAWALI_API
{
  namespace rat
  {

    /// A ratexpset can implement several different sets of identities on
    /// expressions.  This type can represents one of those sets.
    enum class identities
    {
      /// Trivial identities only.
      trivial,

      /// Trivial identities only + associativity.
      associativity,

      /// Trivial identities plus series identities.
      series,
    };

    std::string to_string(identities i);
    std::istream& operator>>(std::istream& is, identities& i);
    std::ostream& operator<<(std::ostream& os, identities i);

    identities join(identities i1, identities i2);
    identities meet(identities i1, identities i2);

  } // namespace rat
}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_IDENTITIES_HH
