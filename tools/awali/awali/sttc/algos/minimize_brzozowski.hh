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

#ifndef AWALI_ALGOS_MINIMIZE_BRZOZOWSKI_HH
# define AWALI_ALGOS_MINIMIZE_BRZOZOWSKI_HH

#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/core/transpose_view.hh>

namespace awali { namespace sttc {


  /*-------------------------------------------.
  | minimization with Brzozowski's algorithm.  |
  `-------------------------------------------*/

  template <typename Aut>
  inline
  typename Aut::element_type::automaton_nocv_t
  minimize_brzozowski(const Aut& a)
  {
    auto codet = determinize(transpose_view(a),false);
    return determinize(transpose_view(codet),false);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_MINIMIZE_BRZOZOWSKI_HH
