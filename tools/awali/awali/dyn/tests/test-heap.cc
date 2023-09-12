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

#include<iostream>
#include <awali/utils/heap.hh>
#include<cstdlib>
#include<ctime>

using namespace awali;

int main() {
  utils::min_heap<int,int> h(100);
  std::srand(time(nullptr));
  for(int i=0; i<100;++i) {
    int k=std::rand()%10000;
    h.emplace(k,k);
  }
  h.heapify();
  while(!h.empty()){
    int k=h.pop();
    std::cout << k << std::endl;
  }
  std::cout << "------" << std::endl;
  //push
  for(int i=0; i<100;++i) {
    int k=std::rand()%10000;
    h.push(k,10000-k);
  }
  while(!h.empty()){
    int k=h.pop();
    std::cout << k << std::endl;
  }

  std::cout << "------" << std::endl;
  size_t t[100];
  int k[100];
  //push & update
  for(int i=0; i<100;++i) {
    k[i]=std::rand()%10000;
    t[i]=h.push(k[i],k[i]);
  }
  for(int i=0; i<100;++i)
    h.update(t[i], 10000-k[i]);
  while(!h.empty()){
    int kn=h.pop();
    std::cout << kn << std::endl;
  }


  return 0;
}
