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

#ifndef AWALI_MISC_ADD_EPSILON_TRANS_HH
#define AWALI_MISC_ADD_EPSILON_TRANS_HH

namespace awali {
  namespace sttc {
    namespace internal {
      //If epsilon transitions are not allowed

      template<bool HAS_ONE>
      struct if_has_one {

        template<typename Aut>
        static inline
        auto
        new_epsilon_trans(Aut,
                          state_t, state_t,
                          ::awali::sttc::weight_t_of<Aut>)
          -> transition_t
        {
          throw std::runtime_error("No epsilon transition");
        }

        template<typename Aut>
        static inline
        auto
        set_epsilon_trans(Aut, state_t, state_t,
                          ::awali::sttc::weight_t_of<Aut>)
          -> transition_t
        {
          throw std::runtime_error("No epsilon transition");
        }

        template<typename Aut>
        static inline
        auto
        add_epsilon_trans(Aut,
                          state_t, state_t,
                          ::awali::sttc::weight_t_of<Aut>)
          -> ::awali::sttc::weight_t_of<Aut>
        {
          throw std::runtime_error("No epsilon transition");
        }

        template<typename Labelset>
        static inline
        typename Labelset::value_t
        get_epsilon() {
          throw std::runtime_error("No epsilon in labelset");
        }

        template<typename Labelset>
        static inline
        bool
        is_epsilon(const typename Labelset::value_t&) {
          return false;
        }

        template<typename Aut>
        static inline
        void
        del_epsilon_trans(Aut, state_t, state_t)
        {
          throw std::runtime_error("No epsilon transition");
        }
      };


      //If epsilon transitions are allowed


      template<>
      struct if_has_one<true> {

        template<typename Aut>
        static inline
        auto
        new_epsilon_trans(Aut a,
                          state_t src, state_t dst,
                          ::awali::sttc::weight_t_of<Aut> w)
          -> transition_t
        {
          return a->new_transition(src, dst, a->context().labelset()->one(),w);
        }

        template<typename Aut>
        static inline
        auto
        set_epsilon_trans(Aut a,
                          state_t src, state_t dst,
                          ::awali::sttc::weight_t_of<Aut> w)
          -> transition_t
        {
          return a->set_transition(src, dst, a->context().labelset()->one(),w);
        }

        template<typename Aut>
        static inline
        auto
        add_epsilon_trans(Aut a,
                          state_t src, state_t dst,
                          ::awali::sttc::weight_t_of<Aut> w)
          -> ::awali::sttc::weight_t_of<Aut>
        {
          return a->add_transition(src, dst, a->context().labelset()->one(),w);
        }

        template<typename Aut>
        static inline
        void
        del_epsilon_trans(Aut a,
                          state_t src, state_t dst)
        {
          a->del_transition(src, dst, a->context().labelset()->one());
        }

        template<typename Labelset>
        static inline
        typename Labelset::value_t
        get_epsilon() {
          return Labelset::one();
        }

        template<typename Labelset>
        static inline
        bool
        is_epsilon(const typename Labelset::value_t& l) {
          return Labelset::is_one(l);
        }

      };
    }

    /** Helper to create a new epsilon transition
     *
     * Like the new_transition method of automata, 
     * this function must be called only in the case where 
     * there is no existing epsilon transition with the same ends. 
     *
     * @tparam Aut the type of the automaton
     * @param a the automaton
     * @param src the source state of the transition
     * @param dst the destination state of the transition
     * @param w the weight of the transition
     * @return the identifier of the created transition
     */
    template <typename Aut>
    inline
    transition_t
    new_epsilon_trans(Aut a, state_t src, state_t dst, weight_t_of<Aut> w) {
      return internal::if_has_one<labelset_t_of<Aut>::has_one()>::new_epsilon_trans(a, src, dst, w);
    }

    /** Helper to set an epsilon transition
     *
     * Like the set_transition method of automata, 
     * this function either creates or replaces an epsilon transition.
     *
     * @tparam Aut the type of the automaton
     * @param a the automaton
     * @param src the source state of the transition
     * @param dst the destination state of the transition
     * @param w the weight of the transition
     * @return the identifier of the created transition
     * @throw runtime_error if the automaton does not admit epsilon transitions
     */
    template <typename Aut>
    inline
    transition_t
    set_epsilon_trans(Aut a, state_t src, state_t dst, weight_t_of<Aut> w) {
      return internal::if_has_one<labelset_t_of<Aut>::has_one()>::set_epsilon_trans(a, src, dst, w);
    }

    /** Helper to add an epsilon transition
     *
     * Like the add_transition method of automata, 
     * this function either create an epsilon transition
     * or add the weight to an existing epsilon-transition with the same ends.
     *
     * @tparam Aut the type of the automaton
     * @param a the automaton
     * @param src the source state of the transition
     * @param dst the destination state of the transition
     * @param w the weight of the transition
     * @return the resulting weight of the transition
     * @throw runtime_error if the automaton does not admit epsilon transitions
     */
    template <typename Aut>
    inline
    weight_t_of<Aut>
    add_epsilon_trans(Aut a, state_t src, state_t dst, weight_t_of<Aut> w) {
      return internal::if_has_one<labelset_t_of<Aut>::has_one()>::add_epsilon_trans(a, src, dst, w);
    }

    /** Helper to delete an epsilon transition
     *
     * Like the del_transition method of automata, 
     * this function removes, if it exists, the epsilon transition with the given ends.
     *
     * @tparam Aut the type of the automaton
     * @param a the automaton
     * @param src the source state of the transition
     * @param dst the destination state of the transition
     * @throw runtime_error if the automaton does not admit epsilon transitions
     */
    template <typename Aut>
    inline
    void
    del_epsilon_trans(Aut a, state_t src, state_t dst) {
      return internal::if_has_one<labelset_t_of<Aut>::has_one()>::del_epsilon_trans(a, src, dst);
    }

    //Without weight

    /** Helper to create a new epsilon transition
     *
     * Like the new_transition method of automata, 
     * this function must be called only in the case where 
     * there is no existing epsilon transition with the same ends. 
     *
     * @tparam Aut the type of the automaton
     * @param a the automaton
     * @param src the source state of the transition
     * @param dst the destination state of the transition
     * @return the identifier of the created transition
     * @throw runtime_error if the automaton does not admit epsilon transitions
     */
    template <typename Aut>
    transition_t
    new_epsilon_trans(const Aut a, state_t src, state_t dst) {
      return internal::if_has_one<labelset_t_of<Aut>::has_one()>::new_epsilon_trans(a, src, dst, a->context().weightset()->one());
    }

    /** Helper to add an epsilon transition
     *
     * Like the add_transition method of automata, 
     * this function either create an epsilon transition
     * or add the weight to an existing epsilon-transition with the same ends.
     *
     * @tparam Aut the type of the automaton
     * @param a the automaton
     * @param src the source state of the transition
     * @param dst the destination state of the transition
     * @return the resulting weight of the transition
     * @throw runtime_error if the automaton does not admit epsilon transitions
     */
    template <typename Aut>
    inline
    weight_t_of<Aut>
    add_epsilon_trans(const Aut a, state_t src, state_t dst) {
      return internal::if_has_one<labelset_t_of<Aut>::has_one()>::add_epsilon_trans(a, src, dst, a->context().weightset()->one());

    }

    /** Helper to set an epsilon transition
     *
     * Like the set_transition method of automata, 
     * this function either creates or replaces an epsilon transition.
     *
     * @tparam Aut the type of the automaton
     * @param a the automaton
     * @param src the source state of the transition
     * @param dst the destination state of the transition
     * @return the identifier of the created transition
     * @throw runtime_error if the automaton does not admit epsilon transitions
     */
    template <typename Aut>
    inline
    transition_t
    set_epsilon_trans(const Aut a, state_t src, state_t dst) {
      return internal::if_has_one<labelset_t_of<Aut>::has_one()>::set_epsilon_trans(a, src, dst, a->context().weightset()->one());
    }
    /*
      template <typename Aut>
      inline
      label_t_of<Aut>
      get_epsilon(const Aut a) {
      return if_has_one<labelset_t_of<Aut>::has_one()>::template get_epsilon<labelset_t_of<Aut>>();
      }
    */

    /** Helper to get the value of epsilon
     *
     * @tparam Labelset the label set
     * @return the value of epsilon
     * @throw runtime_error if the label set does not admit epsilon transitions
     */
    template <typename Labelset>
    inline
    typename Labelset::value_t
    get_epsilon() {
      return internal::if_has_one<Labelset::has_one()>::template get_epsilon<Labelset>();
    }

    /** Helper to test if a label is epsilon
     *
     * @tparam Labelset the label set
     * @param l the label
     * @return true if the label is epsilon
     * @throw runtime_error if the label set does not admit epsilon transitions
     */
    template <typename Labelset>
    inline
    bool
    is_epsilon(const typename Labelset::value_t& l) {
      return internal::if_has_one<Labelset::has_one()>::template is_epsilon<Labelset>(l);
    }

  }
}//end of ns awali::stc

#endif //!AWALI_MISC_ADD_EPSILON_TRANS_HH
