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

#include<awali/common/is_iterable.hxx>
#include<awali/common/priority.hh>
#include <awali/common/linked_map.hxx>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <list>
#include <cassert>
#include <unordered_map>

using namespace awali;

template<typename O, typename P>
auto fail_for_iterable(O v, priority::HIGH<P>)
->typename std::enable_if<is_iterable<O>::value,bool>::type
{
  throw std::runtime_error("Argument is iterabl");
}

template<typename T> struct argument_type;
template<typename T, typename U> struct argument_type<T(U)> { typedef U type; };
#define UNPACK(t) argument_type<void(t)>::type

#define PRINT(type) std::cout << std::endl << #type << ":";
#define PRINT2(type1,type2) std::cout << std::endl << #type1 << " with " << #type2 << std::endl;

#define ASSERT_ITERABLE(type) PRINT(type) assert(is_iterable<UNPACK(type)>::value);

#define ASSERT_ITERABLE_WITH(type1,type2) PRINT2(type1,type2) if (!is_iterable_with<UNPACK(type1),type2>::value) throw std::runtime_error("");

static int i = 1;

template<typename T>
void assert_iterable(bool b = true) {
  if (b != is_iterable<T>::value)
    throw std::runtime_error("Not iterable: "+std::to_string(i));
  i++;
}


template<typename T, typename X>
void assert_iterable_with(bool b = true) {
  if (b != is_iterable_with<T,X>::value)
    throw std::runtime_error("Not iterable with "+std::to_string(i));
  i++;
}

int main() 
{
/* 1*/ assert_iterable<std::vector<std::string>>();
/* 2*/ assert_iterable<std::list<std::string> const>();
/* 3*/ assert_iterable<std::vector<std::string>>();
/* 4*/ assert_iterable<linked_map_t<std::string,std::string>>();

/* 5*/ assert_iterable_with<std::vector<std::string>,std::string>();
/* 6*/ assert_iterable_with<std::vector<std::string>,std::string&>();
/* 7*/ assert_iterable_with<std::vector<std::string>,std::string const&>();
/* 8*/ assert_iterable_with<std::vector<std::string>,int>(false);
/* 9*/ assert_iterable_with<std::vector<std::string> const,std::string&>(false);
/*10*/ assert_iterable_with<std::vector<std::string> const,
                            std::string const&>();
/*11*/ assert_iterable_with<std::list<std::pair<std::string const,std::string>>,
                            std::pair<std::string,std::string>&>(false);




//   using pair_t = linked_map_t<std::string,std::string>::value_type;
//   std::vector<pair_t> vect = {{"toto","tutu"},{"titi","tutu"}};
//   linked_map_t<std::string,std::string> map (vect);
//   std::cout << vect.size() << std::endl;
//   for (auto p: map)
//     std::cout << p.first << ":" << p.second << std::endl;
//   linked_map_t<std::string,std::string> map2 (std::move(vect));
//   std::cout << vect.size() << std::endl;
//   for (auto p: map2)
//     std::cout << p.first << ":" << p.second << std::endl;
}
