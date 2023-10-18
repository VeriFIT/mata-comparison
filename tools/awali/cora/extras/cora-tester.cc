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
#include <iostream>
#include <stdlib.h>
#include <string>

bool contentOfFileIs(const std::string& file, const std::string& content);
bool compareFiles(const std::string& p1, const std::string& p2);

int main (int argc, char** argv) {
  if (argc != 4)  {
    std::cerr << "\tUsage: cora-tester <cora-exec> <file> <line-to-test>" << std::endl;
    exit(1);
  }

  std::string cora (argv[1]);
  std::ifstream file (argv[2]);
  int n = atoi(argv[3]);
  char aline[512];

  int j = 0;
  for (int i=0; i<=n; i++) {
    file.getline(aline, 512);
    if (file.bad() || file.eof() || file.fail()) {
      std::cerr << "Problem reading line " << j << "(" << i << " meaningful) of file" << std::endl;
      exit(1);
    }
    if (aline[0] == '#' || aline[0] == '\0')
      i--;
    j++;
  }
  std::string line(aline);

//   std::cout << line << std::endl;
  std::size_t index = line.rfind("==");
  bool verbatim = true;
  bool isomorph = true;
  if (index==std::string::npos) { // are isomorph
    verbatim = false;
    index = line.find("~=");
    if (index == std::string::npos) { //are equivalent
      isomorph = false;
      index = line.find("~~");
    }
  }

  if (index == std::string::npos) {
    std::cerr << "Problem on line " << n << ": " << "Didn't find token \"==\" \"~=\" or \"~=\"." << std::endl;
    exit(1);
  }
  std::string left = line.substr(0, index);
  std::string right = line.substr(index+2);
  while (right[0] == ' ') {
    right.erase(0,1);
  }

  std::string outname1 = "/tmp/cora-test"+std::string(argv[3])+".out1";
  std::string outname2 = "/tmp/cora-test"+std::string(argv[3])+".out2";
  std::string outname3 = "/tmp/cora-test"+std::string(argv[3])+".out3";

  std::string command1 = left + " > " +outname1;
  std::string command2 = right + " > " +outname2;
  std::string replacee = "${CORA}";
  size_t x = cora.size();
  size_t y = replacee.size();
  {
    size_t pos = 0;
    while ( (pos = command1.find(replacee,pos)) != std::string::npos) {
      command1 = command1.replace(pos, y, cora);
      pos += x;
     }
   }
  size_t pos = 0;
  while ( (pos = command2.find(replacee,pos)) != std::string::npos) {
    command2 = command2.replace(pos, y, cora);
    pos += x;
   }

  //std::cerr << command1 << std::endl;
  //std::cerr << command2 << std::endl;

  for (std::string command : {command1,command2}) {
    int res = system(command.c_str());
    if (res != 0) {
      std::cerr << "Failure: command '" << command << "' returned non-zero status." << std::endl;
      exit(2);
    }
  }
  if (verbatim) {
    bool b = compareFiles(outname1, outname2);

    if (!b) {
      std::cerr << "Failure: command '" << command1 <<"' and command '" << command2 << "' do not produce the same result (see " << outname1 << "," << outname2 << ")" << std::endl;
      exit(3);
    } 
    exit (0);
  }

  std::string rel = isomorph ? "isomorphic" : "equivalent";
  std::string command3 = cora+" are-"+rel+" "+outname1+" "+outname2+" > "+outname3;
  int res = system(command3.c_str());
  if (res != 0) {
    std::cerr << "Failure: command '" << command3
              << "' returned non-zero status." << std::endl;
    exit(2);
  }
  bool b = contentOfFileIs(outname3,"true");
  if (!b) {
      std::cerr << "Failure: command '" << command1 <<"' and command '" << command2 << "' do not produce the same result, according to command '"<< command3 << "' (see " << outname1 << "," << outname2 << ")" << std::endl;
  }
}

bool contentOfFileIs(const std::string& file, const std::string& content) {
  std::ifstream f1(file, std::ifstream::in);
  if (f1.fail()) //file problem
    return false;
  size_t nchar = content.size()+1;
  char* buffer = (char*) malloc(sizeof(char)*nchar);
  f1.get(buffer,nchar);
  std::string s(buffer);
  free(buffer);
  return (!s.compare(content));
}

bool compareFiles(const std::string& p1, const std::string& p2) {
  /*
  std::string s("diff ");
  s+=p1; s+=' '; s+=p2;
  std::cout << "*****" << s << std::endl;
  return !system(s.c_str());
  */
  std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

  if (f1.fail() || f2.fail()) {
    return false; //file problem
  }

  if (f1.tellg() != f2.tellg()) {
    return false; //size mismatch
  }

  //seek back to beginning and use std::equal to compare contents
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));
}
