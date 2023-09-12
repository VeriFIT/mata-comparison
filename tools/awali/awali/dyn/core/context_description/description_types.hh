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

#ifndef DYN_CONTEXT_DESCRIPTION_TYPES_HH
#define DYN_CONTEXT_DESCRIPTION_TYPES_HH

#include<vector>
#include<memory>

namespace awali {
namespace dyn {
namespace context {
    
    struct context_description_impl;
    struct labelset_description_impl;
    struct weightset_description_impl;

    using context_description = std::shared_ptr<context_description_impl>;
    using labelset_description = std::shared_ptr<labelset_description_impl>;
    using weightset_description = std::shared_ptr<weightset_description_impl>;
    
    
    enum class CTypes {
      ONESET, NULLABLE, LETTERSET, INTLETTERSET, WORDSET, TUPLE
        };

    struct WTypes {
      static const int RATEXP = -1;
      static const int SERIES = -2;
      static const int TUPLE = -3;
    };
    
    struct labelset_description_impl {
      CTypes type_;
      std::vector<std::string> alphabet;
      std::vector<int> int_alphabet;
      std::vector<labelset_description> children_;
    };
    
    struct context_description_impl {
      labelset_description ls_;
      weightset_description ws_;
    };
    
    
    struct weightset_description_impl {
      int type_;
      context_description ct_;
      std::vector<weightset_description> children_;
      int characteristic;
    };

    //***********************
    
    inline
    labelset_description make_labelset_description() {
      return std::make_shared<labelset_description_impl>();
    }
    
    inline
    weightset_description make_weightset_description() {
      return std::make_shared<weightset_description_impl>();
    }
    
    inline
    context_description make_context_description() {
      return std::make_shared<context_description_impl>();
    }
    
    //**************************

}}} //end of namespaces awali::dyn::context, awali::dyn, and awali

#endif
