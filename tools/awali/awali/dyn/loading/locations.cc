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

#include <awali/dyn/loading/locations.hh>

#include<cstdlib>
#include<iostream>
#include<fstream>
#include <vector>
#include <map>

#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <awali/dyn/config.hh>
//#include <awali/common/json-utils.hh>
#include <awali/dyn/algos/sys.hh>

namespace awali { namespace dyn {
  namespace loading {

    const std::string&  get_dot_binary() {
      static std::string dot_binary 
      #ifdef DOT_BIN
      #define STR_VALUE(arg)      #arg
      #define STR(name) STR_VALUE(name)
      = STR(DOT_BIN);
      #undef STR
      #undef STR_VALUE
      #else
      = system("which dot > /dev/null 2>&1")?"":"dot";
      #endif
      return dot_binary;
    }
    
    std::vector<std::string> get_lib_directory() {
#define STR_VALUE(arg)      #arg
#define STR(name) STR_VALUE(name)
#ifdef AWALICPP_MODULE_COMPILE_DIR
      std::string local = std::string(STR(AWALICPP_MODULE_COMPILE_DIR));
#else
      std::string local(std::getenv("HOME"));
      local.append("/.awali/modules");
#endif
      std::string global = std::string(STR(AWALICPP_MODULE_INSTALL_DIR));
#undef STR
#undef STR_VALUE
      return {local,global};
    }

    std::vector<std::string> get_examples_directory() {
      #define STR_VALUE(arg)      #arg
      #define STR(name) STR_VALUE(name)
      #ifdef AWALI_SHARE_PATH
      std::string local(std::string(STR(AWALI_SHARE_PATH))+"/awali");
      #else
      throw std::runtime_error("Share directory not set.");
      #endif
      #ifdef AWALICPP_SHARE_INSTALL_PREFIX
      std::string global(std::string(STR(AWALICPP_SHARE_INSTALL_PREFIX))+"/awali");
      #else
      throw std::runtime_error("Installed share directory not set.");
      #endif

      #undef STR
      #undef STR_VALUE

      return {local, global};
    }

    std::vector<std::string> get_sources_directory() {
      #define STR_VALUE(arg)      #arg
      #define STR(name) STR_VALUE(name)
      #ifdef AWALI_SHARE_PATH
      std::string local(STR(AWALI_SHARE_PATH));
      #else
      throw std::runtime_error("Share directory not set.");
      #endif
      #ifdef AWALICPP_SHARE_INSTALL_PREFIX
      std::string global(std::string(STR(AWALICPP_SHARE_INSTALL_PREFIX))+"/awali/src/");
      #else
      throw std::runtime_error("Installed share directory not set.");
      #endif

      #undef STR
      #undef STR_VALUE

      return {local, global};
    }

    std::vector<std::string> get_dynlib_directory() {
      #define STR_VALUE(arg)      #arg
      #define STR(name) STR_VALUE(name)

      #ifdef AWALI_DYNLIB_PATH
      std::string local = std::string(STR(AWALI_DYNLIB_PATH));
      #else
      throw std::runtime_error("Dynamic-library directory not set.");
      #endif

      #ifdef AWALICPP_DYNLIB_INSTALL_DIR
      std::string global = std::string(STR(AWALICPP_DYNLIB_INSTALL_DIR));
      #else
      throw std::runtime_error("Installed Dynamic-library directory not set.");
      #endif

      return {local,global};
    }

//     std::vector<std::string> get_share_directory() {
//       #define STR_VALUE(arg)      #arg
//       #define STR(name) STR_VALUE(name)
//       #ifdef AWALI_SHARE_PATH
//       std::string local(STR(AWALI_SHARE_PATH));
//       #else
//       throw std::runtime_error("Share directory not set.");
//       #endif
//       #ifdef AWALICPP_SHARE_INSTALL_PREFIX
//       std::string global(STR(AWALICPP_SHARE_INSTALL_PREFIX));
//       #else
//       throw std::runtime_error("Installed share directory not set.");
//       #endif
// 
//       #undef STR
//       #undef STR_VALUE
// 
//       return {local, global};
//     }

    std::vector<std::string> get_include_directory() {
      #define STR_VALUE(arg)      #arg
      #define STR(name) STR_VALUE(name)

      #ifdef AWALI_INCLUDE_PATH
      std::string local(STR(AWALI_INCLUDE_PATH));
      #else
      throw std::runtime_error("Include directory not set.");;
      #endif

      #ifdef AWALICPP_HEADER_INSTALL_PREFIX
      std::string global(STR(AWALICPP_HEADER_INSTALL_PREFIX));
      #else
      throw std::runtime_error("Include directory not set.");;
      #endif

      #undef STR
      #undef STR_VALUE

      return {local, global};
    }

//     std::string get_source_directory() {
//       #ifdef AWALI_SOURCE_PATH
//       #define STR_VALUE(arg)      #arg
//       #define STR(name) STR_VALUE(name)
//       return std::string(STR(AWALI_SOURCE_PATH));
//       #undef STR
//       #undef STR_VALUE
//       #else
//       throw std::runtime_error("Source directory not set.");
//       #endif
//     }

//     std::string get_awali_directory() {
//       #ifdef AWALI_PATH
//       #define STR_VALUE(arg)      #arg
//       #define STR(name) STR_VALUE(name)
//       return std::string(STR(AWALI_PATH));
//       #undef STR
//       #undef STR_VALUE
//       #else
//       const char* dir=std::getenv("AWALI_PATH");
//       if(dir==NULL) {
//         std::cerr << "Environment variable AWALI_PATH is not set" << std::endl;
//         std::cerr << "Please define : export AWALI_PATH=[...]/awali" << std::endl;
//         exit(0);
//       }
//       return std::string(dir);
//       #endif
//     }

    int make_awali_dir() {
      struct stat st;
      std::string dir1= get_lib_directory()[0];
      std::string dir2= get_compilation_directory("")[0];
      std::string surdir= dir1.substr(0,dir1.find_last_of('/')); 
      for(std::string dir : {surdir,dir1,dir2}) {
        if(stat(dir.data(), &st)==-1) {
          if(errno == ENOENT) {
            if(mkdir(dir.data(), S_IRWXU)!=0) {
              *error_stream << "Unable to create directory :"
                            << dir << std::endl;
              return -1;
            }

          }
          else {
            *error_stream << "Something wrong with this directory name:"
                          << dir << std::endl;
            return -1;
          }
        }
        else if (!S_ISDIR(st.st_mode)) {
          *error_stream << "The following file exists but is not a directory:"
                        << dir << std::endl;
          return -1;
        }
      }
      return 0;
    }

    std::vector<std::string> get_compilation_directory(std::string const& lname)
    {
      #ifdef AWALI_TMP_DIR
      #define STR_VALUE(arg)      #arg
      #define STR(name) STR_VALUE(name)
        return {std::string(STR(AWALI_TMP_DIR))+"/"+lname};
      #undef STR
      #undef STR_VALUE
      #else
        std::string local(std::getenv("HOME"));
        local.append("/.awali/tmp/");
        local.append(lname);
        return {std::move(local)};
      #endif
    }

    std::vector<std::string> get_dyn_config_hh_directory() {
      #ifdef AWALI_DYN_CONFIG_HH_PATH
      #define STR_VALUE(arg)      #arg
      #define STR(name) STR_VALUE(name)
        return {std::string(STR(AWALI_DYN_CONFIG_HH_PATH))};
      #undef STR
      #undef STR_VALUE
      #else
        throw std::runtime_error("Directory of \"config.hh\" not set.");
      #endif

    }

//     std::vector<file_loc_t> example_automata() {
//       std::map<std::string, file_loc_t> result;
//       std::vector<std::string> dirs = get_share_directory();
//       for (std::string dir : dirs) {
//         dir+="/awali/automata";
//         DIR* d = opendir(dir.c_str());
//         struct dirent * entry;
//         while( (d != nullptr) && ((entry=readdir(d))!=nullptr) ) {
//           std::string file (entry->d_name);
//           size_t u = file.find_last_of('.');
//           if ( (file.substr(u+1)=="json")) {
//             std::string file_no_ext = file.substr(0,u);
// 
//             std::ifstream fic(dir+'/'+file);
//             std::string desc = internal::get_first_attr(fic);
//             if(desc=="Description") {
//               desc = internal::parsestring(fic);
//             } else {
//               desc = "";
//             }
// 
//             fic.close();
//             if (result.find(file_no_ext) == result.end())
//               result[file_no_ext] = file_loc_t{dir,file_no_ext,file.substr(u+1),desc};
//           }
//         }
//   if(d != nullptr)
//     closedir(d);
//       }
//       std::vector<file_loc_t> real_result;
//       real_result.reserve(result.size());
//       for (const auto &s : result) {
//         real_result.push_back(s.second);
//       }
//       return real_result;
//     }

     void 
     add_files_from(std::string const& dir,
      std::map<std::string, file_loc_t>& result,
      std::string const& prefix, bool recurse, std::string const& type) 
    { 
      DIR* d = opendir(dir.c_str());
      struct dirent * entry;
      while( (d != nullptr) && ((entry=readdir(d))!=nullptr) ) {
        std::string file (entry->d_name);
        size_t u = file.find_last_of('.');
        if (entry->d_type == DT_DIR && file[0] != '.')  {
          if (recurse)
            add_files_from(dir+"/"+file,result,prefix+file+"::", true, type);
        } 
        else if ( (file.substr(u+1)=="json")) {
          std::string name = file.substr(0,u);
          std::string fullname = prefix+name;
          if (result.find(prefix+name) == result.end())
            result.emplace(std::move(fullname), 
                           file_loc_t{dir, std::move(name), "json", type});
        }
      }
      if(d != nullptr)
        closedir(d);
    }

    std::map<std::string,file_loc_t> example_automata(bool recurse) {
      return examples({"automata"}, recurse);
    }
    std::map<std::string,file_loc_t> example_ratexps(bool recurse) {
      return examples({"ratexps"}, recurse);
    }

    std::map<std::string,file_loc_t> 
    examples( std::vector<std::string> subdirs, bool recurse )
    {
      std::map<std::string, file_loc_t> result;
      std::vector<std::string> dirs = get_examples_directory();
      for (std::string dir : dirs)
        for (std::string subdir : subdirs) 
          add_files_from(dir+"/"+subdir, result, "", recurse, subdir);
      return result;
    }



  }

}}//end of ns awali::dyn
