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

#ifndef AWALI_ALGOS_ENUMERATE_HH
# define AWALI_ALGOS_ENUMERATE_HH

# include <algorithm>
# include <iostream>
# include <list>
# include <map>
# include <queue>
# include <vector>

#include <awali/sttc/ctx/context.hh>
//#include <awali/sttc/labelset/labelset.hh>
#include <awali/sttc/weightset/polynomialset.hh>
#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/algos/accessible.hh>

namespace awali { namespace sttc {



  /*-----------------------.
  | enumerate(automaton).  |
  `-----------------------*/

  namespace internal
  {
    template <typename Aut>
    class enumerater
    {
    public:
      using automaton_t = Aut;
      using context_t = context_t_of<Aut>;
      static_assert(context_t::labelset_t::is_free(),
                    "requires free labelset");

      using labelset_t = labelset_t_of<automaton_t>;
      using weightset_t = weightset_t_of<automaton_t>;
      using wordset_t = typename labelset_trait<labelset_t>::wordset_t;
      using wordset_context_t = context<wordset_t,weightset_t>;
      using polynomialset_t = polynomialset<wordset_context_t>;
      using polynomial_t = typename polynomialset_t::value_t;
      using label_t = label_t_of<automaton_t>;
      using weight_t = weight_t_of<automaton_t>;
      using genset_t = typename labelset_t_of<automaton_t>::genset_t;
      using word_t = typename labelset_t::word_t;

      /// Same as polynomial_t::value_type.
      using monomial_t = std::pair<word_t, weight_t>;
      using queue_t = std::list<std::pair<state_t, monomial_t>>;

      enumerater(const automaton_t& aut)
        : aut_(aut) {}

      /// The weighted accepted word with length at most \a max.
      polynomial_t enumerate(unsigned max)
      {
        if(is_empty(trim(aut_)))
          return ps_.zero();
        queue_t queue;
        queue.emplace_back(aut_->pre(), ps_.monomial_one());

        // We match words that include the initial and final special
        // characters.
        max += 2;
        for (unsigned i = 0; i < max && !queue.empty(); ++i)
          propagate_(queue);

        polynomial_t res;
        // Return the past of post(), but remove the initial and final
        // special characters for the words.
        for (const auto& m: achieved_)
          ps_.add_here(res, ls_.undelimit(m.first), m.second);
        return res;
      }

      /// The shortest accepted weighted words, or throw an exception.
      // FIXME: code duplication.
      polynomial_t shortest(unsigned num)
      {
        if(is_empty(trim(aut_)))
          return ps_.zero();
        queue_t queue;
        queue.emplace_back(aut_->pre(), ps_.monomial_one());

        polynomial_t res;
        while (achieved_.size() < num && !queue.empty())
          propagate_(queue);

        // Return the past of post(), but remove the initial and final
        // special characters for the words.
        for (const auto& m: achieved_)
          {
            ps_.add_here(res, ls_.undelimit(m.first), m.second);
            if (--num == 0)
              break;
          }
        return res;
      }

    private:
      /// Process once all the states of \a q1.
      /// Save into q1 the new states to visit.
      void propagate_(queue_t& q1)
      {
        queue_t q2;
        while (!q1.empty())
          {
            state_t s;
            monomial_t m;
            tie(s, m) = std::move(q1.front());
            q1.pop_front();
            for (const auto t: aut_->all_out(s))
              {
                // FIXME: monomial mul.
                monomial_t n(ls_.concat(m.first, aut_->label_of(t)),
                             ws_.mul(m.second, aut_->weight_of(t)));
		if(aut_->dst_of(t) == aut_->post())
		  ps_.add_here(achieved_, n);
                q2.emplace_back(aut_->dst_of(t), n);
              }
          }
        q1.swap(q2);
      }

      const automaton_t& aut_;
      const weightset_t& ws_ = *aut_->weightset();
      const polynomialset_t ps_{get_wordset_context(aut_->context())};
      const labelset_t_of<polynomialset_t>& ls_ = *ps_.labelset();
      /// For each state, the first orders of its past.
      polynomial_t achieved_;
    };
  }

    /** Gives the weight associated with each word shorter than \p max_length by \p aut.
     *
     * @tparam Automaton type
     * @param aut the automaton
     * @param max_length the maximal length of words
     * @return a polynomial which is a map from words to weights.
     * @pre The labelset of the automaton \p aut must be free.
     */
  template <typename Automaton>
  inline
  typename internal::enumerater<Automaton>::polynomial_t
  enumerate(const Automaton& aut, unsigned max_length)
  {
    internal::enumerater<Automaton> enumerater(aut);
    return enumerater.enumerate(max_length);
  }

    /** Computes the weight of the \p num smallest words accepted by the automaton.
     *
     * If less than \p num words are accepted by the automaton \p aut,
     * the map is partially filled. In particular, if there is no
     * accepted word, the map is empty.
     *
     * @tparam Automaton
     * @param aut
     * @param num
     * @return a polynomial which is a map from words to weights.
     * @pre The labelset of the automaton \p aut must be free. 
     */
  template <typename Automaton>
  inline
  typename internal::enumerater<Automaton>::polynomial_t
  shortest(const Automaton& aut, unsigned num)
  {
    internal::enumerater<Automaton> enumerater(aut);
    return enumerater.shortest(num);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_ENUMERATE_HH
