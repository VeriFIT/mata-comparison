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

#ifndef DYN_ABSTRACT_CONTEXT_HH
#define DYN_ABSTRACT_CONTEXT_HH

#include<memory>
#include<string>
#include<vector>
#include<list>

#include <awali/dyn/core/any.hh>
#include <awali/dyn/core/typedefs.hh>


namespace awali { namespace dyn {

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

    /** Adds a letter in the alphabet. */
    virtual void add_letter(label_t let) = 0;

    /** Tests whether @pname{let} is a valid letter. */
    virtual bool has_letter(label_t let) const = 0;

    /** Returns `true` if one (neutral multiplicative element of the weightset)
     * should be printed in expressions.
     */
    virtual bool show_one() const = 0;

    /** Returns the value *one*, that is, the neutral multiplicative element of
     * the weightset. */
    virtual weight_t weight_one() const = 0;

    /** Gives the value *zero*, that is, the neutral additive element of the
     * weightset. 
     */
    virtual weight_t weight_zero() const = 0;
    virtual bool is_finite_weightset() const =0;
    virtual bool is_locally_finite_weightset() const =0;

    virtual weight_t add_weights(weight_t l, weight_t r) const = 0;
    virtual weight_t mul_weights(weight_t l, weight_t r) const = 0;
    virtual weight_t div_weights(weight_t l, weight_t r) const = 0;
    
    virtual weight_t get_weight(std::string w) const =0;
    virtual label_t get_label(std::string w) const =0;
    virtual label_t get_label(std::string w, size_t i) const =0;

    virtual std::string weight_to_string(weight_t w) const=0;
    virtual std::string label_to_string(label_t w) const=0;

   
    virtual std::list<any_t> expand_label(any_t l) const=0;

    
    virtual ~abstract_context_t() {}
  };
  

}}//end of ns awali::dyn
#endif
