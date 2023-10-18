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

int main(int argc, char **argv) {
  if(argc!=2){
    std::cerr << "Usage : " << argv[0] << " [file]" << std::endl;
    return 1;
  }
  //Open a json description of an automaton
  std::ifstream file(argv[1]);
  automaton_t aut;
  file >> aut;
  /* other available syntaxes:
    //1
    file >> awali::JSON >> aut;
    //2
    aut=parse_automaton(file);

    It is also possible to load fado or grail automata, e.g.
    file >> awali::GRAIL >> aut
  */
  file.close();
  std::cout << "Print the automaton (json)" << std::endl;
  std::cout << aut << std::endl;
  //or std::cout << io::json << aut << std::endl;
  //or json(aut, std::cout) << std::endl;
  std::cout << "Print the automaton (dot)" << std::endl;
  std::cout << awali::DOT << aut << std::endl;
  //or dot(aut, std::cout) << std::endl;
  std::cout << "Print the automaton (fado)" << std::endl;
  std::cout << awali::FADO << aut << std::endl;
  //or fado(aut, std::cout) << std::endl;
  std::cout << "Print the automaton (grail)" << std::endl;
  std::cout << awali::GRAIL << aut << std::endl;
  //or grail(aut, std::cout) << std::endl;
  std::cout << "Print the automaton as a pdf image" << std::endl;
  std::cout << awali::PDF << aut << std::endl;
  //or grail(aut, std::cout) << std::endl;
  return 0;
}
