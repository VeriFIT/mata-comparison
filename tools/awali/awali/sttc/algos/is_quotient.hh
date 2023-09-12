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


#ifndef AWALI_ALGOS_IS_QUOTIENT_HH
# define AWALI_ALGOS_IS_QUOTIENT_HH

# include <list>
# include <vector>
# include <map>
# include <set>
#include <awali/sttc/algos/accessible.hh>
namespace awali { namespace sttc {


  namespace internal
  {
    template <typename Aut1, typename Aut2>
    class is_quotient_computer_t
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

        using pair_t = std::pair<state_t, state_t>;

        const automaton1_t& a1_;
        const automaton2_t& a2_;

        std::map<state_t, state_t> morphism;
        std::vector<bool> visited;

        bool visit(state_t s) {
          return true;
        }
//           if (visited[s])
//             return true;
//           state1_t s_im= morphism[s];
//           visited[s] = true;
// //           std::vector<transition2_t> out_trs = a2_->out();
//           std::set<label2_t> out_labels;
//           for (transition2_t tr_id: out_trs){
//             label2_t label= a2_->label_of(tr_id);
//             out_labels.insert(label);
//           }
//
//           bool ret_val= visit_aux(s, s_im, out_labels.cbegin());
//           visited[s] = ret_val;
//           return ret_val;
//
//
//         bool visit_aux(state2_t s, state1_t s_im, typename std::set<label2_t>::iterator &label_it)
//         {
//
//         }

      public:
        is_quotient_computer_t(const automaton1_t &a1, const automaton2_t &a2)
        : a1_(a1)
        , a2_(a2)
        {
          visited.resize(a2_->num_states()+2, false);
          morphism.insert(pair_t(a2_->pre(),a1_->pre()));
          morphism.insert(pair_t(a2_->post(),a1_->post()));
        }

        bool operator()()
        {
          return visit(a2_->pre());
        }
    };

  }//end of namespace internal

  template <typename Aut1, typename Aut2>
  bool
  is_quotient(const Aut1& a1, const Aut2& a2)
  {
    internal::is_quotient_computer_t<Aut1, Aut2> computer(a1, a2);
    return computer();
  }

}}//end of ns awali::stc

#endif //AWALI_ALGOS_IS_QUOTIENT_HH
