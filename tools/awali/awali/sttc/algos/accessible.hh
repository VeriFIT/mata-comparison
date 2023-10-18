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

/* @file accessible.hh
 *
 * This files contains static functions computing accessible states.
 */
#ifndef AWALI_ALGOS_ACCESSIBLE_HH
#define AWALI_ALGOS_ACCESSIBLE_HH

#include <deque>
#include <queue>
#include <map>
#include <unordered_set>

#include <awali/sttc/algos/copy.hh>
#include <awali/sttc/algos/sub_automaton.hh>
#include <awali/sttc/core/transpose_view.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/misc/set.hh>

namespace awali {
  namespace sttc {


  /*--------------------------------------------------.
  | Sets of accessible, coaccessible, useful states.  |
  `--------------------------------------------------*/

  // The set of accessible states, including pre(), and possibly post().
  //breadth first search
    template <typename Set, typename Aut>
    void
    fill_with_accessible_states(Set& res, const Aut& aut, bool include_pre_post=false) {
      // Reachable states.
      const auto& a = *aut;
      if(include_pre_post)
        res.emplace(a.pre());
      
      // States work list.
      using worklist_t = std::queue<state_t>;
      worklist_t todo;
      todo.emplace(a.pre());
      
      while (!todo.empty())
        {
          const state_t src = todo.front();
          todo.pop();
          
          for (auto tr : a.all_out(src))
            {
              state_t dst = a.dst_of(tr);
              // If we have not seen it already, explore its successors.
              if(dst != a.post() || include_pre_post)
                if (res.emplace(dst).second)
                  todo.emplace(dst);
            }
        }
    }
    
  /** @brief List of accessible states
   *
   * Computes the list of accessible states in aut.
   *
   * @tparam Aut The static type of automaton
   * @param aut  A static Awali automaton that can be read-only (including view)
   * @param include_pre_post  if true, the pre-initial and the post-final states may be added to the result.
   *
   * @return a standard set
   **/
    template <typename Aut>
    std::set<state_t>
    accessible_states(const Aut& aut, bool include_pre_post=false)
    {
      // Reachable states.
      std::set<state_t> res;
      fill_with_accessible_states(res, aut, include_pre_post);
      return res;
    }

    template <typename Set, typename Aut>
    void
    fill_with_coaccessible_states(Set& res, const Aut& aut, bool include_pre_post=false) {
      fill_with_accessible_states(res, transpose_view(aut), false);
    }
    
    // The set of coaccessible states, including post(), and possibly pre().
    /** @brief List of coaccessible states
     *
     * Computes the list of coaccessible states in aut.
     *
     * @tparam Aut The static type of automaton
     * @param aut  A static Awali automaton that can be read-only (including view)
     * @param include_pre_post  if true, the pre-initial and the post-final states may be added to the result.
     *
     * @return a standard set
     **/
    template <typename Aut>
    std::set<state_t>
    coaccessible_states(const Aut& aut, bool include_pre_post=false)
    {
      return accessible_states(transpose_view(aut), false);
    }

    // The set of coaccessible states, including post(), and possibly pre().
    /** @brief List of useful states
     *
     * Computes the list of useful states in aut.
     *
   * @tparam Aut The static type of automaton
     * @param aut  A static Awali automaton that can be read-only (including view)
     * @param include_pre_post  if true, the pre-initial and the post-final states may be added to the result.
     *
     * @return a standard set
     **/
    template <typename Aut>
    std::set<state_t>
    useful_states(const Aut& aut, bool include_pre_post=false)
    {
      auto accessible = accessible_states(aut, false);
      auto coaccessible = coaccessible_states(aut, false);
      return internal::intersection(accessible, coaccessible);
    }


    /*----------------------------------------------------.
      | Number of accessible, coaccessible, useful states.  |
      `----------------------------------------------------*/

    /** @brief Number of accessible states
     *
     * Computes the number of accessible states (not counting pre-initial and post-final state).
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     *
     * @return A size_t number
     */
    template <typename Aut>
    size_t
    num_accessible_states(const Aut& aut)
    {
      std::unordered_set<state_t> set;
      fill_with_accessible_states(set,aut);
      return set.size();
    }

    /** @brief Number of coaccessible states
     *
     * Computes the number of coaccessible states (not counting pre-initial and post-final state).
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     *
     * @return A size_t number
     */
    template <typename Aut>
    size_t
    num_coaccessible_states(const Aut& aut)
    {
      std::unordered_set<state_t> set;
      fill_with_coaccessible_states(set,aut);
      return set.size();
    }

    /** @brief Number of useful states
     *
     * Computes the number of useful states (not counting pre-initial and post-final state).
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     *
     * @return A size_t number
     */
    template <typename Aut>
    size_t
    num_useful_states(const Aut& aut)
    {
      auto set = useful_states(aut);
      return set.size();
    }


    /*-----------------------------------------------.
      | accessible, coaccessible, useful subautomata.  |
      `-----------------------------------------------*/

    /** @brief Accessible subautomaton
     * 
     * Computes a fresh automaton with a copy of the accessible part of aut.
     * If the keep_history flag is true, the result is endowed with
     * a SINGLE history.
     *
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     * @param keep_history A Boolean that tells whether the history must be registered.
     *
     * @return A mutable Awali automaton
     */
    template <typename Aut>
    typename Aut::element_type::automaton_nocv_t
    accessible(const Aut& aut, bool keep_history=true)
    {
      return copy(aut, accessible_states(aut), keep_history,false,true);
    }
    
    /** @brief In-place accessible subautomaton
     * 
     * Remove every non accessible state of aut.
     *
     * @tparam Aut The static type of automaton
     * @param aut A static mutable Awali automaton
     */
    template <typename Aut>
    void
    accessible_here(Aut& aut)
    {
      return sub_automaton(aut, accessible_states(aut));
    }

    /** @brief Coaccessible subautomaton
     * 
     * Computes a fresh automaton with a copy of the coaccessible part of aut.
     * If the keep_history flag is true, the result is endowed with
     * a SINGLE history.
     *
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     * @param keep_history A Boolean that tells whether the history must be registered.
     *
     * @return A mutable Awali automaton
     */
    template <typename Aut>
    typename Aut::element_type::automaton_nocv_t
    coaccessible(const Aut& aut, bool keep_history=true)
    {
      return copy(aut, coaccessible_states(aut), keep_history, false, true);
    }

    /** @brief In-place coaccessible subautomaton
     * 
     * Remove every non coaccessible state of aut.
     *
     * @tparam Aut The static type of automaton
     * @param aut A static mutable Awali automaton
     */
    template <typename Aut>
    void
    coaccessible_here(Aut& aut)
    {
      return sub_automaton(aut, coaccessible_states(aut));
    }

    /** @brief Trim subautomaton
     * 
     * Computes a fresh automaton with a copy of the useful part of aut.
     * If the keep_history flag is true, the result is endowed with
     * a SINGLE history.
     *
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     * @param keep_history A Boolean that tells whether the history must be registered.
     *
     * @return A mutable Awali automaton
     */
    template <typename Aut>
    typename Aut::element_type::automaton_nocv_t
    trim(const Aut& aut, bool keep_history=true)
    {
      return copy(aut, useful_states(aut), keep_history, false, true);
    }

    /** @brief In-place trim subautomaton
     * 
     * Remove every useless state of aut.
     *
     * @tparam Aut The static type of automaton
     * @param aut A static mutable Awali automaton
     */
    template <typename Aut>
    void
    trim_here(Aut& aut)
    {
      return sub_automaton(aut, useful_states(aut));
    }
    
    /*----------------------------------------------------------------.
      | is_trim, is_accessible, is_coaccessible, is_empty, is_useless.  |
      `----------------------------------------------------------------*/

    /** @brief Test whether the automaton is trim
     * 
     * @param aut A static Awali automaton that can be read-only (including view)
     *
     * @return true if the automaton is trim; false otherwise.
     */
    template <typename Aut>
    bool is_trim(const Aut& aut)
    {
      return num_useful_states(aut) == aut->num_states();
    }

    /** @brief Test whether the automaton has useful states
     * 
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     *
     * @return true if the automaton has no useful state; false otherwise.
     */
    template <typename Aut>
    bool is_useless(const Aut& aut)
    {
      return num_useful_states(aut) == 0;
    }

    /** @brief Test whether every state of the automaton is accessible
     * 
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     *
     * @return true if every state is accessible; false otherwise.
     */
    template <typename Aut>
    bool is_accessible(const Aut& aut)
    {
      return num_accessible_states(aut) == aut->num_states();
    }

    /** @brief Test whether every state of the automaton is coaccessible
     * 
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     *
     * @return true if every state is coaccessible; false otherwise.
     */
    template <typename Aut>
    bool is_coaccessible(const Aut& aut)
    {
      return num_coaccessible_states(aut) == aut->num_states();
    }

    template <typename Aut>
    bool is_empty(const Aut& aut) ATTRIBUTE_PURE;

    /** @brief Test whether the automaton has no state
     * 
     * Beware that the automaton may have a transition from pre_ to post_
     *
     * @tparam Aut The static type of automaton
     * @param aut A static Awali automaton that can be read-only (including view)
     *
     * @return true if the automaton has no state; false otherwise.
     */
    template <typename Aut>
    bool is_empty(const Aut& aut)
    {
      return aut->num_states() == 0;
    }

  }
}//end of ns awali::stc

#endif // !AWALI_ALGOS_ACCESSIBLE_HH
