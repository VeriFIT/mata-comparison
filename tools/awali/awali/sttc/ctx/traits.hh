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

#ifndef AWALI_CTX_TRAITS_HH
# define AWALI_CTX_TRAITS_HH

# include <memory>

#include <awali/sttc/core/rat/identities.hh> // FIXME: likely useless

namespace awali {
  namespace sttc {
    namespace internal {
      /// T without reference or const/volatile qualifiers.
      template <typename T>
      using base_t
      = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
    }



  /*------------------.
  | Computing types.  |
  `------------------*/

# define DEFINE(Type)                                                   \
  namespace internal                                                      \
  {                                                                     \
    template <typename ValueSet>                                        \
    struct Type ## _of_impl                                             \
    {                                                                   \
      using type = typename ValueSet::Type;                             \
    };                                                                  \
  }                                                                     \

  DEFINE(context_t)
  DEFINE(label_t)
  DEFINE(labelset_t)
  DEFINE(weight_t)
  DEFINE(weightset_t)

# undef DEFINE
# define DEFINE(Type)                                                   \
  template <typename ValueSet>                                          \
  using Type ## _of                                                     \
  = typename internal::Type ## _of_impl<internal::base_t<ValueSet>>::type;

    /** Helper to retrieve the type of the context of a value set.
     * For instance, if \p ValueSet is an a type of automaton,
     * context_t_of&lt;ValueSet&gt; is the type of its context.
     * @tparam ValueSet the type of the value set.
     */
  DEFINE(context_t)
    /** Helper to retrieve the type of the labels of a value set.
     * See {@link context_t_of}
     * @tparam ValueSet the type of the value set.
     */
  DEFINE(label_t)
    /** Helper to retrieve the type of the labelset of a value set.
     * See {@link context_t_of}
     * @tparam ValueSet the type of the value set.
     */
  DEFINE(labelset_t)
    /** Helper to retrieve the type of the weights of a value set.
     * See {@link context_t_of}
     * @tparam ValueSet the type of the value set.
     */
  DEFINE(weight_t)
    /** Helper to retrieve the type of the weightset of a value set.
     * See {@link context_t_of}
     * @tparam ValueSet the type of the value set.
     */
  DEFINE(weightset_t)

# undef DEFINE


  /*------------------.
  | shared pointers.  |
  `------------------*/

  // Forward traits about shared_ptr<T> to T.
  namespace internal
  {
# define DEFINE(Traits)                                         \
    template <typename ValueSet>                                \
    struct Traits ## _t_of_impl<std::shared_ptr<ValueSet>>      \
      : Traits ## _t_of_impl<base_t<ValueSet>>                  \
    {};

    DEFINE(context)
    DEFINE(label)
    DEFINE(labelset)
    DEFINE(weight)
    DEFINE(weightset)
# undef DEFINE
  }
}}//end of ns awali::stc

#endif // !AWALI_CTX_TRAITS_HH
