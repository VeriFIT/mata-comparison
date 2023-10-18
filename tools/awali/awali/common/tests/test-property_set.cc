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

#include <awali/common/property_set.hh>
#include <awali/common/enum_utils.cc>
#include <awali/sttc/weightset/n.hh>
#include <awali/sttc/weightset/z.hh>
#include <awali/sttc/weightset/q.hh>
#include <awali/common/priority.hh>

using namespace awali;

 template<typename WS, typename P>
 std::string
 my_func_ (WS w, priority::ONE<P>) 
 {
   return "DEFAULT";
 }

template<typename WS, typename P>
auto
my_func_ (WS w, priority::TWO<P>) 
-> std::enable_if_t<WS::properties().satisfies(DOMAIN),std::string>
{
  return "DOMAIN";
}

template<typename WS, typename P>
auto
my_func_ (WS w, priority::THREE<P>) 
-> std::enable_if_t<WS::properties().satisfies(POSITIVE || DIVISION_RING), 
                    std::string>
{
  return "POSITIVE || DIVISION_RING";
}

template<typename WS, typename P>
auto
my_func_ (WS w, priority::FOUR<P>) 
-> std::enable_if_t<WS::properties().satisfies(COMMUTATIVE && FINITE), 
                    std::string>
{
  return "COMMUTATIVE && FINITE";
}

template<typename WS>
std::string
my_func(WS w)
{
  return my_func_(w, priority::value);
}


// Enjoying C++17
//
template<typename WS>
auto
my_func3_finite (WS w) /* Defined only if WS is FINITE */
-> std::enable_if_t<WS::properties().satisfies(FINITE), 
                    std::string>
{
  return "FINITE";
}

template<typename WS>
auto
my_func3_field (WS w) /* Defined only if WS is a FIELD */
-> std::enable_if_t<WS::properties().satisfies(FIELD), 
                    std::string>
{
  return "FIELD";
}

template<typename WS>
std::string
my_func3 (WS ws) { /* This always compiles !! */
  if constexpr (WS::properties().satisfies(FIELD))
    return my_func3_field(ws);
  else if constexpr (WS::properties().satisfies(FINITE))
    return my_func3_finite(ws);
  else
    return "";
}



// Attempt to improve readability further
//
template<typename WS, property_set_t::value_t v, typename b>
using enable_if_ws_t = std::enable_if_t<WS::properties().satisfies(v), b>;

template<typename RetType> 
struct enable_with_type
{
  template<typename WS>
  struct if_ws {
    template<property_set_t::value_t v>
    using is = std::enable_if_t<WS::properties().satisfies(v), RetType>;
    
    template<property_set_t::value_t... Vs>
    using is_either 
    = std::enable_if_t<std::disjunction_v< std::integral_constant<bool, WS::properties().satisfies(Vs)>...>, RetType>;
  };
};


 template<typename WS, typename P>
 std::string
 my_func_2 (WS w, priority::ONE<P>) 
 {
   return "DEFAULT";
 }

template<typename WS, typename P>
auto
my_func_2 (WS w, priority::TWO<P>) 
-> enable_if_ws_t<WS, DOMAIN, std::string>
{
  return "DOMAIN";
}

template<typename WS, typename P>
auto
my_func_2 (WS w, priority::THREE<P>) 
-> typename enable_with_type<std::string>::if_ws<WS>::is_either<POSITIVE, DIVISION_RING>
{
  return "POSITIVE || DIVISION_RING";
}

template<typename WS, typename P>
auto
my_func_2 (WS w, priority::FOUR<P>) 
-> typename enable_with_type<std::string>::if_ws<WS>::is<COMMUTATIVE && FINITE>
{
  return "COMMUTATIVE && FINITE";
}

template<typename WS>
std::string
my_func2(WS w)
{
  return my_func_2(w, priority::value);
}



int main (int argc, char** argv) 
{

  property_set_t s = POSITIVE && FINITE && RING;
  
  auto c1 = s || LOCALLY_FINITE;

  property_condition_t<1> x = FINITE;
  auto c2 = c1 && FINITE;
  if (s.satisfies((DIVISION_RING && FINITE) || POSITIVE))
    std::cout << "Hello" << std::endl;
  if (s.satisfies(DIVISION_RING || (POSITIVE && FINITE)))
    std::cout << "World" << std::endl;
  if (s.satisfies((DIVISION_RING && FINITE) || DIVISION_RING))
    std::cout << "!" << std::endl;


  awali::sttc::b b{};
  std::cout << "B 1: " << my_func(b) << std::endl;
  std::cout << "B 2: " << my_func2(b) << std::endl;
  std::cout << "B 3: " << my_func3(b) << std::endl;
 
  awali::sttc::n n{};
  std::cout << "N 1: " << my_func(n) << std::endl;
  std::cout << "N 2: " << my_func2(n) << std::endl;
  std::cout << "N 3: " << my_func3(n) << std::endl;

  awali::sttc::z z{};
  std::cout << "Z 1: " << my_func(z) << std::endl;
  std::cout << "Z 2: " << my_func2(z) << std::endl;
  std::cout << "Z 3: " << my_func3(z) << std::endl;
  
  awali::sttc::q q{};
  std::cout << "Q 1: " << my_func(q) << std::endl;
  std::cout << "Q 2: " << my_func2(q) << std::endl;
  std::cout << "Q 3: " << my_func3(q) << std::endl;

  std::cout << std::endl;

  for ( ws_property_t p : {
     COMMUTATIVE
    ,LOCALLY_FINITE
    ,POSITIVE
    ,FINITE
    ,RING
    ,DOMAIN
    ,DIVISION_RING
    ,FIELD
    }) {
    if (n.properties().satisfies(p))
      std::cout << "B is (a) " << internal::string_of_enum("ws_property_t",p) << std::endl;
    if (n.properties().satisfies(p))
      std::cout << "N is (a) " << internal::string_of_enum("ws_property_t",p) << std::endl;
    if (z.properties().satisfies(p))
      std::cout << "Z is (a) " << internal::string_of_enum("ws_property_t",p) << std::endl;
    if (q.properties().satisfies(p))
      std::cout << "Q is (a) " << internal::string_of_enum("ws_property_t",p) << std::endl;
  }


}
