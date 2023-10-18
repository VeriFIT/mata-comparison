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

#ifndef AWALI_STTC_LR_PARSE_NUMBER_HH
#define AWALI_STTC_LR_PARSE_NUMBER_HH

#include<complex>
#include<iostream>

#include<awali/common/parse_exception.hh>
#include<awali/common/qfraction.cc>


/* In this file, there is two kind of functions.
 * - lr_scan_..(s,p,..) looks how many characters left of position p in string s
 *   can be eaten to the described purposes.
 *   It returns the number of characters found.
 *   Parameter p, a reference to an int, is decreased by that number.
 *
 * - lr_parse_<type>   parses from left to right an instance of <type>
 *   Generally does not allow the empty string to be parsed but each function
 *   usually allows two optional parameters `allow_empty` and `value_if_empty`
 *   to control this behavior.
 */

namespace awali {
namespace sttc {
namespace internal {

  /** Scans for a character in @pname{t} left of position @pname{p} in
   *  @pname{s}.
   * Returns 1 if both (p-1) is nonnegative and s[p-1] is in c-string
   * @pname{t}; or returns 0 otherwise.
   * Parameter @pname{p} is modified accordingly.
   * @return the number of characters scanned (0 or 1)
   */
  unsigned
  lr_scan_one(std::string const& s, size_t& p, char const* t)
  {
    if (p<=0)
      return 0;
    char c;
    for (unsigned x = 0; (c = t[x]) != '\0'; x++)
      if (c == s[p-1]) {
        --p;
        return 1;
      }
    return 0;
  }


  /** Checks whether there is in \pname{t} left of position @pname{p} in
   *  @pname{s}.
   *  If a character is found, @pname{p} is modified accordingly (i.e.,
   *  decreased by 1) and the found character is returned.
   *  If no such character is found: if @pname{allow_empty} is `true` then
   *  returns @pname{value_if_empty} and @pname{p} is not modified, otherwise,
   *  an exception is raised.
   *  @return The found char or @pname{value_if_empty} if none was found and
   *  @pname{allow_empty} is true.
   */
  char
  lr_parse_one(std::string const& s, size_t& p, char const* t,
               bool allow_empty = false, char value_if_empty = '\0')
  {
    if (lr_scan_one(s,p,t))
      return s[p];
    else if (allow_empty)
      return value_if_empty;

    std::string s2;
    if (p<=6)
      s2 = s.substr(p);
    else
      s2 = ".."+s.substr(p-4,4);
    throw parse_exception("Could not parse a char in \"" + std::string(t)
                          + "\", right of position " + std::to_string(p)
                          + " in string \"" + s
                          + "\", i.e., starting from the right of \"" + s2
                          + "\".");
  }


  /** Scans for as many [0-9] digits as possible, left of position @pname{p}
   * in string @pname{s}.
   * Parameter @pname{p} is decreased by the amount of character scanned.
   * @param s String to scan
   * @param p Position left of which the scan starts.
   * @return the number of digits found (might be 0).
   */
  unsigned
  lr_scan_digits(std::string const& s, size_t& p)
  {
    int i = p;
    while (i>0 && std::isdigit(static_cast<unsigned char>(s[i-1])))
      --i;
    unsigned res = (p-i);
    p = i;
    return res;
  }
  

  /** Reads an unsigned int written in decimal left of position
   * @pname{p} in string @pname{s}, by using as many character as possible
   * Parameter @pname{p} is decreased by the number of characters thus consumed.
   *
   * @param s String in which the uint is read
   * @param p Position left of which the uint is read ; decreased by
   *        the number of character effectively used.
   * @param allow_empty Whether this function is allowed to read 0 characters;
   *        if set to `false` this function raises a {@link parse_exception}
   *        if it cannot read at least one valid character.
   * @param value_if_empty value returned if no characters and
   *        @pname{allow_empty} is `true`.
   * @return the parsed uint
   */
  unsigned
  lr_parse_uint(std::string const& s, size_t& p, bool allow_empty=false, 
                unsigned value_if_empty = 0)
  {
    size_t i = p;
    if (lr_scan_digits(s,i)) {
      std::stringstream ss (s.substr(i,p-i));
      unsigned x;
      ss >> x;
      p=i;
      return x;
    }
    if (allow_empty)
      return value_if_empty;
    std::string s2;
    if (p<=6)
      s2 = s.substr(p);
    else
      s2 = ".."+s.substr(p-4,4);
    throw parse_exception("Could not parse an unsigned integer right of position "
                          + std::to_string(p)+" in string \"" + s
                          + "\", i.e., starting from the right of \"" + s2
                          + "\".");
  }


  //
  unsigned
  lr_scan_sign(std::string const& s, size_t& p)
  {
    return lr_scan_one(s,p, "+-");
  }
  
  char
  lr_parse_sign(std::string const& s, size_t& p, bool allow_empty=true, 
                char value_if_empty='\0')
  {
    return lr_parse_one(s,p, "+-", allow_empty, value_if_empty);
  }


  unsigned
  lr_scan_e(std::string const& s, size_t& p)
  {
    return lr_scan_one(s,p,"eE");
  }
  
  /** Parses the radix point. */
  char
  lr_parse_e(std::string const& s, size_t& p, bool allow_empty=true, 
               char value_if_empty='\0') 
  {
    return lr_parse_one(s,p,"eE", allow_empty, value_if_empty);
  }


  unsigned
  lr_scan_dot(std::string const& s, size_t& p) {
    return lr_scan_one(s,p,".");
  }
  

  char
  lr_parse_dot(std::string const& s, size_t& p, bool allow_empty=true, 
  char value_if_empty='\0') {
    return lr_parse_one(s,p,".", allow_empty, value_if_empty);
  }


//   unsigned
//   lr_scan_decimal(std::string const& s, size_t& p)
//   {
//     unsigned res = 0;
//     if (res += lr_scan_digits(s,p)) {
//       if (res += lr_scan_dot(s,p))
//         res += lr_scan_digits(s,p);
//       res += lr_scan_sign(s,p);
//       return res;
//     }
//     else
//       return res;
//   }


  unsigned
  lr_scan_int(std::string const& s, size_t& p)
  {
    unsigned res = 0;
    if (res += lr_scan_digits(s,p))
      return (res+lr_scan_sign(s,p));
    else
      return false;
  }

  /** Reads an int written in decimal left of position @pname{p} in string
   * @pname{s}, by using as many characters as possible
   * Parameter @pname{p} is decreased by the number of characters thus consumed.
   *
   * @param s String in which the int is read
   * @param p Position left of which the int is read; decreased by
   *        the number of character effectively used.
   * @param allow_empty Whether this function is allowed to read 0 characters;
   *        if set to `false` this function raises a {@link parse_exception}
   *        if it cannot read at least one valid character.
   * @param value_if_empty Value returned if no character was consumes and
   *        @pname{allow_empty} is `true`.
   * @return the parsed int
   */
  int
  lr_parse_int(std::string const& s, size_t& p, bool allow_empty=false,
               int value_if_empty = 0)
  {
    size_t i = p;
    if (lr_scan_int(s,i)) {
      std::stringstream ss (s.substr(i,p-i));
      int x;
      ss >> x;
      p=i;
      return x;
    }
    if (allow_empty)
      return value_if_empty;
    std::string s2;
    if (p<=6)
      s2 = s.substr(p);
    else
      s2 = ".."+s.substr(p-4,4);
    throw parse_exception("Could not parse an integer right of position "
                          + std::to_string(p)+" in string \"" + s
                          + "\", i.e., starting from the right of \"" + s2
                          + "\".");
  }


  unsigned
  lr_scan_double(std::string const& s, size_t& pp)
  {
    size_t p = pp;
    bool right_digits = lr_scan_digits(s,p);
    if (right_digits)
      lr_scan_sign(s,p);
    bool e = lr_scan_e(s,p);
    if (e) {
      if (!right_digits)
        return 0;
      else
        right_digits = lr_scan_digits(s,p);
    }
    bool dot = lr_scan_dot(s,p);
    bool left_digits = dot ? lr_scan_digits(s,p) : false;
    if (right_digits || dot || left_digits) {
      lr_scan_sign(s,p);
      unsigned res = pp-p;
      pp=p;
      return res;
    }
    return 0;
  }


  unsigned
  lr_scan_complex(std::string const& s, size_t& pp)
  {
    size_t p = pp;
    bool imag = lr_scan_one(s,p,"i");
    bool sign = false;
    if (lr_scan_double(s,p)) {
      if (s[p] == '+' || s[p] == '-')
        sign = true;
    }
    else {
      if (imag) {
        if(lr_scan_sign(s,p))
          sign = true;
      }
      else /* We failed to parse anything */
        return 0;
    }

    if (sign) {
      if ( imag || lr_scan_one(s,p,"i") )
        if ( !lr_scan_double(s,p) )
          lr_scan_sign(s,p);
    }
    unsigned res = pp-p;
    pp=p;
    return res;
  }


  static unsigned
  powten(unsigned i) {
    if (i == 0)
      return 1;
    unsigned x = powten(i/2);
    x = x*x;
    if (i%2)
      return x;
    else
      return x*10;
  }


  /** Reads a {@link q_fraction_t} left of position @pname{p} in string, by
   * using as many characters as possible.
   * Acceptable format:
   * - `<int> / <uint>`
   * - `<int> . <uint>`
   * Parameter @pname{p} is decreased by the number of characters used.
   *
   * @param s String in which the {@link q_fraction_t} is read.
   * @param p Position left of which the {@link q_fraction_t} is read;
   *        it is decreased by the number of character effectively used.
   * @param allow_empty Whether this function is allowed to read 0 characters;
   *        if set to `false` this function raises a {@link parse_exception}
   *        if it cannot read at least one valid character.
   *        Default is `false`
   * @param value_if_empty Value returned if no character was consumes and
   *        @pname{allow_empty} is `true`.
   *        Default is 1.
   * @return The parsed {@link q_fraction_t}.
   */
  q_fraction_t
  lr_parse_qfraction(std::string const& s, size_t& p, bool allow_empty=false,
                     q_fraction_t value_if_empty = {1,1})
  {
    size_t pp = p;
    q_fraction_t::den_t den = lr_parse_uint(s, pp, true);
    q_fraction_t::num_t num = 1;
    if ((p!=pp) && lr_parse_one(s, pp, "/", true))
      num = lr_parse_int(s,pp);
    else { /* No denominator */
      num = den;
      den = 1u;
      if (lr_parse_sign(s,pp,true)) { /* integer */
        if (s[pp] == '-')
          num = -num;
      }
      else if (lr_parse_dot(s, pp, true)) { /* decimal */
        den = pow(10,p-(pp+1));
        num = num + den*lr_parse_int(s,pp,true);
      }
    }

    if (p != pp) {
      p = pp;
      return {num,den};
    }

    if (allow_empty)
      return value_if_empty;
    std::string s2;
    if (p<=6)
      s2 = s.substr(p);
    else
      s2 = ".."+s.substr(p-4,4);
    throw parse_exception("Could not parse a q fraction right of position"
    + std::to_string(p) + " in string \"" + s +
    "\", i.e., starting from the right of \"" + s2 + "\".");
  }


  /** Reads a double left of position @pname{p} in string, by using as many
   * characters as possible.
   * Parameter @pname{p} is decreased by the number of characters used.
   *
   * @param s String in which the double is read.
   * @param p Position left of which the double is read;
   *        it is decreased by the number of character effectively used.
   * @param allow_empty Whether this function is allowed to read 0 characters;
   *        if set to `false` this function raises a {@link parse_exception}
   *        if it cannot read at least one valid character.
   *        Default is `false`.
   * @param value_if_empty Value returned if no character was consumed and
   *        @pname{allow_empty} is `true`.
   *        Default is 1.0.
   * @return The parsed double.
   */
  double
  lr_parse_double(std::string const& s, size_t& p, bool allow_empty=false,
                  double value_if_empty = 0)
  {
    size_t i = p;
    if (lr_scan_double(s,i)) {
      std::stringstream ss (s.substr(i,p-i));
      double d;
      ss >> d;
      p=i;
      return d;
    }
    if (allow_empty)
      return value_if_empty;

    std::string s2;
    if (p<=6)
      s2 = s.substr(0,p);
    else
      s2 = ".."+s.substr(p-4,4);
    throw parse_exception("Could not parse a floating point number right of"
                          " position " + std::to_string(p) + " in string \""
                          + s + "\", i.e., starting from the right of \"" + s2
                          + "\".");
  }


  /** Reads a complex number left of position @pname{p} in string, by using as
   * many characters as possible.
   * Acceptable format:
   * - `<double>`
   * - `<double> i` 
   * - `<double> [+|-] <double without sign or empty> i`
   * - `<double or empty> i [+|-] <double without sign>`
   *
   * Parameter @pname{p} is decreased by the number of characters used.
   *
   * @param s String in which the complex number is read.
   * @param p Position left of which the complex number is read;
   *        it is decreased by the number of character effectively used.
   * @param allow_empty Whether this function is allowed to read 0 characters;
   *        if set to `false` this function raises a {@link parse_exception}
   *        if it cannot read at least one valid character.
   *        Default is `false`.
   * @param value_if_empty Value returned if no character was consumed and
   *        @pname{allow_empty} is `true`.
   *        Default is 1.0.
   * @return The parsed complex number.
   */
  std::complex<double>
  lr_parse_complex(std::string const& s, size_t& p, bool allow_empty=false,
                  std::complex<double> value_if_empty = {0,0})
  {
    size_t i = p;
    if ( lr_scan_complex(s,i) ) {
      size_t j = p;
      double imag = 0;
      double real = 0;
      for (int x = 0; (x<2 && j>i) ; x++) {
        if (lr_scan_one(s,j,"i")) { /* Parsing the imaginary part*/
          size_t k = j;
          imag = lr_parse_double(s,j, true, 1); // We allow empty string which
                                                // means value 1.0
          if (k == j && lr_scan_sign(s,j) && s[j] == '-')  // treats the case
            imag = -1*imag;                                // "+i" and "-i"
        }
        else
          real = lr_parse_double(s,j);
      }
      p=i;
      return {real,imag};
    }
    if (allow_empty)
      return value_if_empty;

    std::string s2;
    if (p<=6)
      s2 = s.substr(p);
    else
      s2 = ".."+s.substr(p-4,4);
    throw parse_exception("Could not parse a complex number right of position "
                          + std::to_string(p) + " in string \"" + s
                          + "\", i.e., starting from the right of \"" + s2
                          + "\".");
  }



} // end of namespace awali::sttc::internal
} // end of namespace awali::sttc
} // end of namespace awali

#endif // AWALI_STTC_LR_PARSE_NUMBER_HH
