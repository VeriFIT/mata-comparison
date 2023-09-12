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

#ifndef AWALI_ALGOS_IS_VALID_HH
# define AWALI_ALGOS_IS_VALID_HH

# include <stdexcept>
# include <type_traits>
# include <unordered_map>
# include <utility>
# include <vector>

#include <awali/sttc/algos/constant_term.hh>
#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/is_eps_acyclic.hh>
#include <awali/sttc/algos/is_proper.hh>
#include <awali/sttc/algos/proper.hh>
#include <awali/sttc/core/kind.hh>
#include <awali/common/enums.hh>

namespace awali {
  namespace sttc {
    
    
    /*,----------------.
      | is_valid(aut). |
      `----------------'*/

    namespace internal {
      
      /// Copy of \a aut, with absolute values.
      /// Templated to avoid useless instantiations.
      template <typename Aut>
      typename Aut::element_type::automaton_nocv_t
      absval(const Aut& aut)
      {
        auto res = copy(aut);
        // Apply absolute value to the weight of each transition.
        const auto& ws = *aut->weightset();
        for (auto t: res->transitions())
          res->set_weight(t, ws.abs(res->weight_of(t)));
        return res;
      }
      
      /// Whether proper_here(aut) succeeds.
      /// Destroys aut.
      template <typename Aut>
      bool is_properable(Aut&& aut)
      {
        return in_situ_remover(aut, false);
      }
      

      template <typename Aut, bool has_one = context_t_of<Aut>::has_one()>
      class is_valider {
      public:
        using automaton_t = typename std::remove_cv<Aut>::type;
        using weightset_t = weightset_t_of<automaton_t>;
        
        static bool is_valid(const automaton_t& aut) {
          return is_valid_<weightset_t::star_status()>(aut);
        }
        
      private:
        template <star_status_t Status>
        static
        typename std::enable_if<Status == star_status_t::TOPS,
                                bool>::type
        is_valid_(const automaton_t& aut) {
          return (is_proper(aut)
                  || is_eps_acyclic(aut)
                  || is_properable(copy(aut)));
        }

        template <star_status_t Status>
        static
        typename std::enable_if<Status == star_status_t::ABSVAL,
                                bool>::type
        is_valid_(const automaton_t& aut) {
          return (is_proper(aut)
                  || is_eps_acyclic(aut)
                  || is_properable(absval(aut)));
        }

        template <star_status_t Status>
        static
        typename std::enable_if<Status == star_status_t::STARRABLE,
                                bool>::type
        is_valid_(const automaton_t&) {
          return true;
        }
        
        template <star_status_t Status>
        static
        typename std::enable_if<Status == star_status_t::NON_STARRABLE,
                                bool>::type
        is_valid_(const automaton_t& aut) {
          return is_proper(aut) || is_eps_acyclic(aut);
        }
      };

      template <typename Aut>
      class is_valider<Aut, false> {
        using automaton_t = typename std::remove_cv<Aut>::type;
      public:
        static constexpr bool is_valid(const automaton_t&)
        {
          return true;
        }
      };
      
    }


    /** @brief Tests if \p aut is valid.
     *
     * This function tests if the automaton \p aut is *valid*.
     * An automaton is valid if, for every input, the sum of weights
     * of accepting paths labeled by this input is defined.
     *
     * Automata without epsilon-transitions are always valid.
     * An automaton where the number of accepting paths is finite 
     * for every input is valid; 
     * thus if the subgraph of epsilon-transitions is acyclic, 
     * the automaton is valid.
     *
     * If the subgraph of epsilon-transitions has cycles, 
     * the behaviour depends on the nature of the weightset: 
     *
     * -- starrable: the star is defined for every element, 
     *   thus every automaton is valid;
     *    such weightsets are B or N[k];    
     * -- non_starrable: the star is defined for no element except zero,
     *    thus the automaton is not valid if there are epsilon-cycles;
     *    Z or Z/nZ are such weightsets;     
     * -- tops (topologically ordered positive semirings) :
     *    positive semirings where the domain of star is downward closed,
     *    e.g. Q+, R+, tropical semirings,...
     *    in this case, the proper algorithm is performed; 
     *    it fails if and only if the automaton is not valid;    
     * -- absval: semirings where absolute value allows to defined 
     *    absolute summations; this brings back to the case of tops. 
     *  
     * @param aut The tested automaton
     * @return `true` if and only if the automaton is valid
    */
    template <typename Aut>
    inline
    bool is_valid(const Aut& aut)
    {
      return internal::is_valider<Aut>::is_valid(aut);
    }
    
    /*,-------------------.
      | is_valid(ratexp). |
      `-------------------'*/
    
    template <typename RatExpSet>
    bool
    is_valid(const RatExpSet& rs, const typename RatExpSet::ratexp_t& e)
    {
      rat::constant_term_visitor<RatExpSet> constant_term{rs};
      try
        {
          constant_term(e);
          return true;
    }
      catch (const std::runtime_error&)
        {
          return false;
        }
    }
    
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_IS_VALID_HH
