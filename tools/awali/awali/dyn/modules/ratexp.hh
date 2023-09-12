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

#ifndef DYN_MODULES_RATEXP_HH
#define DYN_MODULES_RATEXP_HH

#include <iostream>
#include <set>
#include <unordered_map>
#include <awali/dyn/core/ratexp.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/core/context_description.hh>
#include <awali/dyn/options/options.hh>

namespace awali {
  namespace dyn {


    /** Computes a rational expression equivalent to \p aut.
     *
     * @param aut
     * @param opts A set of options; only {@link STATE_ELIM_ORDER} is meaningful.
     * @return a new rational expression, equivalent to \p aut
     */
    ratexp_t aut_to_exp(automaton_t aut, options_t opts = {});

    namespace lift {
      /** Builds some kind of copy of \p aut with weights that are
       * rational expressions with the context of \p aut, and with no labels.
       *
       * This is the first step in order to execute a step-by-step state
       * elimination process. (See {@link eliminate_state}).
       *
       * @param aut
       * @return a new automaton
       *
       * \b Example
       * \code
       * automaton_t aut = .. // some boolean automaton over alphabet {a,b}
       * automaton_t lifted = lift(aut); // lifted is an automaton whose weights
       *                                 // are boolean ratexp over {a,b}
       * state_t s = lifted->add_state();
       * lifted->set_transition(s,s,"","(a+b)*");
       * \endcode
       */
      automaton_t lift(automaton_t aut);
    }

    /** Expands a rational expression
     * @param exp
     */
    ratexp_t expand(ratexp_t exp);

    /** Builds an expression equivalent to \p exp that is in star normal form.
     *
     * @return A new rational expression with same context as \p exp.
     */
    ratexp_t star_normal_form(ratexp_t exp);

    /** Return the star height of \p exp.
     *
     * @param exp
     * */
    unsigned star_height(ratexp_t exp);

    /** Tests whether \p exp is valid that is, if every monomial in the series
     * it represents is properly weighted.
     *
     * This is always true for rational expression
     * weighted by booleans.
     * @param exp
     * @return `true` if \p exp is valid
     */
    bool is_valid(ratexp_t exp);

    /** Returns the constant term of \p exp, that is the weight of epsilon.
     * @param exp
     * @return a weight
     */
    weight_t constant_term(ratexp_t exp);


    json_ast_t to_json_ast(ratexp_t exp, 
                            json_ast_t extra_medata = json_ast::empty());

    namespace internal {
      std::ostream& json(ratexp_t aut, std::ostream& out, 
                         json_ast_t extra_medata = json_ast::empty());
    }

  }
}//end of ns awali::dyn

#endif
