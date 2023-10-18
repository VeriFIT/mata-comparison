
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

#ifndef DYN_CORE_RATEXP_HH
#define DYN_CORE_RATEXP_HH

#include<memory>
#include<awali/dyn/core/abstract_ratexp.hh>
#include<awali/dyn/core/abstract_context.hh>
#include<awali/dyn/core/context_description.hh>

namespace awali {
namespace dyn {

/** Main class for representing rational expresson at the dynamical layer
 *
 *  A {@link ratexp_t} is essentially a shared pointer to a
 *  {@link abstract_ratexp_t}.
 *  See the documentation of that class for help on the methods on ratexps.
 *
 *  Aside from that, this class contain static methods to constructs rational
 *  expressions in different context.
 *  See also group {@link Factories}.
 *
 *  All examples below assume: {@code using namespace awali::dyn}
 *
 *  @example (basic)
    \code
      ratexp_t exp = ratexp_t::from("(a+b)*a(a+b)*"); // Ratexp for words over {a,b}  with at least one b.
    \endcode
    \code
      ratexp_t exp = ratexp_t::from("(a+b)*a(a+b)*", "B", "abc"); // Same ratexp but the alphabet is {a,b,c} although no 'c' appears in the ratexp.
    \endcode

    @example (weights are integers)
    \code
      ratexp_t exp = ratexp_t::from("(<1>a+<-1>b)*","Z-min-plus"); // Expression that counts the number of a's minus the number of b's
    \endcode

    @example (labels are tuples : rational expression for transducers)
    \code
      ratexp_t exp = ratexp_t::with_tuple_labels::from("([a,a]+[b,b])*",2); // Expression representing the identity over alphabet {a,b}
      transducer_t tdc = exp_to_aut(exp);
    \endcode

    @example (weights are integers, labels are integers)
    \code
      ratexp_t exp = ratexp_t::with_int_labels::from("(0+1)*1(<2>0+<2>1)*","Z"); // Expression that associated each binary string with its evaluation in base 2.
    \endcode

 */
class ratexp_t : public std::shared_ptr<abstract_ratexp_t> {
public:

  /** Buils an automaton_t that is essentially a `nullptr`; should generally
   * not be used.
   */
  ratexp_t() : std::shared_ptr<abstract_ratexp_t>() {}


  /** Builds a ratexp_t from a shared pointer to a class derived
   * of {@link abstract_ratexp_t}; should generally not be
   * used.
   *
   *  Both @pname{ptr} and the built {@link automaton_t} will share ownership
   *  of pointed object.
   */
  template<class T>
  ratexp_t(const std::shared_ptr<T>& ptr,
           typename std::enable_if<
             std::is_base_of<abstract_ratexp_t,T>::value,
             int
           >::type = 0)
    : std::shared_ptr<abstract_ratexp_t>(ptr)
  {}
  
  /** \ingroup Factories
   * Builds the expression represented by @pname{str} and whose labelset is
   * described by  @pname{ld} and weightset by @pname{wd}.
   *
   * This function is the most powerful constructor provided by the dyn
   * layer of Awali.  It allows to combine almost arbitrarily all
   * implemented weightset and labelsets.
   *
   * Experts only.
   *
   * @param str String representation of 
   * @param ld A description of the labelset
   * @param wd A description of the weightset of the ratexp to build
   * @param fixed_alphabet If `false`, the alphabet of returned {@link ratexp_t}
   *                       will potentially be augmented with letters 
   *                       encountered while parsing @pname{str}
   * @see Documentation of namespace {@link dyn::context}
   */
  ratexp_t(std::string str,
           context::labelset_description ld,
           context::weightset_description wd,
           bool fixed_alphabet = true);


  /** Sum of two rational expressions. */
  ratexp_t operator+ (ratexp_t exp);

  /* Concatenation of two rational expressions. */
  ratexp_t operator* (ratexp_t exp);

  /** \private
   * This is necessary otherwise the previous function precedes. */
  abstract_ratexp_t& operator*() {
    return shared_ptr<abstract_ratexp_t>::operator*();
  }

  /** \ingroup Factories
   * Builds a rational expression from its string representation.
   *  @param str String representation of the rational expression to build
   *  @param weightset String representation of a weightset.
   *  @param alphabet If set to `"auto"` (default), the alphabet is inferred as
   *  part of the construction.  Otherwise each caracter is a letter in the
   *  alphabet. If you need alphabet {a,u,t,o}, use string "otua".
   *  @return a new rational expression.
   */
  static ratexp_t from (std::string str,
                        std::string weightset = "B",
                        std::string alphabet ="auto");

  /** \ingroup Factories
   * Builds a rational expression from a string representation and a context.
   *
   * @param str String representation of the rational expresion to build
   * @param ctx Context (that is, weightset and labelset) of the rational expression to build
   * @param fixed_alphabet If set to `true` the function will raise an
   * exception when encountering characters that are not in the labelset;
   * otherwise, it will silently add them to the alphabet.
   * @return a new rational expression
   */
  static ratexp_t from_context (std::string str,
                                context_t ctx,
                                bool fixed_alphabet = true);

  /** \ingroup Factories
   * @deprecated Since version 2.1, this function is replaced by the constructor {@link ratexp_t(std::string,context::labelset_description,context::weightset_description,bool)}.
   */
  static ratexp_t from_context(std::string str,
                               context::labelset_description ld,
                               context::weightset_description wd,
                               bool fixed_alphabet = true);

  /** \ingroup Factories
   * Helper class that contains convenience factories to build expressions
   * whose labels are integers.
  */
  struct with_int_labels {
    /** \ingroup Factories
     *
     */
    static ratexp_t from_range(std::string str,
                               int l,
                               int u,
                               std::string weightset = "B");

    /** \ingroup Factories
     *
     */
    static ratexp_t from(std::string str, std::string weightset = "B");

    /** \ingroup Factories
     *
     */
    static ratexp_t from_size(std::string str,
                              unsigned n,
                              std::string weightset = "B");
  };

  /** \ingroup Factories
   * Helper class that contains convenience factories to build expressions
   * whose labels are tuples.
   */
  struct with_tuple_labels {
    /** \ingroup Factories
     *
     */
    static ratexp_t from(std::string str,
                         unsigned n,
                         std::string weightset = "B");

    /** \ingroup Factories
     *
     */
    static ratexp_t from(std::string str,
                         std::vector<std::string> alphabets,
                         std::string weightset ="B");
  };

};

}} // end of namespaces awali::dyn, and awali
#endif
