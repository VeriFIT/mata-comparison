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

#ifndef DYN_MODULES_OUTPUT_HH
#define DYN_MODULES_OUTPUT_HH

#include <awali/common/json_ast.hh>
#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/core/transducer.hh>
#include <awali/dyn/modules/automaton.hh>
#include <awali/dyn/loading/handler.hh>

namespace awali {
  namespace dyn {

    namespace internal {
      std::ostream& daut(automaton_t aut, std::ostream& out);
      std::ostream& dot(automaton_t aut, std::ostream& out, options_t opts = {});
      std::ostream& fado(automaton_t aut, std::ostream& out);
      std::ostream& grail(automaton_t aut, std::ostream& out);
      std::ostream& efsm(automaton_t aut, std::ostream& out);
      std::ostream& json(automaton_t aut, std::ostream& out);
      std::ostream& img(automaton_t aut, std::ostream& o, std::string img,
                        options_t opts);
      std::ostream& pdf(automaton_t aut, std::ostream& o, options_t opts = {});
      std::ostream& svg(automaton_t aut, std::ostream& o, options_t opts = {});

      automaton_t fado(std::istream& in);
      automaton_t grail(std::istream& in);
    }


    
    json_ast_t to_json_ast(automaton_t aut, 
      json_ast_t extra_medata = json_ast::empty());
//   struct io {
//    private :
//     io(std::string name,unsigned v);
//     const std::string name;
//     const static std::vector<const io*>& instances();
//    public :
//     unsigned int v;
//     static const io dot;
//     static const io fado;
//     static const io grail;
//     static const io json;
//     static const io pdf;
//     static const io svg;
//
//
//     static io of_string(const std::string& str);
//   };

//   const io io::dot;
//   const io io::fado;
//   const io io::grail;
//   const io io::json;
//   const io io::pdf;
//   const io io::svg;





  }
}//end of ns awali::dyn

#endif
