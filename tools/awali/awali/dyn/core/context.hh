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

#ifndef DYN_CORE_CONTEXT_HH
#define DYN_CORE_CONTEXT_HH

#include<list>
#include<memory>
#include<string>
#include<vector>

#include <awali/dyn/core/abstract_context.hh>
#include <awali/dyn/core/context_description.hh>

namespace awali { namespace dyn {

  /** Dynamical wrapper for a context, that is a weightset and a labelset 
   *
   * It is essentially a shared pointer to an {@link abstract_context_t}; 
   * see the documentation of that class for an account on the services 
   * provded by contexts.
   *
   * Objects of type `context_t` hold an actual static context, by opposition
   * to objects of type {@link context::context_description}, which just hold
   * informations about a context.
   *
   */
  class context_t 
  : public std::shared_ptr<abstract_context_t> 
  {
   public:
    
    /** Buils an empty context (i.e. a `nullptr`); should generally not be
     * used. 
     */
    context_t();

    /** Builds a context from a shared pointer to a class derived
     * of {@link abstract_context_t}; should generally not be
     * used. 
     *
     *  Both @pname{ptr} and the built context will share ownership of
     *  pointed object.
     * */
    template <class T>
    context_t(
      const std::shared_ptr<T> &ptr,
      typename std::enable_if<
        std::is_base_of<abstract_context_t,T>::value,int
      >::type = 0 
    ) 
      : std::shared_ptr<abstract_context_t>(ptr)
    {}

    /** General constructor for building a context. */
    context_t(context::labelset_description ld, 
              context::weightset_description wd);

  };

}}//end of ns awali::dyn
#endif
