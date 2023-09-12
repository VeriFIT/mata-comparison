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

#ifndef AWALI_COMMON_ENUMS_HH
#define AWALI_COMMON_ENUMS_HH

namespace awali {

  /* ========================================================================
      For developpers: whenever an enum is added here, do not forget to 
      register it in enum_utils.hh and its values in enum_utils.cc
     ======================================================================== */

  /** @addtogroup Options 
   * @{
   */


  /** Used in some algorithms in which one may considers transitions forward or
   * backwards.
   */
  enum direction_t {
    FORWARD,
    BACKWARD
  };


  /** The different layout that we may pass to program `dot` to compute geometry
   * of automata
   */
  enum layout_t {
    /** Spring left to right layout. */
    VERTICAL,
    /** Spring top down layout. */
    HORIZONTAL,
    /** Circular layout. */
    CIRCULAR
  };


  /** The different algorithms for computing the minimal automaton. */
  enum minim_algo_t {
    /** Determinizes, then computes the minimal quotient (see {@link quotient_algo_t}). */
    DETERMINIZE_QUOTIENT,
    /** Transposes, then determinizes, then transposes, then determinizes. */
    BRZOZOWSKI
  };


  /** The différent algorithms for computing the minimal quotient. */
  enum quotient_algo_t {
    MOORE,
    HOPCROFT
  };

  /// The different algorithms for transforming an expression into an automaton
  enum exp_to_aut_algo_t {
    /** This automaton without epsilon-transitions is incrementally built from the expression. 
     * It is isomorphic to the Position automaton.
     * It has n+1 states, where n is the litteral length of the expression, i.e. the number of letters. 
     */
    GLUSHKOV,
    /// Alias to {@link GLUSHKOV}
    STANDARD=GLUSHKOV,
    /** Also known as Partial derived terms. 
     * Described by Antomirov for Boolean semrings; extension to weighted automata by Lombardy and Sakarovitch.
     * It is a quotient of the Standard automaton.
     */
    DERIVED_TERM,
    /// A variant of the Derived term automaton where left factors of expressions which are sums are "broken".
    BREAKING_DERIVED_TERM,
    /// Alias to {@link BREAKING_DERIVED_TERM}
    BREAKING = BREAKING_DERIVED_TERM,
    /** The classical Thompson automaton, with epsilon-transitions. 
     * It has exactly 2n states, where n is the size of the expressions (letters + operators). 
     * The maximal out- and in-degree of every state is 2; if it is 2, the labels of transitions are epsilon. 
     * It has one and only one initial (resp. final) state with no incoming (resp. outgoing) transition. 
     */
    THOMPSON,
    /// A variant of the Thompson automaton, with fewer states
    COMPACT_THOMPSON,
    /** A variant of the Thompson automaton, with no circuit of epsilon-transitions; in this variant, the initial state may be another final state.*/
    WEIGHTED_THOMPSON,
    /// Standard automaton followed by quotient
    STANDARD_AND_QUOTIENT
  };


  /** The different strategies for choosing which state to eliminate first when
   * transforming an automaton into an expression.
   */
  enum state_elim_order_t {
    /** States are eliminated by increasing in-out degree. The in-out degree of
     * a state is the product of the number of incoming transition by the
     * number of outgoing transitions.
     */
    MIN_INOUT_DEGREE,
    /** States are eliminated by increasing id number (that is, in an arbitrary
     * order).
     */
    MIN_ID,
    /// Alias to {@link MIN_ID}
	ID_ORDER=MIN_ID
  };


  /** The different format for input/output of automata and expressions. */
  enum io_format_t {
    /** The native Awali format. */
    FSM_JSON_V1 = 0x101,

    /** The current Awali format. */
    JSON = FSM_JSON_V1,

    /** Used for pure-text representation.  Rational expression only. */
    TEXT = 1,

    /** FADO format.  Boolean automata only. */
    FADO,
    
    /** Grail format.  Boolean automata only. */
    GRAIL,

    /** Format used by graphviz.  Output only. */
    DOT,    
    
    /** A pdf image is produced by graphviz.  Output only. */
    PDF,
        
    /** Image produced by graphviz.  Output only. */
    SVG,

    /** Deprecated Awali and Vaucanson-R format.     
     * Warning: this format does not follow Json syntax.    
     * Input only.
     */
    FSM_JSON_V0
  };

  inline bool is_true_json(io_format_t format) {return (0x100 & format);}
  inline bool is_json_like(io_format_t format) 
  {return ((format == FSM_JSON_V0) || is_true_json(format)); }



  /** The different behaviours a weightset may have with respect to the star.
   */
  enum star_status_t {
    /// The star of every element exists.
    STARRABLE,
    NON_STARRABLE,
    TOPS,
    ABSVAL
  };


  /** @brief The different kinds of history.
   *
   * This class represents the different kinds of history a state may have.
   * For instance, if an automaton is the result of a determinisation, then
   * the history of a state is of kind PARTITION.
   * */
  enum class history_kind_t {

    /** @brief The states comes from a single state. */
    SINGLE,

    /** @brief The states comes from a tuple of state.
     * It is typically, the history kind of an automaton resulting from a
     * product of automata.
     */
    TUPLE,

    /** @brief The state comes from a rational expression. */
    RATEXP,

    /** @brief The state history is expressed directy as a string.
     * It is typically the history kind typically used when no other kind is
     * proper.
     * */
    STRING,

    /** @brief The state has no history. */
    NO_HISTORY,

    /** @brief The state is a set of states of another automaton.
     * It is typically the history kind of an automaton resulting from a
     * determinization process.
     */
    PARTITION
  };


  /** @} */ //end of group Options

}// end of namespace awali
#endif
