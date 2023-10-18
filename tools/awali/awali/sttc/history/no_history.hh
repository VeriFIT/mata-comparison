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

#ifndef AWALI_NO_HISTORY_HH
# define AWALI_NO_HISTORY_HH

#include <awali/sttc/history/history.hh>

namespace awali { namespace sttc {

    /** @brief specialisation of history_base
     *
     * This type is used to specify that the automaton has no history.
     */
    class no_history final : public history_base {
    public:

      std::ostream&
      print_state_name(state_t, std::ostream& o,
                       const std::string& /*fmt*/) const override {
        return o;
      }


      history_kind_t get_nature() const override {
        return history_kind_t::NO_HISTORY;
      }

      bool has_history(state_t) const override {
        return false;
      }

      bool remove_history(state_t) override {
        return false;
      };


      bool has_history() const override {
        return false;
      }

      state_t get_state(state_t) override {
        throw std::runtime_error("Origin state not available");
      }

      std::vector<state_t> get_state_set(state_t) override {
        throw std::runtime_error("Origin state set not available");
      }
    };
  }
}//end of ns awali::stc

#endif // !AWALI_NO_HISTORY_HH
