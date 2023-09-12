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

#ifndef AWALI_RATEXP_HISTORY_HH
# define AWALI_RATEXP_HISTORY_HH

# include <string>

#include <awali/sttc/misc/escape.hh>
#include <awali/sttc/misc/map.hh>
#include <awali/sttc/misc/stream.hh> // format
#include <awali/sttc/core/rat/ratexpset.hh>
#include <awali/sttc/history/history.hh>

namespace awali {
  namespace sttc {

    /*-------------------.
      | ratexp_automaton.  |
      `-------------------*/
    //Make every state in relation with a rational expression
    /** @brief specialisation of history_base
     *
     * @tparam RatExpSet the context of rational expressions
     *
     * This type is used to specified that the states of the automaton
     * correspond to rational expressions.
 has been built
     * from a source automaton, and that states correspond to subsets of
     * states of the source automaton.
     *
     * This type of history is filled in {@link derived_term}.
     */
    template <typename RatExpSet>
    class ratexp_history  : public history_base {
    public:
      using ratexpset_t = RatExpSet;
      using ratexp_t = typename ratexpset_t::value_t;

    private:
      std::map<state_t, ratexp_t> origins_;
      ratexpset_t rs_;
    public:
      ratexp_history(const ratexpset_t& rs)
        : rs_(rs)
      {}

      ///@return history_kind_t::RATEXP
      history_kind_t get_nature() const
      {
        return history_kind_t::RATEXP;
      }

      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt) const
      {
        auto i = origins().find(s);
        o << str_escape(format(rs_, i->second, fmt));
        return o;
      }

      /// map between states and rational expressions
      const std::map<state_t, ratexp_t>& origins() const
      {
        return origins_;
      }

      bool remove_history(state_t s) {
        origins_.erase(s);
        return true;
      };

      bool has_history(state_t s) const {
        return (origins_.find(s)!=origins_.end());
      }

      ///set the history of state \p s
      void
      add_state(state_t s,const ratexp_t& expr)
      {
        origins_[s] = expr;
      }

      const ratexp_t& get_ratexp(state_t s) {
        return origins_[s];
      }
      
      /** @brief unsupported method : use {@link get_ratexp}
       * @throw runtime_error
       */
      state_t get_state(state_t s) {
        throw std::runtime_error("Origin state not available");
      }

      /** @brief unsupported method : use {@link get_ratexp}
       * @throw runtime_error
       */
      std::vector<state_t> get_state_set(state_t s) {
        throw std::runtime_error("Origin state set not available");
      }
    };

  }
}//end of ns awali::stc

#endif // !AWALI_RATEXP_HISTORY_HH
