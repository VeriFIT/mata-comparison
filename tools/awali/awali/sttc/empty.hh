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

#ifndef AWALI_EMPTY_HH
# define AWALI_EMPTY_HH

# include <functional> // std::hash
# include <iostream>

namespace awali {
  namespace sttc {

    /// Empty labels, for LAO.
    struct empty_t
    {};

    // So that we can make a map with empty_t as key, for
    // mutable_automaton's entries.
    inline
    bool operator<(empty_t, empty_t)
    {
      return false;
    }

    // For unordered containers.
    inline
    bool operator==(empty_t, empty_t)
    {
      return true;
    }
  }
}//end of ns awali::stc

namespace std
{
  // For unordered containers.
  template <>
  struct hash<awali::sttc::empty_t>
  {
    size_t operator()(awali::sttc::empty_t) const
    {
      return 0;
    }
  };

  std::ostream& operator<< (std::ostream& o, const awali::sttc::empty_t& e) {
    return o;
  }
}


#endif // !AWALI_EMPTY_HH
