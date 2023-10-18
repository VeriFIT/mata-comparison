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

#include <fstream>

#include <awali/dyn.hh>

using namespace awali::dyn;
using namespace awali;

int main(int argc, char **argv) {
  if(argc!=2){
    std::cerr << "Usage : " << argv[0] << " [file]" << std::endl;
    return 1;
  }
  //Open a json description of an automaton
  //std::ifstream file(argv[1]);
  automaton_t aut = load(argv[1]);
  
  std::cout << "Output the automaton in json format to file `out.json`" 
            << std::endl;
  save(aut,"out.json");

  std::cout << "Output the automaton in grail format to file `out.grail`"
            << std::endl;
  save(aut,"out.grail", {IO_FORMAT = GRAIL});
  
  std::cout << "Output the automaton in dot format to file `out.gv`" 
            << std::endl;
  save(aut,"out.gv", {IO_FORMAT = DOT});

  std::cout << "Output the automaton in pdf format (image) to `out.pdf`" 
            << std::endl;
  save(aut,"out.pdf", {IO_FORMAT = PDF});

  return 0;
}
