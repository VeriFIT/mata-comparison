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

#ifndef AWALI_CORE_PERMUTATION_DECORATOR_HH
# define AWALI_CORE_PERMUTATION_DECORATOR_HH

# include <map>
# include <queue>
# include <unordered_map>

#include <awali/sttc/core/automaton_decorator.hh>

namespace awali { namespace sttc {


  namespace internal
  {
    template <typename Aut>
    class permutation_automaton_impl
      : public automaton_decorator<typename Aut::element_type::automaton_nocv_t>
    {
    public:
      /// Input automaton type.
      using automaton_t = Aut;
      /// Sorted automaton type.
      using automaton_nocv_t = typename automaton_t::element_type::automaton_nocv_t;
      using super_t = automaton_decorator<automaton_nocv_t>;
      using full_context_t=context_t_of<Aut>;

    public:
      permutation_automaton_impl(const automaton_t& input)
        : super_t(make_shared_ptr<automaton_nocv_t>(real_context(input)))
        , input_(input)
      {
        map_[input_->pre()] = super_t::pre();
        map_[input_->post()] = super_t::post();
        todo_.push({input_->pre(), super_t::pre()});
      }

      static std::string sname()
      {
        return "permutation_automaton<" + automaton_t::element_type::sname() + ">";
      }

      std::string vname(bool full = true) const
      {
        return "permutation_automaton<" + input_->vname(full) + ">";
      }

      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt = "text") const
      {
        return input_->print_state_name(origins().at(s), o, fmt);
      }

      state_t
      state(state_t s)
      {
        // Benches show that the map_.emplace technique is slower, and
        // then that operator[] is faster than emplace.
        state_t res;
        auto i = map_.find(s);
        if (i == std::end(map_))
          {
            res = super_t::add_state();
            map_[s] = res;
            todo_.push({s, res});
          }
        else
          res = i->second;
        return res;
      }

      /// A map from each state to the origin state set it stands for.
      using origins_t = std::map<state_t, state_t>;

      /// Ordered map: state -> its derived term.
      const origins_t&
      origins() const
      {
        if (origins_.empty())
          for (const auto& p: map_)
            origins_[p.second] = p.first;
        return origins_;
      }

      using pair_t = std::pair<state_t, state_t>;
      std::queue<pair_t> todo_;

      /// Input-state -> sorted-state.
      std::unordered_map<state_t, state_t> map_;

      mutable origins_t origins_;

      /// Input automaton.
      const automaton_t input_;
    }; // class
  } // namespace internal

  /// A permutation automaton as a shared pointer.
  template <typename Aut>
  using permutation_automaton
    = std::shared_ptr<internal::permutation_automaton_impl<Aut>>;

}}//end of ns awali::stc

#endif // ! AWALI_CORE_PERMUTATION_DECORATOR_HH
