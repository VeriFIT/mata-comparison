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

#ifndef AWALI_PARSE_EXCEPTION_HH
#define AWALI_PARSE_EXCEPTION_HH

#include<stdexcept>
#include<istream>

namespace awali {
  /** Exceptions thrown during parsing
   */
  struct parse_exception : public std::runtime_error {
    /** Simple constructor
     *
     * @param s the message of the exception
     */
    parse_exception(const std::string& s) : std::runtime_error(s) {}

    /** Exception with location
     *
     * This constructor is devoted to exceptions which occur during
     * the parsing of a stream.
     * The constructor computes the position (line + column)
     * of the stream \a i and adds this information before the message \a s 
     *
     * @param i the stream on which the exception is thrown
     * @param s the message of the exception
     */
    parse_exception(std::istream& i, const std::string& s);

    /** Exception with location
     *
     * This constructor is devoted to exceptions which occur during
     * the parsing of a string.
     * The constructor adds the position \a p before the message \a s 
     *
     * @param p the position on which the exception is thrown
     * @param s the message of the exception
     */
    parse_exception(size_t p, const std::string& s);
  };
  
}//end of ns awali::stc

#endif
