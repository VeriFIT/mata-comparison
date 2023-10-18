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

#ifndef AWALI_CORE_AUTOMATON_DECORATOR_HH
# define AWALI_CORE_AUTOMATON_DECORATOR_HH

#include <awali/sttc/misc/memory.hh>
#include <awali/sttc/ctx/traits.hh>

namespace awali { namespace sttc {

  namespace internal
  {
    /// Aggregate an automaton, and forward calls to it.
    ///
    /// \tparam Aut
    ///   The type of the wrapped automaton.
    /// \tparam Context
    ///   The context we pretend to feature.
    template <typename Aut,
              typename Context = context_t_of<Aut>>
    class automaton_decorator
    {
    public:
      /// The type of automaton to wrap.
      using automaton_t = Aut;

      /// The (shared pointer) type to use it we have to create an
      /// automaton of the same (underlying) type.
      using automaton_nocv_t = typename automaton_t::element_type::automaton_nocv_t;

      using context_t = Context;
      using kind_t = typename context_t::kind_t;

      using labelset_t = typename context_t::labelset_t;
      using labelset_ptr = typename context_t::labelset_ptr;
      using label_t = typename labelset_t::value_t;

      using weightset_t = typename context_t::weightset_t;
      using weightset_ptr = typename context_t::weightset_ptr;
      using weight_t = typename weightset_t::value_t;

    public:
      automaton_decorator(automaton_t aut)
        : aut_{aut}
      {}

      automaton_decorator(const automaton_decorator& aut)
        : aut_(aut.aut_)
      {}

      automaton_decorator(const context_t& ctx)
        : aut_(make_shared_ptr<automaton_t>(ctx))
      {}

      automaton_decorator(automaton_decorator&& aut)
      {
        std::swap(aut_, aut.aut_);
      }

      automaton_decorator& operator=(automaton_decorator&& that)
      {
        if (this != &that)
          aut_ = std::move(that.aut_);
        return *this;
      }

      automaton_t
      strip_history()
      {
        return aut_;
      }

    protected:
      /// The wrapped automaton, possibly const.
      // Must be defined early to please decltype.
      automaton_t aut_;


    public:
      /*------------.
      | constexpr.  |
      `------------*/

# define DEFINE(Name)                                                   \
      template <typename... Args>                                       \
      static constexpr                                                  \
      auto                                                              \
      Name(Args&&... args)                                              \
        -> decltype(automaton_t::element_type::Name(std::forward<Args>(args)...))     \
      {                                                                 \
        return automaton_t::element_type::Name(std::forward<Args>(args)...);          \
      }

      DEFINE(null_state)
      DEFINE(null_transition)
      DEFINE(post)
      DEFINE(pre)
      DEFINE(sname)

#undef DEFINE

      /*--------.
      | const.  |
      `--------*/

# define DEFINE(Name)                                           \
      template <typename... Args>                               \
      auto                                                      \
      Name(Args&&... args) const                                \
        -> decltype(aut_->Name(std::forward<Args>(args)...))    \
      {                                                         \
      return aut_->Name(std::forward<Args>(args)...);           \
    }

      DEFINE(all_in)
      DEFINE(all_out)
      DEFINE(all_states)
      DEFINE(all_transitions)
      DEFINE(context)
      DEFINE(dst_of)
      DEFINE(final_transitions)
      DEFINE(get_final_weight)
      DEFINE(get_initial_weight)
      DEFINE(get_transition)
      DEFINE(has_transition)
      DEFINE(has_state)
      DEFINE(in)
      DEFINE(initial_transitions)
      DEFINE(is_final)
      DEFINE(is_initial)
      DEFINE(label_of)
      DEFINE(labelset)
      DEFINE(num_all_states)
      DEFINE(num_finals)
      DEFINE(num_initials)
      DEFINE(num_states)
      DEFINE(num_transitions)
      DEFINE(out)
      DEFINE(outin)
      DEFINE(print_state)
      DEFINE(print_state_name)
      DEFINE(src_of)
      DEFINE(states)
      DEFINE(transitions)
      DEFINE(vname)
      DEFINE(weight_of)
      DEFINE(weightset)

# undef DEFINE


      /*------------.
      | non const.  |
      `------------*/

# define DEFINE(Name)                                           \
      template <typename... Args>                               \
      auto                                                      \
      Name(Args&&... args)                                      \
        -> decltype(aut_->Name(std::forward<Args>(args)...))    \
      {                                                         \
      return aut_->Name(std::forward<Args>(args)...);           \
      }

      DEFINE(add_final)
      DEFINE(add_initial)
      DEFINE(add_transition)
      DEFINE(add_transition_copy)
      DEFINE(add_weight)
      DEFINE(del_state)
      DEFINE(del_transition)
      DEFINE(lmul_weight)
      DEFINE(add_state)
      DEFINE(new_transition)
      DEFINE(new_transition_copy)
      DEFINE(rmul_weight)
      DEFINE(set_final)
      DEFINE(set_initial)
      DEFINE(set_transition)
      DEFINE(set_weight)
      DEFINE(unset_final)
      DEFINE(unset_initial)

# undef DEFINE
    };
  }
}}//end of ns awali::stc

#endif // !AWALI_CORE_AUTOMATON_DECORATOR_HH
