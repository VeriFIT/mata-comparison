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

#ifndef AWALI_ALGOS_AUT_TO_EXP_HH
# define AWALI_ALGOS_AUT_TO_EXP_HH

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/lift.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/misc/vector.hh>
#include <awali/sttc/labelset/traits.hh>

namespace awali { namespace sttc {


  /*----------------.
  | state_chooser.  |
  `----------------*/

  /// A state (inner) from an automaton.
  template <typename Aut,
            typename Lifted = internal::lifted_automaton_t<Aut>>
  using state_chooser_t =
    std::function<state_t(const Lifted&)>;


  /*--------------------------.
  | Naive heuristics degree.  |
  `--------------------------*/

  template <typename Aut>
  state_t
  next_heuristic(const Aut& a)
  {
    state_t best = 0;
    bool best_has_loop = false;
    size_t best_degree = std::numeric_limits<size_t>::max();
    for (auto s: a->states())
      {
        size_t out = 0;
        // Since we are in LAO, there can be at most one such loop.
        bool has_loop = false;
        // Don't count the loops as out-degree.
        for (auto t: a->all_out(s))
          if (a->dst_of(t) != s)
            ++out;
          else
            has_loop = true;
        size_t in = a->all_in(s).size();
        size_t degree = in * out;
        // We prefer to delete a state that has no loop transition.
        if (degree < best_degree
            || (degree == best_degree && has_loop < best_has_loop))
          {
            best = s;
            best_degree = degree;
            best_has_loop = has_loop;
          }
      }
    assert(best);
    return best;
  }

  /*--------------------------.
  | Choose states in order.   |
  `--------------------------*/

/* */
  template <typename Aut>
  state_t
  next_in_order(const Aut& a)
  {
    state_t best = 0;
    for (auto s: a->states())
      {
        best = s;
      }
    assert(best);
    return best;
   }
/* */

  /*------------------.
  | eliminate_state.  |
  `------------------*/

  namespace internal
  {
    template <typename Aut, typename Kind = typename context_t_of<Aut>::kind_t>
    struct state_eliminator;

    template <typename Aut>
    struct state_eliminator<Aut, labels_are_one>
    {
      static_assert(context_t_of<Aut>::is_lao,
                    "requires labels_are_one");

      using automaton_t = typename std::remove_cv<Aut>::type;
      using weightset_t = weightset_t_of<automaton_t>;
      /// State selector type.
      using state_chooser_t = std::function<state_t(const automaton_t&)>;

      state_eliminator(automaton_t& aut)
        : debug_(0)
        , aut_(aut)
      {}

      /// Eliminate state s.
      void operator()(state_t s)
      {
        require(aut_->has_state(s), "not a valid state: " + std::to_string(s));

        // The loop's weight.
        auto loop = ws_.zero();
        assert(aut_->outin(s, s).begin()==aut_->outin(s, s).end()
                || ++(aut_->outin(s, s).begin())==aut_->outin(s, s).end());
        // There is a single possible loop labeled by \e, but it's
        // easier and symmetrical with LAR to use a for-loop.
        for (auto t: to_vector(aut_->outin(s, s)))
          {
            loop = ws_.add(loop, aut_->weight_of(t));
            aut_->del_transition(t);
          }
        loop = ws_.star(loop);

        // Get all the predecessors, and successors, except itself.
        auto outs = aut_->all_out(s);
        for (auto in: aut_->all_in(s))
          for (auto out: outs)
            aut_->add_transition
              (aut_->src_of(in), aut_->dst_of(out),
               aut_->label_of(in),
               ws_.mul(aut_->weight_of(in), loop, aut_->weight_of(out)));
        aut_->del_state(s);
      }

      /// Eliminate all the states, in the order specified by \a next_state.
      void operator()(const state_chooser_t& next_state)
      {
        while (aut_->num_states())
          operator()(next_state(aut_));
      }

    private:
      /// Debug level.  The higher, the more details are reported.
      int debug_;
      /// The automaton we work on.
      automaton_t& aut_;
      /// Shorthand to the weightset.
      const weightset_t& ws_ = *aut_->weightset();
    };

    template <typename Aut>
    struct state_eliminator<Aut, labels_are_ratexps>
    {
      static_assert(context_t_of<Aut>::is_lar,
                    "requires labels_are_ratexps");
      // FIXME: ratexpset<lal_char(a-c)_z>_q for instance cannot work,
      // because we need to move the q weights inside the
      // lal_char(a-c)_z ratexps, which obviously not possible.  So we
      // need to require that there is a subtype relationship between
      // the weightset and the weightset of the ratexp.
      //
      // Yet as of 2014-07, there is no means to check that subtype
      // relationship in Vaucanson.

      using automaton_t = typename std::remove_cv<Aut>::type;
      using ratexpset_t = labelset_t_of<automaton_t>;
      using weightset_t = weightset_t_of<automaton_t>;
      /// State selector type.
      using state_chooser_t = std::function<state_t(const automaton_t&)>;

      state_eliminator(automaton_t& aut)
        : debug_(0)
        , aut_(aut)
      {}

      /// Eliminate state s.
      void operator()(state_t s)
      {
        require(aut_->has_state(s), "not a valid state: " + std::to_string(s));

        // The loops' ratexp.
        auto loop = rs_.zero();
        for (auto t: to_vector(aut_->outin(s, s)))
          {
            loop = rs_.add(loop,
                           rs_.lmul(aut_->weight_of(t), aut_->label_of(t)));
            aut_->del_transition(t);
          }
        loop = rs_.star(loop);

        // Get all the predecessors, and successors, except itself.
        auto outs = aut_->all_out(s);
        for (auto in: aut_->all_in(s))
          for (auto out: outs)
            aut_->add_transition
              (aut_->src_of(in), aut_->dst_of(out),
               rs_.mul(rs_.lmul(aut_->weight_of(in), aut_->label_of(in)),
                       loop,
                       rs_.lmul(aut_->weight_of(out), aut_->label_of(out))));
        aut_->del_state(s);
      }

      /// Eliminate all the states, in the order specified by \a next_state.
      void operator()(const state_chooser_t& next_state)
      {
        while (aut_->num_states())
          operator()(next_state(aut_));
      }

    private:
      /// Debug level.  The higher, the more details are reported.
      int debug_;
      /// The automaton we work on.
      automaton_t& aut_;
      /// Shorthand to the labelset, which is a ratexpset.
      const ratexpset_t& rs_ = *aut_->labelset();
      /// Shorthand to the weightset.
      const weightset_t& ws_ = *aut_->weightset();
    };
  }

  /*-------------.
  | aut_to_exp.  |
  `-------------*/
    /** Generic state elimination algorithm
     *
     * The automaton is first converted into an automaton where the labels are lifted to weights as rational expressions.
     * Then the algorithm iterates over states. For every state, for every pair formed by an incoming transition *t* and an outgoing one *t'*, a transition is added from the source of *t* to the destination of *t'*.
     * When every pair has been processed, the state is deleted.
     *
     * @tparam Aut the type of the automaton
     * @tparam Context the context of rational expressions
     * @param a the automaton
     * @param next_state an iterator on states which determines the elimination ordering
     * @return a rational expression describing the behaviour of the automaton
     */
  template <typename Aut,
            typename Context = ratexpset_of<context_t_of<Aut>>>
  typename Context::ratexp_t
  aut_to_exp(const Aut& a,
             const state_chooser_t<Aut>& next_state)
  {
    // State elimination is performed on the lifted automaton.
    auto aut = lift(a);
    internal::state_eliminator<decltype(aut)> eliminate_states(aut);
    eliminate_states(next_state);
    return aut->get_initial_weight(aut->post());
  }


    /** Default state elimination algorithm
     *
     * Based on {@link aut_to_exp}.
     * In this algorithm, every state has a rank which is the product of the number of its incoming transitions (except loops) by its outgoing transitions (except loops)
     *
     * The set of remaining states is (partially) ordered :
     * a state *s* is smaller than a state *t* if :        
     *   - the rank of *s* is smaller than the rank of *t*;
     *   - or they have the same rank, s has no loop and t has.      
     *
     * The states are processed with respect to this ordering (the rank may evolve during the algorithm)
     *
     * @tparam Aut the type of the automaton
     * @tparam Context the context of rational expressions
     * @param a the automaton
     * @return a rational expression describing the behaviour of the automaton
     */
  template <typename Aut,
            typename Context = ratexpset_of<context_t_of<Aut>>>
  typename Context::ratexp_t
  aut_to_exp(const Aut& a)
  {
    state_chooser_t<Aut> next = next_heuristic<internal::lifted_automaton_t<Aut>>;
    return aut_to_exp(a, next);
  }

/**/
    /** Basic state elimination algorithm
     *
     * Based on {@link aut_to_exp}.    
     * In this algorithm, the state are processed with respect to their index, which generally corresponds to the order of creation.
     * @tparam Aut the type of the automaton
     * @tparam Context the context of rational expressions
     * @param a the automaton
     * @return a rational expression describing the behaviour of the automaton
     */
  template <typename Aut,
            typename Context = ratexpset_of<context_t_of<Aut>>>
  typename Context::ratexp_t
  aut_to_exp_in_order(const Aut& a)
  {
    state_chooser_t<Aut> next = next_in_order<internal::lifted_automaton_t<Aut>>;
    return aut_to_exp(a, next);
  }
/**/

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_AUT_TO_EXP_HH
