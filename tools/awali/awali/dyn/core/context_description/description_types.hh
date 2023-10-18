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
      ONESET, NULLABLE, LETTERSET, INTLETTERSET, INTWORDSET, WORDSET, TUPLE
        };

    struct WTypes {
      static const int RATEXP = -1;
      static const int SERIES = -2;
      static const int TUPLE = -3;
    };
    
    struct labelset_description_impl {
      CTypes type_;
      std::vector<std::string> alphabet = {};
      std::vector<int> int_alphabet = {};
      std::vector<labelset_description> children_ = {};
      
      inline bool operator==(labelset_description_impl const& other) const {
        bool b =    type_ == other.type_
                 && alphabet == other.alphabet
                 && int_alphabet == other.int_alphabet
                 && (children_.size() == other.children_.size());
        if (!b)
          return b;
        for (size_t i = 0; i < children_.size(); ++i)
          if ( !(*(children_[i]) == *(other.children_[i])) )
            return false;
        return true;
      }
      
      inline bool is_lao() const { return (type_ == CTypes::ONESET); }
      inline bool is_lan() const { return (type_ == CTypes::NULLABLE); }
      inline bool is_lat() const { return (type_ == CTypes::TUPLE);}
      inline bool is_lal() const { 
        return (type_ == CTypes::LETTERSET || type_ == CTypes::INTLETTERSET );}
      inline bool is_law() const {  
        return (type_ == CTypes::WORDSET || type_ == CTypes::INTWORDSET );}

      inline bool letters_are_int() const { 
        if (type_ == CTypes::INTLETTERSET || type_ == CTypes::INTWORDSET)
          return true;
        if(is_lan() || is_lan()) {
          for (auto child : children_)
            if (!child->letters_are_int())
              return false;
          return true;
        }
        return false;
      }
      
      inline bool letters_are_char() const { 
        if (type_ == CTypes::LETTERSET || type_ == CTypes::WORDSET)
          return true;
        if(is_lan() || is_lan()) {
          for (auto child : children_)
            if (!child->letters_are_int())
              return false;
          return true;
        }
        return false;
      }

      size_t tape_number() const {
        if (!is_lat())
          return 1;
        return children_.size();
      }

    };
    
    struct context_description_impl {
      labelset_description ls_;
      weightset_description ws_;
      
      inline labelset_description labelset() const { return ls_; }
      inline weightset_description weightset() const { return ws_; }
      
      inline bool operator==(context_description_impl const& other) const {
        return ls_ == other.labelset() && ws_ == other.weightset(); }
    };
    
    
    struct weightset_description_impl {
      int type_;
      context_description ct_ = nullptr;
      std::vector<weightset_description> children_ = {};
      int characteristic = -1;
      inline bool operator==(weightset_description_impl const& other) const {
        if (!(      type_ == other.type_
               && (   (ct_ == nullptr && other.ct_ == nullptr) 
                   || *ct_ == *(other.ct_))
               && children_.size() == other.children_.size()
               && characteristic == other.characteristic))
           return false;
        for (size_t i = 0; i < children_.size(); ++i)
          if ( !(*(children_[i]) == *(other.children_[i])) )
            return false;
        return true;
      }
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
