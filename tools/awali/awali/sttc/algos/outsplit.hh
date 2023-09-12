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

#ifndef AWALI_ALGOS_OUTSPLIT_HH
# define AWALI_ALGOS_OUTSPLIT_HH

# include <unordered_set>

#include <awali/sttc/labelset/tupleset.hh>
#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/misc/pair.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/algos/sort.hh>

namespace awali { namespace sttc {

  namespace internal
  {
    //In place outsplitter
    template <typename Aut>
    class outsplitter
    {
      using automaton_t = Aut;
      using label_t = label_t_of<automaton_t>;

    public:
      //Pred :  Aut, transition_t -> bool
      template <typename Pred>
      void operator()(Aut& aut, const Pred& pred)
      {
        std::unordered_set<state_t> add_states;
        for (auto st : aut->states())
        {
          if(add_states.find(st)!=add_states.end())
            continue;
          bool true_out = false;

          std::vector<transition_t> false_transitions;
          for (auto tr : aut->all_out(st))
          {
            if(pred(aut,tr))
              true_out = true;
            else
              false_transitions.emplace_back(tr);
          }
          if(true_out && !false_transitions.empty()) {
            state_t ns=aut->add_state();
            aut->set_state_name(ns, aut->get_state_name(st)+"'");
            add_states.emplace(ns);
            for(auto tr : false_transitions) {
              aut->set_transition(ns, aut->dst_of(tr),
                                  aut->label_of(tr), aut->weight_of(tr));
              aut->del_transition(tr);
            }
            for(auto tr : aut->all_in(st)) {
              aut->set_transition(aut->src_of(tr),ns,
                                  aut->label_of(tr), aut->weight_of(tr));
            }
          }
        }
      }

    };

    template <typename Labelset,size_t I> struct select_one;
    template <typename... T,size_t I>
    struct select_one<tupleset<T...>,I>{
      using tp_t = tupleset<T...>;

        static constexpr
        bool has_one() {
        return std::tuple_element<I,std::tuple<T...>>::type::has_one();
      }

        template<typename LS, typename Tuple>
        static
        bool get(LS ts, Tuple tu) {
        return std::get<I>(ts->sets()).is_one(std::get<I>(tu));
      }
    };
  } // namespace internal


  template<size_t I, typename Aut>
  typename std::enable_if<internal::select_one<labelset_t_of<Aut>,I>::has_one(), void>::type
  outsplit_here(Aut& aut)
  {
    internal::outsplitter<Aut> split;
    auto ls=aut->context().labelset();
    split(aut, [ls](const Aut& aut_, transition_t tr) {
        return internal::select_one<labelset_t_of<Aut>,I>::get(ls,aut_->label_of(tr));
      });
  }

  template<size_t I, typename Aut>
  typename std::enable_if<!internal::select_one<labelset_t_of<Aut>,I>::has_one(), void>::type
  outsplit_here(Aut& aut)
  {}

  template <size_t I, typename Aut>
  inline
  Aut
  outsplit(const Aut& aut, bool keep_history=true)
  {
    auto a = copy(aut, keep_history);
    outsplit_here<I>(a);
    sort_tape<I>(a);
    return a;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_OUTSPLIT_HH
