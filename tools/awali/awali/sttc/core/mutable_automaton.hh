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

#ifndef AWALI_CORE_MUTABLE_AUTOMATON_HH
# define AWALI_CORE_MUTABLE_AUTOMATON_HH

# include <algorithm>
# include <cassert>
# include <vector>
# include <sstream>
# include <string>

//#include <awali/sttc/core/crange.hh>
#include <awali/common/types.hh>
#include <awali/sttc/core/transition.hh>
#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/memory.hh>
#include <awali/sttc/misc/cont_filter.hh>
#include <awali/sttc/history/no_history.hh>
#include <awali/sttc/history/string_history.hh>

namespace awali {
  namespace sttc {
    
    namespace internal {
      template <typename Context>
      class mutable_automaton_impl;
    }
    template <typename Context>
    using mutable_automaton
    = std::shared_ptr<internal::mutable_automaton_impl<Context>>;
    
    namespace internal
    {
      template <typename Context>
      class mutable_automaton_impl
      {
      public:
        using context_t = Context;
        /// The (shared pointer) type to use it we have to create an
        /// automaton of the same (underlying) type.
        using automaton_nocv_t = mutable_automaton<context_t>;
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
        using history_t = std::shared_ptr<history_base>;
        using names_t = std::shared_ptr<string_history>;

        /// The algebraic type of this automaton.
        context_t ctx_;

        /// Data stored per transition.
        using stored_transition_t = transition_tuple<state_t, label_t, weight_t>;

        /// All the incoming/outgoing transition handles of a state.
        using tr_cont_t = std::vector<transition_t>;

        /// Data stored for each state.
        struct stored_state_t
        {
          tr_cont_t succ;
          tr_cont_t pred;
        };
        /// All the automaton's states.
        using st_store_t = std::vector<stored_state_t>;
        /// All the automaton's transitions.
        using tr_store_t = std::vector<stored_transition_t>;
        /// A list of unused indexes in the states/transitions tables.
        using free_store_t = std::vector<state_t>;
      private:

        st_store_t states_;
        /// Free indexes in states_.
        free_store_t states_fs_;
        tr_store_t transitions_;
        /// Free indexes in transitions_.
        free_store_t transitions_fs_;
        /// Label for initial and final transitions.
        label_t prepost_label_;
        /// History !
        history_t history_;
        // State names
        names_t names_;
      public:
        mutable_automaton_impl() = delete;
        mutable_automaton_impl(const mutable_automaton_impl&) = delete;
        mutable_automaton_impl(const context_t& ctx)
          : ctx_{ctx}
          , states_{2}
          , prepost_label_(ctx.labelset()->special())
          , history_(std::make_shared<no_history>())
          , names_(std::make_shared<string_history>())
        {}

        mutable_automaton_impl(mutable_automaton_impl&& that)
          : ctx_(that.ctx_)
          , prepost_label_(that.prepost_label_) {
          *this = std::move(that);
        }

        mutable_automaton_impl& operator=(mutable_automaton_impl&& that) {
          if (this != &that) {
            ctx_ = std::move(that.ctx_);
            prepost_label_ = std::move(that.prepost_label_);
            std::swap(states_, that.states_);
            std::swap(states_fs_, that.states_fs_);
            std::swap(transitions_, that.transitions_);
            std::swap(transitions_fs_, that.transitions_fs_);
            history_ = that.history_;
            names_ = that.names_;
          }
          return *this;
        }

        // Related sets
        ///////////////

        static std::string sname() {
          return "mutable_automaton<" + context_t::sname() + ">";
        }

        std::string vname(bool full = true) const {
          return "mutable_automaton<" + context().vname(full) + ">";
        }

        const context_t& context() const { return ctx_; }
        const weightset_ptr& weightset() const { return ctx_.weightset(); }
        const labelset_ptr& labelset() const { return ctx_.labelset(); }

        // Special states and transitions
        /////////////////////////////////

        // pseudo-initial and final states.
        // The code below assumes that pre() and post() are the first
        // two states of the automaton.  In other words, all other states
        // have greater numbers.  We also assume that pre()<post().
        static constexpr state_t      pre()  { return 0U; }
        static constexpr state_t      post()  { return 1U; }
        // Invalid transition or state.
        static constexpr state_t      null_state()      { return -1U; }
        static constexpr transition_t null_transition() { return -1U; }

        label_t prepost_label() const {
          return prepost_label_;
        }

        // Statistics
        /////////////

        size_t num_all_states() const { return states_.size() - states_fs_.size(); }
        size_t num_states() const { return num_all_states() - 2; }
        size_t num_initials() const { return states_[pre()].succ.size(); }
        size_t num_finals() const { return states_[post()].pred.size(); }
        size_t num_transitions() const {
          return (transitions_.size()
                  - transitions_fs_.size() - num_initials() - num_finals());
        }

        // Queries on states
        ////////////////////

        bool
        has_state(state_t s) const {
          // Any number outside our container is not a state.
          // (This includes "null_state()".)
          if (s >= states_.size())
            return false;
          const stored_state_t& ss = states_[s];

          // Erased states have 'invalid' as their first successor.
          return ss.succ.empty() || ss.succ.front() != null_transition();
        }

        state_t max_state() const {
          state_t s=states_.size();
          for(; !has_state(s); --s)
            ;
          return s;
        }

        bool
        is_initial(state_t s) const {
          return has_transition(pre(), s, prepost_label_);
        }

        bool
        is_final(state_t s) const {
          return has_transition(s, post(), prepost_label_);
        }

        ATTRIBUTE_PURE
        weight_t
        get_initial_weight(state_t s) const {
          transition_t t = get_transition(pre(), s, prepost_label_);
          if (t == null_transition())
            return weightset()->zero();
          else
            return weight_of(t);
        }

        ATTRIBUTE_PURE
        weight_t
        get_final_weight(state_t s) const {
          transition_t t = get_transition(s, post(), prepost_label_);
          if (t == null_transition())
            return weightset()->zero();
          else
            return weight_of(t);
        }

        // Queries on transitions
        /////////////////////////

        transition_t
        get_transition(state_t src, state_t dst, label_t l) const {
          assert(has_state(src));
          assert(has_state(dst));
          const tr_cont_t& succ = states_[src].succ;
          const tr_cont_t& pred = states_[dst].pred;
          const auto& ls = *this->labelset();
          if (succ.size() <= pred.size()) {
            auto i =
              std::find_if(begin(succ), end(succ),
                           [this,l,ls,dst] (transition_t t) -> bool {
                             const stored_transition_t& st = transitions_[t];
                             return (st.dst == dst
                                     && ls.equals(st.get_label(), l));
                           });
            if (i != end(succ))
              return *i;
          }
          else {
            auto i =
              std::find_if(begin(pred), end(pred),
                           [this,l,ls,src] (transition_t t) -> bool {
                             const stored_transition_t& st = transitions_[t];
                             return (st.src == src
                                     && ls.equals(st.get_label(), l));
                           });
            if (i != end(pred))
              return *i;
          }
          return null_transition();
        }

        bool
        has_transition(state_t src, state_t dst, label_t l) const {
          return get_transition(src, dst, l) != null_transition();
        }

        bool
        has_transition(transition_t t) const {
          // Any number outside our container is not a transition.
          // (This includes "null_transition()".)
          if (t >= transitions_.size())
            return false;

          // Erased transition have invalid source state.
          return transitions_[t].src != null_state();
        }

        state_t src_of(transition_t t) const   { return transitions_[t].src; }
        state_t dst_of(transition_t t) const   { return transitions_[t].dst; }
        label_t label_of(transition_t t) const {
          return transitions_[t].get_label();
        }

        weight_t weight_of(transition_t t) const {
          return transitions_[t].get_weight();
        }

        // Edition of states
        ////////////////////

      protected:
        /// Remove t from the outgoing transitions of the source state.
        void
        del_transition_from_src(transition_t t) {
          stored_transition_t& st = transitions_[t];
          auto& succ = states_[st.src].succ;
          auto tsucc = std::find(succ.begin(), succ.end(), t);
          assert(tsucc != succ.end());
          *tsucc = std::move(succ.back());
          succ.pop_back();
        }

        /// Remove t from the ingoing transition of the destination state.
        void
        del_transition_from_dst(transition_t t) {
          stored_transition_t& st = transitions_[t];
          auto& pred = states_[st.dst].pred;
          auto tpred = std::find(pred.begin(), pred.end(), t);
          assert(tpred != pred.end());
          *tpred = std::move(pred.back());
          pred.pop_back();
        }

        void
        del_transition_container(tr_cont_t& tc, bool from_succ) {
          for (auto t: tc) {
            if (from_succ)
              del_transition_from_dst(t);
            else
              del_transition_from_src(t);
            transitions_[t].src = null_state();
          }
          transitions_fs_.insert(transitions_fs_.end(), tc.begin(), tc.end());
          tc.clear();
        }

      public:
        state_t
        add_state() {
          state_t s;
          if (states_fs_.empty()) {
            s = states_.size();
            states_.resize(s + 1);
          }
          else {
            s = states_fs_.back();
            states_fs_.pop_back();
          }
          stored_state_t& ss = states_[s];
          // De-invalidate this state: remove the invalid transition.
          ss.succ.clear();
          return s;
        }

        history_t history() const {
          return history_;
        }

        void set_history(history_t h) {
          history_ = h;
        }

        void strip_history() {
          history_ = std::make_shared<no_history>();
        }

        void strip_names() {
          history_ = std::make_shared<string_history>();
        }

        std::ostream& print_state(state_t s, std::ostream& o) const {
          if(names_->has_history(s))
            return names_->print_state_name(s, o, "text");
          if(s == pre())
            return o << "_";
          if(s == post())
            return o << "_";
          return o << '$' << (s-2);
        }

        std::ostream& print_state_name(state_t s, std::ostream& o,
                         const std::string& fmt = "text") const {
          return print_state(s, o);
        }

        std::string get_state_name(state_t s) const {
          if(names_->has_history(s))
            return names_->get_state_name(s);
          std::ostringstream o;
          print_state(s, o);
          return o.str();
        }

        std::ostream& print_state_history(state_t s, std::ostream& o,
                            const std::string& fmt = "text") const {
          if(history_->has_history(s))
            return history_->print_state_name(s, o, fmt);
          return print_state_name(s, o, fmt);
        }

        bool has_history(state_t s) const {
          return history_->has_history(s);
        }

        void set_state_names_from_history() {
          for(auto s : states())
            if(history_->has_history(s)) {
              std::ostringstream o;
              print_state_history(s, o);
              set_state_name(s,o.str());
            }
        }
        
        bool has_explicit_name(state_t s) const {
          return names_->has_history(s);
        }

        void set_state_name(state_t s, const std::string& n) {
          names_->add_state(s, n);
        }

        state_t get_state_by_name(const std::string& name) const {
          for(auto i : states()) {
            std::ostringstream os;
            print_state_name(i,os);
            if(os.str()==name)
              return i;
          }
          return null_state();
        }

        bool set_name(const std::string& n) {
          if(n.length()>0 && n[0]=='$')
            return false;
          names_->set_name(n);
          return true;
        }
        
        void set_desc(const std::string& d) {
          names_->set_desc(d);
        }
        
        const std::string& get_name() const {
          return names_->get_name();
        }
        
        const std::string& get_desc() const {
          return names_->get_desc();
        }
      
        
        void del_state(state_t s) {
          assert(has_state(s));
          assert(s > post()); // Cannot erase pre() and post().
          stored_state_t& ss = states_[s];
          del_transition_container(ss.pred, false);
          del_transition_container(ss.succ, true);
          history_->remove_history(s);
          names_->remove_history(s);
          ss.succ.emplace_back(null_transition()); // So has_state() can work.
          states_fs_.emplace_back(s);
        }

        void set_initial(state_t s, weight_t w) {
          set_transition(pre(), s, prepost_label_, w);
        }

        void set_initial(state_t s) {
          set_initial(s, weightset()->one());
        }

        weight_t add_initial(state_t s, weight_t w) {
          return add_transition(pre(), s, prepost_label_, w);
        }

        void unset_initial(state_t s) {
          set_initial(s, weightset()->zero());
        }

        void set_final(state_t s, weight_t w) {
          set_transition(s, post(), prepost_label_, w);
        }

        void set_final(state_t s) {
          set_final(s, weightset()->one());
        }

        weight_t add_final(state_t s, weight_t w) {
          return add_transition(s, post(), prepost_label_, w);
        }

        void unset_final(state_t s) {
          set_final(s, weightset()->zero());
        }

        // Edition of transitions
        /////////////////////////

        void
        del_transition(transition_t t)
        {
          assert(has_transition(t));
          // Remove transition from source and destination.
          del_transition_from_src(t);
          del_transition_from_dst(t);
          // Actually erase the transition.
          transitions_[t].src = null_state();
          transitions_fs_.emplace_back(t);
        }

        /// Remove the transition (src, l, dst).
        transition_t
        del_transition(state_t src, state_t dst, label_t l)
        {
          transition_t t = get_transition(src, dst, l);
          if (t != null_transition())
            del_transition(t);
          return t;
        }

        /// Remove all the transitions between s and d.
        void
        del_transition(state_t s, state_t d)
        {
          // Make a copy of the transition indexes, as the iterators are
          // invalidated by del_transition(t).
          auto ts = outin(s, d);
          for (auto t: tr_cont_t{ts.begin(), ts.end()})
            del_transition(t);
        }

        /// Create a transition between two states.  There must not exist
        /// a previous transition with same (src, dst, l).
        ///
        /// \param src  source state
        /// \param dst  destination state
        /// \param l    label of the transition
        /// \param w    weight of the transition
        ///
        /// \pre the label is _not checked_, for efficiency.
        /// \pre ! has_transition(src, dst, l).
        transition_t
        new_transition(state_t src, state_t dst, label_t l, weight_t w)
        {
          assert(!has_transition(src, dst, l));
          if (weightset()->is_zero(w))
            return null_transition();
          else
            {
              transition_t t;
              if (transitions_fs_.empty())
                {
                  t = transitions_.size();
                  transitions_.resize(t + 1);
                }
              else
                {
                  t = transitions_fs_.back();
                  transitions_fs_.pop_back();
                }
              stored_transition_t& st = transitions_[t];
              st.src = src;
              st.dst = dst;
              // FIXME: When src == pre() || dst == post(), label must be empty.
              st.set_label(l);
              st.set_weight(w);
              states_[src].succ.emplace_back(t);
              states_[dst].pred.emplace_back(t);
              return t;
            }
        }

        /// Copy the label of a transition between two states, creating a new
        /// transition.
        /// There must not exist a previous transition with same (src, dst,
        /// label_of(l)).
        ///
        /// \param src  source state
        /// \param dst  destination state
        /// \param aut  the automaton whose transition will be copied.
        /// \param t    transition of \a aut whose label and weight are to copy
        /// \param transpose   whether label and weight should be transposed
        ///
        /// \pre the label is _not checked_, for efficiency.
        /// \pre ! has_transition(src, dst, label_of(l)).
        template <typename A>
        transition_t
        new_transition_copy(state_t src, state_t dst,
                            const A& aut, transition_t t,
                            bool transpose = false)
        {
          auto l = aut->label_of(t);
          auto w = aut->weight_of(t);
          if (transpose)
            {
              l = aut->labelset()->transpose(l);
              w = aut->weightset()->transpose(w);
            }
          return new_transition(src, dst,
                                labelset()->conv(*aut->labelset(), l),
                                weightset()->conv(*aut->weightset(), w));
        }

        /// Same as above, with weight one.
        transition_t
        new_transition(state_t src, state_t dst, label_t l)
        {
          return new_transition(src, dst, l, weightset()->one());
        }

        /// Set a transition between two states.  Override any possible
        /// existing transition with same states and label.
        ///
        /// \param src  source state
        /// \param dst  destination state
        /// \param l    label of the transition
        /// \param w    weight of the transition
        ///
        /// \pre the label is _not checked_, for efficiency.  Letters out
        /// of the alphabet will be accepted.
        transition_t
        set_transition(state_t src, state_t dst, label_t l, weight_t w)
        {
          // It's illegal to connect pre() to post().
          // FIXME: reenable except for labels_are_one.
          // assert(src != pre() || dst != post());
          // It's illegal to leave post().
          assert(src != post());
          // It's illegal to go to pre().
          assert(dst != pre());
          transition_t t = get_transition(src, dst, l);
          if (t == null_transition())
            t = new_transition(src, dst, l, w);
          else if (weightset()->is_zero(w))
            {
              del_transition(t);
              t = null_transition();
            }
          else
            {
              stored_transition_t& st = transitions_[t];
              st.set_label(l);
              st.set_weight(w);
            }
          return t;
        }

        /// Same as above, with unit weight.
        transition_t
        set_transition(state_t src, state_t dst, label_t l)
        {
          return set_transition(src, dst, l, weightset()->one());
        }

        template <typename A>
        transition_t
        set_transition_copy(state_t src, state_t dst,
                            const A& aut, transition_t t,
                            bool transpose = false)
        {
          auto l = aut->label_of(t);
          auto w = aut->weight_of(t);
          if (transpose)
            {
              l = aut->labelset()->transpose(l);
              w = aut->weightset()->transpose(w);
            }
          return set_transition(src, dst,
                                labelset()->conv(*aut->labelset(), l),
                                weightset()->conv(*aut->weightset(), w));
        }

        /// Add a transition between two states.  Merge with an existing
        /// one with same label.
        ///
        /// \param src  source state
        /// \param dst  destination state
        /// \param l    label of the transition
        /// \param w    weight of the transition
        ///
        /// \pre the label is _not checked_, for efficiency.  Letters out
        /// of the alphabet will be accepted.
        weight_t
        add_transition(state_t src, state_t dst, label_t l, weight_t w)
        {
          transition_t t = get_transition(src, dst, l);
          if (t == null_transition())
            new_transition(src, dst, l, w);
          else
            {
              w = weightset()->add(weight_of(t), w);
              set_weight(t, w);
            }
          return w;
        }

        /// Same as above, with weight one.
        weight_t
        add_transition(state_t src, state_t dst, label_t l)
        {
          return add_transition(src, dst, l, weightset()->one());
        }

        /// Add a transition between two states, copying the label from the given
        /// transition.  Merge with an existing one with same label.
        ///
        /// \param src  source state
        /// \param dst  destination state
        /// \param aut  the automaton whose transition will be copied.
        /// \param t    transition of \a aut whose label and weight are to copy
        /// \param transpose   whether label and weight should be transposed
        ///
        /// \pre the label is _not checked_, for efficiency.  Letters out
        /// of the alphabet will be accepted.
        template <typename A>
        weight_t
        add_transition_copy(state_t src, state_t dst,
                            const A& aut, transition_t t,
                            bool transpose = false)
        {
          auto l = aut->label_of(t);
          auto w = aut->weight_of(t);
          if (transpose)
            {
              l = aut->labelset()->transpose(l);
              w = aut->weightset()->transpose(w);
            }
          return add_transition(src, dst,
                                labelset()->conv(*aut->labelset(), l),
                                weightset()->conv(*aut->weightset(), w));
        }

        weight_t
        set_weight(transition_t t, weight_t w)
        {
          if (weightset()->is_zero(w))
            del_transition(t);
          else
            transitions_[t].set_weight(w);
          return w;
        }

        weight_t
        add_weight(transition_t t, weight_t w)
        {
          return set_weight(t, weightset()->add(weight_of(t), w));
        }

        weight_t
        lmul_weight(transition_t t, weight_t w)
        {
          return set_weight(t, weightset()->mul(w, weight_of(t)));
        }

        weight_t
        rmul_weight(transition_t t, weight_t w)
        {
          return set_weight(t, weightset()->mul(weight_of(t), w));
        }

        // Iteration on states and transitions
        //////////////////////////////////////

        using states_output_t = indice_filter<st_store_t>;

        /// All states excluding pre()/post().
        /// Guaranteed in increasing order.
        states_output_t
        states() const {
          return states_output_t(states_, [this] (const stored_state_t& ss) -> bool
                                 {
                                   return (ss.succ.empty()
                                           || ss.succ.front() != this->null_transition());
                                 }, 2);
        }
        /// All states including pre()/post().
        /// Guaranteed in increasing order.
        states_output_t
        all_states() const {
          return states_output_t(states_, [this] (const stored_state_t& ss) -> bool
                                 {
                                   return (ss.succ.empty()
                                           || ss.succ.front() != this->null_transition());
                                 });
        }

        using transitions_output_t = indice_filter<tr_store_t>;

        /// All the transition indexes between visible states.
        transitions_output_t
        transitions() const
        {
          return transitions_output_t (transitions_, [this] (const stored_transition_t& tr) -> bool
                                       {
                                         state_t src = tr.src;
                                         if (src == this->null_state() || src == this->pre())
                                           return false;
                                         return tr.dst != this->post();
                                       });
        }

        /// All the transition indexes between all states (including pre and post).
        transitions_output_t
        all_transitions() const
        {
          return transitions_output_t(transitions_, [this] (const stored_transition_t& tr) -> bool
                                      {
                                        return tr.src != this->null_state();
                                      });
        }

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
        transitions_s_output_t
        out(state_t s) const
        {
          assert(has_state(s));
          return transitions_s_output_t(states_[s].succ, [this] (const transition_t& i) -> bool
                                        {
                                          return this->transitions_[i].dst != this->post();
                                        });
        }

        /// Indexes of all transitions leaving state \a s.
        /// Invalidated by del_transition() and del_state().
        const tr_cont_t&
        all_out(state_t s) const
        {
          assert(has_state(s));
          return states_[s].succ;
        }

        tr_cont_t&
        all_out_(state_t s)
        {
          assert(has_state(s));
          return states_[s].succ;
        }

        /// Indexes of all transitions leaving state \a s on label \a l.
        /// Invalidated by del_transition() and del_state().
        transitions_s_output_t
        out(state_t s, const label_t& l) const
        {
          assert(has_state(s));
          return transitions_s_output_t(states_[s].succ, [this,l] (const transition_t& i) -> bool
                                        {
                                          return this->labelset()->equals(this->transitions_[i].get_label(), l);
                                        });
        }

        /// Indexes of visible transitions arriving to state \a s.
        /// Invalidated by del_transition() and del_state().
        transitions_s_output_t
        in(state_t s) const
        {
          assert(has_state(s));
          return transitions_s_output_t(states_[s].pred, [this] (const transition_t& i) -> bool
                                        { return this->transitions_[i].src != this->pre(); });
        }

        /// Indexes of all transitions arriving to state \a s.
        /// Invalidated by del_transition() and del_state().
        const tr_cont_t&
        all_in(state_t s) const
        {
          assert(has_state(s));
          return states_[s].pred;
        }

        /// Indexes of visible transitions arriving to state \a s on label \a l.
        /// Invalidated by del_transition() and del_state().
        transitions_s_output_t
        in(state_t s, const label_t& l) const
        {
          assert(has_state(s));
          return transitions_s_output_t(states_[s].pred, [this,l] (const transition_t& i) -> bool
                                        {
                                          return this->labelset()->equals(this->transitions_[i].get_label(), l);
                                        });
        }

        /// Indexes of visible transitions from state \a s to state \a d.
        /// Invalidated by del_transition() and del_state().
        transitions_s_output_t
        outin(state_t s, state_t d) const
        {
          assert(has_state(s));
          assert(has_state(d));
          return transitions_s_output_t(states_[s].succ, [this,d] (const transition_t& i) -> bool
                                        { return this->transitions_[i].dst == d; });
        }
      };
    }

    template <typename Context>
    mutable_automaton<Context>
    make_mutable_automaton(const Context& ctx)
    {
      return make_shared_ptr<mutable_automaton<Context>>(ctx);
    }
  }
}//end of ns awali::stc

#endif // !AWALI_CORE_MUTABLE_AUTOMATON_HH
