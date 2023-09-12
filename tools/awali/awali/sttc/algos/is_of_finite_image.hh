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

#ifndef AWALI_ALGOS_FINITE_IMAGE_HH
#define AWALI_ALGOS_FINITE_IMAGE_HH

#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/algos/is_acyclic.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh> // is_epsilon

namespace awali {
  namespace sttc {
    /** @brief Check whether the image of every word is finite
     *
     * The function actually tests whether there exists
     * some circuit with label epsilon on tape \p I,
     * and non empty output on the other tapes.
     *
     * @tparam I the index of the tape
     * @tparam Tdc the type of transducer
     * @param tdc the transducer
     * @return true if and only if the image of every word read on tape \p I is finite
     */
    template<unsigned I, typename Tdc>
    bool is_of_finite_image(const Tdc& tdc) {
      using labelset_t = labelset_t_of<Tdc>;
      using Ilabelset_t = typename labelset_t::template valueset_t<I>;
      auto ptdc = proper(tdc);
      for(auto tr : ptdc->transitions()) {
        if(!is_epsilon<Ilabelset_t>(std::get<I>(ptdc->label_of(tr))))
          ptdc->del_transition(tr);
      }
      return is_acyclic(ptdc);
    }
  }
}//end of ns awali::stc

#endif
