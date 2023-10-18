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

#ifndef AWALI_LABELSET_LABELSET_HH
# define AWALI_LABELSET_LABELSET_HH

#include <awali/sttc/weightset/polynomialset.hh>
#include <awali/sttc/labelset/traits.hh>

namespace awali {
  namespace sttc {

    namespace internal
    {
      /*---------------.
        | make_wordset.  |
        `---------------*/

      template <typename LabelSet>
      using law_t = typename labelset_trait<LabelSet>::wordset_t;

      /// The wordset of a labelset.
      template <typename LabelSet>
      inline law_t<LabelSet>
      make_wordset(const LabelSet& ls)
      {
        return labelset_trait<LabelSet>::wordset_t::value(ls);
      }

      /*--------------------.
        | make_word_context.  |
        `--------------------*/

      template <typename Ctx>
      using word_context_t
      = context<law_t<labelset_t_of<Ctx>>, weightset_t_of<Ctx>>;

      /// The wordset context of a labelset.
      template <typename LabelSet, typename WeightSet>
      inline word_context_t<context<LabelSet, WeightSet>>
        make_word_context(const context<LabelSet, WeightSet>& ctx)
      {
        return {make_wordset(*ctx.labelset()), *ctx.weightset()};
      }

      /*--------------------------.
        | make_word_polynomialset.  |
        `--------------------------*/

      template <typename Ctx>
      using word_polynomialset_t = polynomialset<word_context_t<Ctx>>;

      /// The polynomialset of words of a labelset (not necessarily on
      /// words itself).
      template <typename Ctx>
      inline auto
      make_word_polynomialset(const Ctx& ctx)
        -> word_polynomialset_t<Ctx>
      {
        return make_word_context(ctx);
      }
    }

  }
}//end of ns awali::stc

#endif // !AWALI_LABELSET_LABELSET_HH
