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

#ifndef DYN_CONTEXT_DESCRIPTION_OLD_HH
#define DYN_CONTEXT_DESCRIPTION_OLD_HH

#include <awali/dyn/core/context_description.hh>

namespace awali {
  namespace dyn {
    namespace deprecated {
      using context_description= awali::dyn::context::context_description;
      using weightset_description= awali::dyn::context::weightset_description;
      using labelset_description= awali::dyn::context::labelset_description;
      
      
      context_description parse_context(std::istream& i);
      
      int parse_characteristic(std::istream& i);

      weightset_description parse_weightset(std::istream& i);
      
      labelset_description parse_labelset(std::istream& i);
     
      labelset_description parse_labels(std::istream& i);
      
      weightset_description parse_weights(std::istream& i);      
    }//end of ns awali::dyn::deprecated
  }//end of ns awali::dyn
} //end of ns awali

#endif
