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

#ifndef DYN_BASIC_WEIGHTSET_HH
#define DYN_BASIC_WEIGHTSET_HH

#include <awali/dyn/core/context_description/weightsets/abstract_weightset.hh>

/* The description of each weightset is given in context_description.hh
   where abstract_weightset objects are created
*/

namespace awali {
namespace dyn {
namespace context {

    struct basic_weightset : abstract_weightset {
    private:
      const std::string class_name;
      const std::vector<std::string> promotion;
    public:
      basic_weightset(const std::string& public_name, 
                                          const std::string& class_name, 
                                          std::initializer_list<std::string> pr,
                                          const std::string& desc
		      ) : abstract_weightset(public_name, desc), class_name(class_name), 
                          promotion(pr) {}
                          
      std::string const& tostring(weightset_description, bool) const override
      {
        return class_name;
      }
      //Returns an empty shared pointer if the string does not correspond to the weightset
    weightset_description fromstring(const std::string &k) const override;

      inline
      weightset_description parse_weightset(json::object_t const* p) const override {
                return fromstring(p->at("semiring")->to_string());
      }

      bool is_promotable_to(weightset_description ws) const override ;

      virtual std::vector<std::string> const& possible_promotions() const override;
      
      virtual ~basic_weightset() {}
      };


    }
  }
} //end of namespaces awali::dyn::context, awali::dyn, and awali

#endif // DYN_BASIC_WEIGHTSET_HH
