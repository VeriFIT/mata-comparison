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

#ifndef AWALI_STRING_HISTORY_HH
#define AWALI_STRING_HISTORY_HH

# include <string>
# include <map>
#include <awali/sttc/history/history.hh>

namespace awali {
  namespace sttc {

    //Make every state in relation with a string
    class string_history  : public history_base
    {
    private:
      using origins_t = std::map<state_t, std::string>;
      origins_t origins_;
      std::string name_;
      std::string desc_;
    public:
      string_history()
      {}

      history_kind_t get_nature() const
      {
        return history_kind_t::STRING;
      }

      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt) const
      {
        auto i = origins().find(s);
        o << i->second;
        return o;
      }

      const std::string& get_state_name(state_t s) const
      {
        auto i = origins().find(s);
        return i->second;
      }

      void set_name(const std::string& n) {
        name_ = n;
      }

      void set_desc(const std::string& d) {
        desc_ = d;
      }

      const std::string& get_name() const {
        return name_;
      }
      
      const std::string& get_desc() const {
        return desc_;
      }
      
      const origins_t& origins() const
      {
        return origins_;
      }

      bool remove_history(state_t s) {
        return origins_.erase(s);
      };

      bool has_history(state_t s) const {
        return (origins_.find(s)!=origins_.end());
      }

      void
      add_state(state_t s,const std::string& str)
      {
        origins_[s] = str;
      }

      state_t get_state(state_t s) {
        throw std::runtime_error("Origin state not available");
      }

      std::vector<state_t> get_state_set(state_t s) {
        throw std::runtime_error("Origin state set not available");
      }
    };

  }
}//end of ns awali::stc

#endif // !AWALI_STRING_HISTORY_HH
