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

#ifndef AWALI_PARTITION_HISTORY_HH
# define AWALI_PARTITION_HISTORY_HH

# include <map>
# include <set>

#include <awali/sttc/history/history.hh>

# include <stdexcept>

namespace awali {
  namespace sttc {

        //Make every state in relation with the set of states of another automaton
    /** @brief specialisation of history_base
     *
     * @tparam Autb the type of the source automaton
     *
     * This type is used to specify that the automaton has been built
     * from a source automaton, and that states correspond to subsets of
     * states of the source automaton.
     *
     * This type of history is filled in {@link determinize}, {@link min_quotient}, and related functions.
     */
  template <typename Autb>
    class partition_history  : public history_base
    {
    public:
      //the subsets of source states are std::set of states
      using state_set_t = std::set<state_t>;

    private:
      std::map<state_t, state_set_t> origins_;
      Autb from_;

    public:
      partition_history(const Autb& source)
        : from_(source)
      {}

      ///@return history_kind_t::PARTITION
      history_kind_t get_nature() const
      {
        return history_kind_t::PARTITION;
      }

      bool has_history(state_t s) const {
        return (origins_.find(s)!=origins_.end());
      }

      bool remove_history(state_t s) {
        return origins_.erase(s);
      }

      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt) const
      {
        const auto& set = origins_.at(s);
        const char* separator = "{";
        for (auto s : set)
          {
            o << separator;
            from_->print_state_history(s, o, fmt);
            separator = ", ";
          }
        return o << "}";
      }

      /// map between states of the automaton and subsets of states of the source
      const std::map<state_t, state_set_t>& origins() const
      {
        return origins_;
      }

      ///set the history of state \p s
      void
      add_state(state_t s,const state_set_t& set)
      {
        origins_[s] = set;
      }

      /** @brief unsupported method : use {@link get_state_set}
       * @throw runtime_error
       */
     state_t get_state(state_t s) {
        throw std::runtime_error("Origin state not available");
      }

      std::vector<state_t> get_state_set(state_t s) {
        std::vector<state_t> v(origins_[s].begin(),origins_[s].end());
        return v;
      }

    }; // class

}}//end of ns awali::stc

#endif // !AWALI_PARTITION_HISTORY_HH
