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

#ifndef AWALI_ALGOS_TRANSPOSE_HH
# define AWALI_ALGOS_TRANSPOSE_HH

#include <awali/sttc/core/automaton_decorator.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/core/rat/ratexpset.hh>
#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/misc/attributes.hh>

namespace awali { namespace sttc {


  /*-----------------------.
  | transpose(automaton).  |
  `-----------------------*/
  namespace internal
  {

    template <typename Aut>
        struct trans_tuple {
      using automaton_t = Aut;
      using label_t = label_t_of<automaton_t>;
      using weight_t = weight_t_of<automaton_t>;

          state_t src;
          state_t dst;
          label_t label;
          weight_t weight;

          trans_tuple(const Aut& aut, transition_t& t) :
          src(aut->src_of(t)), dst(aut->dst_of(t)), label(aut->label_of(t)), weight(aut->weight_of(t)) {}

        };
  }

    template <typename Aut>
        void transpose_here(Aut& aut) {
      //using automaton_t = Aut;

          std::vector<transition_t> oldtr;
          std::vector<internal::trans_tuple<Aut>> trs;


          for(auto t : aut->all_transitions())
          {
                trs.emplace_back(internal::trans_tuple<Aut>(aut, t));
                oldtr.emplace_back(t);
          }

          for(auto t :oldtr)
                aut->del_transition(t);
          for(auto t : trs){
                if(t.src==aut->pre())
                        t.src=aut->post();
                if(t.dst==aut->post())
                        t.dst=aut->pre();
                aut->add_transition(t.dst, t.src, aut->labelset()->transpose(t.label), aut->weightset()->transpose(t.weight));
          }
        }

    template <typename Aut,
                typename AutOut = typename Aut::element_type::automaton_nocv_t>
    AutOut transpose(Aut& aut, bool keep_history=true) {
      AutOut out = copy(aut, keep_history);
       transpose_here(out);
       if(aut->get_name().empty()) {
         out->set_name("transpose");
         out->set_desc("Automaton obtained by a transposition");
       }
       else {
         out->set_name("transpose-"+aut->get_name());
         out->set_desc("Transposition of "+aut->get_name());
       }
       return out;
    }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_TRANSPOSE_HH
