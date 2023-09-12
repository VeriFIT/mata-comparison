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

#ifndef AWALI_DYN_LOADING_HANDLER_CC
#define AWALI_DYN_LOADING_HANDLER_CC

#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/loading/locations.hh>

namespace awali { namespace dyn {
  namespace loading {

    static std::map<std::string,void*> loaded_handler;

//     struct test_t { 
//       test_t() { dlopen(NULL,RTLD_NOW|RTLD_GLOBAL); }
//     };
// 
//     static test_t test;

    /* This function returns a pointer on the function 'name' of the library
       corresponding to the 'context'.
       If the library does not exist or does contain this function,
       make_library is called.
    */

    
    void* get_handler(const std::string name, const std::string group, 
        const std::string& static_context) 
    {
      std::string unique_name(name+"@"+group+"@"+static_context);
      auto handler_it= loaded_handler.find(unique_name);
      if (handler_it != loaded_handler.end())
        return handler_it->second;
      make_awali_dir();
      std::string p(libname(static_context,group)+".so");

      std::string local = get_lib_directory()[0]+"/"+p;
      std::string global = get_lib_directory()[1]+"/"+p;
      auto handle = dlopen(local.c_str(), RTLD_NOW);
      if (handle==nullptr) {
        handle =dlopen(global.c_str(), RTLD_NOW);
        if (handle==nullptr) {
          make_library(group, static_context);
          handle = dlopen(local.c_str(), RTLD_NOW);
          if(handle==nullptr)
            throw std::runtime_error(std::string("dlopen failed after compilation: ") + dlerror());
        }
      }
      auto bridge =  dlsym(handle, name.c_str());
      if (bridge==nullptr)
        throw std::runtime_error(std::string("dlsym failed: ") + dlerror());
      loaded_handler[unique_name] = bridge;
      return bridge;
    }

    void* get_handler(const std::string name, const std::string group,
                      const std::string& static_context1,
                      const std::string& static_context2) {
      make_awali_dir();
      std::string p(libname(static_context1+"_"+static_context2,group)+".so");
      std::string unique_name(name+"@"+group+"@"+p);
      auto handler_it= loaded_handler.find(unique_name);
      if (handler_it != loaded_handler.end())
        return handler_it->second;
      std::string local = get_lib_directory()[0]+"/"+p;
      std::string global = get_lib_directory()[1]+"/"+p;
      auto handle = dlopen(local.c_str(), RTLD_NOW);
      if (handle==nullptr) {
        handle = dlopen(global.c_str(), RTLD_NOW);
        if (handle==nullptr) {
          make_library(group, static_context1, static_context2);
          handle = dlopen(local.c_str(), RTLD_NOW);
          if(handle==nullptr)
            throw std::runtime_error(std::string("dlopen failed after compilation: ") + dlerror());
        }
      }
      auto bridge =  dlsym(handle, name.c_str());
      if (bridge==nullptr) {
        throw std::runtime_error(std::string("dlsym failed: ") + dlerror());
      }
      loaded_handler[unique_name] = bridge;
      return bridge;
    }

    void* get_handler(const std::string name, const std::string& static_context) {
      return get_handler(name, name, static_context);
    }



  }//end of ns loading
}}//end of ns awali::dyn


#include<awali/dyn/loading/handler_decl.hxx>

#endif
