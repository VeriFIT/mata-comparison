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

#ifndef AWALI_SINGLE_HISTORY_HH
# define AWALI_SINGLE_HISTORY_HH

# include <deque>
# include <iostream>
# include <map>
# include <utility>

#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/map.hh> // has
#include <awali/sttc/history/history.hh>
# include <stdexcept>

namespace awali {
  namespace sttc {

    /*------------------------------.
      | single_history<Aut, Autb>.  |
      `------------------------------*/
    /** @brief specialisation of history_base
     *
     * @tparam Autb the type of the source automaton
     *
     * This type is used to specify that a state of the automaton
     * correspond a state of the source automaton.
     *
     * This type of history is filled in {@link copy}, in
     * conversion functions an in functions that call {@link copy} before
     * applying an in-place algorithm. 
     */
    template <typename Autb>
    class single_history : public history_base
    {
    public:
      ///@return history_kind_t::SINGLE
      history_kind_t get_nature() const
      {
        return history_kind_t::SINGLE;
      }

      single_history(const Autb& source) : from_(source)
      {}

      const history_base& source_history() {
        return *(from_->history());
      }
      
      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt) const
      {
        o << '>';
        from_->print_state_history(origins_[s],o,fmt);
        return o;
      }


      /// map between states and states of the source automaton
      const std::map<state_t, state_t>& origins() const
      {
        return origins_;
      }

      bool has_history(state_t s) const {
        return (origins_.find(s)!=origins_.end());
      }

      bool remove_history(state_t s) {
        return origins_.erase(s);
      };

      ///set the history of state \p s
      void
      add_state(state_t s,const state_t& sb)
      {
        origins_[s] = sb;
      }


      state_t get_state(state_t s) {
        return origins_[s];
      }

      /** @brief unsupported method : use {@link get_state}
       * @throw runtime_error
       */
      std::vector<state_t> get_state_set(state_t s) {
        throw std::runtime_error("Origin state set not available");
      }

    private:
      // Origin automaton, supplied at construction time.
      Autb from_;
      mutable std::map<state_t, state_t> origins_;
    };
  }
}//end of ns awali::stc

#endif // !AWALI_SINGLE_HISTORY_HH
