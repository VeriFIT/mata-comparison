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

#ifndef STTC_ALGOS_FACTOR_HH
#define STTC_ALGOS_FACTOR_HH

#include <vector>
#include <unordered_map>

#include <awali/sttc/history/partition_history.hh>

namespace awali { namespace sttc {

  namespace internal {

    template <typename Aut>
    class tarjaner_t {

      using automaton_t = Aut;
      using weight_t = weight_t_of<automaton_t>;
      using label_t = label_t_of<automaton_t>;
      using context_t= context_t_of<automaton_t>;


      using result_t = std::pair<
            std::unordered_map<state_t, unsigned int>, // state -> scc
            std::vector<std::vector<state_t>> >;       // scc -> state

      typedef struct {
        state_t id;
        unsigned int index;
        unsigned int current_root;
        bool onstack;
      } tarjan_state_t;


      typedef struct {
        std::stack<state_t> stack;
        std::unordered_map<state_t, tarjan_state_t*> map; /* Coloring of visited states */
        std::unordered_map<state_t, unsigned int> scc_of;
        std::vector<std::vector<state_t>> states_of;
        unsigned int cur_num = 0; /* Number of visited states */
        unsigned int cur_scc = 2; /* Number of created scc.
                            Starts at 2 in order to reserve 0 and 1 for
                            the scc of subliminal states in condensation */
      } tarjan_data_t;

      automaton_t aut;
      tarjan_data_t data;



      std::vector<state_t> successors (automaton_t aut, state_t stt) {
        std::vector<state_t> res;
        for (transition_t tr : aut->out(stt))
          res.push_back(aut->dst_of(tr));
        return res;
      }

    public:
      void
      visit_recursive (const automaton_t& aut, state_t stt)
      {
        tarjan_state_t* cur = new tarjan_state_t
        { stt, data.cur_num, data.cur_num, true };
        data.map.insert(std::pair<state_t, tarjan_state_t*>{stt,cur});
        data.stack.push(stt);
        data.cur_num++;

        for (state_t succ : successors(aut, stt)) {
          auto it= data.map.find(succ);
          if (it == data.map.end()) {
            visit_recursive (aut, succ);
            it= data.map.find(succ);
          }
          if (it->second->onstack)
            cur->current_root = utils::min(cur->current_root,
                                           it->second->current_root);
          // else do nothing: successor was already put on stack and removed
        }

        if (cur->index == cur->current_root) {
          state_t top;
          data.states_of.push_back({});
          std::vector<state_t>& back = data.states_of.back();
          do {
            top= data.stack.top();
            data.stack.pop();
            data.map[top]->onstack= false;
            back.push_back(top);
            data.scc_of[top]= data.cur_scc;
          } while (top != cur->id);
          data.cur_scc++;
        }

      }

      void
      compute_sccs_recursive ()
      {
        for(state_t stt : aut->states())
          if (data.map.find(stt) == data.map.end())
            visit_recursive(aut, stt);
      }


      typedef struct {
        tarjan_state_t* src;
        std::vector<state_t> dst_to_treat;
        long unsigned int remaining;
        tarjan_state_t* caller;
      } token_t ;

      inline
      token_t make_token(tarjan_state_t* t, std::vector<state_t> v,
                         tarjan_state_t* c = nullptr)
      {
        return token_t{t, v, v.size(), c};
      }



      void
      visit_iterative (state_t stt)
      {

        if (data.map.find(stt) != data.map.end())
          return;
        std::stack<token_t> tstack; /* Stack of what would be recursive calls */
        {
          tarjan_state_t* t_stt= new tarjan_state_t
                                 { stt, data.cur_num, data.cur_num, true };
          data.cur_num++;
          data.map.insert(std::pair<state_t, tarjan_state_t*>(stt, t_stt));
          tstack.push( make_token(t_stt, std::move(successors(aut, stt))) );
          data.stack.push(stt);
        }
        while (!tstack.empty())
        {
          token_t& token= tstack.top();
          //           std::cerr << "Treating " << (token.src->id-2)  << "; "
          //                     << token.remaining << " successors remains."
          //                     <<  std::endl;
          if ( token.remaining > 0)
          {
            state_t dst= token.dst_to_treat[--token.remaining];
            auto it = data.map.find(dst);
            if ( it == data.map.end()) {
              //                 std::cerr << "\tPushing " << (dst-2)  << "." <<  std::endl;
              tarjan_state_t* t_dst=
              new tarjan_state_t{ dst, data.cur_num, data.cur_num, true };
              data.cur_num++;
              data.map.insert(std::pair<state_t, tarjan_state_t*>(dst, t_dst));
              tstack.push( make_token(t_dst, std::move(successors(aut, dst)), token.src) );
              data.stack.push(dst);
            } else if ( it->second->onstack ) {
              token.src->current_root = utils::min(token.src->current_root,
                                                    it->second->current_root);
              //                 std::cerr << "\t\tNew root: " << token.src->current_root << "." << std::endl;
            }
          }
          else
          {
            if (token.caller != nullptr) {
              token.caller->current_root =
              utils::min( token.caller->current_root,
                          token.src->current_root );
            }
            if (token.src->index == token.src->current_root)
            {
              data.states_of.push_back({});
              std::vector<state_t>& back = data.states_of.back();
              state_t bot = token.src->id;
              state_t top;
              do {
                top= data.stack.top();
                data.stack.pop();
                data.map[top]->onstack= false;
                back.push_back(top);
                data.scc_of[top]= data.cur_scc;
              } while (top != bot);
              data.cur_scc++;
            }
            tstack.pop();
          }
        }
      }

      void
      compute_sccs_iterative ()
      {
        for(state_t stt : aut->states())
          visit_iterative(stt);
      }


      //This function should not be called before iterative or recursive
      void
      add_subliminal_sccs ()
      {
        data.scc_of[0]=0;
        data.scc_of[1]=1;

        auto it = data.states_of.begin();
        data.states_of.insert(it,{1});

        it = data.states_of.begin();
        data.states_of.insert(it,{0});
      }


      //This function should not be called before iterative or recursive
      //and assumes subliminal sccs/states are properly mapped.
      automaton_t
      condensation ()
      {
        automaton_t out= make_mutable_automaton<context_t>(aut->context());
        auto history = std::make_shared<partition_history<automaton_t>>(aut);
        out->set_history(history);
        unsigned int max= data.states_of.size();
        for (unsigned int i=2; i<max; i++) {
          out->add_state();
        }
        unsigned int i = 0;
        for (std::vector<state_t> scc : data.states_of) {
          std::set<state_t> hist;
          hist.insert(scc.begin(), scc.end());
          history->add_state(i, std::move(hist));
          i++;
        }

        for (auto it = data.scc_of.cbegin(); it != data.scc_of.cend() ; it++) {
          for (transition_t t: aut->all_out(it->first)) {
            state_t dest = aut->dst_of(t);
            weight_t w= aut->weight_of(t);
            label_t l= aut->label_of(t);
            out->add_transition(it->second, data.scc_of[dest], l, w);
          }
        }
        return out;
      }


      //Once this function is called, the object tarjaner_t should not be
      //used any longer
      result_t
      get_result ()
      {
        return { std::move(data.scc_of), std::move(data.states_of) };
      }



      tarjaner_t(automaton_t a) : aut(a)
      {
        size_t n=aut->num_states()+2;
        data.map.reserve(n);
        data.scc_of.reserve(n);
      }

      ~tarjaner_t()
      {
        for (auto it= data.map.cbegin(); it != data.map.cend(); it++)
          delete (it->second);
      }

    };// end of class tarjaner_t


  }

    /** Recursive computation of strongly connected components
     *
     *
     * @tparam Aut the type of the automaton
     * @param aut the automaton
     * @return a pair where the first component is a map from states into strongly connected components, and the second component gives, for each scc, the list of its states.
     */

  template <typename Aut>
  std::pair< std::unordered_map<state_t, unsigned int>,
             std::vector<std::vector<state_t>> >
  scc_recursive (Aut aut)
  {
    internal::tarjaner_t<Aut> tarjaner(aut);
    tarjaner.compute_sccs_recursive();
    return tarjaner.get_result();
  }


    /** Iterative computation of strongly connected components
     *
     *
     * @tparam Aut the type of the automaton
     * @param aut the automaton
     * @return a pair where the first component is a map from states into strongly connected components, and the second component gives, for each scc, the list of its states.
     */
  template <typename Aut>
  std::pair< std::unordered_map<state_t, unsigned int>,
             std::vector<std::vector<state_t>> >
  scc_iterative (Aut aut)
  {
    internal::tarjaner_t<Aut> tarjaner(aut);
    tarjaner.compute_sccs_iterative();
    return tarjaner.get_result();
  }


    /** Condense each strongly connected component to a state
     *
     * A new automaton is created, where every state corresponds to a
     * scc of the input \p aut.     
     * Let s(p) be the state of the result corresponding to the scc of 
     * the state p of \p aut; for every transition from p to q in
     * \p aut,
     * a transition with the same label and the same weight, from s(p)
     * to s(q) is added to the result.    
     * In particular, the initial (resp. final) weight of s(p) is the
     * sum of the initial (resp. final) weights of the states of the
     * scc of p.
     *
     * @tparam Aut the type of the automaton
     * @param aut the automaton
     * @return a pair where the first component is the condensed automaton, and the second one is a pair
where the first component is a map from states into strongly connected components, and the second component gives, for each scc, the list of its states.
     */
  template <typename Aut>
  std::pair<  Aut, std::pair< std::unordered_map<state_t, unsigned int>,
                              std::vector<std::vector<state_t>> >  >
  condensation (Aut aut)
  {
    internal::tarjaner_t<Aut> tarjaner(aut);
    tarjaner.compute_sccs_iterative();
    tarjaner.add_subliminal_sccs();
    Aut out = tarjaner.condensation();
    return {out, tarjaner.get_result()};
  }

    /** Computes the strongly connected component of a state
     *
     * @tparam Aut the type of the automaton
     * @param aut the automaton
     * @param s a state of \p aut
     * @return a vector containing all states in the same scc as \p s
     */
  template <typename Aut>
  std::vector<state_t>
  scc_of (Aut aut, state_t s)
  {
    internal::tarjaner_t<Aut> tarjaner(aut);
    tarjaner.visit_iterative(s);
    auto res= tarjaner.get_result();
    return std::move(res.second[res.second.size()-1]);
  }


}}// end of ns awali::sttc

#endif
