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

#ifndef AWALI_ALGOS_PARTIAL_IDENTITY_HH
# define AWALI_ALGOS_PARTIAL_IDENTITY_HH

# include <unordered_map>

#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/misc/set.hh>
#include <awali/sttc/labelset/tupleset.hh>
#include <awali/sttc/misc/sub_tuple.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/history/single_history.hh>

namespace awali { namespace sttc {

  /*--------------------------------------------.
  | partial identity (automaton -> transducer). |
  `--------------------------------------------*/

  namespace internal
  {
    template <size_t N, typename Labelset>
    struct multitupleset {
      using type=typename concat_tupleset
        <tupleset<Labelset>,
         typename multitupleset<N-1,Labelset>::type>::type;
    };

    template <size_t N>
    struct multituple {
      template<typename T>
      using type = typename std::cons_tuple<T, typename multituple<N-1>::template type<T>>::type;

      template<typename T>
      static
      auto get(const T& v) -> type<T> {
        return std::tuple_cat(std::make_tuple(v),multituple<N-1>::get(v));
      }
    };

    template <typename Labelset>
    struct multitupleset<1u, Labelset> {
      using type=tupleset<Labelset>;
    };

    template <>
    struct multituple<1u> {
      template<typename T>
      using type = std::tuple<T>;

      template<typename T>
      static
      auto get(const T& v) -> type<T> {
        return std::make_tuple(v);
      }
    };

    template <typename Aut, size_t I>
    struct partial_identiter
    {
      using in_context_t = context_t_of<Aut>;
      using in_labelset_t = labelset_t_of<in_context_t>;
      using weightset_t = weightset_t_of<in_context_t>;
      using out_labelset_t =  typename multitupleset<I,in_labelset_t>::type;
      using out_context_t = context<out_labelset_t, weightset_t>;
      using out_automaton_t = mutable_automaton<out_context_t>;

      partial_identiter(const Aut& in)
        : in_(in)
      {
        in_context_t in_context = in->context();
        auto in_labelset = in_context.labelset();
        auto weightset = in_context.weightset();
        out_labelset_t out_labelset{multituple<I>::get(*in_labelset)};
        out_context_t out_context{out_labelset, *weightset};
        out_ = make_mutable_automaton(out_context);

      }

      void operator()()
      {
        // Copy the states.  We cannot iterate on the transitions
        // only, as we would lose the states without transitions.
        for (auto s: in_->states()) {
	  out_state[s] = s;
	  out_->add_state(s);
	  if(in_->has_name(s))
	    out_->set_state_name(s, in_->get_state_name(s));	     
	}
        out_state[in_->pre()]= out_->pre();
        out_state[in_->post()]= out_->post();

        for (auto t : in_->all_transitions())
          {
            auto src = out_state.find(in_->src_of(t));
            auto dst = out_state.find(in_->dst_of(t));
            if (src != out_state.end() && dst != out_state.end()){
              out_->new_transition(src->second, dst->second,
                                   multituple<I>::get(in_->label_of(t)), in_->weight_of(t));
            }
          }
      }

      void set_history() {
        auto history = std::make_shared<single_history<Aut>>(in_);
        out_->set_history(history);
        for (auto p: in_->all_states()) {
            history->add_state(out_state[p], p);
	    if(in_->has_name(p)) {
	      out_->set_state_name(out_state[p], in_->get_state_name(p));
	    }
	}
      }

      /// Input automaton.
      const Aut& in_;
      /// Output automaton.
      mutable_automaton<out_context_t> out_;
      /// input state -> output state.
      std::unordered_map<state_t, state_t> out_state;
    };
  }

    /** Builds a transducer realizing the identity on a language
     *
     * @tparam I the number of tapes of the result
     * @tparam Aut the type of the automaton
     * @param in the automaton
     * @param keep_history if true the result is endowed with a {@link single_history} to the input automaton
     * @return a transducer with \p I tapes realizing the identity on the language of \a aut
     */
  template <size_t I, typename Aut>
  inline
  auto
  partial_identity(const Aut& in, bool keep_history=true) -> typename internal::partial_identiter<Aut, I>::out_automaton_t
  {
    internal::partial_identiter<Aut, I> id(in);
    id();
    if(keep_history)
      id.set_history();
    return id.out_;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_PARTIAL_IDENTITY_HH
