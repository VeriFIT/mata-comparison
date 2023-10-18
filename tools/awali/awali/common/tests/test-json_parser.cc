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

#include<vector>
#include<awali/common/json.cc>

using namespace awali;
using namespace awali::json;

void
test(std::string const& str, json::node_t* witness) {
  std::cout << str << std::endl;
  std::stringstream ss(str);
  json::node_t* node = json::parse(ss);
  if(!json::internal::equal(node,witness)) {
    throw std::runtime_error("FAILURE");
  }
  delete (node);
  delete (witness);
}


int main() {
  std::vector<std::pair<std::string,json::node_t*>> v=
  { 
    {"2",            new int_t(2)},
    {"-12",          new int_t(-12)},
    {"2.1",          new json::float_t(2.1)},
    {"\"test\"",     new string_t("test")},
    {"null",         new null_t()},
    {"false",        new bool_t(false)},
    {"true",         new bool_t(true)},
    {"{}",           new object_t()},
    {"{\"key\":12}", new object_t("key",new json::int_t(12))},
    {"[]",           new array_t()},
    {"[12.1,\"test2\"]", new array_t({new json::float_t(12.1), 
                                      new string_t("test2")}) },
  };
  for (auto p: v) {
    test(p.first,p.second);
  }
}
