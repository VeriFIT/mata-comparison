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

#ifndef DYN_ALL_HH
#define DYN_ALL_HH

#include<memory>
#include<set>
#include<string>
#include<iostream>
#include<map>
#include<vector>
#include<unordered_map>

#include<awali/common/enum_utils.hh>

/* ************************************************************************** */
/*                                   TYPES                                    */
/* ************************************************************************** */

/* Contains type `ratexp_t` and associated method */
#include<awali/dyn/core/abstract_ratexp.hh>


/* Contains type `automaton_t` and associated method */
#include<awali/dyn/core/automaton.hh>

/* Contains type `automaton_t` and associated method */
#include<awali/dyn/core/transducer.hh>

/* Contains functions to build sttc contexts from strings (expert) */
#include<awali/dyn/core/context_description.hh>

#include<awali/dyn/core/context_description_deprecated.hh>

/* Contains type `context_t`, that is a pair (labelset,weightset) */
#include<awali/dyn/core/abstract_context.hh>

/* Contains the type `qfraction_t` used for manipulated automata weighted over
 * rational numbers.
 */
#include<awali/common/qfraction.hh>

/* Contains enums of the different methods available for some algorithms 
 * (typically minimization, proper and quotient, etc.).
 */
#include<awali/common/enums.hh>

/* Contains the type json_ast_t that represent an abstract json tree.
 */
#include<awali/common/json_ast.hh>

#include<awali/common/json/equality.hh>

#include<awali/common/json/smart_printer.hh>

#include<awali/common/json/node.hh>

#include<awali/common/json/utils.hh>

/* Contains enums of the different methods available for some algorithms 
 * (typically minimization, proper and quotient, etc.).
 */
#include<awali/common/version.hh>

/* ************************************************************************** */
/*                                   MODULES                                  */
/* ************************************************************************** */

/* Module containing functions about traversing automata (accessible, trim, 
 * coaccessible, etc.) 
 */
#include<awali/dyn/modules/accessible.hh>


/* Contains function to test whether two automata are equivalent, that is,
 * associate each word with the same weight.
 */
#include<awali/dyn/modules/are_equivalent.hh>


/* Module containing misc automaton functions : copy, weightset modification,
 * named state addition isomorphism, multiplication of an automaton
 */
#include<awali/dyn/modules/automaton.hh>


/* Module containing the functions to build automata and rational expressions:
 * - build automata and rational expressions from context description (expert)
 * - build empty automata from strings describing alphabet and weightset
 * - build rational expressions from strings describing its value and weightset
 */
#include<awali/dyn/modules/context.hh>


/* Module containing algorithm related to derivation of rational expression, in
 * particular one efficient way to compute automata from rational expressions.
 */
#include<awali/dyn/modules/derivation.hh>

/* Module containing the derminization algorithm.
 * Also contained related functions such as complementation or reduce
 * (pseudo-determinization for automata weighted over Z or a field).
 */
#include<awali/dyn/modules/determinize.hh>

/** Module containing state elimination function */
#include<awali/dyn/modules/eliminate.hh>

/* Contains evaluation of words for automata, that is computation of the weight
 * with which a word is accepted.  
 * Also contained related functions, such that enumerating the smallest
 * accepted word and the shortest accepted words.
 */
#include<awali/dyn/modules/eval.hh>

/* Contains functions computing the automaton of accepting the suffixes, 
 * prefixes, or factors of the languages accepted by a given automaton.
 */
#include<awali/dyn/modules/factor.hh>

/* Contains factories to build example automata from known families. */
#include<awali/dyn/modules/factories.hh>

/* Module containing algorithm on graph (e.g., sccs) */
#include<awali/dyn/modules/graph.hh>

/* Contains facilities to import and export automata. 
 * See also "awali/dyn/algos/sys.hh"
 */
#include<awali/dyn/modules/output.hh>

/* Contains the function to build a transducer that realises the identity
 * over the words accepted by a given automaton (other words have no image).
 */
#include<awali/dyn/modules/partial_id.hh>

/* Contains the classical "intersection" product and related algorithm
 * (shuffle, infiltration, union) 
 */
#include<awali/dyn/modules/product.hh>

/* Contains the functions to test and apply promotions of weightets
 */
#include<awali/dyn/modules/promotion.hh>

/* Contains emptyword-removal algorithms. */
#include<awali/dyn/modules/proper.hh>

/* Contains function related to automaton quotient (sometimes called
 * automaton morphism or bissimulation) 
 */
#include<awali/dyn/modules/quotient.hh>

/* Contains functions related to rational expressions (construction from string
 * or from automaton, algorithms)
 */
#include<awali/dyn/modules/ratexp.hh>

/* Contains unary product */
#include<awali/dyn/modules/singleproduct.hh>

/* Contains functions to compute and manipulate standard automata */
#include<awali/dyn/modules/standard.hh>

/* Contains functions specific to transducers */
#include<awali/dyn/modules/transducer.hh>

/* Module containing transpose function. */
#include<awali/dyn/modules/transpose.hh>

/* Contains word automata algorithms. */
#include<awali/dyn/modules/words.hh>

/* ************************************************************************** */
/*                EXTRA FUNCTIONS WRITTEN AT DYNAMICAL LAYER                  */
/* ************************************************************************** */

/* Contains variants of other functions, provided for easier use. 
 * For instance, provides symmetric functions through transposition.
 */
#include<awali/dyn/algos/aliases.hh>

/* Contains the evaluation for transducers. */
#include<awali/dyn/algos/tdc_eval.hh>

/* Contains algorithm to generate a random deterministic boolean automata of a
 * given length 
 */
#include<awali/dyn/algos/random.hh>


/* Contains algorithm circulation? */
#include<awali/dyn/algos/tdc_circulation.hh>



/* awali::dyn functions to 
 * - load automata from files (or istreams);
 * - display an automaton using `graphviz` and `dotty`;
 * - writing pdf image of an automaton to an ostream.
 */
#include<awali/dyn/algos/sys.hh>

#include<awali/dyn/options/option.hh>

#include<awali/dyn/loading/locations.hh>

#include<awali/common/no_such_file_exception.hh>

#endif
