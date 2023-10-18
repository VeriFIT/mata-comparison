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

/* This file is a false header used to define the differents groups of Doxygen.
 */


/** \defgroup Products Products
 *
 * This group contains the functions that take two (or more)
 * automata/transducers as input.
 *
 */

/** \defgroup Standard Standard
 *
 * Contains all functions related to standard automata.
 * 
 */

/** \defgroup Transducer Transducer
 * Contains functions related to transducers.
 */

/** \defgroup Options Options
 * Contains all enums describing the different algorithms to execute a
 * transformation, and the option mechanism used at dyn layer.
 *
 * # The options mechanism
 * The options mechanism emulates the named arguments paradigm that is included
 * in more recent languages.
 *
 * @example 
 * \code
 * options_t opts = {IN_PLACE = true, QUOTIENT_ALGO = MOORE};
 * opts += {MINIM_ALGO = 42} // This fails at runtime: a type check is done
 * opts += {MINIM_ALGO = BRZOZOWSKI } // This succeeds and is the prefered way
 * opts += {MINIM_ALGO = "brzozowski"}  // This succeeds but should only be used
 *                                      // if a std::string is given from an 
 *                                      // external source.
 * opts += {MINIM_ALGO = "brzzwski"} // This fails with a std::domain_error
 * \endcode
 *
 *
 */

/** \defgroup Factories Factories & Constructors
 *
 * Contains all functions that allows to construct automata and expressions.
 */


/** \defgroup Quotient Quotient
 *
 *  Contains functions related to automata/transducer quotient (in particular,
 *  minimisation.
 */
