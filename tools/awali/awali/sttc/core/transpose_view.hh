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

#ifndef AWALI_CORE_TRANSPOSE_VIEW_HH
# define AWALI_CORE_TRANSPOSE_VIEW_HH

#include <awali/sttc/core/mutable_automaton.hh>
# include <memory>

namespace awali { namespace sttc {

  namespace internal
  {
    template <typename Aut>
    class transpose_view_impl
    {
    public:
      using context_t = typename Aut::context_t;
      using labelset_t = labelset_t_of<context_t>;
      using weightset_t = weightset_t_of<context_t>;
      using kind_t = typename context_t::kind_t;

      using labelset_ptr = typename context_t::labelset_ptr;
      using weightset_ptr = typename context_t::weightset_ptr;

      /// Transition label.
      using label_t = typename labelset_t::value_t;
      /// Transition weight.
      using weight_t = typename weightset_t::value_t;
      /// History.
      using history_t = typename Aut::history_t;

      using st_store_t = typename Aut::st_store_t;
      using tr_store_t = typename Aut::tr_store_t;
      using tr_cont_t = typename Aut::tr_cont_t;

      transpose_view_impl() = delete;
      transpose_view_impl(const transpose_view_impl&) = delete;
      transpose_view_impl(const Aut& aut)
        : aut_{&aut}
      {}
    private:
      const Aut* aut_;
    public:
      transpose_view_impl(transpose_view_impl&& that) = delete;

      transpose_view_impl& operator=(transpose_view_impl&& that) = delete;

      // Related sets
      ///////////////

      static std::string sname()
      {
        return "transpose_view<" + context_t::sname() + ">";
      }

      std::string vname(bool full = true) const
      {
        return "transpose_view<" + context().vname(full) + ">";
      }

      const context_t& context() const { return aut_->context(); }
      const weightset_ptr& weightset() const { return aut_->weightset(); }
      const labelset_ptr& labelset() const { return aut_->labelset(); }

      // Special states and transitions
      /////////////////////////////////

      // pseudo-initial and final states.
      // The code below assumes that pre() and post() are the first
      // two states of the automaton.  In other words, all other states
      // have greater numbers.
      static constexpr state_t      pre()  { return Aut::post(); }
      static constexpr state_t      post()  { return Aut::pre(); }

      label_t prepost_label() const
      {
        return aut_->prepost_label();
      }

      // Statistics
      /////////////

      size_t num_all_states() const { return aut_->num_all_states(); }
      size_t num_states() const { return aut_->num_states(); }
      size_t num_initials() const { return aut_->num_initials(); }
      size_t num_finals() const { return aut_->num_finals(); }
      size_t num_transitions() const { return aut_->num_transitions(); }

      // Queries on states
      ////////////////////

      bool has_state(state_t s) const { return aut_->has_state(s); }

      state_t max_state() const { return aut_->max_state(); }

      bool is_initial(state_t s) const { return aut_->is_final(s); }
      bool is_final(state_t s) const { return aut_->is_initial(s); }
      weight_t get_initial_weight(state_t s) const { return aut_->get_final_weight(s); }
      weight_t get_final_weight(state_t s) const { return aut_->get_initial_weight(s); }

      // Queries on transitions
      /////////////////////////

      transition_t get_transition(state_t src, state_t dst, label_t l) const
      {
        return aut_->get_transition(dst, src, l);
      }

      bool
      has_transition(state_t src, state_t dst, label_t l) const
      {
        return aut_->has_transition(dst, src, l);
      }

      bool has_transition(transition_t t) const {return aut_->has_transition(t); }

      state_t src_of(transition_t t) const   { return aut_->dst_of(t); }
      state_t dst_of(transition_t t) const   { return aut_->src_of(t); }
      label_t label_of(transition_t t) const { return aut_->label_of(t); }
      weight_t weight_of(transition_t t) const { return aut_->weight_of(t); }

      history_t history() const { return aut_->history(); }

     std::ostream&
      print_state(state_t s, std::ostream& o) const {
        return aut_->print_state(s, o);
      }

      std::ostream&
      print_state_name(state_t s, std::ostream& o,
                       const std::string& fmt = "text") const {
        return aut_->print_state_name(s, o, fmt);
      }

      void set_name(const std::string& n) {
        aut_->set_name(n);
      }
      
      void set_desc(const std::string& d) {
        aut_->set_desc(d);
      }
      
      const std::string& get_name() const {
        return aut_->get_name();
      }
      
      const std::string& get_desc() const {
        return aut_->get_desc();
      }

      std::string get_state_name(state_t s) const {
        return aut_->get_state_name(s);
      }

      std::ostream& print_state_history(state_t s, std::ostream& o,
                                        const std::string& fmt = "text") const {
        return aut_->print_state_history(s, o, fmt);
      }

      bool has_history(state_t s) const {
        return aut_->has_history(s);
      }

      bool has_explicit_name(state_t s) const {
        return aut_->has_explicit_name(s);
      }

      state_t get_state_by_name(const std::string& name) const {
        return aut_->get_state_by_name(name);
      }

      // Iteration on states and transitions
      //////////////////////////////////////

      using states_output_t = indice_filter<st_store_t>;

      /// All states excluding pre()/post().
      /// Guaranteed in increasing order.
      states_output_t states() const { return aut_->states(); }

      /// All states including pre()/post().
      /// Guaranteed in increasing order.
      states_output_t all_states() const { return aut_->all_states(); }

      using transitions_output_t = indice_filter<tr_store_t>;

      /// All the transition indexes between visible states.
      transitions_output_t transitions() const { return aut_->transitions(); }

      /// All the transition indexes between all states (including pre and post).
      transitions_output_t all_transitions() const { return aut_->all_transitions(); }

      using transitions_s_output_t = cont_filter<tr_cont_t>;

      /// Indexes of transitions to visible initial states.
      transitions_s_output_t
      initial_transitions() const
      {
        return out(pre());
      }

      /// Indexes of transitions from visible final states.
      transitions_s_output_t
      final_transitions() const
      {
        return in(post());
      }

      /// Indexes of visible transitions leaving state \a s.
      /// Invalidated by del_transition() and del_state().
      transitions_s_output_t out(state_t s) const { return aut_->in(s); }

      /// Indexes of all transitions leaving state \a s.
      /// Invalidated by del_transition() and del_state().
      const tr_cont_t& all_out(state_t s) const { return aut_->all_in(s); }

      /// Indexes of all transitions leaving state \a s on label \a l.
      /// Invalidated by del_transition() and del_state().
    transitions_s_output_t out(state_t s, const label_t& l) const
      {
        return aut_->in(s, l);
      }

      /// Indexes of visible transitions arriving to state \a s.
      /// Invalidated by del_transition() and del_state().
      transitions_s_output_t in(state_t s) const { return aut_->out(s); }

      /// Indexes of all transitions arriving to state \a s.
      /// Invalidated by del_transition() and del_state().
      const tr_cont_t& all_in(state_t s) const { return aut_->all_out(s); }

      /// Indexes of visible transitions arriving to state \a s on label \a l.
      /// Invalidated by del_transition() and del_state().
      transitions_s_output_t in(state_t s, const label_t& l) const
      {
        return aut_->out(s, l);
      }

      /// Indexes of visible transitions from state \a s to state \a d.
      /// Invalidated by del_transition() and del_state().
      transitions_s_output_t outin(state_t s, state_t d) const
      {
        return aut_->outin(d,s);
      }
    };
  }

  template <typename Aut>
  std::shared_ptr<internal::transpose_view_impl<Aut>>
  transpose_view(std::shared_ptr<Aut> aut)
  {
    return std::make_shared<internal::transpose_view_impl<Aut>>(*aut);
  }
}}//end of ns awali::stc

#endif // !AWALI_CORE_TRANSPOSE_VIEW_HH
