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

#ifndef DYN_ABSTRACT_AUTOMATON_HH
#define DYN_ABSTRACT_AUTOMATON_HH

#include<vector>
#include<iostream>
#include<string>
#include<memory>
#include <awali/common/enums.hh>

#include <awali/dyn/modules/context.hh>
#include <awali/dyn/core/context.hh>
#include <awali/dyn/core/context_description.hh>
#include <awali/dyn/core/typedefs.hh>
#include <awali/dyn/options/options.hh>

#include <awali/common/qfraction.hh>


#include<memory>

namespace awali { namespace dyn {

  /** 
  Abstract interface listing the services provided by automata at the
  dynamical layer.

  
  # Difference between adding and setting transitions.
 
  First, note that there is no difference if the automaton is Booolean (that
  is, if it as "classical" automaton).

  In the following, let us assume that the automaton is weighted over a
  semiring (W,+,x).

  Let `src` and `dst` be two states and `lbl` be a label. (`lbl` might be
  epsilon if the automaton allows it, or a tuple in the case of transducers.)
  In Awali, there is at most one transitions `tr` going from `src` to `dst` and
  labelled by `lbl`.  In addition, This transition also bears  weight `wght`.

  When modifying the transition going from `src` to `dst` and labelled by `lbl`,
  one has to decide what to do with `wght`.
  - `set_transition(src,dst,lbl,new_wght)` ignores `wght`, and sets the weight of `tr` to `new_wght`
  - `add_transition(src,dst,lbl,new_wght)` modifies the weights of `tr` so that it now is `wght+new_wght`.

  If the automaton features no transition going from `src` to `dst` and
  labelled by `lbl`, everything behaves as if it had such a transition weighted
  by zero (in fact, the neutral element for +).
  In this case both functions above do the same thing.

  */
  struct abstract_automaton_t {

    /** Returns the phony state, used for instance when no state is found. */
    virtual state_t null_state () const = 0;
    
    /** Returns the phony transition_t, used for instance when transition is
     *  found.
     */
    virtual transition_t null_transition () const = 0;
   
    /** Returns the number of states in this {@link abstract_automaton_t}. */
    virtual size_t num_states() const = 0;
    
    /** Returns the number of initial states in this {@link abstract_automaton_t}. */
    virtual size_t num_initials() const = 0;
    
    /** Returns the number of final states in this {@link abstract_automaton_t}. */
    virtual size_t num_finals() const = 0;

    /** Returns the number of transitions in this {@link abstract_automaton_t}. */
    virtual size_t num_transitions() const = 0;

    /** Return the name of this {@link abstract_automaton_t}.
     @see set_name
    */
    virtual const std::string& get_name() const = 0;
    
    /** Return the description of this {@link abstract_automaton_t}. 
    @see set_desc
    */
    virtual const std::string& get_desc() const = 0;


    /** Sets the name of this {@link abstract_automaton_t}.
   
    - The name of an automaton is meant to be used as filename so it should not
      contain special characters (but no check is done to enforce that).
    - When an automaton is output in JSON format, the name is written in the
      metadata section.
    - Name is sometimes used in error message.
    - The name might be automatically generated when applying algorithm.

    @param name The new name of this {@link abstract_automaton_t}.
    */
    virtual void set_name(const std::string& name) = 0;

    /** Sets the description of this {@link abstract_automaton_t}.
     
     When an automaton is output in JSON format, the description is written
       in the metadata section.
     @param desc New description of this {@link abstract_automaton_t}.
     */
    virtual void set_desc(const std::string& desc) = 0;

    /** Returns `true` if this {@link abstract_automaton_t} contains state @pname{s}.
     @param s
     */
    virtual bool has_state(state_t s) const = 0;

    /** Returns `true` if given state @pname{s} is initial in 
    this {@link abstract_automaton_t}.
      
    If this {@link abstract_automaton_t} is weighted over a weightset (W,+,.),
    it returns `true` if the initial weight associated with @pname{s} is not
    the neutral element for +.
    */
    virtual bool is_initial(state_t s) const = 0;

    /** Returns `true` if given state @pname{s} is final in 
    this {@link abstract_automaton_t}.
      
    If this {@link abstract_automaton_t} is weighted over a weightset (W,+,.),
    it returns `true` if the final weight associated with @pname{s} is not
    the neutral element for +.
    */
    virtual bool is_final(state_t s) const = 0;

    /** Gets the initial weight of @pname{s}. */
    virtual weight_t get_initial_weight(state_t s) const = 0;
    
    /** Gets the final weight of @pname{s}. */
    virtual weight_t get_final_weight(state_t s) const = 0;

    /** Adds a new isolated states.
     * This method is **not** equivalent to calling 
     * {@link add_state(std::string)} with an empty string as parameter.
     * This method leaves undefined the name of the new state.
     * */
    virtual state_t add_state() = 0;

    /** Adds a new isolated states with a custom name. */
    virtual state_t add_state(std::string name) = 0;

    /** Deletes state @pname{s} from this {@link abstract_automaton_t}.*/
    virtual void del_state(state_t s) = 0;

    /** Sets initial weight of state @pname{s} to @pname{w} (useful only for weightset automata). */
    virtual void set_initial(state_t s, weight_t w) = 0;

    /** Sets state @pname{s} as initial.
     *
     * If this {@link abstract_automaton_t} is weighted over (W,+,x), this
     * method sets the initial weight of @pname{s} to the neutral element for x.
     */
    virtual void set_initial(state_t s) = 0;

    
    /** Adds some initial weight of state @pname{s} to @pname{w} (useful only for weighted automata). 
     *
     * Let (W,+,x) be the weightset of this {@link abstract_automaton_t}.
     * A call to this method makes the initial weight of @pname{s} to become
     * *old+w*, where *old* is the initial weight of @pname{s} prior to the
     * call.
     *
     * @return the new initial weight of @pname{s}
     */
    virtual weight_t add_initial(state_t s, weight_t w) = 0;


    /** Sets state @pname{s} as non-initial
     *
     * If this {@link abstract_automaton_t} is weighted over (W,+,x), this
     * method sets the initial weight of @pname{s} to the neutral element for +.
     */
    virtual void unset_initial(state_t s) = 0;


    /** Sets final weight of state @pname{s} to @pname{w} (useful only for
     * weightset automata). */
    virtual void set_final(state_t s, weight_t w) = 0;


    /** Sets state @pname{s} as final.
     *
     * If this {@link abstract_automaton_t} is weighted over (W,+,x), this
     * method sets the final weight of @pname{s} to the neutral element for x.
     */
    virtual void set_final(state_t s) = 0;
    

    /** Adds some final weight of state @pname{s} to @pname{w} (useful only for
     * weighted automata). 
     *
     * Let (W,+,x) be the weightset of this {@link abstract_automaton_t}.
     * A call to this method makes the final weight of @pname{s} to become
     * *old+w*, where *old* is the final weight of @pname{s} prior to the
     * call.
     *
     * @return the new initial weight of @pname{s}
     */
    virtual weight_t add_final(state_t s, weight_t w) = 0;


    /** Sets state @pname{s} as non-final
     *
     * If this {@link abstract_automaton_t} is weighted over (W,+,x), this
     * method sets the final weight of @pname{s} to the neutral element for +.
     */
    virtual void unset_final(state_t s) = 0;

    
    /** Gets the transition identifier going from state @pname{src} to state
     * @pname{dst} and labelled by @pname{label}.
     *
     * @return A transition id if there is such a transition or a phony
     * transition id otherwise. 
     * Use {@link has_transition} or {@link null_transition} to know if this id is phony or not.
     */
    virtual transition_t get_transition(state_t src, state_t dst, label_t label) const = 0;

    /** Returns `true` if there is a transition going from state @pname{src} to
     * state @pname{dst} and labelled by @pname{label}.
     */
    virtual bool has_transition(state_t src, state_t dst, label_t label) const = 0;
    /** Returns `true` if a transition with identifier @pname{t} exists in this {@link abstract_automaton_t}.
     */
    virtual bool has_transition(transition_t t) const = 0;

    /** Returns the source state of transition @pname{t}. */
    virtual state_t src_of(transition_t t) const = 0;
    
    /** Returns the destination state of transition @pname{t}. */
    virtual state_t dst_of(transition_t t) const = 0;

    /** Returns the label of transition @pname{t}. */
    virtual label_t label_of(transition_t t) const = 0;

    /** Returns the label of all tapes on the transition @pname{t} (transducers only). */
    virtual std::list<label_t> labels_of(transition_t t) const = 0;
    
    /** Returns the label of the @pname{i}-th tape on the transition @pname{t} (transducers only). */
    virtual label_t label_of(transition_t t, unsigned i) const = 0;

    /** Returns the weight of transition @pname{t}. */
    virtual weight_t weight_of(transition_t t) const = 0;

    /** Returns `true` if transition @pname{t} is an epsilon-transition */
    virtual bool is_eps_transition(transition_t t) const = 0;

   
    /** Sets (and possibly replaces) a transition going from @pname{src}
     * to @pname{dst}, labelled by @pname{label} and with weight @pname{weight}.
     *
     * If there is, prior to the call to this method, a transition going
     * from @pname{src} to @pname{dst} and labelled by @pname{label}, its
     * weight is overriden by @pname{weight}.
     * Use {@link add_transition} if you want to add the weights instead.
     *
     * @return Transition corresponding to the transition set.
     
     * @see The respective section in the detailed description of
     * {@link abstract_automaton_t} for explanation of a difference with
     * #add_transition(state_t,state_t,label_t, weight_t);
     */
    virtual transition_t set_transition(state_t src, state_t dst, 
                                        label_t label, weight_t weight) = 0;

    /** Sets a transition going from @pname{src} to @pname{dst} and labelled by
     * @pname{label}.
     *
     *  If this {@link abstract_automaton_t} is weighted over (W,+,x), this is
     *  the same as `set_transition(src,dst,label,one)` where one is the
     *  neutral element for x.
     *
     * @see The section of the detailed description that describes
     * the difference between setting and adding a transition in a weighted
     * automaton.
     */
    virtual transition_t set_transition(state_t src, state_t dst, label_t label)
    = 0;

    /** Sets an epsilon transition going from @pname{src} to @pname{dst}.
     *
     * @pre This {@link abstract_automaton_t} needs to allow epsilon transitions. (See {@link is_eps_allowed}).
     *
     * @see The section of the detailed description that describes
     * the difference between setting and adding a transition in a weighted
     * automaton.
     */
    virtual transition_t set_eps_transition(state_t src, state_t dst) = 0;

    /** Sets an epsilon transition going from @pname{src} to @pname{dst} and
     * weighted by @pname{weight}.
     *
     * @pre This {@link abstract_automaton_t} needs to allow epsilon
     * transitions. (See {@link is_eps_allowed}).
     *
     * @see The section of the detailed description that describes
     * the difference between setting and adding a transition in a weighted
     * automaton.
     */
    virtual transition_t set_eps_transition(state_t src, state_t dst, 
                                            weight_t weight) = 0;

    /** Adds a transition going from @pname{src} to @pname{dst}, labelled by
     * @pname{label} and with weight @pname{weight}; or adds weight to an
     * existing transition.
     *
     * @return The weight of the transition going from @pname{src} to
     * @pname{dst} and labelled by @pname{label}.
     *
     * @see The section of the detailed description that describes
     * the difference between setting and adding a transition in a weighted
     * automaton.
     */
    virtual weight_t add_transition(state_t src, state_t dst, label_t label, 
                                    weight_t weight) = 0;

    /** Adds a transition going from @pname{src} to @pname{dst}, labelled by
     * @pname{label}.
     *
     *  If this {@link abstract_automaton_t} is weighted over (W,+,x), this is
     *  the same as `add_transition(src,dst,label,one)` where one is the
     *  neutral element for x.
     *
     * @return The weight of the transition going from @pname{src} to
     * @pname{dst} and labelled by @pname{label}.
     *
     * @see The section of the detailed description that describes
     * the difference between setting and adding a transition in a weighted
     * automaton.
     */
    virtual weight_t add_transition(state_t src, state_t dst, label_t label) 
    = 0;
    
    /** Adds an epsilon transition going from @pname{src} to @pname{dst}.
     *
     * @pre This {@link abstract_automaton_t} needs to allow epsilon transitions. (See {@link is_eps_allowed}).
     *
     * @see The section of the detailed description that describes
     * the difference between setting and adding a transition in a weighted
     * automaton.
     */
    virtual weight_t add_eps_transition(state_t src, state_t dst) = 0;

    /** Adds an epsilon transition going from @pname{src} to @pname{dst} and
     * weighted by @pname{weight}.
     *
     * @pre This {@link abstract_automaton_t} needs to allow epsilon
     * transitions. (See {@link is_eps_allowed}).
     *
     * @see The section of the detailed description that describes
     * the difference between setting and adding a transition in a weighted
     * automaton.
     */
    virtual weight_t add_eps_transition(state_t src, state_t dst, 
                                        weight_t weight) = 0;

    /** Sets to @pname{w} the weight of transition @pname{tr}. */ 
    virtual weight_t set_weight(transition_t tr, weight_t w) = 0;

    /** Adds @pname{w} to the weight of transition @pname{tr}. */ 
    virtual weight_t add_weight(transition_t tr, weight_t w) = 0;
    
    /** Multiplies by @pname{w} on the left the weight of transition @pname{tr}. */
    virtual weight_t lmul_weight(transition_t tr, weight_t w) = 0;
    
    /** Multiplies by @pname{w} on the right the weight of transition @pname{tr}. */
    virtual weight_t rmul_weight(transition_t tr, weight_t we) = 0;

    /** Deletes transition @pname{tr}. */
    virtual void del_transition(transition_t tr) = 0;

    /** Deletes the transition labelled by @pname{label} and going from state @pname{src} to state @pname{dst}. */
    virtual transition_t del_transition(state_t src, state_t dst, 
                                        label_t label) = 0;
    /** Deletes all transitions going from state @pname{src} to state @pname{dst}. */
    virtual void del_transition(state_t src, state_t dst) = 0;

    /** Deletes the epsilon transition going from state @pname{src} to state @pname{dst}. */
    virtual void del_eps_transition(state_t src, state_t dst) = 0;

    
    /** Returns the states of this {@link abstract_automaton_t}. 
     *
     * @param opts A set of options;  Only @ref PREPOST_PARADIGM is
     * meaningful.
     * @return The states in this automaton.
     * If @ref PREPOST_PARADIGM is `true` (defaults is `false`), the
     * "preinitial" and @ref post "postfinal" states are also returned.
     */
    std::vector<state_t> states(options_t opts = {}) const
    {
      return internal::states(this, opts[PREPOST_PARADIGM]);
    }
    
    /** Returns the transitions in this {@link abstract_automaton_t}. 
     *
     * @param opts A set of options;  only @ref PREPOST_PARADIGM is
     * meaningful.
     * @return The transitions in this automaton.
     * If @ref PREPOST_PARADIGM is `true` (default is `false`), the initial and
     * final transitions are returned. 
     */
    std::vector<transition_t> transitions(options_t opts = {}) const
    {
      return internal::transitions(this, opts[PREPOST_PARADIGM]);
    }

    /** Returns the initial states of this {@link abstract_automaton_t}. */
    virtual std::vector<state_t> initial_states() const = 0;

    /** Returns the final states of this {@link abstract_automaton_t}. */
    virtual std::vector<state_t> final_states() const = 0;

//     virtual std::vector<transition_t> final_transitions() const = 0;
//     virtual std::vector<transition_t> initial_transitions() const = 0;
    
    /** Returns the destinations of all transitions going out of @pname{s}. */
    virtual std::vector<state_t> successors(state_t s) const = 0;
    
    /** Returns the destinations of all transitions labelled by @pname{label} going out of @pname{s}.*/
    virtual std::vector<state_t> successors(state_t s, label_t label) const = 0;
    
    /** Returns the sources of all transitions coming in state @pname{s}. */
    virtual std::vector<state_t> predecessors(state_t s) const = 0;

    /** Returns the sources of all transitions labelled by @pname{label} that
     * are coming in state @pname{s}. */
    virtual std::vector<state_t> predecessors(state_t s, label_t label) const = 0;

    /** Alias to @ref outgoing(state_t, options_t)const "outgoing". */
    std::vector<transition_t> out(state_t s, options_t opts = {}) const
    { return outgoing(s, opts); }

    /** Alias to @ref outgoing(state_t, label_t)const. */
    std::vector<transition_t> out(state_t s, label_t label) const 
    { return outgoing(s, label); }

    /** Alias to @ref incoming(state_t, options_t)const "incoming". */
    std::vector<transition_t> in(state_t s, options_t opts = {}) const 
    { return incoming(s, opts); }


    /** Alias to @ref incoming(state_t,label_t)const. */
    virtual std::vector<transition_t> in(state_t s, label_t label) const 
    {
      return incoming(s,label);
    }

    /** Returns all transitions going from @pname{src} to @pname{dst}. */
    virtual std::vector<transition_t> outin(state_t src, state_t dst) const = 0;

    /** Returns the preinitial state.
     *
     *  See @ref PREPOST_PARADIGM for details.
     * */
    virtual state_t pre() const = 0 ;

    /** 
     * Returns the *postfinal* state. 
     *
     * See @ref PREPOST_PARADIGM for details.
     */
    virtual state_t post() const = 0 ;

//     virtual std::vector<state_t> all_states() const = 0; //including pre() and post()
//     virtual std::vector<transition_t> all_transitions() const  = 0; // including initial and final
//     virtual std::vector<transition_t> all_out(state_t s) const = 0; // including final
//     virtual std::vector<transition_t> all_in(state_t s) const = 0; // including initial

    /** Return the alphabet of this {@link abstract_automaton_t}. */
    virtual std::vector<label_t> alphabet() const = 0;

    /** Returns `true` if this {@link abstract_automaton_t} has multiple tapes. */
    virtual bool is_transducer() const =0;

    /** Returns `true` if the labelset of this {@link abstract_automaton_t} is
     * Z.*/
    virtual bool is_int_automaton() const =0;

    /** Returns `true` if epsilon transitions are allowed. */
    virtual bool is_eps_allowed() const = 0;

    /** Gets the context of this {@link abstract_automaton_t} */
    virtual context_t get_context() const = 0;

    /** @brieftag{experimental} Gives the kind of data this 
     * {@link abstract_automaton_t} comes from. 
     * @experimental 
     */
    virtual history_kind_t history_kind() const = 0;
    

    /** @brieftag{experimental} Gives the the origin of state @pname{s}, in the
     * case where it comes from a single state.
     * \attention This function assumes without check that the history kind is
     * {@link history_kind_t::SINGLE}. Use with caution.
     * @experimental 
     */
    virtual state_t origin_of(state_t s) const = 0;


    /** @brieftag{experimental} Gives the origin of state @pname{s}, in the
     * case where it comes from a tuple of states.
     * \attention This function assumes without check that the history kind is
     * {@link history_kind_t::TUPLE}. Use with caution.
     * @experimental 
     */
    virtual std::vector<state_t> origins_of(state_t s) const = 0;

    /** Deletes all state history. */
    virtual void strip_history() = 0;

    /** Gets the name of state @pname{s}.
        If the state has no name, one is generated (but not registered).

        @see has_explicit_name(state_t)
     */
    virtual std::string get_state_name(state_t s) const =0;

    /** Returns true if state @pname{s} has a name set */
    virtual bool has_explicit_name(state_t s) const =0;


    /** Sets to @pname{name} the name of @pname{s}. */
    virtual void set_state_name(state_t s, std::string name) =0;

    /** Gets one of the state with given name.
     * If there is no state with given name, a phony state is returned.
     * Use method {@link has_state} to test if a state is phony or not.
     */
    virtual state_t get_state_by_name(std::string name) const =0 ;

    /** @brieftag{experimental} Sets the name of each state based on its history. 
     * 
     * @experimental
     */
    virtual void set_state_names_from_history() =0;


    /** Returns the transitions coming in state \pname{s}.
     * @param s
     * @param opts A set of options;  only @ref PREPOST_PARADIGM is
     * meaningful.
     * @return The transitions in this automaton.
     * If @ref PREPOST_PARADIGM is `true` (default is `false`), the initial
     * transitions coming in @pname{s} are also returned. 
     */
    std::vector<transition_t> incoming(state_t s, options_t opts = {}) const 
    {
      return internal::incoming(this, s, opts[PREPOST_PARADIGM]);
    }
    
    /** Returns the transitions coming in state \pname{s} and labelled by
     * @pname{label}. */
    virtual std::vector<transition_t> incoming(state_t s, label_t label) const = 0;
    
    /** Returns the transitions going out ofstate \pname{s}.
     * @param s
     * @param opts A set of options;  only @ref PREPOST_PARADIGM is
     * meaningful.
     * @return The transitions in this automaton.
     * If @ref PREPOST_PARADIGM is `true` (default is `false`), the initial
     * transitions coming in @pname{s} are also returned. 
     */
    std::vector<transition_t> outgoing(state_t s, options_t opts = {}) const
    {
      return internal::outgoing(this, s, opts[PREPOST_PARADIGM]);
    }
    
    /** Returns the transitions going out of state \pname{s} and labelled by
     * @pname{label}. */
    virtual std::vector<transition_t> outgoing(state_t s, label_t label) const = 0;
    virtual ~abstract_automaton_t() {}
  };



}
}//end of ns awali::dyn

#endif
