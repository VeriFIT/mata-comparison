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

#ifndef AWALI_ALGOS_LAL_LAN_CONVERSION_HH
# define AWALI_ALGOS_LAL_LAN_CONVERSION_HH

#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/labelset/traits.hh>

namespace awali {
  namespace sttc {
    namespace internal {
      template<typename Aut, typename L> struct dispatch_lal_lan{
        using l_automaton_t = Aut;
        using n_automaton_t = Aut;
        static const Aut proper_(const Aut& aut, direction_t dir, bool prune, bool keep_history) {
          raise("to_lal: ","unsupported operation");
        }
        
        static Aut allow_eps_(const Aut&  aut, bool keep_history) {
          raise("to_lan: ","unsupported operation");
        }
      };

      template<typename Aut, typename T>
      struct dispatch_lal_lan<Aut,letterset<T>> {
        using nullable_t = typename labelset_trait<letterset<T>>::nullable_t;
        using n_context_t = sttc::context<nullable_t, weightset_t_of<Aut>>;
        using l_automaton_t = Aut;
        using n_automaton_t = mutable_automaton<n_context_t>;
        
        static const Aut proper_(const Aut& aut, direction_t dir, bool prune, bool keep_history) {
          return copy(aut, keep_history);
        }
        
        static n_automaton_t allow_eps_(const Aut&  aut, bool keep_history) {
          auto res = sttc::make_mutable_automaton<n_context_t>(sttc::get_nullable_context(aut->context()));
          sttc::copy_into(aut,res, keep_history);
          return res;
        }
      };
      
      template<typename Aut, typename T>
      struct dispatch_lal_lan<Aut,nullableset<T>> {
        using letterset_t = typename labelset_trait<nullableset<T>>::not_nullable_t;
        using l_context_t = sttc::context<letterset_t, weightset_t_of<Aut>>;
        using l_automaton_t = mutable_automaton<l_context_t>;
        using n_automaton_t = Aut;
        
        static l_automaton_t proper_(const Aut& aut, direction_t dir, bool prune, bool keep_history) {
          auto tmp = sttc::proper(aut, dir, prune, keep_history);
          auto res = sttc::make_mutable_automaton<l_context_t>(sttc::get_not_nullable_context(aut->context()));
          sttc::copy_into(tmp,res, keep_history);
          if(keep_history) {
            auto final_history = std::make_shared<single_history<n_automaton_t>>(aut);
            auto & tmp_history = tmp->history()->template as<single_history<n_automaton_t>>();
            auto & res_history = res->history()->template as<single_history<n_automaton_t>>();
            for(auto s: res->states())
              final_history->add_state(s,tmp_history.get_state(res_history.get_state(s)));
            tmp->strip_history();
            res->set_history(final_history);
          }
          return res;
        }
        
        static const Aut allow_eps_(const Aut& aut, bool keep_history) {
          return copy(aut, keep_history);
        }
      };
    }
    
    template<typename Aut>
    auto
    to_lal(const Aut& aut, direction_t dir = BACKWARD,
           bool prune = true, bool keep_history=true) -> typename internal::dispatch_lal_lan<Aut,labelset_t_of<Aut>>::l_automaton_t {
      return internal::dispatch_lal_lan<Aut,labelset_t_of<Aut>>::proper_(aut, dir, prune, keep_history);
    }
                                                                      
    template<typename Aut>
    auto
    to_lan(const Aut& aut, bool keep_history=true) -> typename internal::dispatch_lal_lan<Aut,labelset_t_of<Aut>>::n_automaton_t {
      return internal::dispatch_lal_lan<Aut,labelset_t_of<Aut>>::allow_eps_(aut, keep_history);
    }
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_LAL_LAN_CONVERSION_HH
