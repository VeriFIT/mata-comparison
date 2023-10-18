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

#ifndef COMMON_QFRACTION_CC
#define COMMON_QFRACTION_CC

#include <awali/common/qfraction.hh>
#include <stdexcept>


namespace awali {

  q_fraction_t::q_fraction_t()
  : num(0), den(1)
  {
  }

  q_fraction_t::q_fraction_t(long n, long d)
  {
    if (d < 0) {
      num = -n;
      den = (unsigned) -d;
    } 
    else  {
      num = n;
      den = d;
    }

  }

  q_fraction_t::q_fraction_t(num_t n, den_t d)
    : num(n)
    , den(d)
  {}

  q_fraction_t& q_fraction_t::reduce()
  {
    int gc = utils::gcd(abs(num), den);
    num /= gc;
    den /= gc;
    return *this;
  }
  
  bool q_fraction_t::operator!=(const q_fraction_t& w) const
  {
    return (! ((*this) == w));
  }

  bool q_fraction_t::operator==(const q_fraction_t& w) const
  {
    return num * w.den == w.num * den;
  }

  bool q_fraction_t::operator<(const q_fraction_t& w) const
  {
    return this->num * w.den < w.num * this->den;
  }


  std::ostream& operator<<(std::ostream& o, const q_fraction_t& v)
  {
    o << v.num;
    if (v.den != 1)
      o << '/' << v.den;
    return o;
  }
  


  q_fraction_t 
  q_fraction_t::operator-(q_fraction_t const& other) 
  const 
  {
    q_fraction_t res = { ((long) this->num) * ((long) other.den)
                         - ((long) this->den) * ((long) other.num),
                        ((long) this->den) * ((long)other.den)    };
    return res.reduce();
  }

  q_fraction_t 
  q_fraction_t::operator*(q_fraction_t const& other) 
  const 
  {
    q_fraction_t res = { ((long) this->num) * ((long) other.num),
                         ((long) this->den) * ((long) other.den) };
    return res.reduce();
  }
  
  q_fraction_t 
  q_fraction_t::operator/(q_fraction_t const& other) 
  const 
  {
    q_fraction_t res = {((long) this->num) * ((long) other.den),
                        ((long) this->den) * ((long) other.num) };
    return res.reduce();
  }

  q_fraction_t 
  q_fraction_t::operator+(q_fraction_t const& other) 
  const 
  {
    q_fraction_t res = {((long) this->num) * ((long) other.den) 
                        + ((long) this->den) * ((long) other.num),
                        ((long) this->den) * ((long) other.den)  };
    return res.reduce();
  }

  std::istream& operator>>(std::istream& i, q_fraction_t& v) {
    int num;
    if (! (i >> num))
      throw std::invalid_argument("fraction : invalid numerator");

    // If we have a slash after the numerator then we have a
    // denominator as well.
    if (i.peek() != '/') {
      v.num=num;
      v.den=1u;
      return i;;
    }
    char eater;
    i >> eater;
    
    // operator>> with an istream and an unsigned int silently
    // mangles a negative number into its two's complement
    // representation as a positive number.
    if (i.peek() == '-')
      {
        i >> eater;
        num = - num;
      }
    
    unsigned int den;
    if (i >> den)
      {
        // Make sure our rational respects our constraints.
        if (den == 0)
          throw std::domain_error("fraction : zero denominator");
        v.num=num;
        v.den=den;
        v.reduce();
        return i;
      }
    else
      throw std::invalid_argument("fraction : invalid numerator");
  }
  
} //end of namespace awali

namespace std {
  //hash_function

  bool operator< (const std::complex<double>& lhs,
                  const std::complex<double>& rhs)
  {
    if(std::norm(lhs) == std::norm(rhs))
      return std::arg(lhs) < std::arg(rhs);
    return std::norm(lhs) < std::norm(rhs);
  }

  std::ostream& operator<<(std::ostream& o, const std::complex<double>& v)
  {
    if(v.imag()==0)
      o << v.real();
    else {
      if(v.real()==0) {
        if (v.imag() != 1)
          o << v.imag();
        o << 'i';
      } else {
        o << v.real();
        if (v.imag() > 0)
          o << '+';
        else if (v.imag() == -1)
          o << '-';
        if ((v.imag() != 1) && (v.imag() != -1))
          o <<v.imag();
        o << 'i';
      }
    }
    return o;
  }

  ostream& operator<<(ostream& o, const basic_string<int>& s) {
    std::string sep="";
    for(auto c : s) {
      o << sep;
      sep=":";
      o << c;
    }
    return o;
  }

}//end of ns std

#endif
