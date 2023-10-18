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

#ifndef DYN_FACTORIES_HH
#define DYN_FACTORIES_HH

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/loading/handler.hh>
#include <vector>

namespace awali {
  namespace dyn {
    namespace factory {
    /** \ingroup Factories
     *  @brief Returns an automaton which recognizes words with a specific n-ultimate letter
     *
     * The factory builds an automaton with \p n +1 states
     * that recognizes a language over the \p alphabet of char.   
     * The alphabet must contain at least two letters.
     * The order of letters in \p alphabet is significant:
     * the automaton accepts every word where the n-ultimate letter
     * is the first letter of \p alphabet.
     *
     * The automaton is co-deterministic hence unambiguous;
     * a \p semiring for weights can be specified
     * every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     *
     * @param n a positive integer: the position to last of the specified letter
     * @param alphabet a list of letters
     * @param semiring the description of a weightsed
     * @return an automaton with \p n +1 states
     * @throw runtime_error if \p n is not positive or if \p alphabet has less than 2 letters
     **/
    automaton_t n_ultimate(unsigned n, const std::string& alphabet, const std::string& semiring="B");

    /** \ingroup Factories
     * @brief Returns an automaton which recognizes numbers in base \p b which are multiple of \p k
     *
     * The factory builds a deterministic automaton with \p k states over an \p alphabet of char.
     * The letters of \p alphabet are sorted, and the letter with index *i* represents the digit *i*. \p alphabet must contain at least \p b letters. 
     * If the alphabet has more than \p b letters, the larger letters do not label any transition.    
     * If \p alphabet is set as "auto", an alphabet of digits is automatically
     * built, capital letters are used for digits larger than 9; 
     * the base must be at most 36.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     *
     * @param k a positive integer
     * @param b a positive integer
     * @param alphabet an unordered list of letters
     * @param semiring the description of a weightsed
     * @return an automaton with \p k states
     * @throw runtime_error if \p k is not positive,
     *                   or if \p b is smaller than 2,
     *                   or if \p alphabet has less than \p b letters,
     *                   or if \p alphabet is "auto" and \p b is larger than 36
     * @see int_divkbaseb for the factory with int letters.
     **/
    automaton_t divkbaseb(unsigned k, unsigned b, const std::string& alphabet="auto", const std::string& semiring="B");

    /** \ingroup Factories
     * @brief Returns a double ring automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z.      
     * For every state p,      
     * - the first letter of the \p alphabet labels transitions from p to p+1;    
     * - the second one labels transitions from p to p-1;
     * - if the \p alphabet has more than 2 letters, the other letters do not label any transition.
     *
     * State 0 is initial; \p finals is the list of the final states.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * @param n a positive integer
     * @param finals a vector of states
     * @param alphabet a list of letters
     * @param semiring the description of a weightsed
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is not positive
     *                   or if \p alphabet has less than 2 letters
     **/
    automaton_t double_ring(unsigned n, const std::vector<unsigned>& finals, const std::string& alphabet, const std::string& semiring="B");

    /** \ingroup Factories
     * @brief Returns a "ladybird" automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z.    
     * For every state p,     
     * - the first letter of the alphabet labels transitions from p to p+1;    
     * - the second letter of the alphabet labels transitions from p to p;    
     * - the third letter of the alphabet labels transitions from p to p and from p to 0;   
     * -- If the alphabet has more than 3 letters, the other letters do not label any transition.
     * State 0 is both initial and final.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * The minimal automaton of the language accepted by ladybird \p n has 2^\p n states.    
     * The name ladybird comes from the shape of the 6-state automaton.
     *
     * @param n a positive integer
     * @param alphabet an unordered list of letters
     * @param semiring the description of a weightsed
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is not positive
     *                   or if \p alphabet has less than 3 letters
     **/
    automaton_t ladybird(unsigned n, const std::string& alphabet, const std::string& semiring="B");

    /** \ingroup Factories
     * @brief Returns an automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z.    
     * For every state p,     
     * - the first letter of the \p alphabet labels transitions from p to p+1;    
     * - the other letters label transitions from p to
     *       - 0 if p =\p n-1,
     *       - \p n -1 otherwise;
     *
     * State 0 is both initial and final.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * The shortest synchronization word for this automaton has (\p n -1)^2 letters
     *
     * @param n a positive integer
     * @param alphabet a list of letters
     * @param semiring the description of a weightsed
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is not positive
     **/
    automaton_t cerny(unsigned n, const std::string& alphabet, const std::string& semiring="B");
    
    /** \ingroup Factories
     * @brief Returns an automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z.    
     * For every state p,     
     * - the first letter of the \p alphabet labels transitions from p to p+1;    
     * - the second letter of the \p alphabet labels transitions from p to    
     *        - 1 if p=0,
     *        - 0 if p=1,
     *        - p otherwise;
     * - the third letter  of the \p alphabet labels transitions from p to    
     *        - 0 if p=\p n -1,
     *        - p otherwise;
     * - the other letters do not label any transition.
     *      
     * State 0 is initial, state n-1 is final.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * This automaton is the "universal witness" described in     
     * Janusz A. Brzozowski and David Liu,
     * *Universal Witnesses for State Complexity of Basic Operations Combined with Reversal*,     
     *  CIAA 2013,
     * Lecture Notes in Computer Science, vol. 7982, pp. 72-83.   
     * doi : 10.1007/978-3-642-39274-0_8
     *
     * @param n an integer larger than 1
     * @param alphabet a list of at least 3 letters
     * @param semiring the description of a weightsed
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is lesser than 2
     *                   or if \p alphabet has less than 3 letters
     **/
    automaton_t witness(unsigned n, const std::string& alphabet, const std::string& semiring="B");
    
    /*,------------------.
      | with int letters |
      `------------------'*/
    
    /** \ingroup Factories
     * @brief returns an automaton which recognizes words with a specific n-ultimate letter
     *
     * The factory builds an automaton with \p n +1 states
     * that recognizes a language over the alphabet of integers [\p a, \p b].   
     * The alphabet must contain at least two letters.
     * The automaton accepts every word where the n-ultimate letter
     * is \p a.   
     *
     * The automaton is co-deterministic hence unambiguous;
     * a \p semiring for weights can be specified
     * every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     *
     * @param n a positive integer: the position to last of the specified letter
     * @param a an integer
     * @param b an integer larger than a
     * @param semiring the description of a weightsed
     * @return an automaton with \p n+1 states
     * @throw runtime_error if \p n is not positive or if \p b - \p a is smaller than 1.
     **/
    automaton_t n_ultimate(unsigned n, int a, int b, const std::string& semiring="B");


    /** \ingroup Factories
     * @brief returns an automaton which recognizes numbers in base \p b which are multiple of \p k
     *
     * The factory builds a deterministic automaton with \p k states over the alphabet [0; \p b].
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     *
     * @param k a positive integer
     * @param b a positive integer
     * @param semiring the description of a weightsed
     * @return an automaton with \p k states
     * @throw runtime_error if \p k is not positive,
     *                   or if \p b is smaller than 2
     **/
    automaton_t int_divkbaseb(unsigned k, unsigned b, const std::string& semiring="B");

    /** \ingroup Factories
     * @brief Returns a double ring automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z,
     * over the alphabet {\p a,\p a+1}.       
     * The first letter of the alphabet labels transitions from p to p+1 for every state p, the second one labels transitions from p to p-1.
     * State 0 is initial; \p finals is the list of the final states.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * @param n a positive integer
     * @param finals a vector of states
     * @param a an integer
     * @param semiring the description of a weightsed
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is not positive
     **/
    automaton_t double_ring(unsigned n, const std::vector<unsigned>& finals, int a, const std::string& semiring="B");


    /** \ingroup Factories
     * @brief Returns an "ladybird" automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z,
     * over the alphabet {\p a, \p a +1, \p a +2}.
     * For every state p,     
     * - the first letter of the alphabet labels transitions from p to p+1;    
     * - the second letter of the alphabet labels transitions from p to p;    
     * - the third letter of the alphabet labels transitions from p to p and from p to 0;   
     * State 0 is both initial and final.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * The minimal automaton of the language accepted by ladybird \p n has 2^\p n states.    
     * The name ladybird comes from the shape of the 6-state automaton.
     *
     * @param n a positive integer
     * @param a an integer
     * @param semiring the description of a weightsed
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is not positive
     **/
    automaton_t ladybird(unsigned n, int a, const std::string& semiring="B");


    /** \ingroup Factories
     * @brief Returns an automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z,
     * over the alphabet [\p a, \p b].      
     * For every state p,     
     * - the first letter of the alphabet labels transitions from p to p+1;    
     * - the other letters label transitions from p to
     *       - 0 if p =n-1,
     *       - \p n -1 otherwise;
     *
     * State 0 is both initial and final.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * The shortest synchronization word for this automaton has (\p n -1)^2 letters
     *
     * @param n a positive integer
     * @param a an integer
     * @param b an integer larger than \p a
     * @param semiring the description of a weightsed
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is not positive
     **/
    automaton_t cerny(unsigned n, int a, int b, const std::string& semiring="B");
    
    /** \ingroup Factories
     * @brief Returns an automaton with \p n states
     *
     * The factory builds an automaton with \p n states with indices in Z/\p n Z,
     * over the alphabet {\p a, \p a +1, \p a +2}.
     * For every state p,     
     * - the first letter of the alphabet labels transitions from p to p+1;    
     * - the second letter of the alphabet labels transitions from p to    
     *        - 1 if p=0,
     *        - 0 if p=1,
     *        - p otherwise;
     * - the third letter of the alphabet labels transitions from p to    
     *        - 0 if p=\p n -1,
     *        - p otherwise;
     * - the other letters do not label any transition.

     * State 0 is initial, state n-1 is final.
     *
     * Every transition is created with weight one; thus, if weighted,
     *  the automaton realizes the characteristic series of the language described above.
     * 
     * This automaton is the "universal witness" described in     
     * Janusz A. Brzozowski and David Liu,
     * *Universal Witnesses for State Complexity of Basic Operations Combined with Reversal*,     
     *  CIAA 2013,
     * Lecture Notes in Computer Science, vol. 7982, pp. 72-83.   
     * doi : 10.1007/978-3-642-39274-0_8
     *
     * @param n an integer larger than 1
     * @param a an integer
     * @param semiring the description of a weightsed
     * @return an automaton with \p n states
     * @throw runtime_error if \p n is lesser than 2
     **/
    automaton_t witness(unsigned n, int a, const std::string& semiring="B");
   }// end of ns awali::dyn::factory
  } // end of ns awali::dyn
}//end of ns awali

#endif
