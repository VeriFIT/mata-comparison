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

#ifndef AWALI_ALGOS_IS_PROPER_HH
# define AWALI_ALGOS_IS_PROPER_HH

# include <type_traits>

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/core/kind.hh>

namespace awali { namespace sttc {


  namespace internal
  {
    template <typename Aut>
    typename std::enable_if<labelset_t_of<Aut>::has_one(),
                            bool>::type
    is_proper_(const Aut& aut)
    {
      for (auto t: aut->transitions())
        if (aut->labelset()->is_one(aut->label_of(t)))
          return false;
      return true;
    }

    template <typename Aut>
    constexpr
    typename std::enable_if<!labelset_t_of<Aut>::has_one(),
                            bool>::type
    is_proper_(const Aut&)
    {
      return true;
    }
  }

  /**@brief Test whether an automaton is proper.

     An automaton is proper iff it contains no epsilon-transition.

     @param aut The tested automaton
     @return true iff the automaton is proper
  */
  template <typename Aut>
  bool is_proper(const Aut& aut) ATTRIBUTE_CONST;

  template <typename Aut>
  bool
  is_proper(const Aut& aut)
  {
    return internal::is_proper_(aut);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_IS_PROPER_HH
