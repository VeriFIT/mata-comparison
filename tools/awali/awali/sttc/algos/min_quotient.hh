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

#ifndef AWALI_ALGOS_MIN_QUOTIENT_HH
#define AWALI_ALGOS_MIN_QUOTIENT_HH

#include <awali/common/enums.hh>
#include <awali/sttc/algos/quotient/moore_quotient.hh>
#include <awali/sttc/algos/quotient/hopcroft_quotient.hh>
#include <awali/sttc/algos/quotient/congruence_det.hh>
#include <awali/sttc/algos/merge.hh>

namespace awali {
  namespace sttc {
    template <typename Aut>
    Aut min_quotient(const Aut& aut, quotient_algo_t algo=MOORE, 
                     bool keep_history=true) 
    {
      std::vector<std::vector<state_t> > equiv;
      std::vector<std::list<state_t> > equil;
      switch(algo) {
      case MOORE :
        moore_quotient(aut, equiv);
        return merge(aut, equiv, keep_history);
      case HOPCROFT :
        hopcroft_quotient(aut, equil);
        return merge(aut, equil, keep_history);
      default:
        raise("Quotient algo is either MOORE or HOPCROFT");
      }
    }

    //For deterministic automata only
    template <typename Aut>
    Aut minimize(const Aut& aut, quotient_algo_t algo=MOORE, 
                 bool keep_history=true) 
    {
      std::vector<std::vector<state_t> > equiv;
      std::vector<std::list<state_t> > equil;
      switch(algo) {
      case MOORE :
        moore_det(aut, equiv);
        return merge(aut, equiv, keep_history);
      case HOPCROFT :
        hopcroft_quotient(aut, equil, true);
        return merge(aut, equil, keep_history);
      default:
        raise("Quotient algo is either MOORE or HOPCROFT");
      }
    }

    template <typename Aut>
    Aut min_quotient_det(const Aut& aut, quotient_algo_t algo=MOORE, bool
                         keep_history=true) 
    {
      return minimize(aut, algo, keep_history);
    }

  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_MIN_QUOTIENT_HH
