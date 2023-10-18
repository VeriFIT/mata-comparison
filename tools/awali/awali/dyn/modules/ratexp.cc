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

#ifndef DYN_MODULES_RATEXP_CC
#define DYN_MODULES_RATEXP_CC

#include <iostream>
#include <set>
#include <unordered_map>
#include <awali/dyn/core/abstract_ratexp.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/core/context_description.hh>

#include <awali/dyn/modules/ratexp.hh>

namespace awali {
  namespace dyn {

//   ratexp_t aut_to_exp(automaton_t aut) {
//     return loading::call1<ratexp_t>("aut_to_exp","ratexp", aut);
//   }
//     mod.js 200316
    ratexp_t aut_to_exp(automaton_t aut, options_t opts)
    {
      switch (opts[STATE_ELIM_ORDER]) {
      case MIN_ID:  // = ID_ORDER
        return loading::call1<ratexp_t>("aut_to_exp_in_order", "ratexp", aut);
      case MIN_INOUT_DEGREE:
        return loading::call1<ratexp_t>("aut_to_exp_heuristic", "ratexp", aut);
      }
      throw std::runtime_error("dyn::aut_to_exp Unreachable statement");
    }

    automaton_t lift::lift(automaton_t aut)
    {
      return loading::call1<automaton_t>("lift", "ratexp", aut, &lift);
    }

    ratexp_t expand(ratexp_t exp)
    {
      return loading::call1<ratexp_t>("expand", "ratexp", exp);
    }

    ratexp_t star_normal_form(ratexp_t exp)
    {
      return loading::call1<ratexp_t>("star_normal_form", "ratexp", exp);
    }

    unsigned star_height(ratexp_t exp)
    {
      return loading::call1<unsigned>("star_height", "ratexp", exp);
    }

    bool is_valid(ratexp_t exp)
    {
      return loading::call1<bool>("is_valid", "ratexp", exp);
    }

    weight_t constant_term(ratexp_t exp)
    {
      return loading::call1<weight_t>("constant_term", "ratexp", exp);
    }

    ratexp_t ratexp_support(ratexp_t exp) {
      return loading::call1<ratexp_t>("ratexp_support", "ratexp", exp);
    }

    ratexp_t ratexp_copy(ratexp_t exp) {
      return loading::call1<ratexp_t>("ratexp_copy", "ratexp", exp);
    }

    ratexp_t
    ratexp_characteristic(ratexp_t exp, std::string const& semiring)
    {
      std::string stat_ctx
        = exp->get_context()->labelset_name() + "_"
          + context::tostring(context::weightset(semiring), false);
      return loading::call0<ratexp_t>("ratexp_characteristic", "ratexp",
                                         stat_ctx, exp);
    }

    json_ast_t to_json_ast(ratexp_t exp, json_ast_t em)
    {
      return loading::call1<json_ast_t>("to_json_tree", "ratexp", exp, em);
    }

    std::ostream& 
    internal::json(ratexp_t ratexp, 
                   std::ostream& out, 
                   json_ast_t extra_metadata)
    {
      json_ast_t ast = to_json_ast(ratexp, extra_metadata);
      return ::awali::put(ast, out);
    }
  

  }
}//end of ns awali::dyn

#endif
