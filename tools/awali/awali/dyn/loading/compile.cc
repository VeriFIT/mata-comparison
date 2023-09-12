
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

#ifndef AWALI_DYN_LOADING_COMPILE_CC
#define AWALI_DYN_LOADING_COMPILE_CC

#include <sys/stat.h>

#include <vector>
#include <fstream>

#include <awali/dyn/loading/compile.hh>

#include <awali/dyn/loading/locations.hh>
#include <awali/dyn/loading/parse_ctx.hh>
#include <awali/dyn/algos/sys.hh>
#include <awali/dyn/config.hh>


namespace awali { namespace dyn {
  namespace loading {

    static void
    output_header_files (std::ostream& o,
       const std::set<std::string> &weightsets,
       const std::set<std::string> &labelsets) {
      for(auto l : labelsets)
  o << "#include <awali/sttc/ctx/" << l << ".hh>" << std::endl;
      for(auto w : weightsets)
  o << "#include <awali/sttc/weightset/" << filename_of_ws(w) << ".hh>"
    << std::endl;      
    }
      

    static void
    make_set_types_file (std::ostream& o,
       const std::vector<std::string> &types) {
      o << "#include <awali/sttc/ctx/context.hh>" << std::endl;
      
      int i = 1;
      for (auto s : types) {
  std::string ws;
  std::string ls;      
  std::set<std::string> weightsets;
  std::set<std::string> labelsets;
  
  size_t p=s.length();
  rec_parse_context(s, p, weightsets, labelsets, ws, ls);
  output_header_files (o, weightsets, labelsets);
  o << "namespace awali {" << std::endl
    << "  using labelset" << i << "_t = sttc::ctx::" << ls << ';'
    << std::endl
    << "  using weightset" << i << "_t = sttc::" << classname_of_ws(ws)
    << ';' << std::endl
    << "  using context" << i
    << "_t = sttc::context<labelset" << i << "_t, weightset" << i
    << "_t>;" << std::endl
    << "}" << std::endl;
  i++;
      }
      o << "namespace awali {" << std::endl
  << "  typedef labelset1_t labelset_t; " << std::endl
  << "  typedef weightset1_t weightset_t; " << std::endl
  << "  typedef context1_t context_t;" << std::endl
  << "}" << std::endl;
    }
    
    std::string
    libname (const std::vector<std::string>& contexts,
       const std::string& name) {
      std::string result{"lib"};
      auto c = contexts.begin ();
      result += *c;
      
      for (c++; c != contexts.end (); c++) 
  result += "_" + *c;
    
      for(size_t i=0, j=result.length(); i<j; ++i)
  if(result[i]=='<' || result[i]=='>')
    result[i]='-';
      result += "-" + name;
      
      return result;
    }
      
    std::string
    libname(const std::string& ctx, const std::string& name) {
      std::vector<std::string> tmp{ctx};
      return libname (tmp, name);
    }

    std::string
    libname(const std::string& ctx1, const std::string& ctx2,
      const std::string& name) {
      std::vector<std::string> tmp{ctx1, ctx2};
      return libname (tmp, name);
    }

    static std::string
    tmp_comp_dir(const std::string& lname) {
      make_awali_dir();
      struct stat st;
      std::string dir= get_compilation_directory(lname)[0];
      if(stat(dir.data(), &st)==-1)
        {
          if(errno == ENOENT)
            if(mkdir(dir.data(), S_IRWXU)!=0)
              throw std::runtime_error("Unable to create directory: "+dir);
        }
      return dir;
    }

#define STR_VALUE(arg)      #arg
#define STR(name) STR_VALUE(name)

    static std::string 
    compile(const std::string name, const std::string& static_context) {
      make_awali_dir();
      std::string lname = libname(static_context,name);
      std::string compile_dir = tmp_comp_dir(lname);
      *warning_stream << 
        std::string("Compiling module \"" + name 
                    + "\" for a new automaton context ("
                    + static_context + ")." ) <<  std::endl;
      std::string cxx{STR(CXX_COMPILER)};
      std::string opt_11{STR(CXX_FLAGS)};
//       std::string vauc_path{STR(COMPIL_FLAGS)" "};
      std::vector<std::string> includes = {  compile_dir };
      std::vector<std::string> tmp;
      tmp = dyn::loading::get_include_directory();
      includes.insert(includes.end(), tmp.begin(), tmp.end());
      tmp = dyn::loading::get_dyn_config_hh_directory();
      includes.insert(includes.end(), tmp.begin(), tmp.end());
      tmp = dyn::loading::get_sources_directory();
      includes.insert(includes.end(), tmp.begin(), tmp.end());
//       if (!dyn_config_hh_include.empty())
//         dyn_config_hh_include= " -I"+dyn_config_hh_include+" ";
//       std::string dyn_source_dir = dyn::loading::get_source_directory();
//       if (!dyn_source_dir .empty())
//         dyn_source_dir= " -I"+dyn_source_dir+" ";

      std::string includes_conc("");
      std::string sep(" -I");
      for (const std::string& s: includes) {
        includes_conc+=sep;
        includes_conc+=s;
      }

      std::string cxx_flags = " "+opt_11+" -Wall -Wextra -Wsign-conversion -g -rdynamic -w -O3 -fpic";


      std::string source;

      bool fail = true;
      for (const std::string& s:  dyn::loading::get_sources_directory()) {
        std::string temp = s+"/awali/dyn/bridge_sttc/"+name+".cc";
        struct stat buffer;
        if (stat (temp.c_str(), &buffer) == 0) {
          source=temp;
          fail = false;
          break;
        }
      }

      if (fail)
        std::runtime_error("Could not find source file: awali/dyn/bridge_sttc/"+name+".cc");

//       std::string lname=libname(static_context,name);

      std::string modulepath = get_lib_directory()[0] + "/" + lname + ".so";
      std::string objectpath =  compile_dir + "/" +lname + ".o";
      std::string depmakepath = compile_dir + "/Makefile";
      std::string compile_cmd = cxx + cxx_flags + includes_conc

/* This is to cater to the new way osx compiles. */
#ifdef CMAKE_OSX_SYSROOT
#define STR_VALUE(arg)      #arg
#define STR(name) STR_VALUE(name)
        + " -isysroot "+STR(CMAKE_OSX_SYSROOT)
#define STR_VALUE(arg)      #arg
#define STR(name) STR_VALUE(name)
#endif
        +
        + " -c " + source
        + " -o " + objectpath
        + " -MMD" 
        + " -MF " + depmakepath
        + " -MT " + modulepath;

      //std::cerr << "Compile line : " << compile_cmd << std::endl;
      int sys_ret_val = system(compile_cmd.c_str());
      {
        std::ofstream o(depmakepath, std::ofstream::app);
        o << "\ttrue\n"; //add a phony command in the makefile
        o.close(); // immediately write everything to minimize chances 
                   //// to cancel computation in the middle of this.
      }
      if (sys_ret_val != 0) {
        std::stringstream ss;
        ss << "Compilation of module " << name
           << " for " << static_context << " failed, command was: "
           << compile_cmd << std::endl;
        std::string msg;
        std::getline(ss, msg);
         throw std::runtime_error(msg);
      }

      *warning_stream << std::string("Linking module \"" + name + "\" for a new automaton context (" + static_context + ").") << std::endl;
//       std::string ld_flags = " -shared"
      std::string lib_cmd = cxx+" -shared"
      +" -o " + modulepath
      +" "+objectpath 
//       +" -L"+get_dynlib_directory()[0]
//       +" -Wl,-rpath,"+get_dynlib_directory()[0]
//       +" -lawalidyn"
        ;

      sys_ret_val = system(lib_cmd.c_str());
      if (sys_ret_val != 0) {
        std::stringstream ss;
        ss << "Linking of module " << name
        << " for " << static_context << " failed, command was: "
        << lib_cmd << std::endl;
        std::string msg;
        std::getline(ss, msg);
        throw std::runtime_error(msg);
      }
      return modulepath;
    }

#undef STR_VALUE
#undef STR




    /* This function add the function 'name' to the library corresponding to 'context'.
       To this end, a source file is generated from the template dedicated to the
       function 'name' with the informations given by 'context'. It is compiled, add to
       the static library, and the dynamic library is rebuilt from the static one.
    */
    
    std::string
    make_library (const std::string &name,
      const std::vector<std::string> &contexts,
      bool check_dependency) {
      std::string lname= libname (contexts, name);
      std::string compile_dir = tmp_comp_dir(lname);
      if (check_dependency) {
        std::string cmd("make '"+get_lib_directory()[0]+"/"+lname+".so' --quiet -q --makefile "+compile_dir+"/Makefile 2>/dev/null");
        int sys_ret_val = system(cmd.c_str());
        /*if (sys_ret_val == -1)
          throw std::runtime_error("Unable to check dependencies " +
                                   compile_dir+"/"+lname+".d");*/
        // If make does fails (file does not exists) and 
        // returns 0, ie that the module is up to date.
        if ((sys_ret_val != -1) && (WEXITSTATUS(sys_ret_val) == 0))
          return get_lib_directory()[0]+"/"+lname+".so";
      }
      std::string dir = tmp_comp_dir (lname);
      std::ofstream st(dir+"/set-types.hh");
      make_set_types_file(st, contexts);
      st.close();

      std::string ctx = contexts[0];
      for (unsigned int i = 1; i < contexts.size (); i++)
  ctx += "_" + contexts[i];
      return compile (name, ctx);
    }
  
    std::string
    make_library(const std::string &name, const std::string &static_context,
     bool check_dependency) {
      std::vector<std::string> ctx{static_context};
      return make_library(name, ctx, check_dependency);
    }
    
    std::string
    make_library(const std::string &name, const std::string &static_context1,
     const std::string &static_context2, bool check_dependency) {
      std::vector<std::string> ctx{static_context1, static_context2};
      return make_library(name, ctx, check_dependency);
    }
  }
}}//end of ns awali::dyn

#endif
