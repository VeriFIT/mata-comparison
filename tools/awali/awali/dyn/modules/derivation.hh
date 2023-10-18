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

#ifndef DYN_DERIVATION_HH
#define DYN_DERIVATION_HH
#include <map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/ratexp.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
  namespace dyn {
    /*
     TODO
      std::map<ratexp_t, weight_t> derivation(std::map<ratexp_t, weight_t> e, label_t a, bool breaking = false)
      std::map<ratexp_t, weight_t> derivation(std::map<ratexp_t, weight_t> e, std::string w, bool breaking = false)
      */


    namespace internal {

      /** Derives a rational expression with respect to some label.
       * @param exp The rational expression to derive
       * @param lab The rational with respect to which the expression is derived.
       * @param breaking Whether to use the breaking variant.
       * @return return
       * */
      std::map<ratexp_t, weight_t> derivation(ratexp_t exp, label_t lab,
                                              bool breaking = false);

      /** Builds an automaton from a rational expression using the derived term
       * algorithm
       *
       * @param exp the rational expression
       * @return an automaton that accepts
       * @param opts A set of options. Only {@link KEEP_HISTORY} is meaningful.
       * @see exp_to_aut
       * */
      automaton_t derived_term(ratexp_t exp, options_t opts = {});

      /** Builds an automaton from a rational expression using the breaking
       * derived term algorithm
       *
       * @param exp the rational expression
       * @return an automaton that accepts
       * @param opts A set of options. The following options are meaningful:
       * {@link KEEP_HISTORY}.
       * @see exp_to_aut
       * */
      automaton_t breaking_derived_term(ratexp_t exp, options_t opts = {});
    }
  }
}//end of ns awali::dyn

#endif
