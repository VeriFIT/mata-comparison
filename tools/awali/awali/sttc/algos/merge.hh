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

#ifndef AWALI_ALGOS_MERGE_HH
# define AWALI_ALGOS_MERGE_HH

# include <algorithm> // min_element.
# include <unordered_map>
# include <unordered_set>

#include <awali/sttc/algos/accessible.hh>
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/history/partition_history.hh>

namespace awali {
  namespace sttc {


    namespace internal
    {
      // Apply a merge onto an automaton: fuse equivalent states.
      template <typename Aut, typename StateList>
      class merger
      {
      public:
        using automaton_t = Aut;
        using class_t = unsigned;
        using class_to_set_t = std::vector<StateList>;
        using set_t = StateList;
        using state_to_class_t = std::unordered_map<state_t, class_t>;
        using class_to_state_t = std::vector<state_t>;

        // \param class_to_set  The equivalence classes.
        //             Might be modified to put the states with the
        //             smallest ID first in their class.
        merger(class_to_set_t& class_to_set, bool keep_history)
          : class_to_set_(class_to_set)
          , num_classes_(class_to_set_.size())
          , keep_history_(keep_history)
        { }

        /// Build the resulting automaton.
        automaton_t build_result_(const automaton_t& aut)
        {
          state_to_class_t state_to_class;
          for (unsigned c = 0; c < num_classes_; ++c)
            for (auto s: class_to_set_[c])
              state_to_class[s] = c;

          automaton_t res = make_shared_ptr<automaton_t>(aut->context());
          class_to_res_state_.resize(num_classes_);
          for (unsigned c = 0; c < num_classes_; ++c)
            {
              const set_t& set = class_to_set_[c];
              state_t s = *set.begin();
              if(s==aut->pre())
                class_to_res_state_[c]=res->pre();
              else if(s == aut->post())
                class_to_res_state_[c]=res->post();
              else {
                class_to_res_state_[c]=res->add_state();
                res->set_state_name(class_to_res_state_[c],
                                    aut->get_state_name(*std::min_element(begin(class_to_set_[c]),
                                                                          end(class_to_set_[c]))));
              }
            }
          for (unsigned c = 0; c < num_classes_; ++c)
            {
              // Copy the transitions of the first state of the class in
              // the result.
              state_t s = *class_to_set_[c].begin();
              state_t src = class_to_res_state_[c];
              for (auto t : aut->all_out(s))
                {
                  state_t d = aut->dst_of(t);
                  state_t dst = class_to_res_state_[state_to_class[d]];
                  res->add_transition(src, dst, aut->label_of(t), aut->weight_of(t));
                }
            }
          if(keep_history_) {
            auto history = std::make_shared<partition_history<automaton_t>>(aut);
            res->set_history(history);
            for (unsigned c = 0; c < num_classes_; ++c){
              std::set<state_t> from;
              for(auto s : class_to_set_[c])
                from.emplace(s);
              history->add_state(class_to_res_state_[c], from);
            }
          }
          return res;
        }

        /// The minimized automaton.
        automaton_t operator()(const automaton_t& aut)
        {
          return build_result_(aut);
        }

      private:
        class_to_set_t& class_to_set_;
        unsigned num_classes_;
        class_to_state_t class_to_res_state_;
        bool keep_history_;
      };
    } // internal::

    template <typename Aut, typename StateList>
    inline
    auto
    merge(const Aut& a, std::vector<StateList>& classes, bool keep_history=true)
      -> Aut
    {
      internal::merger<Aut, StateList> merge(classes, keep_history);
      return merge(a);
    }

  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_MERGE_HH
