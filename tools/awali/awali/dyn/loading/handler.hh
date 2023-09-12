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

#ifndef AWALI_DYN_LOADING_HANDLER_HH
#define AWALI_DYN_LOADING_HANDLER_HH
#include <dlfcn.h>
#include <iostream>
#include <awali/dyn/loading/compile.hh>
#include <unordered_map>
#include <map>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/transducer.hh>

namespace awali {
  namespace dyn {
    namespace loading {

      /* This function returns a pointer on the function 'name' of the library
         corresponding to the 'context'.
         If the library does not exist or does contain this function,
         make_library is called.
      */
      
 
      void* get_handler(const std::string name, const std::string group,
                        const std::string& static_context);

      void* get_handler(const std::string name, const std::string group,
                        const std::string& static_context1,
                        const std::string& static_context2);

      void* get_handler(const std::string name, const std::string& static_context);

//       template<typename TYPERET, TYPERET (*FCT)(automaton_t)>
//       TYPERET call(std::string name, std::string group, automaton_t aut) {
//         std::string stat_ctx = aut->get_context()->sname();
//         typedef TYPERET (*bridge_t)(automaton_t);
//           auto bridge = (bridge_t) get_handler(name, group, aut->get_context()->sname());
//           return bridge(aut);
//       }
// 

    template<typename RET_T, typename... ARGS_T>
    RET_T
    call0(std::string name, std::string group, std::string stat_ctx,
      ARGS_T... args)
    {
        typedef RET_T (*bridge_t)(ARGS_T...);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(args...);
    }


    template<typename RET_T, typename ARG1_T, typename... ARGS_T>
    RET_T
    call1(std::string name, std::string group,
      ARG1_T arg1, ARGS_T... args)
    {   
      std::string stat_ctx = arg1->get_context()->sname();
      typedef RET_T (*bridge_t)(ARG1_T, ARGS_T...);
      bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
      return bridge(arg1, args...);
    }


    template<typename RET_T, typename ARG2_T, typename ARG1_T, 
        typename... ARGS_T>
    RET_T
    call2(std::string name, std::string group,
      ARG1_T arg1, ARG2_T arg2, ARGS_T... args)
    {   
      std::string stat_ctx1 = arg1->get_context()->sname();
      std::string stat_ctx2 = arg2->get_context()->sname();
      typedef RET_T (*bridge_t)(ARG1_T, ARG2_T, ARGS_T...);
      bridge_t bridge 
      = (bridge_t) get_handler(name, group, stat_ctx1, stat_ctx2);
      return bridge(arg1, arg2, args...);
    }


      //In name call_X_Y : 
      // - first X arguments are part of sttc context
      // - following Y arguments are not
      // The last argument (named `fct`) is only used for helping template substution. When it does not work, templates parameters should be put by hand and this argument may be omitted.
    

      /* In the special case where X=0, the static context is explicitely given as argument 3 */
      template<typename RET_T, typename ARG1_T>
      RET_T 
      call_0_1(std::string name, std::string group, std::string stat_ctx,
          ARG1_T other1,
          RET_T(*fct)(ARG1_T) = nullptr) 
      {
        typedef RET_T (*bridge_t)(ARG1_T);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(other1);
      }

      template<typename RET_T, typename ARG1_T, typename ARG2_T>
      RET_T 
      call_0_2(std::string name, std::string group, std::string stat_ctx,
          ARG1_T other1, ARG2_T other2,
          RET_T(*fct)(ARG1_T, ARG2_T) = nullptr) 
      {
        typedef RET_T (*bridge_t)(ARG1_T, ARG2_T);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(other1, other2);
      }

      template<typename RET_T, typename ARG1_T, typename ARG2_T, typename
      ARG3_T>
      RET_T 
      call_0_3(std::string name, std::string group, std::string stat_ctx,
          ARG1_T other1, ARG2_T other2, ARG3_T other3,
          RET_T(*fct)(ARG1_T, ARG2_T, ARG3_T) = nullptr) 
      {
        typedef RET_T (*bridge_t)(ARG1_T, ARG2_T, ARG3_T);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(other1, other2, other3);
      }

      template<typename RET_T, typename ARG1_T>
      RET_T 
      call_1_0(std::string name, std::string group, ARG1_T aut,
          RET_T(*fct)(ARG1_T) = nullptr) 
      {
        std::string stat_ctx = aut->get_context()->sname();
        typedef RET_T (*bridge_t)(ARG1_T);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(aut);
      }


      template<typename RET_T, typename ARG1_T, typename ARG2_T>
      RET_T 
      call_1_1(std::string name, std::string group, ARG1_T aut1, 
          ARG2_T other1, RET_T(*fct)(ARG1_T, ARG2_T) = nullptr) 
      {
        std::string stat_ctx = aut1->get_context()->sname();
        typedef RET_T (*bridge_t)(ARG1_T,ARG2_T);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(aut1, other1);
      }

      template<typename RET_T, typename ARG1_T, typename ARG2_T, 
          typename ARG3_T>
      RET_T 
      call_1_2(std::string name, std::string group, ARG1_T aut1, 
          ARG2_T other1, ARG3_T other2, RET_T(*fct)(ARG1_T, ARG2_T, ARG3_T) = nullptr) 
      {
        std::string stat_ctx = aut1->get_context()->sname();
        typedef RET_T (*bridge_t)(ARG1_T,ARG2_T,ARG3_T);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(aut1, other1, other2);
      }

      template<typename RET_T, typename ARG1_T, typename ARG2_T, 
          typename ARG3_T, typename ARG4_T>
      RET_T 
      call_1_3(std::string name, std::string group, ARG1_T aut1, 
          ARG2_T other1, ARG3_T other2, ARG4_T other3,
          RET_T(*fct)(ARG1_T, ARG2_T, ARG3_T, ARG4_T) = nullptr) 
      {
        std::string stat_ctx = aut1->get_context()->sname();
        typedef RET_T (*bridge_t)(ARG1_T,ARG2_T,ARG3_T,ARG4_T);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(aut1, other1, other2, other3);
      }

      template<typename RET_T, typename ARG1_T, typename ARG2_T, 
          typename ARG3_T, typename ARG4_T, typename ARG5_T>
      RET_T 
      call_1_4(std::string name, std::string group, ARG1_T aut1, 
          ARG2_T other1, ARG3_T other2, ARG4_T other3, ARG5_T other4,
          RET_T(*fct)(ARG1_T, ARG2_T, ARG3_T, ARG4_T, ARG5_T) = nullptr) 
      {
        std::string stat_ctx = aut1->get_context()->sname();
        typedef RET_T (*bridge_t)(ARG1_T,ARG2_T,ARG3_T,ARG4_T, ARG5_T);
        bridge_t bridge = (bridge_t) get_handler(name, group, stat_ctx);
        return bridge(aut1, other1, other2, other3, other4);
      }

      template<typename RET_T, typename ARG1_T, typename ARG2_T>
      RET_T 
      call_2_0(std::string name, std::string group, ARG1_T aut1,
        ARG2_T aut2, RET_T(*fct)(ARG1_T, ARG2_T) = nullptr)
      {
        std::string stat_ctx1 = aut1->get_context()->sname();
        std::string stat_ctx2 = aut2->get_context()->sname();
        typedef RET_T (*bridge_t)(ARG1_T, ARG2_T);
        bridge_t bridge 
        = (bridge_t) get_handler(name, group, stat_ctx1, stat_ctx2);
        return bridge(aut1, aut2);
      }

      template<typename RET_T, typename ARG1_T, typename ARG2_T, 
        typename ARG3_T>
      RET_T 
      call_2_1(std::string name, std::string group, ARG1_T aut1,
        ARG2_T aut2, ARG3_T other1, RET_T(*fct)(ARG1_T, ARG2_T, ARG3_T) = nullptr)
      {
        std::string stat_ctx1 = aut1->get_context()->sname();
        std::string stat_ctx2 = aut2->get_context()->sname();
        typedef RET_T (*bridge_t)(ARG1_T, ARG2_T, ARG3_T);
        bridge_t bridge 
        = (bridge_t) get_handler(name, group, stat_ctx1, stat_ctx2);
        return bridge(aut1, aut2, other1);
      }

      template<typename TYPERET, typename ARG1, typename ARG2,
        TYPERET(*FCT)(ARG1,ARG2)>
      TYPERET call_tdc2(std::string name, std::string group, ARG1 aut1, ARG2 aut2) 
      {
        std::string stat_ctx1 = aut1->get_context()->sname();
        std::string stat_ctx2 = aut2->get_context()->sname();
        std::string stat_ctx = stat_ctx1+"_"+stat_ctx2;
        typedef TYPERET (*bridge_t)(ARG1, ARG2);
        static std::unordered_map<std::string, void*> bridges;
        auto it = bridges.find(stat_ctx);
        if(it == bridges.end()) {
          auto bridge = (bridge_t) get_handler(name, group, stat_ctx1, stat_ctx2);
          bridges.emplace(stat_ctx, (void*) bridge);
          return bridge(aut1, aut2);
        }
        else {
          return ((bridge_t)it->second)(aut1, aut2);
        }
      }


//       template<typename TYPERET, typename T2, TYPERET (*FCT)(automaton_t,T2)>
//       TYPERET call2(std::string name, std::string group, automaton_t aut, T2 arg) {
//         std::string stat_ctx = aut->get_context()->sname();
//         typedef TYPERET (*bridge_t)(automaton_t,T2);
//         static std::unordered_map<std::string, void*> bridges;
//         auto it = bridges.find(stat_ctx);
//         if(it == bridges.end()) {
//           auto bridge = (bridge_t) get_handler(name, group, aut->get_context()->sname());
//           bridges.emplace(stat_ctx, (void*) bridge);
//           return bridge(aut, arg);
//         }
//         else {
//           return ((bridge_t)it->second)(aut, arg);
//         }
//       }
// 
//       template<typename TYPERET, typename T2, typename T3, TYPERET (*FCT)(automaton_t,T2,T3)>
//       TYPERET call3(std::string name, std::string group, automaton_t aut, T2 arg2, T3 arg3) {
//         std::string stat_ctx = aut->get_context()->sname();
//         typedef TYPERET (*bridge_t)(automaton_t,T2,T3);
//         static std::unordered_map<std::string, void*> bridges;
//         auto it = bridges.find(stat_ctx);
//         if(it == bridges.end()) {
//           auto bridge = (bridge_t) get_handler(name, group, aut->get_context()->sname());
//           bridges.emplace(stat_ctx, (void*) bridge);
//           return bridge(aut, arg2, arg3);
//         }
//         else {
//           return ((bridge_t)it->second)(aut, arg2, arg3);
//         }
//       }
// 
//       template<typename TYPERET, typename T2, typename T3, typename T4, TYPERET (*FCT)(automaton_t,T2,T3,T4)>
//       TYPERET call4(std::string name, std::string group, automaton_t aut, T2 arg2, T3 arg3, T4 arg4) {
//         std::string stat_ctx = aut->get_context()->sname();
//         typedef TYPERET (*bridge_t)(automaton_t,T2,T3,T4);
//         static std::unordered_map<std::string, void*> bridges;
//         auto it = bridges.find(stat_ctx);
//         if(it == bridges.end()) {
//           auto bridge = (bridge_t) get_handler(name, group, aut->get_context()->sname());
//           bridges.emplace(stat_ctx, (void*) bridge);
//           return bridge(aut, arg2, arg3, arg4);
//         }
//         else {
//           return ((bridge_t)it->second)(aut, arg2, arg3, arg4);
//         }
//       }
// 
//       template<typename T, T (*FCT)(automaton_t,automaton_t)>
//       T call_mixte(std::string name, std::string group, automaton_t aut1, 
//                 automaton_t aut2) 
//       {
//         std::string stat_ctx1 = aut1->get_context()->sname();
//         std::string stat_ctx2 = aut2->get_context()->sname();
//         std::string stat_ctx = stat_ctx1+"_"+stat_ctx2;
//         typedef T (*bridge_t)(automaton_t, automaton_t);
//         static std::unordered_map<std::string, void*> bridges;
//         auto it = bridges.find(stat_ctx);
//         if(it == bridges.end()) {
//           auto bridge = (bridge_t) get_handler(name, group, stat_ctx1, stat_ctx2);
//           bridges.emplace(stat_ctx, (void*) bridge);
//           return bridge(aut1, aut2);
//         }
//         else {
//           return ((bridge_t)it->second)(aut1, aut2);
//         }
//       }
// 
//       template<typename T, typename T2, T (*FCT)(automaton_t,automaton_t,T2)>
//       T call_mixte2(std::string name, std::string group, automaton_t aut1, automaton_t aut2, T2 arg2) {
//         std::string stat_ctx1 = aut1->get_context()->sname();
//         std::string stat_ctx2 = aut2->get_context()->sname();
//         std::string stat_ctx = stat_ctx1+"_"+stat_ctx2;
//         typedef T (*bridge_t)(automaton_t, automaton_t, T2);
//         static std::unordered_map<std::string, void*> bridges;
//         auto it = bridges.find(stat_ctx);
//         if(it == bridges.end()) {
//           auto bridge = (bridge_t) get_handler(name, group, stat_ctx1, stat_ctx2);
//           bridges.emplace(stat_ctx, (void*) bridge);
//           return bridge(aut1, aut2, arg2);
//         }
//         else {
//           return ((bridge_t) it->second)(aut1, aut2, arg2);
//         }
//       }
    }
  }
}//end of ns awali::dyn

#define HANDLER_EXTERN extern
#include<awali/dyn/loading/handler_decl.hxx>
#undef HANDLER_EXTERN

#endif
