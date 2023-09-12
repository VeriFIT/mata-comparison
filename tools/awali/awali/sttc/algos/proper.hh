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

#ifndef AWALI_ALGOS_PROPER_HH
# define AWALI_ALGOS_PROPER_HH

# include <stdexcept>
# include <type_traits>
# include <unordered_map>
# include <unordered_set>
# include <utility>
# include <vector>

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/dot.hh>
#include <awali/sttc/algos/is_eps_acyclic.hh>
#include <awali/sttc/algos/is_proper.hh>
#include <awali/sttc/algos/is_valid.hh>
#include <awali/sttc/algos/transpose.hh>
#include <awali/sttc/core/kind.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/common/enums.hh>
#include <awali/utils/heap.hh>
#include <awali/common/ato.hh>

#define STATS

namespace awali { namespace sttc {


  namespace internal
  {
    /// Debug level set in the user's environment.
    static inline
    int debug_level()
    {
      if (auto cp = getenv("AWALI_DEBUG")) {
          return  *cp ? awali::atoi(cp) : 1;
      }
        else
        return 0;
    }

    /// This class contains the core of the proper algorithm.
    ///
    /// This class is specialized for labels_are_letter automata since
    /// all these methods become trivial.
    template <typename Aut,
              bool has_one = labelset_t_of<Aut>::has_one()>
    class properer
    {
      using automaton_t = typename std::remove_cv<Aut>::type;
      using weightset_t = weightset_t_of<automaton_t>;
      using weight_t = typename weightset_t::value_t;
      using label_t = label_t_of<automaton_t>;
      using transitions_t = std::vector<transition_t>;

    public:
      /// Get ready to eliminate spontaneous transitions.
      /// \param aut    the automaton in which to remove them
      /// \param prune  whether to delete states that become inaccessible
      properer(automaton_t& aut, bool prune = true)
        : debug_(debug_level())
        , aut_(aut)
        , ws_(*aut->weightset())
        , empty_word_(aut->labelset()->one())        
        , todo_(aut->num_states())
        , prune_(prune)
      {}

      /**@brief Remove the epsilon-transitions of the input
         The behaviour of this method depends on the star_status of the weight_set:
         -- starrable : always valid, does not throw any exception
         -- tops : the proper algo is directly launched on the input;
         if it returns false, an exception is launched
         -- non_starrable / absval:
         is_valid is called before launching the algorithm.
         @param aut The automaton in which epsilon-transitions will be removed
         @param prune   Whether to remove states that become inaccessible.
         @throw runtime_error if the input is not valid
      */
      static void proper_here(automaton_t& aut, bool prune = true)
      {
        if (!is_proper(aut))
          proper_here_<weightset_t::star_status()>(aut, prune);
      }

      /**
         @brief The core of the (backward) epsilon-removal.

         For each state s
         if s has an epsilon-loop with weight w
         if w is not starrable, return false
         else compute ws = star(w)
         endif
         remove the loop
         else
         ws = 1
         endif
         for each incoming epsilon transition e:p-->s with weight h
         for each outgoing transition s--a|k-->q
         add (and not set) transition p-- a | h.ws.k --> q
         endfor
         if s is final with weight h
         add final weight h.ws to p
         endif
         remove e
         endfor
         endfor
         return true

         If the method returns false, \a aut is corrupted.

         @param aut The automaton in which epsilon-transitions will be removed
         @param prune   Whether to remove states that become inaccessible.
         @return true if the proper succeeds, or false otherwise.
      */

      static bool in_situ_remover(automaton_t& aut, bool prune = true)
      {
        try
          {
            properer p(aut, prune);
            p.in_situ_remover_();
            return true;
          }
        catch (const std::runtime_error&)
          {
            return false;
          }
      }

    private:
        /// \brief State profile.
        ///
        /// used for priority queue
        /// entries are ordered w.r.t prioritary criteria :
        /// 1. number of outgoing spontaneous transitions
        /// 2. number of outgoing transitions
        /// 3. number of incoming spontaneous transitions
        using state_profile = std::tuple<size_t,size_t,size_t>;

      /// Build the profiles and the heap for states with incoming
      /// spontaneous transitions.
      void build_heap_()
      {
        for (auto s: aut_->states())
          // We don't care about states without incoming spontaneous
          // transitions.
          if (auto in_sp = aut_->in(s, empty_word_).size())
            {
              auto out_sp = aut_->out(s, empty_word_).size();
              auto out = aut_->all_out(s).size();
              auto t = todo_.emplace(s, state_profile
                                     {out_sp, out, in_sp});
              tickets_.emplace(s, t);
          }
        todo_.heapify();
      }

      void update_heap_(state_t s)
      {
        if (3 < debug_)
          {
            std::cerr << "update heap (" << s << ") ";
          }
        auto t = tickets_.find(s);
        if (t != tickets_.end())
          todo_.update(t->second,
                       state_profile{aut_->out(s, empty_word_).size(),
                           aut_->all_out(s).size(),
                           aut_->in(s, empty_word_).size()});
      }

#ifdef STATS
      unsigned added_ = 0;
      unsigned removed_ = 0;
#endif

      /// For each state (s), for each incoming epsilon-transitions
      /// (t), if (t) is a loop, the star of its weight is computed,
      /// otherwise, (t) is stored into the closure list.  Then (t) is
      /// removed.
      ///
      /// Because it is sometimes useful to be able to invoke proper
      /// on a single state, we kept this function free from any
      /// relation with the profiles and the heap.
      ///
      /// For some reason, we get poorer performances if this function
      /// is moved higher, before the state_profile definition.
      void in_situ_remover_(state_t s)
      {
        const auto& tr = aut_->in(s, empty_word_);
        // Iterate on a copy, as we remove these transitions in the
        // loop.
        transitions_t transitions{tr.begin(), tr.end()};
        // The star of the weight of the loop on 's' (1 if no loop).
        weight_t star = ws_.one();
        using state_weight_t = std::pair<state_t, weight_t>;
        std::vector<state_weight_t> closure;
        for (auto t : transitions)
          {
            weight_t weight = aut_->weight_of(t);
            state_t src = aut_->src_of(t);
            if (src == s)  //loop
              star = ws_.star(weight);
            else
              closure.emplace_back(src, weight);
            // Delete incoming epsilon transitions.
            aut_->del_transition(t);
          }

        /*
          For each transition (t : s -- label|weight --> dst),
          for each former
          epsilon transition closure->first -- e|closure->second --> s
          a transition
          (closure->first -- label | closure->second*weight --> dst)
          is added to the automaton (add, not set !!)

          If (s) is final with weight (weight),
          for each former
          epsilon transition closure->first -- e|closure->second --> s
          pair-second * weight is added to the final weight
          of closure->first
        */
        for (auto t: aut_->all_out(s))
          {
            // "Blowing": For each transition (or terminal arrow)
            // outgoing from (s), the weight is multiplied by
            // (star).
            weight_t blow = ws_.mul(star, aut_->weight_of(t));
            aut_->set_weight(t, blow);

            label_t label = aut_->label_of(t);
            state_t dst = aut_->dst_of(t);
            for (auto pair: closure)
              {
                state_t src = pair.first;
                weight_t w = ws_.mul(pair.second, blow);
                aut_->add_transition(src, dst, label, w);
              }
          }
#ifdef STATS
        unsigned added = aut_->all_out(s).size() * closure.size();
        unsigned removed = transitions.size();
#endif
        if (prune_ && aut_->all_in(s).empty())
          {
#ifdef STATS
            removed += aut_->all_out(s).size();
#endif
            aut_->del_state(s);
          }
#ifdef STATS
        added_ += added;
        removed_ += removed;
        if (1 < debug_)
          std::cerr << " -" << removed << "+" << added
                    << " (-" << removed_ << "+" << added_ << ")";
#endif
      }

      /// Remove all the states with incoming spontaneous transitions.
      ///
      /// Set-up and maintain a heap to process states in an order
      /// that attempts to avoid useless introducing useless
      /// spontaneous transitions.
      ///
      /// Think for instance of the applying proper to
      /// thompson(a?{n}): it is much more efficient to work "from
      /// final to initial states", than the converse (which is what
      /// the "implementation order" actually does).  For n=2000, the
      /// "implementation order" takes 102s on my machine, while this
      /// order (and its implementation) takes 15.2s.
      void in_situ_remover_()
      {
        build_heap_();
        /* For each state (s), for each incoming epsilon-transitions
           (t), if (t) is a loop, the star of its weight is computed,
           otherwise, (t) is stored into the closure list.  Then (t)
           is removed.  */

        // The neighbors of s: their profiles need to be updated after
        // s was processed.
        std::unordered_set<state_t> neighbors;
        while (!todo_.empty())
          {
            auto s = todo_.pop();
            tickets_.erase(s);
            neighbors.clear();
            for (auto t: aut_->in(s))
              {
                state_t n = aut_->src_of(t);
                if (n != s)
                  neighbors.emplace(n);
              }
            for (auto t: aut_->out(s))
              {
                state_t n = aut_->dst_of(t);
                if (n != s)
                  neighbors.emplace(n);
              }

            in_situ_remover_(s);

            // Update all neighbors and then the heap.
            for (auto n: neighbors)
              update_heap_(n);
          }
      }

      /// TOPS: valid iff proper succeeds.
      template <star_status_t Status>
      static
      typename std::enable_if<Status == star_status_t::TOPS>::type
      proper_here_(automaton_t& aut, bool = true)
      {
        if (!in_situ_remover(aut))
          raise("invalid automaton");
      }

      /// ABSVAL: valid iff proper succeeds on the "absolute value" of
      /// the automaton.
      template <star_status_t Status>
      static
      typename std::enable_if<Status == star_status_t::ABSVAL>::type
      proper_here_(automaton_t& aut, bool prune = true)
      {
        require(is_valid(aut), "invalid automaton");
        in_situ_remover(aut, prune);
      }

      /// STARRABLE: always valid.
      template <star_status_t Status>
      static
      typename std::enable_if<Status == star_status_t::STARRABLE>::type
      proper_here_(automaton_t& aut, bool prune = true)
      {
        in_situ_remover(aut, prune);
      }

      /// NON_STARRABLE: valid iff there is no epsilon-circuit with
      /// weight zero.  Warning: the property tested here is the
      /// acyclicity, which is equivalent only in zero divisor free
      /// semirings.
      template <star_status_t Status>
      static
      typename std::enable_if<Status == star_status_t::NON_STARRABLE>::type
      proper_here_(automaton_t& aut, bool prune = true)
      {
        require(is_valid(aut), "invalid automaton");
        in_situ_remover(aut, prune);
      }

    private:
      /// Debug level.  The higher, the more details are reported.
      int debug_;
      /// The automaton we work on.
      automaton_t& aut_;
      /// Shorthand to the weightset.
      const weightset_t& ws_;
      /// Shorthand to the empty word.
      label_t empty_word_;

      /// Min-heap to decide the order of state-elimination.
      using heap_t=utils::min_heap<state_t,state_profile>;
      heap_t todo_;
      /// Map: state -> heap-handle.
      std::unordered_map<state_t, typename heap_t::claim_ticket_type> tickets_;

      /// Whether to prune states that become inaccessible.
      bool prune_;
    };


    /*----------------------------------------------.
    | Specialization when there is no 'one' label.  |
    `----------------------------------------------*/

    template <typename Aut>
    class properer<Aut, false>
    {
      using automaton_t = typename std::remove_cv<Aut>::type;
    public:
      static
#ifndef __clang__
      constexpr
#endif
      void proper_here(automaton_t&, bool = true)
      {}
    };

  }


  /*---------.
  | proper.  |
  `---------*/

  /// Blindly eliminate epsilon transitions without checking for the
  /// validity of the automaton.  Return true iff the process worked.
  template <typename Aut>
  inline
  bool in_situ_remover(Aut& aut, bool prune = true)
  {
    return internal::properer<Aut>::in_situ_remover(aut, prune);
  }

  /// Eliminate spontaneous transitions in place.  Raise if the
  /// automaton was not valid.
  template <typename Aut>
  inline
  void proper_here(Aut& aut, direction_t dir = BACKWARD, bool prune = true)
  {
    switch(dir) {
      case BACKWARD:
        internal::properer<Aut>::proper_here(aut, prune);
        break;
      case FORWARD:
        transpose_here(aut);
        internal::properer<Aut>::proper_here(aut, prune);
        transpose_here(aut);
    }
  }

  /// Eliminate spontaneous transitions.  Raise if the input automaton
  /// is invalid.
  template <typename Aut>
  auto
  proper(const Aut& aut, direction_t dir = BACKWARD,
         bool prune = true, bool keep_history = true)
    -> decltype(copy(aut))
  {
    auto res = aut;
    switch (dir)
    {
    case direction_t::BACKWARD:
      res = copy(aut,keep_history);
      proper_here(res, dir, prune);
      return res;
    case direction_t::FORWARD:
      res = transpose(aut,keep_history);
      proper_here(res, direction_t::BACKWARD, prune);
      transpose_here(res);
      return res;
    default:
      raise("invalid direction parameter");
    }
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_PROPER_HH
