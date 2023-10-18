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

#ifndef DYN_ABSTRACT_WEIGHTSET_HH
#define DYN_ABSTRACT_WEIGHTSET_HH

#include <awali/sttc/misc/attributes.hh>

#include <awali/common/json_ast.hh>

#include <awali/dyn/core/context_description/description_types.hh>

#include<vector>
#include<string>
#include<iostream>

namespace awali {
namespace dyn {
namespace context {

  // Every object of type abstract_weightset has a distinct index and is
  // stored into the instances vector defined in context_descritpion.hh
  // The description of each weightset is given in context_description.hh
    struct abstract_weightset {
      static int counter;
      const int index;
    protected:
      const std::string public_name;
      const std::string desc;
    public:

      abstract_weightset(const std::string& public_name,
			 const std::string& desc) :
	index(counter++), public_name(public_name), desc(desc) {}
      
      inline
      int getIndex() const{
              return index;
      }

      virtual const std::string& tostring(weightset_description ws, bool dynamic) const = 0;

      virtual
      const std::string& static_public_name() const {
	return public_name;
      }
      
      virtual
      const std::string& static_desc() const {
	return desc;
      }
      
      virtual weightset_description fromstring(const std::string &k) const =0;

      virtual weightset_description parse_weightset(json::object_t const* i) const = 0;

      virtual std::vector<std::string> const& possible_promotions() const;

      virtual bool is_promotable_to(weightset_description ws) const =0;

      virtual ~abstract_weightset() {}
    };


}}} //end of namespaces awali:dyn::context, awali::dyn, and awali
#endif
