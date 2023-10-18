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

#ifndef DYN_ABSTRACT_CONTEXT_HH
#define DYN_ABSTRACT_CONTEXT_HH

#include<memory>
#include<string>
#include<vector>
#include<list>

#include <awali/dyn/core/any.hh>
#include <awali/dyn/core/typedefs.hh>
#include <awali/dyn/core/context_description/description_types.hh>

namespace awali { namespace dyn {

  class ratexp_t;
  class automaton_t;

  /** Abstract interface representing the weightset and labelset of an
   * automaton, a rational expression or a transducer.
   */
  struct abstract_context_t {

    /** Returns the compile-time internal name of the context.
     * Two @ref automaton_t (resp. ratexp_t, resp. @ref transducer_t) wraps
     * static objects of the exact same type if and only if with contexts that
     * have equal `sname`.
     */
    virtual std::string sname() const =0;

    /** Returns the runtime internal name of the context.
     *
     * The difference with @ref sname is that the vname contains additional
     * information that are not part of the static type
     * (for instance the alphabet in the case of automata).
     */
    virtual std::string vname(bool full = true) const =0;

    /** Gives the name of the labelset. */
    virtual std::string labelset_name() const =0;

    /** Gives the name of the weightset.*/
    virtual std::string weightset_name(std::string format="text") const =0;

    /** Returns the alphabet. */
    virtual std::vector<any_t> alphabet() const = 0;

    /* Returns the object representing epsilon.
     *
     * @beware Although printing the returned value of this function would give
     * `\e`, it is not an std::string internally.
     */
    virtual label_t epsilon() const = 0;

    /** Returns `true` if epsilon is allowed as label. */
    virtual bool is_eps_allowed() const = 0;

    /** Returns the {@link context::context_description} corresponding to this
     * {@link abstract_context_t}. */
    virtual context::context_description description() const = 0;

    /** Adds a letter in the alphabet. */
    virtual void add_letter(label_t let) = 0;

    /** Tests whether @pname{let} is a valid letter for this context. */
    virtual bool has_letter(label_t let) const = 0;

    /** Returns `true` if one (neutral multiplicative element of the weightset)
     * should be printed in expressions.*/
    virtual bool show_one() const = 0;

    /** Returns the value *one*, that is, the neutral multiplicative element of
     * the weightset. */
    virtual weight_t weight_one() const = 0;

    /** Gives the value *zero*, that is, the neutral additive element of the
     * weightset.
     */
    virtual weight_t weight_zero() const = 0;

    /** Returns `true` if the weightset contains a finite number of elements. */
    virtual bool is_finite_weightset() const =0;

    /** Returns `true` if all finitely-generated sub-semiring of the weightset
     * are all finite. */
    virtual bool is_locally_finite_weightset() const =0;

    /** Returns the sum of given weights, as computed by the weightset. */
    virtual weight_t add_weights(weight_t l, weight_t r) const = 0;

    /** Returns the product of given weights, as computed by the weightset. */
    virtual weight_t mul_weights(weight_t l, weight_t r) const = 0;

    /** Returns the quotient of given weights, as computed by the weightset. */
    virtual weight_t div_weights(weight_t l, weight_t r) const = 0;

    /** Converts given string to a weight. */
    virtual weight_t get_word(std::string str) const =0;

    /** Converts given string to a weight. */
    virtual weight_t get_weight(std::string str) const =0;

    /** Converts given string to a label. */
    virtual label_t get_label(std::string str) const =0;

    /** Converts string @pname{s} to a label of the @pname{i}-th tape
     * (transducer only). */
    virtual label_t get_label(std::string str, size_t i) const =0;

    /** Computes the string-representation of given weight. */
    virtual std::string weight_to_string(weight_t w) const=0;

    /** Computes the string-representation of given label. */
    virtual std::string label_to_string(label_t w) const=0;


    /** Expands a label into a list of labels, one label per tape (transducer
     * only).
     *
     * @param lab a proper label for this {@link abstract_context_t}
     * @return A list of label from the sub-labelsets of the labelset of this
     *           {@link abstract_context_t}
     * */
    virtual std::list<any_t> expand_label(label_t lab) const=0;


    /** Builds a {@link ratexp_t} from its string-representation */
    ratexp_t make_ratexp(std::string str, bool fixed_alphabet = true);

    /** Builds an empty {@link automaton_t} with this context. */
    automaton_t make_automaton();


    virtual ~abstract_context_t() {}


    bool operator!=(abstract_context_t const& other) {
      return !(*this == other);
    }
    bool operator==(abstract_context_t const& other) {
      return (vname(true) == other.vname(true));
    }

  protected:
    /** Makes a copy of this {@link abstract_context_t}.
     *
     *  This function should generally not be used because the type
     *  {@link abstract_context_t} is not designed to be manipulated directly :
     *  use {@link context_t} and its default copy constructor instead.
     *
     *  This function exists only to be able to build a {@link context_t} from
     *  a {@link abstract_context_t}, and then call dyn-level functions
     *  without breaking pointer ownership.
     */
    virtual abstract_context_t* copy() = 0;
  };


}}//end of ns awali::dyn
#endif
