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

#ifndef AWALI_ALGOS_DETERMINIZE_HXX
# define AWALI_ALGOS_DETERMINIZE_HXX

# include <set>
# include <stack>
# include <string>
# include <type_traits>
# include <queue>
# include <limits>

#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/map.hh> // sttc::has
#include <awali/sttc/misc/raise.hh> // b
#include <awali/sttc/misc/bitset.hh>
#include <awali/sttc/misc/unordered_map.hh> // sttc::has
#include <awali/sttc/weightset/b.hh> // b
#include <awali/sttc/history/partition_history.hh>

namespace awali { namespace sttc {

  /*----------------------.
  | subset construction.  |
  `----------------------*/
  namespace internal
  {
    /// \brief The subset construction automaton from another.
    ///
    /// \tparam Aut an automaton type.
    /// \pre labelset is free.
    /// \pre weightset is Boolean.
    template <typename Aut,unsigned N>
    class determinization_bitset_impl
    {
      static_assert(labelset_t_of<Aut>::is_free(),
                    "determinize: requires free labelset");
      static_assert(std::is_same<weightset_t_of<Aut>, b>::value,
                    "determinize: requires Boolean weights");

    public:
      using automaton_t = Aut;
      using automaton_nocv_t = mutable_automaton<context_t_of<Aut>>;
      using label_t = label_t_of<automaton_t>;
      using context_t = context_t_of<automaton_t>;
      /// Set of (input) states.
      using state_set = std::bitset<N>;

      /// Build the determinizer.
      /// \param a         the automaton to determinize
      determinization_bitset_impl(const automaton_t& a)
        : input_(a)
        , output_(make_mutable_automaton<context_t>(a->context()))
      {
        // Input final states.
        for (auto t : input_->final_transitions())
          finals_.set(input_->src_of(t));

        // The input initial states.
        //
        // We could start with pre only, but then on an input
        // automaton without initial state, we would produce an empty
        // automaton (no states).  This would not conform to Jacques'
        // definition of determinization.
        state_set pre;
        pre.set(input_->pre());
        todo_.push(pre);
      }

      /// The state for set of states \a ss.
      /// If this is a new state, schedule it for visit.
      state_t state(const state_set& ss)
      {
        // Benches show that the map_.emplace technique is slower, and
        // then that operator[] is faster than emplace.
        state_t res;
        auto i = map_.find(ss);
        if (i == std::end(map_))
          {
            res = output_->add_state();
            map_[ss] = res;
            if ((ss & finals_).any())
              output_->set_final(res);

            todo_.push(ss);
          }
        else
          res = i->second;
        return res;
      }

      /// Determinize all accessible states.
      automaton_nocv_t operator()()
      {
        bool first=true;
        std::map<label_t, state_set, internal::less<labelset_t_of<Aut>>> ml;
        while (!todo_.empty())
          {
            auto ss = std::move(todo_.top());
            state_t src;
            if(first) {
              src = output_->pre();
              first=false;
            }
            else
              src = state(ss);
            todo_.pop();

            ml.clear();
            auto it=internal::get_iterator(ss);
            while(it.has_next())
              {
                auto s=it.next();
                // Cache the output transitions of state s.
                auto i = successors_.find(s);
                if (i == successors_.end())
                  {
                    i = successors_.emplace(s, label_map_t{}).first;
                    auto& j = i->second;
                    for (auto t : input_->out(s))
                      {
                        auto l = input_->label_of(t);
                        j[l].set(input_->dst_of(t));
                      }
                  }

                // Store in ml the possible destination per label.
                for (const auto& p : i->second)
                  {
                    auto j = ml.find(p.first);
                    if (j == ml.end())
                      ml[p.first] = p.second;
                    else
                      j->second |= p.second;
                  }
              }

            // Outgoing transitions from the current (result) state.
            for (const auto& e : ml)
              output_->new_transition(src, state(e.second), e.first);
          }
        return output_;
      }

      void set_history() {
        auto history = std::make_shared<partition_history<automaton_t>>(input_);
        output_->set_history(history);
        if(!input_->get_name().empty()) {
          output_->set_desc("Determinization of "+input_->get_name());
          output_->set_name("det-"+input_->get_name());
        }
        else {
          output_->set_desc("Determinization");
          output_->set_name("det");
        }
        for (const auto& p: map_)
          {
            std::set<state_t> from;
            const auto& ss = p.first;
            auto it=internal::get_iterator(ss);
            while(it.has_next())
              from.emplace(it.next());
            history->add_state(p.second, std::move(from));
          }
      }

    private:
      /// Set of input states -> output state.
      using map = std::unordered_map<state_set, state_t>;
      map map_;

      /// Input automaton.
      automaton_t input_;
      /// Output automaton.
      automaton_nocv_t output_;

      /// The sets of (input) states waiting to be processed.
      using stack = std::stack<state_set>;
      stack todo_;

      /// Set of final states in the input automaton.
      state_set finals_;

      /// successors[SOURCE-STATE][LABEL] = DEST-STATESET.
      using label_map_t = std::unordered_map<label_t, state_set>;
      using successors_t = std::unordered_map<state_t, label_map_t>;
      successors_t successors_;
    };


    /// \brief The subset construction automaton from another.
    ///
    /// \tparam Aut an automaton type.
    /// \pre labelset is free.
    /// \pre weightset is Boolean.
    template <typename Aut>
    class determinization_set_impl
    {
      static_assert(labelset_t_of<Aut>::is_free(),
                    "determinize: requires free labelset");
      static_assert(std::is_same<weightset_t_of<Aut>, b>::value,
                    "determinize: requires Boolean weights");

    public:
      using automaton_t = Aut;
      using automaton_nocv_t = mutable_automaton<context_t_of<Aut>>;
      using label_t = label_t_of<automaton_t>;
      using context_t = context_t_of<automaton_t>;

      /// Set of (input) states.
      using state_set = std::set<state_t>;

      /// Build the determinizer.
      /// \param a         the automaton to determinize
      determinization_set_impl(const automaton_t& a)
        : input_(a)
        , output_(make_mutable_automaton<context_t>(a->context()))
      {
        // Input final states.
        for (auto t : input_->final_transitions())
          finals_.emplace(input_->src_of(t));

        // The input initial states.
        //
        // We could start with pre only, but then on an input
        // automaton without initial state, we would produce an empty
        // automaton (no states).  This would not conform to Jacques'
        // definition of determinization.
        state_set pre;
        pre.emplace(input_->pre());
        todo_.push(pre);
      }

      /// The state for set of states \a ss.
      /// If this is a new state, schedule it for visit.
      state_t state(const state_set& ss)
      {
        state_t res;
        auto i = map_.find(ss);
        if (i == std::end(map_))
          {
            res = output_->add_state();
            map_[ss] = res;
            todo_.push(ss);
          }
        else
          res = i->second;
        return res;
      }

      /// Determinize all accessible states.
      automaton_nocv_t operator()()
      {
        bool first=true;
        std::map<label_t, state_set, internal::less<labelset_t_of<Aut>>> ml;
        while (!todo_.empty())
          {
            auto ss = std::move(todo_.top());
            state_t src;
            if(first) {
                src = output_->pre();
                first=false;
            }
            else
              src = state(ss);
            todo_.pop();

            ml.clear();
            bool isfinal=false;
            for (auto s : ss) {
              for(auto t : input_->all_out(s))
                if(input_->dst_of(t)==input_->post())
                  isfinal=true;
                else {
                  auto j = ml.find(input_->label_of(t));
                  if (j == ml.end()) {
                    state_set st;
                    st.emplace(input_->dst_of(t));
                    ml[input_->label_of(t)] = st;
                  }
                  else
                    j->second.emplace(input_->dst_of(t));
                }
            }
            if(isfinal)
              output_->set_final(src);

        // Outgoing transitions from the current (result) state.
        for (const auto& e : ml)
          output_->new_transition(src, state(e.second), e.first);
        }
        return output_;
      }

      void set_history() {
        auto history = std::make_shared<partition_history<automaton_t>>(input_);
        output_->set_history(history);
        for (const auto& p: map_)
          {
            std::set<state_t> from;
            const auto& ss = p.first;
            for (auto s : ss)
              from.emplace(s);
            history->add_state(p.second, std::move(from));
          }
      }

    private:
      /// Set of input states -> output state.
      using map = std::map<state_set, state_t>;
      map map_;

      /// Input automaton.
      automaton_t input_;
      /// Output automaton.
      automaton_nocv_t output_;

      /// The sets of (input) states waiting to be processed.
      using stack = std::stack<state_set>;
      stack todo_;

      /// Set of final states in the input automaton.
      state_set finals_;

      /// successors[SOURCE-STATE][LABEL] = DEST-STATESET.
      using label_map_t = std::map<label_t, state_set>;
    };
  }

  /*-------------------------------------.
  | universal weighted determinization.  |
  `--------------------------------------*/
  namespace internal
  {
    /// \brief The weighted determinization of weighted automaton.
    ///
    /// This function only applies to true finite state machine.
    /// The result is a deterministic automaton where the weight of the
    /// word is given by the final function.
    /// If the semiring is not locally finite, the termination is not assured.
    ///
    ///
    /// \tparam Aut an weighted automaton type.
    /// \pre labelset is free.
    template <typename Aut>
    class detweighted_algo_impl
    {
      static_assert(labelset_t_of<Aut>::is_free(),
                    "determinize: requires free labelset");

    public:
      using automaton_t = Aut;

      using label_t = label_t_of<automaton_t>;
      using weightset_t = weightset_t_of<automaton_t>;
      using weight_t = weight_t_of<automaton_t>;

      /// An output state is a list of weighted input states.
      struct stateset
      {
        stateset(const automaton_t& aut)
          : aut_(aut)
        {}

        using value_t = state_t;
        using kind_t = void;
        static bool less_than(state_t l, state_t r)
        {
          return l < r;
        }

        automaton_t aut_;
      };

      using state_nameset_t = polynomialset<context<stateset, weightset_t>>;
      using state_name_t = typename state_nameset_t::value_t;

      /// Build the weighted determinizer.
      /// \param a         the weighted automaton to determinize
      /// \param lim        the depth of the determinisation; -1 means infinity
      detweighted_algo_impl(const automaton_t& a, int lim=-1)
        : input_(a)
        , output_(make_mutable_automaton(a->context()))
	, limit_(lim)  
      {}

      automaton_t operator()() {
	return (*this)([](state_name_t) {return true;});
      }

      /// The determinization of weighted automaton
      /// the state is added to the result only if the predicate \p pred is true,
      /// otherwise it is replace by the 'unknown' state
      /// @param pred a lambda state_name_t -> bool
      /// @tparam Pred the type of pred
      template<typename Pred>
	automaton_t operator()(Pred pred)
      {
        state_name_t ss;
	unknown_ = output_->null_state();
        for (auto t : input_->initial_transitions())
          ss.emplace(input_->dst_of(t), input_->weight_of(t));
        output_->set_initial(state_(ss,0,pred(ss)));

        // label -> <destination, sum of weights>.
        std::map<label_t,
                 std::pair<state_name_t, weight_t>,
                 internal::less<labelset_t_of<automaton_t>>> dests;
        while (!todo_.empty())
          {
            ss = std::move(todo_.front().first);
	    unsigned depth = todo_.front().second; 
            todo_.pop();
            auto src = map_[ss];
            dests.clear();
            for (const auto& p : ss)
              {
                auto s = p.first;
                auto v = p.second;
                for (auto t : input_->out(s))
                  {
                    auto l = input_->label_of(t);
                    auto dst = input_->dst_of(t);
                    auto w = ws_.mul(v, input_->weight_of(t));

                    // For each letter, update destination state, and
                    // sum of weights.
                    if (dests.find(l)==dests.end())
                      dests.emplace(l, make_pair(ns_.zero(), ws_.zero()));
                    auto& d = dests[l];
                    ns_.add_here(d.first, dst, w);
                    d.second = ws_.add(d.second, w);
                  }
              }
            for (auto& d : dests)
              output_->new_transition(src,
				      state_(d.second.first, depth+1, pred(d.second.first)),
                                   d.first);
          }
          return output_;
      }

    private:
      /// The state for set of states \a ss.
      /// If this is a new state, schedule it for visit.
      state_t state_(const state_name_t& name, unsigned depth, bool true_state) {
        // Benches show that the map_.emplace technique is slower, and
        // then that operator[] is faster than emplace.
        state_t res;
        auto i = map_.find(name);
        if (i == std::end(map_))
	  if(true_state && (limit_ <0 || depth <= (unsigned) limit_)) {
            res = output_->add_state();
            map_[name] = res;
            for (const auto& p : name)
              if (input_->is_final(p.first))
                output_->add_final(res,
				   ws_.mul(p.second,
					   input_->get_final_weight(p.first)));

            todo_.push(std::make_pair(name,depth));
          }
	  else {
	    if(unknown_ == output_->null_state()) {
	      unknown_ = output_->add_state();
	      for(auto l : output_->labelset()->genset())
		output_->new_transition(unknown_, unknown_, l);
	      output_->set_state_name(unknown_,"...");
	    }
	    res = unknown_;
	  }
        else
          res = i->second;
        return res;
      };

      /// Map from state name to state number.
      std::map<state_name_t, state_t, internal::less<state_nameset_t>> map_;

      /// Input automaton.
      automaton_t input_;

      /// Output automaton.
      automaton_t output_;

      /// Its weightset.
      weightset_t ws_ = *input_->weightset();

      /// The polynomialset that stores weighted states.
      state_nameset_t ns_ = {{stateset(input_), *input_->weightset()}};

      /// We use state numbers as indexes, so we need to know the last
      /// state number.  If states were removed, it is not the same as
      /// the number of states.
      unsigned state_size_ = input_->max_state() + 1;

      /// The sets of (input) states waiting to be processed.
      using queue = std::queue<std::pair<state_name_t,unsigned>>;
      queue todo_;
      /// The maximal depth in the exploration (-1 = infinity)
      int limit_;
      state_t unknown_;
    };
  }


}}//end of ns awali::stc

#endif // !AWALI_ALGOS_DETERMINIZE_HXX
