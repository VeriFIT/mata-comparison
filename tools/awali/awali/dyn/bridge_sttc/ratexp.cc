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

#include <awali/sttc/algos/lift.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/alphabets/char.hh>
#include <awali/sttc/algos/exp_parser.hh>
#include <awali/sttc/algos/js_parser.hh>
#include <awali/sttc/algos/aut_to_exp.hh>
#include <awali/sttc/algos/expand.hh>
#include <awali/sttc/algos/star_height.hh>
#include <awali/sttc/algos/star_normal_form.hh>
#include <awali/sttc/algos/thompson.hh>
#include <awali/sttc/algos/is_valid.hh>
#include <awali/sttc/labelset/traits.hh>

#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/dyn/bridge_sttc/explicit_ratexp.cc>


#include<set-types.hh>

namespace awali {

  extern "C" dyn::ratexp_t aut_to_exp_heuristic(dyn::automaton_t aut) {
      auto a = dyn::get_stc_automaton<context_t>(aut);
      using ratset_t = sttc::ratexpset_of<context_t>;
      auto rs = std::make_shared<ratset_t>(sttc::get_rat_context(a->context()),ratset_t::identities_t::trivial);
      return make_ratexp(sttc::aut_to_exp(a), rs);
  }

  extern "C" dyn::ratexp_t aut_to_exp_in_order(dyn::automaton_t aut) {
      auto a = dyn::get_stc_automaton<context_t>(aut);
      using ratset_t = sttc::ratexpset_of<context_t>;
      auto rs = std::make_shared<ratset_t>(sttc::get_rat_context(a->context()),ratset_t::identities_t::trivial);
      return make_ratexp(sttc::aut_to_exp_in_order(a), rs);
  }

  extern "C" dyn::automaton_t lift(dyn::automaton_t aut) {
      auto a = dyn::get_stc_automaton<context_t>(aut);
      return dyn::make_automaton(sttc::lift(a));
  }

  extern "C" dyn::ratexp_t expand(dyn::ratexp_t exp) {
      auto e= get_stc_ratexp<context_t>(exp);
      auto rs = get_stc_ratexpset<context_t>(exp);
      auto ratset = std::make_shared<decltype(rs)>(rs);
      return make_ratexp(sttc::expand(rs, e),ratset);
  }

  extern "C" dyn::ratexp_t star_normal_form(dyn::ratexp_t exp) {
      auto e= get_stc_ratexp<context_t>(exp);
      auto rs = get_stc_ratexpset<context_t>(exp);
      auto ratset = std::make_shared<decltype(rs)>(rs);
      return make_ratexp(sttc::star_normal_form(rs, e),ratset);
  }

  extern "C" unsigned star_height(dyn::ratexp_t exp) {
      auto e= get_stc_ratexp<context_t>(exp);
      using ratset_t = sttc::ratexpset_of<context_t>;
      return sttc::star_height<ratset_t>(e);
  }

  extern "C" bool is_valid(dyn::ratexp_t exp) {
      auto e= get_stc_ratexp<context_t>(exp);
      using ratset_t = sttc::ratexpset_of<context_t>;
      auto rs = get_stc_ratexpset<context_t>(exp);
      return sttc::is_valid(rs,e);
  }

  extern "C" dyn::any_t constant_term(dyn::ratexp_t exp) {
      auto e= get_stc_ratexp<context_t>(exp);
      using ratset_t = sttc::ratexpset_of<context_t>;
      auto rs = get_stc_ratexpset<context_t>(exp);
      return sttc::constant_term(rs,e);
  }

  extern "C" json_ast_t to_json_tree(dyn::ratexp_t exp,
                                     json_ast_t extra_metadata)
  {
      auto e= get_stc_ratexp<context_t>(exp);
      using ratset_t = sttc::ratexpset_of<context_t>;
      auto rs = get_stc_ratexpset<context_t>(exp);
      return ratexp_to_ast(rs,e, extra_metadata);
  }
}

#include <awali/dyn/core/any.cc>
