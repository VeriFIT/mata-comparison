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

#ifndef AWALI_CORE_RATEXP_AUTOMATON_HH
# define AWALI_CORE_RATEXP_AUTOMATON_HH

# include <memory>
# include <stack>
# include <string>

#include <awali/sttc/misc/escape.hh>
#include <awali/sttc/misc/map.hh>
#include <awali/sttc/misc/stream.hh> // format
#include <awali/sttc/misc/unordered_map.hh>

#include <awali/sttc/core/automaton_decorator.hh>
#include <awali/sttc/core/rat/ratexpset.hh>

//# define DEBUG 1

# if DEBUG
#  define DEBUG_IFELSE(Then, Else) Then
# else
#  define DEBUG_IFELSE(Then, Else) Else
# endif

# define DEBUG_IF(Then) DEBUG_IFELSE(Then,)

namespace awali { namespace sttc {

  /*-------------------.
  | ratexp_automaton.  |
  `-------------------*/

  namespace internal
  {
    /// An incremental automaton whose states are ratexps.
    template <typename Aut>
    class ratexp_automaton_impl : public automaton_decorator<Aut>
    {
    public:
      using automaton_t = Aut;
      using context_t = context_t_of<Aut>;
      using ratexpset_t = ratexpset<context_t>;
      using ratexp_t = typename ratexpset_t::value_t;
      using super_t = automaton_decorator<mutable_automaton<context_t>>;
      using label_t = label_t_of<super_t>;
      using weight_t = weight_t_of<super_t>;

      ratexp_automaton_impl(const context_t& ctx)
        : super_t(ctx)
        , rs_(ctx, rat::identities::trivial)
      {}

      static std::string sname()
      {
        return "ratexp_automaton<" + super_t::sname() + ">";
      }

      std::string vname(bool full = true) const
      {
        return "ratexp_automaton<" + super_t::vname(full) + ">";
      }

      /// Symbolic states to state handlers.
      using smap = std::unordered_map<ratexp_t, state_t,
                                      internal::hash<ratexpset_t>,
                                      utils::equal_to<ratexpset_t>>;

      /// The state for ratexp \a r.
      /// If this is a new state, schedule it for visit.
      state_t state(const ratexp_t& r)
      {
        // Benches show that the map_.emplace technique is slower, and
        // then that operator[] is faster than emplace.
        state_t res;
        auto i = map_.find(r);
        if (i == std::end(map_))
          {
            DEBUG_IF(
                     std::cerr << "New state: ";
                     rs_.print(r, std::cerr) << '\n';
                     );
            res = super_t::add_state();
            map_[r] = res;
            todo_.push(r);
          }
        else
          res = i->second;
        return res;
      }

      using super_t::add_transition;
      void
      add_transition(const ratexp_t& src, const ratexp_t& dst,
                     const label_t& l, const weight_t& w)
      {
        super_t::add_transition(state(src), state(dst), l, w);
      }

      void
      add_transition(state_t src, const ratexp_t& dst,
                     const label_t& l, const weight_t& w)
      {
        super_t::add_transition(src, state(dst), l, w);
      }

      using super_t::set_initial;
      void
      set_initial(const ratexp_t& s, const weight_t& w)
      {
        super_t::set_initial(state(s), w);
      }

      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt) const
      {
        auto i = origins().find(s);
        if (i == std::end(origins()))
          this->print_state(s, o);
        else
          o << str_escape(format(rs_, i->second, fmt));
        return o;
      }

      /// Ordered map: state -> its derived term.
      using origins_t = std::map<state_t, ratexp_t>;
      mutable origins_t origins_;
      const origins_t&
      origins() const
      {
        if (origins_.empty())
          for (const auto& p: map_)
            origins_[p.second] = p.first;
        return origins_;
      }

      /// The ratexp's set.
      ratexpset_t rs_;
      /// States to visit.
      std::stack<ratexp_t> todo_;
      /// ratexp -> state.
      smap map_;
    };
  }

  /// A ratexp automaton as a shared pointer.
  template <typename Aut>
  using ratexp_automaton
    = std::shared_ptr<internal::ratexp_automaton_impl<Aut>>;

}}//end of ns awali::stc

#endif // !AWALI_CORE_RATEXP_AUTOMATON_HH
