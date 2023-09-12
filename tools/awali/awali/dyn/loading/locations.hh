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

#ifndef AWALI_MAKEDIR_HH
#define AWALI_MAKEDIR_HH

#include<string>
#include<vector>
#include<map>

namespace awali { namespace dyn {

  namespace loading {
    /** Return the binary of dot. */
    const std::string& get_dot_binary();

    /** Returns the directories where the static modules are located, sorted by
     * precedence. */
    std::vector<std::string> get_lib_directory();

    /** Returns the directories where the dynamic library is located, sorted by
     * precedence. */
    std::vector<std::string> get_dynlib_directory();

    /** Returns the directories where the example automata are stored, sorted
     * by precedence. */
    std::vector<std::string> get_examples_directory();
    
    /** Returns the directories where the module sources are located, sorted by
     * precedence. */
    std::vector<std::string> get_sources_directory();
    
    /** Returns the directories where the Awali headers are located. */
    std::vector<std::string> get_include_directory();

    /** Returns the temporary compilation directory for the static module named
     * @pname{lname}. */
    std::vector<std::string> get_compilation_directory(std::string lname);

    /** Returns the directory where the config.hh file is located, stored by
     * precedence. */
    std::vector<std::string> get_dyn_config_hh_directory();
    int make_awali_dir();

    struct file_loc_t {
      std::string dir;
      std::string name;
      std::string ext;
      std::string type;
      inline std::string full() { return dir+"/"+name+"."+ext;}
    };

    std::map<std::string,file_loc_t> example_automata(bool recurse = false);
    std::map<std::string,file_loc_t> example_ratexps(bool recurse = false);
    std::map<std::string,file_loc_t> examples
      (
        std::vector<std::string> subdirs = {"automata","ratexps"}, 
        bool recurse = false
      );
  }

}}//end of ns awali::dyn

#endif
