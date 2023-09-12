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

#ifndef VCRS_MISC_HEAP_HH
#define VCRS_MISC_HEAP_HH

#include<vector>
#include<tuple>

namespace awali { namespace utils {

  template < typename T, typename P>
  struct min_heap {
  public :
    using value_type = T;
    using priority_type = P;
    using claim_ticket_type = size_t;
    using store_type = std::tuple<T,P,size_t>;

    min_heap(size_t capacity_min) {
      values_.reserve(capacity_min);
      tickets_.reserve(capacity_min);
    }

    size_t size() const {
      return values_.size();
    }

    size_t empty() const {
      return values_.empty();
    }

    claim_ticket_type emplace(const value_type& val, const priority_type& p) {
      size_t pos = values_.size();
      claim_ticket_type ticket = tickets_.size();
      values_.emplace_back(std::make_tuple(val, p, ticket));
      tickets_.emplace_back(pos);
      return ticket;
    }

    value_type front() const {
      return std::get<0>(values_.front());
    }

    value_type pop() {
      value_type r=front();
      swap(0,values_.size()-1);
      values_.pop_back();
      top_down(0);
      return r;
    }

    claim_ticket_type push(const value_type& val, const priority_type& p) {
      claim_ticket_type ticket=emplace(val,p);
      bottom_up(values_.size()-1);
      return ticket;
    }

    void heapify() {
      if(values_.size()<2)
        return;
      for(size_t i=values_.size()/2-1, l=i+1; i<l; --i)
        top_down(i);
    }

    void update(claim_ticket_type t, const priority_type& p) {
      unsigned i = tickets_[t];
      store_type& s=values_[i];
      if(p < std::get<1>(s)) {
        std::get<1>(s)=p;
        bottom_up(i);
      }
      else {
        std::get<1>(s)=p;
        top_down(i);
      }
    }

    void print() {
      for(size_t i=0;i<size();++i)
        std::cout << std::get<0>(values_[i])
                   << " p: " << std::get<1>(values_[i])
                   << " t: " << std::get<2>(values_[i]) << std::endl;
    }

  private :

    void bottom_up(size_t i) {
      if(i==0)
        return;
      unsigned j=(i-1)/2;
      if(std::get<1>(values_[i])<std::get<1>(values_[j])) {
        swap(i,j);
        bottom_up(j);
      }
    }

    void top_down(size_t i) {
      if(2*i+2 > values_.size())
        return;
      unsigned j;
      if(2*i+2 == values_.size())
        j=2*i+1;
      else
        j=std::get<1>(values_[2*i+1])<std::get<1>(values_[2*i+2])?2*i+1:2*i+2;
      if(std::get<1>(values_[j])<std::get<1>(values_[i])) {
        swap(i,j);
        top_down(j);
      }
    }

    void swap(size_t i, size_t j) {
      store_type tmp=values_[i];
      values_[i]=values_[j];values_[j]=tmp;
      tickets_[std::get<2>(values_[i])]=i;
      tickets_[std::get<2>(values_[j])]=j;
    }

    std::vector<store_type> values_;
    std::vector<size_t> tickets_;

  };




}}//end of ns awali::stc

#endif
