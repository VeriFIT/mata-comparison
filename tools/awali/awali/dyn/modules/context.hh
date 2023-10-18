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

#ifndef DYN_MODULES_CONTEXT_HH
#define DYN_MODULES_CONTEXT_HH

#include<string>

#include <awali/dyn/core/context_description.hh>
#include <awali/dyn/core/context_description_deprecated.hh>
#include <awali/dyn/core/ratexp.hh>
// #include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/abstract_ratexp.hh>
#include <awali/dyn/options/options.hh>
#include<awali/common/json_ast.hh>

namespace awali {
  namespace dyn {

    struct abstract_automaton_t;
    class automaton_t;

    namespace internal {
      

      /* Renames the states of `dst` that have an origin in `orig` based on
       * their name in `orig`
       *
       * This is very unsafe: no verification is done.
       * - `dst` is indeed the origin of dst
       * - the history kind of `dst` is indeed single
       * */
      void rename_from_origin_inplace(automaton_t dst, automaton_t orig);

      automaton_t make_automaton_from_context(context_t ctx);

      automaton_t make_automaton_from_context(context::context_description cd);
      automaton_t make_automaton_from_context(context::labelset_description ls,
                                              context::weightset_description ws);

      context_t make_context(context::context_description cd);
      context_t make_context(context::labelset_description ls,
                             context::weightset_description ws);

//       automaton_t make_automaton(std::string alphabet, 
//                                  std::string semiring ="B");
//       automaton_t make_automaton_with_eps(std::string alphabet,
//                                           std::string semiring ="B");

//       automaton_t make_int_automaton(int a, int b, std::string semiring);
//       automaton_t make_int_automaton(int n, std::string semiring);
//       automaton_t make_int_automaton_with_eps(int a, int b, 
//                                               std::string semiring);
//       automaton_t make_int_automaton_with_eps(int n, std::string semiring);
// 
      
      automaton_t parse_automaton(std::istream& i);

      automaton_t parse_automaton(json_ast_t ast);

      automaton_t deprecated_parse_automaton(std::istream& i);

      ratexp_t make_ratexp_with_context(const std::string& exp,
                                        context_t ctx,
                                        bool fixed_alphabet=true);

      ratexp_t make_ratexp_with_context(std::string const& exp,
                                        context::context_description cd, 
                                        bool fixed_alphabet);

      ratexp_t make_ratexp_with_context(const std::string& exp,
                                        context::labelset_description ls,
                                        context::weightset_description ws,
                                        bool fixed_alphabet=true);

     ratexp_t make_ratexp_with_alphabet(const std::string& exp,
                                        const std::string& alph,
                                        const std::string& sr = "B",
                                        bool fixed_alphabet=true);

      ratexp_t make_ratexp(const std::string& exp,
                           const std::string& sr = "B");
// 
//       ratexp_t make_int_ratexp_with_range(const std::string& exp,
//                                           int a, int b,
//                                           const std::string& sr ="B",
//                                           bool fixed_alphabet=true) ;
//       ratexp_t make_int_ratexp(const std::string& exp,
//                                const std::string& sr = "B");
// 
//       ratexp_t make_ratexp_with_alphabets(const std::string& exp,
//                                           std::vector<std::string> alphabets,
//                                           std::string semiring = "B",
//                                           bool fixed_alphabet=true);
//       ratexp_t make_tuple_ratexp(const std::string& exp,
//                                  unsigned n,
//                                  std::string semiring = "B");
    
    std::vector<state_t> states(abstract_automaton_t const* aut, bool all);
    std::vector<transition_t> transitions(abstract_automaton_t const* aut, bool all);
    std::vector<transition_t> outgoing(abstract_automaton_t const* aut, state_t s , bool all);
    std::vector<transition_t> incoming(abstract_automaton_t const* aut, state_t s , bool all);

    
    } // end of namespace awali::dyn::internal

    ratexp_t parse_ratexp(std::istream& i);


    namespace internal {
      ratexp_t parse_ratexp(json_ast_t obj);
    }
  } //end f namespace awali::dyn
}//end of ns awali

#endif
