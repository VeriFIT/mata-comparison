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

#ifndef DYN_MODULES_OUTPUT_CC
#define DYN_MODULES_OUTPUT_CC

#include <fstream>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/modules/transducer.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/loading/locations.hh>

#include <awali/dyn/modules/context.hh>

#include <awali/dyn/modules/output.hh>

#include <awali/dyn/algos/sys.hh>

#include <awali/common/json_ast.hh>

namespace awali {
  namespace dyn {

//   namespace internal {
//   std::ostream&
//   daut(automaton_t aut, std::ostream& out) {
//     return loading::call1<std::ostream&, automaton_t, std::ostream&>("daut", "output", aut, out);
//   }

    std::ostream&
    internal::dot(automaton_t aut, std::ostream& out, options_t opts)
    {
      return loading::call1<std::ostream&, automaton_t, std::ostream&, bool, bool>("dot",
             "output", aut, out, opts[KEEP_HISTORY], opts[LAYOUT]);
    }

    std::ostream&
    internal::fado(automaton_t aut, std::ostream& out)
    {
      return loading::call1<std::ostream&, automaton_t, std::ostream&>("fado",
             "output", aut, out);
    }

    std::ostream&
    internal::grail(automaton_t aut, std::ostream& out)
    {
      return loading::call1<std::ostream&, automaton_t, std::ostream&>("grail",
             "output", aut, out);
    }

//   std::ostream&
//   efsm(automaton_t aut, std::ostream& out) {
//     return loading::call1<std::ostream&, automaton_t, std::ostream&>("efsm", "output", aut, out);
//   }

    std::ostream&
    internal::json(automaton_t aut, std::ostream& out)
    {
      json_ast_t ast = to_json_ast(aut);
      return ::awali::put(ast, out);
    }


    std::ostream& internal::img(automaton_t aut, std::ostream& o, std::string img,
                                options_t opts)
    {
      std::string dot_binary = loading::get_dot_binary();
      if (dot_binary == "") {
        *warning_stream << "PDF output disabled (requires dot binary)." << std::endl;
        return o;
      }
      std::string name;
      if (aut->get_name() != "")
        name = aut->get_name();
      else
        name = "tmp";
      std::string filename= "/tmp/"+name+".gv";
      std::ofstream of(filename);
      dot(aut, of, opts) << std::endl;
      size_t buffersize = 1024;
      char buffer[buffersize];
      std::shared_ptr<FILE> pipe(popen((dot_binary+" -T"+img+" "+filename).c_str(),
                                       "r"), pclose);
      if (!pipe) throw std::runtime_error("popen() failed!");
      while (true) {
        size_t n = fread(buffer, sizeof(char), buffersize, pipe.get());
        if (n == 0)
          break;
        o.write(buffer, n);
      }
      return o;
    }

    std::ostream& internal::pdf(automaton_t aut, std::ostream& o, options_t opt)
    {
      return img(aut, o, "pdf", opt);
    }

    std::ostream& internal::svg(automaton_t aut, std::ostream& o, options_t opt)
    {
      return img(aut, o, "svg", opt);
    }



    automaton_t internal::fado(std::istream& in)
    {
      char buffer[512];
      automaton_t aut = automaton_t::from("");
      std::unordered_map<std::string, unsigned> states;

      auto get_state = [&](const std::string& s) -> unsigned {
        unsigned i;
        auto it = states.find(s);
        if(it == states.end())
        {
          i = aut->add_state(s);
          states.emplace(std::make_pair(s, i));
          return i;
        }
        return it->second;
      };

      in.getline(buffer, 512);
      std::istringstream is(buffer);
      std::string token;
      std::vector<std::string> final_states;
      std::vector<std::string> initial_states;
      is >> token;
      if(token != "@NFA" && token != "@DFA" && token != "@NDFA")
        throw std::runtime_error("Fado format");
      while(!is.eof()) {
        is >> token;
        if(token =="*")
          break;
        final_states.emplace_back(token);
      }
      while(!is.eof()) {
        is >> token;
        initial_states.emplace_back(token);
      }
      bool first=true;
      while(!in.eof()) {
        in.getline(buffer, 512);
        if(!buffer[0])
          continue;
        is.str(buffer);
        is.seekg(std::ios_base::beg);
        std::string st1, st2, label;
        is >> st1;

        if(first) {
          first=false;
          if(initial_states.empty())
            aut->set_initial(get_state(st1));
        }

        is >> label;
        aut->get_context()->add_letter(label[0]);

        is >> st2;

        aut->set_transition(get_state(st1), get_state(st2), label[0]);
      }
      for(auto s : initial_states)
        aut->set_initial(get_state(s));
      for(auto s : final_states)
        aut->set_final(get_state(s));
      return aut;
    }


    automaton_t internal::grail(std::istream& in)
    {
      char buffer[512];
      dyn::automaton_t aut = automaton_t::from("");
      std::unordered_map<std::string, unsigned> states;

      auto get_state = [&](const std::string& s) -> unsigned {
        unsigned i;
        auto it = states.find(s);
        if(it == states.end())
        {
          i = aut->add_state(s);
          states.emplace(std::make_pair(s, i));
          return i;
        }
        return it->second;
      };

      std::string s1, s2, label;
      std::istringstream is;

      while(!in.eof()) {
        in.getline(buffer, 512);
        if(!buffer[0])
          continue;
        is.str(buffer);
        is.seekg(std::ios_base::beg);
        is >> s1;
        is >> label;
        is >> s2;
        if(label == "|-")
          aut->set_initial(get_state(s2));
        else
          if(label == "-|")
            aut->set_final(get_state(s1));
          else {
            aut->get_context()->add_letter(label[0]);
            aut->set_transition(get_state(s1), get_state(s2), label[0]);
          }
      }
      return aut;
    }


    json_ast_t 
    to_json_ast(automaton_t aut, json_ast_t extra_metadata) {
      return loading::call1<json_ast_t, automaton_t>
               ("to_json_ast", "output",aut,extra_metadata);
    }
  }
}//end of ns awali::dyn

#endif
