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

#ifndef AWALI_ALGOS_ARE_EQUIVALENT_HH
# define AWALI_ALGOS_ARE_EQUIVALENT_HH

#include <awali/common/priority.hh>

#include <awali/sttc/algos/accessible.hh> // is_useless
#include <awali/sttc/algos/complement.hh>
#include <awali/sttc/algos/complete.hh>
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/algos/left_mult.hh>
#include <awali/sttc/algos/product.hh>
#include <awali/sttc/algos/reduce.hh>
#include <awali/sttc/algos/sum.hh>
#include <awali/sttc/algos/lal_lan_conversion.hh>

namespace awali {
  namespace sttc {
   
    /*,---------------------------------------.
      | are_equivalent(automaton, automaton). |
      `---------------------------------------'*/

    namespace internal { 
      //Definitions of internal functions `are_equivalent_`
      
      /// Check equivalence between Boolean automata on a free labelset.
      template <typename Aut1, typename Aut2, typename P>
      auto
      are_equivalent_(const Aut1& aut1, const Aut2& aut2, priority::FIVE<P>)
        -> typename std::enable_if<(labelset_t_of<Aut1>::is_free()
                                    && std::is_same<weightset_t_of<Aut1>, b>::value
                                    && labelset_t_of<Aut2>::is_free()
                                    && std::is_same<weightset_t_of<Aut2>, b>::value),
                                   bool>::type
      {
        return (   is_useless(difference(aut1, aut2))
                   && is_useless(difference(aut2, aut1)));
      }
                        
                        
      /// Check equivalence between automata on fields, or Z.
      template <typename Aut1, typename Aut2, typename T>
      auto
      are_equivalent_(const Aut1& aut1, const Aut2& aut2, priority::FOUR<T>)
        -> decltype(aut2->weightset()->sub,
                    aut2->weightset()->zero,
                    aut2->weightset()->one,
                    bool())
      {
        const auto& ws2 = *aut2->weightset();
        // d = aut1 U -aut2.
        auto d = sum(aut1, left_mult(aut2, ws2.sub(ws2.zero(), ws2.one())));
        return is_empty(reduce(d));
      }
      
      
      template <typename Aut1, typename Aut2, typename P>
      bool
      are_equivalent_(const Aut1& aut1, const Aut2& aut2, priority::ONE<P>)
      {
        raise("Function are_equivalent is only supported for weightsets with subtraction and for boolean automata over a free label-sets with no epsilon-transitions allowed.");
      }
      
    } // end of namespace awali::sttc::internal

    /** @brief Tests if \p aut1 and \p aut2 are equivalent.
     *
     *  @param aut1
     *  @param aut2
     *  @return `true` if \p aut1 and \p aut2 associates every word with the same weight.
     *  @pre The algorithm only work if either:
     *  - both \p aut1 and \p aut2 are over weightset B;
     *  - automaton \p aut2 is over a weightset that is a field, or over Z, and the context of \p aut1 is compatible with the context of \p aut2.
     */
    
    template <typename Aut0, typename Aut2>
    bool are_equivalent(const Aut0& aut1, const Aut2& aut2)
    {
      return internal::are_equivalent_(to_lal(aut1), to_lal(aut2), priority::value);
    }
    
    
    /*,-----------------------------------.
      | difference(automaton, automaton). |
      `-----------------------------------'*/

    /** @brief Computes an automaton that accepts every word accepted by \p aut1 which is not accepted by \p aut2.
     *
     * If \p aut1 is a weighted automaton, the result realizes the series restricted to the complement of the language recognized by \p aut2.
     *
     *  @param aut1 an automaton labeled by letters
     *  @param aut2 a Boolean automaton labeled by letters
     *  @return an automaton
     */
    template <typename Lhs, typename Rhs>
    typename Lhs::element_type::automaton_nocv_t
    difference(const Lhs& aut1, const Rhs& aut2)
    {
      // Meet complement()'s requirements.
      auto a = aut2;
      if (!is_deterministic(a))
        a = complete(determinize(a,false));
      else if (!is_complete(a))
        a = complete(a);
      return product(aut1, complement(a));
    }

    
  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_ARE_EQUIVALENT_HH
