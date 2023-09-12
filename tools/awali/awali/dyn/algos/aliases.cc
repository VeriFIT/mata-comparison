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

#include <awali/dyn/modules/partial_id.hh>
#include <awali/dyn/modules/transpose.hh>
#include <awali/dyn/modules/quotient.hh>
#include <awali/dyn/modules/determinize.hh>
#include <awali/dyn/modules/proper.hh>
#include <awali/dyn/modules/derivation.hh>
#include <awali/dyn/modules/standard.hh>
#include <awali/dyn/algos/aliases.hh>

namespace awali {
  namespace dyn {

    
    automaton_t
    min_coquotient (automaton_t aut, options_t opts)
    {
      automaton_t res = min_quotient(transpose(aut, opts), opts);
      transpose(res, opts+(IN_PLACE=true));
      return res;
    }


    automaton_t
    coquotient (automaton_t aut, std::vector<std::vector<state_t>>& equiv,
      options_t opts) 
    {
      automaton_t res = quotient(transpose(aut), equiv, opts);
      transpose(res, {IN_PLACE=true});
      return res;
    }


    automaton_t
    right_reduce (automaton_t aut)
    {
      automaton_t res = left_reduce(transpose(aut));
      transpose(res, {IN_PLACE=true});
      return res;
    }


    automaton_t exp_to_aut(ratexp_t ratexp, options_t opt)
    {
      switch (opt[EXP_TO_AUT_ALGO]) {
      case GLUSHKOV:
        return internal::standard(ratexp);
      case THOMPSON:
        return internal::thompson(ratexp);
      case COMPACT_THOMPSON:
        return internal::compact_thompson(ratexp);
      case DERIVED_TERM:
        return internal::derived_term(ratexp, opt);
      case WEIGHTED_THOMPSON:
        return internal::weighted_thompson(ratexp);
      case BREAKING_DERIVED_TERM:
        return internal::breaking_derived_term(ratexp, opt);
      case STANDARD_AND_QUOTIENT:
        return min_quotient(internal::standard(ratexp), opt);
      default:
        throw std::runtime_error("exp_to_aut::Unreachable statement!");
      }
    }

  }
}
