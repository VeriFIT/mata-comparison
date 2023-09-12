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

#include <fstream>

#include <awali/dyn.hh>
#include <awali/common/enums.hh>

using namespace awali::dyn;

int main(int argc, char **argv) {
 
  const char* files[]={"a1","fibotdc-lr","morse","b1","gray","oddb","binary","heapmodel","prob-rabin","c1","lamplighter","d1","minab","slowcerny","evena","minblocka","slowgrow","test_q","test_c",NULL};
  //  rotation is not in the list : C values like "i" are not supported
  std::string deprec("deprecated::json-v0::");
  std::map<std::string,loading::file_loc_t> file_map=loading::example_automata(true);
  for(int p=0; files[p]!=NULL; p++) {
    std::cout << files[p] << std::endl;
    std::string f = deprec+files[p];
    loading::file_loc_t fs = file_map[f];
    f=fs.dir+'/'+fs.name+'.'+fs.ext;
    std::cout << f << std::endl;
    std::ifstream file2(f);
    if(!file2)
      throw std::runtime_error("Error opening "+f);
    automaton_t aut= parse_automaton(file2, {IO_FORMAT=awali::FSM_JSON_V0});
    std::cout << aut << std::endl;
    file2.close();
  }
  //print_info(aut);
  return 0;
}
