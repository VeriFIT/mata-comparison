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

/* This file is a false header used to centralize the description of the
 * namespaces of Awali. 
 * Used in particular by Doxygen.
 */

/** @brief Main namespace of Awali */
namespace awali {

  /** @brief Namespace for the static layer of Awali. 
   *
   * Automata, rational expressions and algorithms are templated with the context.
   *
   * */
  namespace sttc {

    /** @brief Namespace containing all available contexts (weigh sets, label
     * sets, etc.).  
     */
    namespace ctx { }

    /** @brief Implementation details of static layer (not stable). */
    namespace internal { }



    /** @brief Namespace about static rational expressions. 
     */
    namespace rat { }

  }

  /** @brief Namespace that contains helper class and function to allow one
   * algorithm to be choosen at compile time depending on context, among a
   * sorted list of possibly available functions. 
    */
  namespace priority { }

  /** @brief Namespace that contains deprecated functions accessible for compatibility with earlier versions. 
    */
  namespace deprecated { }


  /** @brief Namespace for the dynamical layer of Awali.
   *
   * Templates types annotatin automa (among others) have been erased.
   * Each algorithm call an in a given module, possibly compiling it on the fly.
   * 
   * See also: 
   * - class \link abstract_automaton_t \endlink containing most methods for
   *   editing and browsing automata.  The type actually used is 
   *   @ref automaton_t, it is essentially a pointer to an 
   *   @ref abstract_automaton_t but also contains a few constructors as static
   *   method.
   * - class \link transducer_t \endlink containing the corresponding methods
   *   for transducer.
   * - class \link abstract_context_t \endlink containing methods for
   *   manipulating weights and labels.
   * - class @ref ratexp_t  
   * */
  namespace dyn {

    /** @brief Implementation details of dyn layer (not stable). */
    namespace internal { }

    /** @brief Namespace containtaing facilities for on-the-fly compilation */
    namespace loading { }

    /** @brief Namespace containing functions to build arbitrary automata
     * context (advanced and mostly undocumented).
    */
    namespace context { }


    /** @brief Namespace containing what we call *factories*, that is,
     * functions that generates a member in a family of automata, rational
     * expression or transducer.
     */
    namespace factory { }

    /** @brief Namespace containing the functions allowing to execute the
     * state elimination algorithm (aut_to_exp) step-by-step; probably will be moved elsewhere in the future.
     */
    namespace lift { }
  }

  /** @brief Namespace that contains deprecated functions accessible for compatibility with earlier versions. 
    */
  namespace deprecated { }

  /** @brief Namespace containing implementation details of elements common to
    * sttc and dyn layers (not stable).
    */
  namespace internal {  }


  /** @brief Namespace containing the C++ that simplify type for easier to Python. */
  namespace py {

  }


}
