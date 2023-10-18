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

#ifndef AWALI_MISC_PAIR_HH
# define AWALI_MISC_PAIR_HH

#include <awali/sttc/misc/attributes.hh>
#include <awali/utils/hash.hh>

namespace std
{

  /*---------------.
  | hash(pair<T>).  |
  `---------------*/

  template <typename T1, typename T2>
  struct hash<pair<T1, T2>>
  {
    size_t operator()(const pair<T1, T2>& p) const
    {
      size_t res = 0;
      hash_combine(res, p.first);
      hash_combine(res, p.second);
      return res;
    }
  };
}


namespace awali { namespace sttc {


  /*---------------------------------.
  | make_ordered_pair(e1<T>, e2<T>). |
  `---------------------------------*/

  template <typename T>
  std::pair<T, T> make_ordered_pair(T e1, T e2)
  {
    return e1 < e2 ? std::make_pair(e1, e2) : std::make_pair(e2, e1);
  }
}}//end of ns awali::stc

#endif // !AWALI_MISC_PAIR_HH
