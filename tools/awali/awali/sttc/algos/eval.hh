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

#ifndef AWALI_ALGOS_EVAL_HH
# define AWALI_ALGOS_EVAL_HH

# include <algorithm>
# include <vector>

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/labelset/traits.hh>

namespace awali { namespace sttc {

  namespace internal
  {
    template <typename Aut>
    class evaluator
    {
      static_assert(labelset_t_of<Aut>::is_free(),
                    "requires free labelset");

      using automaton_t = Aut;
      using word_t = typename labelset_trait<labelset_t_of<automaton_t>>::wordset_t::word_t;
      using weightset_t = weightset_t_of<automaton_t>;
      using weight_t = typename weightset_t::value_t;
      using labelset_t = labelset_t_of<automaton_t>;

      // state -> weight.
      using weights_t = std::vector<weight_t>;

    public:
      evaluator(const automaton_t& a)
        : a_(a)
        , ws_(*a_->weightset())
        , ls_(*a_->labelset())
      {}

      void check(const word_t& word) const {
        for(auto l : word)
          if(!ls_.genset().has(l))
            throw std::invalid_argument("The word contains some unexpected letters");
      }

      weight_t operator()(const word_t& word) const
      {
        // Initialization.
        const weight_t zero = ws_.zero();
        // FIXME: a perfect job for a sparse array: most of the states
        // will be not visited, nevertheless, because we iterate on
        // all the states, they are costly at each iteration.

        /// An array indexed by state numbers.
        const auto& states = a_->states();
        unsigned last_state = *std::max_element(std::begin(states),
                                              std::end(states));
        // Do not use braces (v1{size, zero}): the type of zero might
        // result in the compiler believing we are building a vector
        // with two values: a_->num_all_states() and zero.
        weights_t v1(last_state + 1, zero);
        v1[a_->pre()] = ws_.one();
        weights_t v2{v1};

        // Computation.
// Failed attempt to refuse words written with letters absent from the labelset (VM)
//         for (auto l : ls.letters_of(ls.delimit(word))) {
//           if (!ls.has(l)) {
//             throw std::runtime_error("Given word is not written with the correct labelset.");
//           }
//         }
        auto lsw = get_wordset(ls_);
        for (auto l : lsw.delimit(word))
          {
            v2.assign(v2.size(), zero);
            for (unsigned s = 0; s < v1.size(); ++s)
              if (!ws_.is_zero(v1[s])) // delete if bench >
                for (auto t : a_->out(s, l))
                  // Introducing a reference to v2[a_->dst_of(tr)] is
                  // tempting, but won't work for std::vector<bool>.
                  // FIXME: Specialize for Boolean?
                  v2[a_->dst_of(t)] =
                    ws_.add(v2[a_->dst_of(t)],
                            ws_.mul(v1[s], a_->weight_of(t)));
            std::swap(v1, v2);
          }
        return v1[a_->post()];
      }
    private:
      const automaton_t& a_;
      const weightset_t& ws_;
      const labelset_t& ls_;
    };

  } // namespace internal

  template <typename Aut>
  inline
  auto
  eval(const Aut& a, const typename labelset_trait<labelset_t_of<Aut>>::wordset_t::word_t& w, bool check_alphabet=true)
    -> weight_t_of<Aut>
  {
    internal::evaluator<Aut> e(a);
    if(check_alphabet)
      e.check(w);
    return e(w);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_EVAL_HH
