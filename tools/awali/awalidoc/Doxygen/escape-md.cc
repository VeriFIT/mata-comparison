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

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>

#include <awali/common/docstring/doc_entries.hh>


int generate_file(awali::docstring::entry_t const& d, 
                  std::ostream& out) 
{

  std::stringstream in(d.content);
  bool in_backquote = false;
  size_t linenumber = 1;
  std::string line = "";
  out << "" << d.title << "{#docstring_" << d.name << "}" << std::endl;
  out << "==" << std::endl;
  while (in.good()) {
    getline(in, line);
    in_backquote = false;
    std::list<size_t> pos;
    std::stringstream  outline("");
    size_t n = 0;
    size_t max = line.size();
    if (4 < line.size())
      max = 4;
    for (size_t i = 0; i<max; i++) {
      if (line[i] == '\t')
        n += 2;
      else if (line[i] == ' ')
        n += 1;
      else
        break;
    }
    if (n<4) { // We ignore each line starting with 4 spaces, \t counts as two
               // spaces.
      for (size_t i = 0; i<line.size(); i++) {
        if (line[i] == '`') {
          in_backquote = !in_backquote;
          outline << '`';
        } 
        else if ((!in_backquote) && 
             (std::string::npos != (awali::docstring::toescape.find(line[i]))))
        {
          if ((i != 0) || line[i] != '>')
            outline << '\\';
        }
        outline << line[i];
      }
      if (in_backquote) {
        std::cerr << "Error: " << "Odd number of backquote at line " 
                  << linenumber << "." << std::endl;
        return 1;
      }
//       for (size_t i:pos)
//         line.insert(i,"\\");
      out << outline.str() << std::endl;
    } 
    else
      out << line << std::endl;
    linenumber++;
  }
  return 0;
}


int main (int argc, char** argv) {
  if (argc <2) {
   std::cerr << "Error: missing argument." << std::endl;
   std::cerr << "\tUsage: escape-md <output-directory>" << std::endl;
   exit (1);
  }
  
  std::string outdir = argv[1];

  for (auto d : awali::docstring::entries) {
    std::string outpath = outdir+"/"+d.name+".doxygen.md";
    std::ofstream out(outpath);
    if (!out.is_open()) {
      std::cerr << "Error opening output file: " << outpath << std::endl;
      return(2);
    }

    bool ret = generate_file(d, out);
    if (ret)
      return ret;
  }
  return 0;
}
