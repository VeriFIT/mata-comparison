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

#ifndef DYN_AUTOMATON_HH
#define DYN_AUTOMATON_HH

#include <awali/dyn/core/abstract_automaton.hh>

namespace awali { namespace dyn {


    /** 
   An {@link automaton_t} is essentially a shared pointer to an 
   {@link abstract_automaton_t}, but also contains static functions
   serving as constructors.
   
   See the documentation of 
   {@link abstract_automaton_t} for help on the methods on automata.
   
   @example (Building an automaton)
   \code
    automaton_t aut = automaton_t::from("ab"); // Builds an empty automaton over the alphabet 
    
    state_t s = aut->add_state(); // adding two states
    state_t t = aut->add_state(); 

    aut->set_initial(s);  // making state `s` initial
    aut->set_final(t);    //making state `t` final

    aut->set_transition(s,s,'b'); // A loop labelled by 'b' on both states
    aut->set_transition(t,t,'b');

    aut->set_transition(s,t,'a'); // Reading an 'a' changes the states
    aut->set_transition(t,s,'a'); 

    //aut now recognizes the words over {a,b} with an odd number of 'a's.
    
    pdfdisplay(aut); // Outputs `aut` to a temporary file as a pdf image
                     // and opens it with the default pdf viewer.
   \endcode

   @example (Building an automaton from an expression)
   \code
     ratexp_t exp = ratexp_t::from("(a+b)*bb(a+b)*");
     automaton_t aut1 = exp_to_aut(exp);
     aut1 = minimal_automaton(aut1); // determinizes and minimizes `aut`

     // The same result may be obtained directly from the expression.
     automaton_t aut2 = minimal_automaton(exp);

     std::cout << are_isomorphic(aut1,aut2) << std::endl; // outputs 1
   \endcode

  @example (Building a weighted automaton)
  \code
    // Builds an automaton with transitions labelled in {a,b} and weighted in Z.
    automaton_t aut = automaton_t::from("ab","Z", false); 
    
    state_t s = aut->add_state();
    state_t t = aut->add_state();

    aut->set_initial(s, 1); // Set initial weight of `s` to 1
    aut->set_final(t, 1); // Set final weight of `t` to 1

    aut->set_transition(s,s,'a',1);
    aut->set_transition(s,s,'b',1);
    aut->set_transition(t,t,'a',1);
    aut->set_transition(t,t,'b',1);
    aut->set_transition(s,t,'b',1);

    // `aut` is an automaton that associate with every word the number of b' in it.
    int i = (int) eval(aut,"bbbaaabbaaab"); // `i` equals 6

    save(aut,"count-b.json"); // Saves `aut` in JSON format to file "count-b.json"
  \endcode

   See also group {@link Factories}.
   
     */
    class automaton_t : public std::shared_ptr<abstract_automaton_t> {
    public:
      
      
      /** Buils an automaton_t that is essentially a `nullptr`; should
       * generally not be used. 
       */
      automaton_t();

    /** Builds a automaton from a shared pointer to a class derived
     * of {@link abstract_automaton_t}; should generally not be
     * used. 
     *
     *  Both @pname{ptr} and the built {@link automaton_t} will share ownership
     *  of pointed object.
     */
      template <class T>
      automaton_t(
        const std::shared_ptr<T> &ptr,
        typename std::enable_if<
          std::is_base_of<abstract_automaton_t,T>::value,int
        >::type = 0 
      ) 
        : std::shared_ptr<abstract_automaton_t>(ptr)
      {}

      /** \ingroup Factories
       * Builds a new automaton whose labelset and weightset are given by
       * @pname{ctx}.
       *
       * This function is mostly useful for copying context, as in example
       * below.
       * @example
         \code
           automaton_t aut = load("a1"); // Here, aut is the automaton that is
                                         // over the context we want to copy
           context_t ctx = aut->get_context();
           automaton_t aut2 = automaton_t::from_context(ctx);
           // aut2 is an empty automaton with the same labelset/weightset as aut
         \endcode
       *
       * @param ctx Context of automaton given
       * @return a new empty automaton
       * @beware 
       * Although the return type is {@link automaton_t}, 
       * this might return a transducer depending on @pname{ctx}.
       * If you know that this will produce a transducer, just store it in 
       * a variable of type {@link transducer_t}:
       * {@code transducer_t tdc = automaton_t::from_context(ctx); }
       */
      static automaton_t from_context(context_t ctx);
      

      /** \ingroup Factories
       *  Builds a new empty automaton whose labelset is described by 
       *  @pname{ld} and weightset by @pname{wd}.
       *
       *  Experts only.
       *
       * This function is the most powerful constructor provided by the dyn
       *  layer of Awali.  It allows to combine almost arbitrarily all
       *  implemented weightset and labelsets.
       *  @example
          \code
            std::vector<context::labelset_description> v = 
              {
                context::letterset("ab"),
                context::nullableset(context::intletterset(12,14)),
                context::wordset("bcd")
              };
            automaton_t aut = automaton_t(context::ltupleset(v),
                                          context::weightset("Z"));
            // aut is a transducer with three tapes:
            // - first tape holds a letter in {a,b} and no epsilon
            // - second tape holds an integer in {12,13,14} or an epsilon
            // - third tape holds a word over {b,c,d}
            // Each triplet is weighted by a relative integer.
          \endcode
       *
       * @param ld A dynamical description of a labelset
       * @param wd A dynamical description of a weightset
       * @return a new empty automaton
       * @beware 
       * Although this is a constructor for type {@link automaton_t}, 
       * this might return a transducer depending on @pname{ld}
       * (as in example above.)
       * If you know that this will produce a transducer, just store it in 
       * a variable of type {@link transducer_t}:
       {@code transducer_t tdc = automaton_t(ld,wd);}
       * @see Documentation of namespace {@link dyn::context}
       */
      automaton_t(context::labelset_description ld,
                  context::weightset_description wd);
      
      /** \ingroup Factories
       * Builds an automaton with labels in given @pname{alphabet} possibly
       * allowing epsilon transitions, and with weights in given
       * @pname{weightset}.
       *  
       * @param alphabet String representation of the alphabet of the returned
       * automaton: each char is a letter of the automaton.
       *
       * @param weightset String representation of the weightset of the built
       * automaton.
       *
       * @param allow_eps_transitions Whether returned automaton should allow
       * epsilon-transitions.  Be aware of the fact that automata with
       * epsilon-transitions and automata without epsilon-transitions have
       * different static types.  Post construction, allowing/disallowing
       * epsilon-transitions always requires a complete copy of the automaton.
       *
       * @return A new empty automaton.
       */
      static automaton_t from(std::string alphabet, std::string weightset = "B",
                              bool allow_eps_transitions = false);
      
      /** \ingroup Factories
       * Builds a boolean automaton over given \p alphabet, that possibly
       * allows eps_transitions.
       * @param alphabet String representation of the alphabet of the returned
       * automaton: each char is a letter of the automaton.
       *
       * @param allow_eps_transitions Whether returned automaton should allow
       * epsilon-transitions. Beware of the fact that automata with
       * epsilon-transitions and automata without epsilon-transitions have
       * different static types.  Post construction, allowing/disallowing
       * epsilon-transitions always requires a complete copy of the automaton.
       * @note 
       * This function simply calls: 
       * \code from(alphabet, "B", allow_eps_transition) \endcode
       */
      static automaton_t from(std::string alphabet, bool allow_eps_transitions,
      std::string = "B");


      /** \private
       *  This function exists because C++ would rather cast `char const*`
       *  to bool rather than std::string.
       */
      static automaton_t from(std::string alphabet, char const* ws);

      /** 
       * Helper class that contains convenience constructor for automata with
       * int labels.
       *
       * @example
       * \code
       *   // Builds an automaton with labels in {0,1,2,3,4} .
       *   automaton_t aut = automaton_t::with_int_labels::from_size(5); 
       * \endcode
       * \code
       *   // Builds an automaton with labels in {2,3,4,5}.
       *   automaton_t aut = automaton_t::with_int_labels::from_range(2,5); 
       * \endcode
       * \code
       *   // Builds an automaton with labels in {2,3,4,5}, and with weights in Z.
       *   automaton_t aut = automaton_t::with_int_labels::from_range(2, 5, "Z"); 
       * \endcode
       *
       * @beware
       * This is an abstract class.  Do not try to instantiate.
       */
      struct with_int_labels {
       private:
        with_int_labels() {}
       public:


        /** \ingroup Factories 
         *  Builds an automaton with alphabet {@pname{l}, @pname{l}+1, ...,
         *  @pname{u}}
         *  and weightset  @pname{weightset}
         *  @param l Lower bound of the range.
         *  @param u Upper bound of the range.
         *  @param weightset
         *  @param allow_eps_transitions If set to `true`, built automaton
         *  allows epsilon-transitions.
         *  @return A new empty automaton.
         */
        static automaton_t from_range(int l, int u, std::string weightset = "B",
                                      bool allow_eps_transitions = false);
  

        /** \ingroup Factories
         *  Builds an automaton with alphabet {@pname{l}, @pname{l}+1, ...,
         *  @pname{u}}, possibly allowing epsilon transitions.
         *
         *  
         *  @param l Lower bound of the range
         *  @param u Upper bound of the range
         *  @param allow_eps_transitions If `true`, built automaton allows
         *  epsilon-transitions.
         *
         *  @return A new empty automaton with transitions labelled over
         *  integers.
         */
        static automaton_t from_range(int l, int u, bool allow_eps_transitions);


        /** \private
         *  This function exists because C++ would rather cast `char const*`
         *  to bool rather than std::string.
         */
        static automaton_t from_range(int l, int u, char const* ws);


        /** \ingroup Factories
         *  Builds an automaton with alphabet {0, 1, ..., \p n-1} and
         *  weightset @pname{weightset}.
         *
         *  @param n size of the alphabet
         *  
         *  @param weightset String representation of the weightset of the built
         *  automaton
         *
         *  @param allow_eps_transitions If set to `true`, built automaton
         *  allows epsilon-transitions.
         *  
         *  @return a new empty automaton.
         */
        static automaton_t from_size(unsigned n, 
                                          std::string weightset = "B",
                                          bool allow_eps_transitions = false);

        /** \ingroup Factories
         *  Builds a Boolean automaton with alphabet {0, 1, ..., @pname{n}-1},
         *  possibly allowing epsilon transitions.
         *
         *  @param n size of the alphabet
         *  
         *  @param allow_eps_transitions If set to `true`, built automaton
         *  allows epsilon-transitions.
         *  
         *  @return a new empty automaton.
         */
        static automaton_t from_size(unsigned n, bool allow_eps_transitions);
        
        /** \private
         *  This function exists because C++ would rather cast `char const*`
         *  to bool rather than std::string.
         */
        static automaton_t from_size(unsigned n, char const* weightset);

      };
    };


}}


#endif
