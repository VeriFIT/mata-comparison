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

#ifndef AWALI_ALGOS_ARE_ISOMORPHIC_HH
# define AWALI_ALGOS_ARE_ISOMORPHIC_HH

# include <algorithm>
# include <map>
# include <stack>
# include <unordered_map>
# include <unordered_set>

#include <awali/sttc/algos/accessible.hh>
#include <awali/utils/hash.hh>
#include <awali/sttc/misc/map.hh>
#include <awali/sttc/misc/vector.hh>

// What we refer to as "sequentiality" in this algorithm is an obvious
// generalization to any context of the notion of sequentiality
// defined only for lal, in its turn an obvious generalization of
// determinism to weighted automata.

namespace awali {
  namespace sttc {
    namespace internal {

      /*-----------------.
        | are_isomorphic.  |
        `-----------------*/

      template <typename Aut1, typename Aut2>
      class are_isomorphicer
      {
      private:
        using automaton1_t = Aut1;
        using context1_t = context_t_of<automaton1_t>;
        using weightset1_t = weightset_t_of<automaton1_t>;
        using labelset1_t = labelset_t_of<context1_t>;
        using label1_t = label_t_of<automaton1_t>;
        using weight1_t = weight_t_of<automaton1_t>;

        using automaton2_t = Aut2;
        using context2_t = context_t_of<automaton2_t>;
        using weightset2_t = weightset_t_of<automaton1_t>;
        using labelset2_t = labelset_t_of<context2_t>;
        using label2_t = label_t_of<automaton2_t>;
        using weight2_t = weight_t_of<automaton2_t>;

        // FIXME: do we want to generalize this to heterogeneous contexts?
        // It is not completely clear whether such behavior is desirable.
        using weightset_t = weightset1_t; // FIXME: join!
        using labelset_t = labelset1_t; // FIXME: join!

        using states_t = std::pair<state_t, state_t>;

        const Aut1& a1_;
        const Aut2& a2_;

        /// See the comment for out_ in minimize.hh.
        template<typename Automaton>
        using dout_t =
          std::unordered_map
          <state_t,
           std::unordered_map<label_t_of<Automaton>,
                              std::pair<weight_t_of<Automaton>,
                                        state_t>,
                              utils::hash<labelset_t_of<Automaton>>,
                              utils::equal_to<labelset_t_of<Automaton>>>>;

        /// For the simpler, faster sequential case.
        dout_t<automaton1_t> dout1_;
        dout_t<automaton2_t> dout2_;

        /// For the nonsequential case.
        template<typename Automaton>
        using nout_t =
          std::unordered_map
          <state_t,
           std::unordered_map
           <label_t_of<Automaton>,
            std::unordered_map<weight_t_of<Automaton>,
                               std::vector<state_t>,
                               utils::hash<weightset_t_of<Automaton>>,
                               utils::equal_to<weightset_t_of<Automaton>>>,
            utils::hash<labelset_t_of<Automaton>>,
            utils::equal_to<labelset_t_of<Automaton>>>>;
        nout_t<automaton1_t> nout1_;
        nout_t<automaton2_t> nout2_;

        /// A worklist of pairs of states which are candidate to be
        /// isomorphic.  Or "A candidate-isomorphic state pair worklist",
        /// written in Reverse-Polish English.
        using pair_t = std::pair<state_t, state_t>;
        using worklist_t = std::stack<pair_t>;
        worklist_t worklist_;

        /// The maps associating the states of a1_ and the states of a2_->
        using s1tos2_t = std::unordered_map<state_t, state_t>;
        using s2tos1_t = std::unordered_map<state_t, state_t>;

        /// A datum specifying if two given automata are isomorphic, and
        /// why if they are not.  This should be a variant record, but
        /// BOOST variants are not really suitable (this is not just a
        /// disjoint union: we also need a case tag), we don't like
        /// unions, and visitors are overkill.  The thing might even get
        /// simpler when we generalize to non-sequential automata.
        struct full_response
        {
          enum class tag
          {
            never_computed = -1,      // We didn't run the algorithm yet.
              isomorphic = 0,           // a1_ and a2_ are in fact isomorphic.
              counterexample = 1,       // We found a counterexample of two states
            // [only possible for sequential automata].
              nocounterexample = 2,     // Exhaustive tests failed [we do this only
            // for non-sequential automata].
              trivially_different  = 3, // Different number of states or transitions,
            // of different sequentiality.
              } response;

          /// Only meaningful if the tag is tag::counterexample.
          pair_t counterexample;

          /// Only meaningful if the tag is tag::isomorphic.
          s1tos2_t s1tos2_;
          s2tos1_t s2tos1_;

          full_response()
            : response(tag::never_computed)
          {}
        } fr_;

        // Return true and fill \a dout if \a a is sequential; otherwise
        // return false and clear dout.  We can't use the is_deterministic
        // algorithm, as it's only defined for lal.
        template <typename Automaton>
        bool is_sequential_filling(const Automaton& a,
                                   dout_t<Automaton>& dout)
        {
          for (auto t : a->all_transitions())
            {
              const state_t& src = a->src_of(t);
              const label_t_of<Automaton>& l = a->label_of(t);
              auto& doutsrc = dout[src];
              if (doutsrc.find(l) == doutsrc.end())
                dout[src][l] = {a->weight_of(t), a->dst_of(t)};
              else
                {
                  dout.clear();
                  return false;
                }
            }
          return true;
        }

        void fill_nouts_()
        {
          for (auto t1 : a1_->all_transitions())
            nout1_[a1_->src_of(t1)][a1_->label_of(t1)][a1_->weight_of(t1)]
              .emplace_back(a1_->dst_of(t1));
          for (auto t2 : a2_->all_transitions())
            nout2_[a2_->src_of(t2)][a2_->label_of(t2)][a2_->weight_of(t2)]
              .emplace_back(a2_->dst_of(t2));
        }

        void clear()
        {
          dout1_.clear();
          dout2_.clear();
          nout1_.clear();
          nout2_.clear();
        }

        bool trivially_different()
        {
          // Automata of different sizes are different.
          if (a1_->num_states() != a2_->num_states())
            return true;
          if (a1_->num_transitions() != a2_->num_transitions())
            return true;

          // The idea of comparing alphabet sizes here is tempting, but
          // it's more useful to only deal with the actually used labels;
          // we consider isomorphic even two automata from labelsets with
          // different alphabets, when the actually used labels match.
          // Building a set of actually used labels has linear complexity,
          // and it's not obvious that performing an additional check now
          // would pay in real usage.  FIXME: benchmark in real cases.

          return false;
        }

      public:
        are_isomorphicer(const Aut1 &a1, const Aut2 &a2)
          : a1_(a1)
          , a2_(a2)
        {}

        const full_response
        get_full_response()
        {
          clear();

          // If we prove non-isomorphism at this point, it will be because
          // of sizes.
          fr_.response = full_response::tag::trivially_different;

          require(is_accessible(a1_),
                  "are-isomorphic: lhs automaton must be accessible");
          require(is_accessible(a2_),
                  "are-isomorphic: rhs automaton must be accessible");

          // Before even initializing our data structures, which is
          // potentially expensive, try to simply compare the number of
          // elements such as states and transitions.
          if (trivially_different())
            return fr_;

          if (is_sequential_filling(a1_, dout1_))
            if (is_sequential_filling(a2_, dout2_))
              return get_full_response_sequential();
            else
              return fr_; // Different sequentiality.
          else
            if (is_sequential_filling(a2_, dout2_))
              return fr_; // Different sequentiality.
            else
              {
                fill_nouts_();
                return get_full_response_nonsequential();
              }
        }

        using states1_t = std::vector<state_t>;
        using states2_t = std::vector<state_t>;

        /// Automaton states partitioned into classes.  It's guaranteed that
        /// a left[right] state in a given class can not be isomorphic to a
        /// right[left] in a different class.  The idea of course is to restrict
        /// the brute-force search to the states within a single class.
        using class_id = std::size_t;
        using class_pair_t = std::pair<states1_t, states2_t>;
        using state_classes_t = std::vector<class_pair_t>;
        state_classes_t state_classes_;

        template<typename Automaton>
        class_id state_to_class(state_t s, Automaton& a)
        {
          class_id res = 0;

          std::hash_combine(res, s == a->pre());
          std::hash_combine(res, s == a->post());

          const auto ws = * a->weightset();
          const auto ls = * a->labelset();

          using transition_t = std::pair<weight_t_of<Automaton>,
                                         label_t_of<Automaton>>;
          using transitions_t = std::vector<transition_t>;
          const auto less_than =
            [&](const transition_t& t1, const transition_t& t2)
            {
              if (ws.less_than(t1.first, t2.first))
                return true;
              else if (ws.less_than(t2.first, t1.first))
                return false;
              else
                return ls.less_than(t1.second, t2.second);
            };

#define HASH_TRANSITIONS(expression, endpoint_getter)                   \
          {                                                             \
            std::unordered_set<state_t> endpoint_states;                \
            transitions_t tt;                                           \
            for (auto& t: expression)                                   \
              {                                                         \
                tt.emplace_back(transition_t{a->weight_of(t), a->label_of(t)}); \
                endpoint_states.emplace(a->endpoint_getter(t));         \
              }                                                         \
            std::sort(tt.begin(), tt.end(), less_than);                 \
            for (const auto& t: tt)                                     \
              {                                                         \
                std::hash_combine(res, ws.hash(t.first));               \
                std::hash_combine(res, ls.hash(t.second));              \
              }                                                         \
            std::hash_combine(res, endpoint_states.size());             \
          }

          // Hash input transitions data, in a way which doesn't depend on
          // state numbers or transition order.
          HASH_TRANSITIONS(a->all_in(s), src_of);

          // Do the same for output transitions.
          HASH_TRANSITIONS(a->all_out(s), dst_of);

#undef HASH_TRANSITIONS

          return res;
        }

        const state_classes_t make_state_classes()
        {
          // Class left and right states:
          std::unordered_map<class_id, std::pair<states1_t, states2_t>> table;
          for (auto s1: a1_->all_states())
            table[state_to_class(s1, a1_)].first.emplace_back(s1);
          for (auto s2: a2_->all_states())
            table[state_to_class(s2, a2_)].second.emplace_back(s2);

          // Return a table without class hashes sorted by decreasing
          // (left) size, in order to perform the most constrained choices
          // first.
          state_classes_t res;
          for (const auto& c: table)
            res.emplace_back(std::move(c.second.first), std::move(c.second.second));
          std::sort(res.begin(), res.end(),
            [](const class_pair_t& c1, const class_pair_t& c2)
              {
          return c1.first.size() > c2.first.size();
        });

          for (auto& c: res)
            {
          // This is mostly to make debugging easier.
          std::sort(c.first.begin(), c.first.end());

          // This call is needed for correctness: we have to start
          // with all classes in their "smallest" configuration in
          // lexicographic order.
          std::sort(c.second.begin(), c.second.end());
        }

          //print_class_stats(res);
          return res;
        }

        /// Handy debugging method.
        void print_class_stats(const state_classes_t& cs)
        {
          size_t max = 0, min = a1_->num_all_states();
          long double sum = 0.0;
          for (const auto& c: cs)
            {
          max = std::max(max, c.first.size());
          min = std::min(min, c.first.size());
          sum += c.first.size();
        }
          long state_no = a1_->num_all_states();
          std::cerr << "State no: " << state_no << "\n";
          std::cerr << "Class no: " << cs.size() << "\n";
          std::cerr << "* min class size: " << min << "\n";
          std::cerr << "* avg class size: " << sum / cs.size() << "\n";
          std::cerr << "* max class size: " << max << "\n";
        }

        /// Handy debugging method.
        void print_classes(const state_classes_t& cs)
        {
          for (const auto& c: cs)
            {
          std::cerr << "* ";
          for (const auto& s1: c.first)
            std::cerr << s1 << " ";
          std::cerr << "-- ";
          for (const auto& s2: c.second)
            std::cerr << s2 << " ";
          std::cerr << "\n";
        }
        }

        bool is_isomorphism_valid()
        {
          try
            {
          return is_isomorphism_valid_throwing();
        }
          catch (const std::out_of_range&)
            {
          return false;
        }
        }
        bool is_isomorphism_valid_throwing()
        {
          std::unordered_set<state_t> mss1;
          std::unordered_set<state_t> mss2;
          std::stack<pair_t> worklist;
          worklist.push({a1_->pre(), a2_->pre()});
          worklist.push({a1_->post(), a2_->post()});
          while (! worklist.empty())
            {
          const auto p = std::move(worklist.top()); worklist.pop();
          const state_t s1 = p.first;
          const state_t s2 = p.second;

          // Even before checking for marks, check if these two states
          // are supposed to be isomorphic with one another.  We can't
          // avoid this just because we've visited them before.
          if (fr_.s1tos2_.at(s1) != s2 || fr_.s2tos1_.at(s2) != s1)
            return false;
          const bool m1 = (mss1.find(s1) != mss1.end());
          const bool m2 = (mss2.find(s2) != mss2.end());
          if (m1 != m2)
            return false;
          else if (m1 && m2)
            continue;
          // Otherwise we have that ! m1 && ! m2.
          mss1.emplace(s1);
          mss2.emplace(s2);

          // If only one of s1 and s2 is pre or post then this is not
          // an isomorphism.
          if ((s1 == a1_->pre()) != (s2 == a2_->pre())
            || (s1 == a1_->post()) != (s2 == a2_->post()))
            return false;

          int t1n = 0, t2n = 0;
          for (auto t1: a1_->all_out(s1))
            {
          auto d1 = a1_->dst_of(t1);
          const auto& w1 = a1_->weight_of(t1);
          const auto& l1 = a1_->label_of(t1);
          const auto& d2s = nout2_.at(s2).at(l1).at(w1);
          auto d2 = fr_.s1tos2_.at(d1); // according to the isomorphism
          if (!internal::has(d2s, d2))
            return false;
          worklist.push({d1, d2});
          ++ t1n;
        }
          for (auto t2: a2_->all_out(s2))
            {
              auto d2 = a2_->dst_of(t2);
              const auto& w2 = a2_->weight_of(t2);
              const auto& l2 = a2_->label_of(t2);
              const auto& d1s = nout1_.at(s1).at(l2).at(w2);
              auto d1 = fr_.s2tos1_.at(d2); // according to the isomorphism
              if (!internal::has(d1s, d1))
                return false;
              worklist.push({d1, d2});
              ++ t2n;
            }
          if (t1n != t2n)
            return false;
            } // while
          return true;
        }

        void update_result_isomorphism()
        {
          for (const auto& c: state_classes_)
            for (int i = 0; i < int(c.first.size()); ++ i)
              {
                state_t s1 = c.first[i];
                state_t s2 = c.second[i];
                fr_.s1tos2_[s1] = s2;
                fr_.s2tos1_[s2] = s1;
              }
        }

        long factorial(long n)
        {
          long res = 1;
          for (long i = 1; i <= n; ++ i)
            res *= i;
          return res;
        }

        /// We need to keep some (small) state between a
        /// next_class_combination call and the next.
        std::vector<long> class_permutation_max_;
        std::vector<long> class_permutation_generated_;

        void initialize_next_class_combination_state()
        {
          class_permutation_max_.clear();
          class_permutation_generated_.clear();
          for (const auto& c: state_classes_) {
            class_permutation_max_.emplace_back(factorial(c.second.size()) - 1);
            class_permutation_generated_.emplace_back(0);
          }
        }

        // Build the next class combination obtained by permuting one
        // class Like std::next_permutation, return true iff it was
        // possible to rearrange into a "greater" configuration; if not,
        // reset all classes to their first configuration.
        bool next_class_combination()
        {
          // This algorithm is strongly reminiscent of natural number
          // increment in a positional system, with carry propagation; in
          // order to generate a configuration we permute the rightmost
          // class; if this permutation resets it to its original value,
          // we propagate the "carry" by continuing to permute the class
          // on the left possibly propagating further left, and so on.

          const int rightmost = int(state_classes_.size()) - 1;

          // Permute the rightmost class.
          if (std::next_permutation(state_classes_[rightmost].second.begin(),
                                    state_classes_[rightmost].second.end()))
            {
              // Easy case: no carry.
              ++ class_permutation_generated_[rightmost];
              return true;
            }

          // Permuting the rightmost class made it go back to its minimal
          // configuration: propagate carry to the left.
          //
          // Carry propagation works by resetting all consecutive
          // rightmost maximum-configuration class to their initial
          // configuration, and permuting the leftmost one which was not
          // maximum.  If no such leftmost class exist then "the carry
          // overflows", and we're done.
          assert(class_permutation_generated_[rightmost]
                 == class_permutation_max_[rightmost]);
          class_permutation_generated_[rightmost] = 0;
          int i;
          for (i = rightmost - 1;
               i >= 0
                 && class_permutation_generated_[i] == class_permutation_max_[i];
               -- i)
            {
              std::next_permutation(state_classes_[i].second.begin(),
                                    state_classes_[i].second.end());
              class_permutation_generated_[i] = 0;
            }
          if (i == -1)
            return false; // Carry overflow.

          // Permute in order to propagate the carry to its final place.
          std::next_permutation(state_classes_[i].second.begin(),
                                state_classes_[i].second.end());
          ++ class_permutation_generated_[i];
          return true;
        }

        const full_response
        get_full_response_nonsequential()
        {
          // Initialize state classes, so that later we can only do
          // brute-force search *within* each class.
          state_classes_ = make_state_classes();

          // Don't even bother to search if the classes of left and right
          // states have different sizes:
          for (const auto& c: state_classes_)
            if (c.first.size() != c.second.size())
              {
                fr_.response = full_response::tag::nocounterexample;
                return fr_;
              }

          // Reorder the right-hand side in all possible ways allowed by
          // classes, until we stumble on a solution.
          initialize_next_class_combination_state();
          do
            {
              update_result_isomorphism();
              if (is_isomorphism_valid())
                {
                  fr_.response = full_response::tag::isomorphic;
                  return fr_;
                }
            }
          while (next_class_combination());

          // We proved by exhaustion that no solution exists.
          fr_.response = full_response::tag::nocounterexample;
          return fr_;
        }

        const full_response
        get_full_response_sequential()
        {
          // If we prove non-isomorphism from now on, it will be by
          // presenting some specific pair of states.
          fr_.response = full_response::tag::counterexample;

          worklist_.push({a1_->pre(), a2_->pre()});

          while (! worklist_.empty())
            {
              const states_t states = worklist_.top(); worklist_.pop();
              const state_t s1 = states.first;
              const state_t s2 = states.second;

              // If we prove non-isomorphism in this iteration, it will be
              // by using the <s1, s2> pair as a counterexample.
              fr_.counterexample = {s1, s2};

              // If the number of transitions going out from the two
              // states is different, don't even bother looking at them.
              // On the other hand if the number is equal we can afford to
              // reason in just one direction: look at transitions from s1
              // and ensure that all of them have a matching one from s2.
              if (dout1_[s1].size() != dout2_[s2].size())
                return fr_;

              for (const auto& l1_w1dst1 : dout1_[s1]) // dout1_.at(s1) may fail.
                {
                  const label1_t& l1 = l1_w1dst1.first;
                  const weight1_t& w1 = l1_w1dst1.second.first;
                  const state_t& dst1 = l1_w1dst1.second.second;

                  const auto& s2out = dout2_.find(s2);
                  if (s2out == dout2_.cend())
                    return fr_;
                  const auto& s2outl = s2out->second.find(l1);
                  if (s2outl == s2out->second.cend())
                    return fr_;
                  weight2_t w2 = s2outl->second.first;
                  state_t dst2 = s2outl->second.second;

                  if (! weightset_t::equals(w1, w2))
                    return fr_;

                  const auto& isomorphics_to_dst1 = fr_.s1tos2_.find(dst1);
                  const auto& isomorphics_to_dst2 = fr_.s2tos1_.find(dst2);

                  if (isomorphics_to_dst1 == fr_.s1tos2_.cend())
                    {
                      if (isomorphics_to_dst2 == fr_.s2tos1_.cend()) // Both empty.
                        {
                          fr_.s1tos2_[dst1] = dst2;
                          fr_.s2tos1_[dst2] = dst1;
                          worklist_.push({dst1, dst2});
                        }
                      else
                        return fr_;
                    }
                  else if (isomorphics_to_dst1 == fr_.s1tos2_.cend()
                           || isomorphics_to_dst1->second != dst2
                           || isomorphics_to_dst2->second != dst1)
                    return fr_;
                } // outer for
            } // while
          fr_.response = full_response::tag::isomorphic;
          return fr_;
        }

        bool operator()()
        {
          const full_response& r = get_full_response();
          return r.response == full_response::tag::isomorphic;
        }

        /// A map from each a2_ state to the corresponding a1_ state.  The
        /// map is ordered, as usual for origins, hence different from
        /// fr_.s2tos1_.
        using origins_t = std::map<state_t, state_t>;

        /// Only meaningful if operator() returned true.
        origins_t
        origins()
        {
          require(fr_.reponse == full_response::tag::isomorphic,
                  __func__, ": isomorphism-check not successfully performed");
          origins_t res;
          for (const auto& s2s1: fr_.s2tos1_)
            res[s2s1.first] = s2s1.second;
          return res;
        }

        /// Print origins.
        static
        std::ostream&
        print(const origins_t& orig, std::ostream& o)
        {
          o << "/* Origins." << std::endl
            << "    node [shape = box, style = rounded]" << std::endl;
          for (auto p : orig)
            if (2 <= p.first)
              {
                o << "    " << p.first - 2
                  << " [label = \"" << p.second << "\"]" << std::endl;
              }
          o << "*/" << std::endl;
          return o;
        }

      };
    }
    /** @brief tests whether two automata are isomorphic
     *
     * @tparam Aut1 the type of the first automaton
     * @tparam Aut2 the type of the second automaton
     * @param a1 the first automaton
     * @param a2 the second automaton
     * @return `true` if and only if the automata are isomorphic
     */
    template <typename Aut1, typename Aut2>
    bool
    are_isomorphic(const Aut1& a1, const Aut2& a2)
    {
      internal::are_isomorphicer<Aut1, Aut2> are_isomorphic(a1, a2);

      return are_isomorphic();
    }

  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_ARE_ISOMORPHIC_HH
