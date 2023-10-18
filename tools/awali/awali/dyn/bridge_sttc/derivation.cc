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

#include <awali/sttc/algos/derivation.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>
#include <awali/dyn/bridge_sttc/explicit_ratexp.cc>

#include<set-types.hh>

namespace awali {

    template< typename Context,
//               typename LS=typename Context::labelset_t,
//               typename WS=typename Context::weightset_t,
              typename T = void>
    struct dispatch { //default
    static dyn::automaton_t derived_term(dyn::ratexp_t exp, bool breaking, bool keep_history) {
      throw std::runtime_error("derived_term only supported for free label-sets with no epsilon-transitions allowed.");
    }
    static std::map<dyn::ratexp_t, dyn::any_t> derivation_label(dyn::ratexp_t exp, dyn::any_t a, bool breaking = false) {
      throw std::runtime_error("derived_term only supported for free label-sets with no epsilon-transitions allowed.");

    }
  };

  template<typename Context>
//         typename LS,
//         typename WS>
    struct dispatch<Context, typename std::enable_if<Context::labelset_t::is_free()>::type> {

    static std::map<dyn::ratexp_t, dyn::any_t> derivation_label(dyn::ratexp_t exp, dyn::any_t a, bool breaking = false) {
      auto e= get_stc_ratexp<context_t>(exp);
      auto rs = get_stc_ratexpset<context_t>(exp);
      using rs_t = decltype(rs);
      auto ratset = std::make_shared<rs_t>(rs);
      sttc::rat::ratexp_polynomial_t<decltype(rs)> poly;
      try{
        poly = sttc::derivation<rs_t>(rs, e, dyn::internal::any_cast<sttc::label_t_of<rs_t>>(a), breaking);
      } catch(...) {
        using wordset_t = typename sttc::labelset_trait<typename rs_t::labelset_t>::wordset_t;
        poly = sttc::derivation<rs_t>(rs, e, dyn::internal::any_cast<typename wordset_t::value_t>(a), breaking);
      }
      std::map<dyn::ratexp_t, dyn::any_t> result;
      for(auto p: poly) {
        dyn::ratexp_t e1 = std::make_shared<dyn::explicit_ratexp_t<context_t>>(p.first, ratset);
        result.emplace(std::make_pair(e1, p.second));
      }
      return result;
    }

    static dyn::automaton_t derived_term(dyn::ratexp_t exp, bool breaking, bool keep_history) {
      auto e= get_stc_ratexp<context_t>(exp);
      auto rs = get_stc_ratexpset<context_t>(exp);
      return dyn::make_automaton(sttc::derived_term<decltype(rs)>(rs, e, breaking, keep_history));
    }
  };

  extern "C" std::map<dyn::ratexp_t, dyn::any_t> derivation_label(dyn::ratexp_t exp, dyn::any_t a, bool breaking = false) {
    return dispatch<context_t>::derivation_label(exp, a, breaking);
  }

  extern "C" dyn::automaton_t derived_term(dyn::ratexp_t exp, bool breaking, bool keep_history) {
    return dispatch<context_t>::derived_term(exp, breaking, keep_history);
  }
}

#include <awali/dyn/core/any.cc>

