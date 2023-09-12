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

#ifndef DYN_CORE_ABSTRACT_RATEXP_HH
#define DYN_CORE_ABSTRACT_RATEXP_HH

#include<memory>
#include<vector>
#include<iostream>
#include <awali/dyn/core/any.hh>
#include <awali/dyn/core/context.hh>
#include <awali/dyn/core/typedefs.hh>

namespace awali { namespace dyn {

//   using label_t = any_t;
//   using weight_t = any_t;

  enum class ExpKind {
    
    /** Only the "empty" rational expression is of this kind (that is, with an
     * empty language).
     *
     * This expression is the neutral element for the @ref ratexp_t::operator+
     * "sum of rational expressions".
     * 
     * @beware 
     * The empty rational expression is not the same thing as the
     * rational expression consisting of a single epsilon.
     */
    ZERO, 
    
    /** Only the "epsilon" rational expression is of this kind (that is, with a
     * language reduced to the empty word).
     *
     *  This expression is the neutral element for the @ref ratexp_t::operator*
     *  "concatenation of rational expressions".
     * */
    ONE, 

    /** Rational expressions of this kind are simply a label. 
     * @see abstract_ratexp_t::value
     * */
    ATOM, 
    
    /** Rational expression which is a sum (or union) of rational expressions.
     * @see 
     * - abstract_ratexp_t::children
     * - abstract_ratexp_t::arity
     */
    SUM, 
    
    /** Rational expression which is a product (or concatenation) of rational
     * expressions.  
     * @see 
     * - abstract_ratexp_t::children
     * - abstract_ratexp_t::arity
     */
    PROD, 

    /** Rational expressions of this kind are the star of another rational
     * expression.
     *
     * The subexpressions is given by @ref abstract_ratexp_t::children().
     */
    STAR
  };


  class ratexp_t;

  
  /** Abstract interface for rational expression at the dynamical layer;
   * lists the services provided by automata.
   *
   * See the static functions in @ref ratexp_t for constructors.
   */
  class abstract_ratexp_t {
   public:

    /** Gets the alphabet of this @ref abstract_ratexp_t */
    virtual std::vector<label_t> alphabet() const = 0;

    /** Gets the context of this @ref abstract_ratexp_t. */
    virtual context_t get_context() const = 0;

    
    virtual std::ostream& print(std::ostream&) const = 0;
    //virtual std::ostream& json(std::ostream&) const = 0;

    /* Multiplies this @ref abstract_ratexp_t by @pname{w} on the left. */
    virtual ratexp_t lmul(weight_t w) const =0;

    /* Multiplies this @ref abstract_ratexp_t by @pname{w} on the right. */
    virtual ratexp_t rmul(weight_t w) const =0;


    virtual ratexp_t add(ratexp_t) const =0;
    virtual ratexp_t mult(ratexp_t) const =0;
    virtual ratexp_t star() const =0;

    /** Returns the number of #children this @ref abstract_ratexp_t has. */
    virtual unsigned arity() const =0;

    /** Returns the children of this @ref abstract_ratexp_t.*/
    virtual const std::vector<ratexp_t>& children() const =0;

    /** Gets the kind of this @ref abstract_ratexp_t. */
    virtual ExpKind get_kind() const =0;

    /** Gets the left weight of this @ref abstract_ratexp_t. */
    virtual weight_t lweight() const =0;

    /** Gets the right weight of this @ref abstract_ratexp_t. */
    virtual weight_t rweight() const =0;

    /** Gets the label of this @ref abstract_ratexp_t (only for expression of
     * kind ExpKind::ATOM).
     */
    virtual label_t value() const =0;

    virtual unsigned size() const =0;
    virtual unsigned length() const =0;
    virtual unsigned height() const =0;

    /** Returns the maximal number of nested stars in this @ref
     * abstract_ratexp_t.
     */
    virtual unsigned star_height() const =0;
    
    virtual ~abstract_ratexp_t() {}
  };


//   ratexp_t operator+(ratexp_t e1, ratexp_t e2);
//   ratexp_t operator*(ratexp_t e1, ratexp_t e2);
  std::ostream& operator<<(std::ostream& o, ratexp_t e);

}}//end of ns awali::dyn

#endif
