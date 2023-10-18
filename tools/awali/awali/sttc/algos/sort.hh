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

#ifndef AWALI_ALGOS_SORT_HH
# define AWALI_ALGOS_SORT_HH


# include <algorithm>

namespace awali { namespace sttc {

  template <typename Aut, typename Compare>
  inline
  void
  sort(Aut a, Compare p)
  {
    for(auto s: a->states()) {
      auto& out = a->all_out_(s);
      std::sort(out.begin(), out.end(), p);
    }
  }

  template <size_t I, typename Tdc>
  inline
  void
  sort_tape(Tdc t)
  {
    sort(t, [t](transition_t t1, transition_t t2) {
        auto ls=t->context().labelset()->template set<I>();
        return ls.less_than(std::get<I>(t->label_of(t1)),
                             std::get<I>(t->label_of(t2)));
      });
  }



}}//end of ns awali::stc

#endif // !AWALI_ALGOS_SORT_HH
