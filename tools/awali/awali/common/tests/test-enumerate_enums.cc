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

#include <awali/common/enum_utils.cc>
#include <typeindex>


using namespace awali;

int main(int argc, char** argv) {

  for (exp_to_aut_algo_t x : internal::values_of<exp_to_aut_algo_t>())
    std::cout << name_of(x) << std::endl;
  std::cout << internal::index_of<exp_to_aut_algo_t>() << std::endl;
  std::cout << internal::index_of<exp_to_aut_algo_t>() << std::endl;
  std::cout << internal::index_of<quotient_algo_t>() << std::endl;

  for (quotient_algo_t x : internal::values_of<quotient_algo_t>())
    std::cout << name_of(x) << std::endl;

  std::cout << std::type_index(typeid(exp_to_aut_algo_t)).hash_code() << std::endl;

}
