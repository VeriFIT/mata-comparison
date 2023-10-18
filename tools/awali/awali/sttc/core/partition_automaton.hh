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

#ifndef AWALI_CORE_PARTITION_AUTOMATON_HH
# define AWALI_CORE_PARTITION_AUTOMATON_HH

# include <map>
# include <set>
# include <vector>

#include <awali/sttc/core/automaton_decorator.hh>

namespace awali { namespace sttc {


  namespace internal
  {
    template <typename Aut>
    class partition_automaton_impl
      : public automaton_decorator<typename Aut::element_type::automaton_nocv_t>
    {
    public:
      using automaton_t = Aut;
      using automaton_nocv_t = typename automaton_t::element_type::automaton_nocv_t;
      using context_t = context_t_of<automaton_t>;
      using label_t = label_t_of<automaton_t>;
      using super_t = automaton_decorator<automaton_nocv_t>;

      /// A set of the original automaton states.
      using state_set_t = std::set<state_t>;

    private:
      /// A map from each state to the origin state set it stands for.
      using origins_t = std::map<state_t, state_set_t>;
      origins_t origins_;

      const automaton_t input_;

    public:
      partition_automaton_impl(const automaton_t& input)
        : super_t(input->context())
        , input_(input)
      {}

      // FIXME: do we want this, or should we leave it to the
      // underlying automaton, as super_t does?  Or is this always
      // subclassed anyway?
      static std::string sname()
      {
        return "partition_automaton<" + automaton_t::element_type::sname() + ">";
      }

      std::string vname(bool full = true) const
      {
        return "partition_automaton<" + input_->vname(full) + ">";
      }

      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt = "text") const
      {
        const auto& set = origins_.at(s);
        const char* separator = "{";
        for (auto s : set)
          {
            o << separator;
            input_->print_state_name(s, o, fmt);
            separator = ", ";
          }
        return o << "}";
      }

      /// Make a new state representing the given input state set,
      /// which is required to be new -- no error-checking on this.
      /// FIXME: shall I keep a reverse-origin for that?
      state_t
      add_state(const state_set_t& set)
      {
        state_t res = add_state();
        origins_[res] = set;
        return res;
      }

      state_t
      add_state(const std::vector<state_t>& v)
      {
        state_set_t set;
        for (auto s: v)
          set.emplace(s);
        return add_state(std::move(set));
      }

      state_t
      add_state()
      {
        return super_t::add_state();
      }
    }; // class
  } // namespace internal

  /// A subset automaton as a shared pointer.
  template <typename Aut>
  using partition_automaton
    = std::shared_ptr<internal::partition_automaton_impl<Aut>>;

}}//end of ns awali::stc

#endif // !AWALI_CORE_PARTITION_AUTOMATON_HH
