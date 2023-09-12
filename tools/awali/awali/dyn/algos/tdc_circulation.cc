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

#include <awali/dyn/algos/tdc_circulation.hh>

namespace awali { namespace dyn {
  namespace internal {
    std::string gcp(const std::string& s1, const std::string& s2) {
      size_t i;
      for(i=0; i<s1.length() && i<s2.length() && s1[i]==s2[i]; ++i)
        ;
      if(i==s1.length() || i==s2.length())
        return s1.substr(0,i);
      if(s1[i]==spec)
        return s2;
      if(s2[i]==spec)
        return s1;
      return s1.substr(0,i);
    }
  }//end of ns internal

  void tdc_circulation_here(transducer_t tdc) {
    std::unordered_map<state_t, std::string> map_;
    std::string c="a";
    c[0]=internal::spec;
    for(auto s : tdc->states()) {
      if(tdc->is_final(s))
        map_[s]=internal::get_final_output(tdc,s);
      else
        map_[s]=c;
    }
    bool next;
    do {
      next=false;
      for(auto tr : tdc->transitions()) {
        auto l=internal::get_tdc_label(tdc, tr);
        std::string p
          = internal::gcp(map_[tdc->src_of(tr)], l[1]+map_[tdc->dst_of(tr)]);
        if(p!=map_[tdc->src_of(tr)]) {
          next=true;
          map_[tdc->src_of(tr)]=p;
        }
      }
    } while(next);

    for(auto s : tdc->states()) {
      if(map_[s]=="" || tdc->is_initial(s))
        continue;
      for(auto tr : tdc->in(s)) {
        auto l=internal::get_tdc_label(tdc, tr);
        internal::set_tdc_transition(tdc,tdc->src_of(tr),s,{l[0],l[1]+map_[s]});
        tdc->del_transition(tr);
      }
    }
    for(auto s : tdc->states()) {
      if(map_[s]=="" || tdc->is_initial(s))
        continue;
      for(auto tr : tdc->out(s)) {
        auto l=internal::get_tdc_label(tdc, tr);
        internal::set_tdc_transition
          ( tdc, s, tdc->dst_of(tr), {l[0],l[1].substr(map_[s].length())} );
        tdc->del_transition(tr);
      }
    }
    for(auto s : tdc->final_states()) {
      if(map_[s]=="" || tdc->is_initial(s))
        continue;
      auto st=internal::get_final_output(tdc,s);
      tdc->unset_final(s);
      internal::set_final_output(tdc,s,st.substr(map_[s].length()));
    }
  }

}}//end of ns awali::dyn
