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

#include <awali/dyn.hh>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace awali::dyn;

int max_length = 7;

std::map<std::string, automaton_t> aut;
std::map<std::string, std::set<std::string>> language;

void populate_map(std::string const& name) {
  aut[name] = load(name);
  std::set<std::string>& set = language[name];
  for (auto& i : enumerate(aut[name], max_length))
    set.insert(std::move((std::string) i.first));
}

void test_product(std::string const& left, std::string const& right) {
  std::cout << std::endl << "* " << left << " x " << right <<std::endl;
  
  automaton_t prod = product(aut[left],aut[right]);
  std::cout << "\tProduct successfully computed" << std::endl;
  std::set<std::string> prod_lang;
  for (auto& i : enumerate(prod, max_length))
    prod_lang.insert(std::move((std::string) i.first));
  std::cout << "\tEnumerate successfully computed" << std::endl;


  auto& left_lang = language.find(left)->second;
  auto& right_lang = language.find(right)->second;
  std::set<std::string> inter_lang;
  for (auto& i : left_lang)
    if (right_lang.count(i))
      inter_lang.insert(std::move(i));
      
  if (inter_lang != prod_lang) {
      std::cout << "\t/!\\ Incorrect support" << std::endl;
      exit(1);
    }
  std::cout << "\tSupport of product is correct" << std::endl;
}

// void test_sum(std::string const& left, std::string const& right) {
//   std::cout << std::endl << "testing " << left << " + " << right <<std::endl;
//   
//   automaton_t sum = awali::dyn::sum(aut[left],aut[right]);
//   std::cout << "\tsum successfully computed" << std::endl;
//   std::set<std::string> sum_lang;
//   for (auto& i : enumerate(sum, max_length))
//     sum_lang.insert(std::move((std::string) i.first));
//   std::cout << "\tenumerate successfully computed" << std::endl;
// 
// 
//   auto& left_lang = language.find(left)->second;
//   auto& right_lang = language.find(right)->second;
//   std::set<std::string> union_lang;
//   for (auto const& i : left_lang)
//       union_lang.insert(i);
//   for (auto const& i : right_lang)
//       union_lang.insert(i);
//       
//   if (union_lang != sum_lang) {
//       std::cout << "\tincorrect support" << std::endl;
//       exit(1);
//     }
//   std::cout << "\tsupport of sum is correct" << std::endl;
// }

int main() {
  std::vector<std::string> number_aut = {"a1", "b1", "binary", "c1", "d1", "e1", "evena", "prob-rabin", "rotation" };

  for (auto const& name: number_aut)
    populate_map(name);

  for (std::string const& name1 : number_aut) 
    for (std::string const& name2 : number_aut) {
      test_product(name1, name2);
//       test_sum(name1, name2);
    }
    
}
