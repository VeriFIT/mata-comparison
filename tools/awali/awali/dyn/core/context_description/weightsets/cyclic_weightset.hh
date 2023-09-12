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

#ifndef DYN_CYCLIC_WEIGHTSET_HH
#define DYN_CYCLIC_WEIGHTSET_HH
#include <awali/dyn/core/context_description/weightsets/abstract_weightset.hh>
#include<sstream>

namespace awali {
namespace dyn {
namespace context {

    struct cyclic_weightset : abstract_weightset {
      
      static std::string static_pub_name;

          static std::string static_pub_description;

      weightset_description fromstring(const std::string &k) const override;
     
      const std::string& static_public_name() const override;

      const std::string& static_desc() const override;

      const std::string& tostring(weightset_description ws, bool dynamic) const override;

      weightset_description parse_weightset(json::object_t const* p) const override;

      virtual bool is_promotable_to(weightset_description ws) const override;
      
    };

}}} //end of namespaces awali::dyn::context, awali::dyn, and awali

#endif //!DYN_CYCLIC_WEIGHTSET_HH
