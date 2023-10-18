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

#ifndef AWALI_ALGOS_IS_AMBIGUOUS_HH
# define AWALI_ALGOS_IS_AMBIGUOUS_HH

#include <awali/sttc/algos/accessible.hh>
#include <awali/sttc/algos/product.hh>
#include <awali/sttc/history/tuple_history.hh>

namespace awali { namespace sttc {

  template <typename Aut>
  bool is_ambiguous(const Aut& aut)
  {
    auto prod = product(aut, aut);
    // Check if there useful states outside of the diagonal.  Since
    // the product is accessible, check only for coaccessibles states.
    auto coaccessible = coaccessible_states(prod);
    for (const auto& o: dynamic_cast<tuple_history<std::tuple<Aut,Aut>>*>(prod->history().get())->origins())
      if (std::get<0>(o.second) != std::get<1>(o.second)
          && coaccessible.find(o.first)!=coaccessible.end())
        {
          if (getenv("AWALI_DEBUG"))
            std::cerr << "ambiguous: " << o.first - 2
                      << " (" << std::get<0>(o.second) - 2
                      << ", " << std::get<1>(o.second) - 2 << ")"
                      << std::endl;
          return true;
        }
    return false;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_IS_AMBIGUOUS_HH
