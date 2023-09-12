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

#include <awali/dyn/loading/handler.hh>
// #include <awali/common/error-stream.hh>

using namespace awali::dyn::loading;

int main(int argc, char** argv) {
    std::string usage("Usage : " + std::string(argv[0]) + " [-d] <module> <context1> [context2]");
    if ((argc<3) || (argc>5)) {
      std::cerr << usage << std::endl;
      return 1;
    }
    std::vector<std::string> args;
    for (int i = 0; i < argc-1; i++)
      args.push_back(std::string(argv[i+1]));

    bool b = false;
    std::string module,context1,context2;
    size_t i = 3;
    if (!args[0].compare("-d")) {
      b = true;
      module = args[1];
      context1 = args[2];
      i=4;
    } else {
      module = args[0];
      context1 = args[1];
    }

    if (args.size() == (i-1)) {
        make_library(module, context1, b);
    } else if (args.size() == i) {
        context2 = (args[i-1]);
        make_library(module, context1, context2, b);
    } else {
      std::cerr << usage << std::endl;
      return 1;
    }
    return 0;
}

