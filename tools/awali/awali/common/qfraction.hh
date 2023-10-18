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

#ifndef COMMON_QFRACTION_HH
#define COMMON_QFRACTION_HH

#include <awali/utils/arith.hh>
#include <awali/utils/hash.hh>
#include <complex>

namespace awali {

  class q_fraction_t {
  public:
    using num_t = int;
    using den_t = unsigned int;

    num_t num;
    den_t den;


    q_fraction_t();
  
    q_fraction_t(num_t n, den_t d = 1u);

    q_fraction_t(long n, long d);

    q_fraction_t& reduce();

    bool operator==(const q_fraction_t& w) const;
    bool operator!=(const q_fraction_t& w) const;
    bool operator<(const q_fraction_t& w) const;


    q_fraction_t operator+(q_fraction_t const&) const;
    q_fraction_t operator-(q_fraction_t const&) const;
    q_fraction_t operator*(q_fraction_t const&) const;
    q_fraction_t operator/(q_fraction_t const&) const;

  };

  std::ostream& operator<<(std::ostream& o, const q_fraction_t& v);

}

namespace std {
  //hash_function

  bool operator< (const std::complex<double>& lhs,
                  const std::complex<double>& rhs);

  std::ostream& operator<<(std::ostream& o, const std::complex<double>& v);
  ostream& operator<<(ostream& o, const basic_string<int>& s);
    
  template<>
  struct hash<awali::q_fraction_t> {
    size_t operator()(const awali::q_fraction_t& f) const {
      size_t res = 0;
      std::hash_combine(res, awali::utils::hash_value(f.num));
      std::hash_combine(res, awali::utils::hash_value(f.den));
      return res;
    }
  };

  template<typename T>
  struct hash<std::complex<T>> {
    size_t operator()(const std::complex<T>& c) const {
      size_t res = 0;
      std::hash_combine(res, awali::utils::hash_value(c.real()));
      std::hash_combine(res, awali::utils::hash_value(c.imag()));
      return res;
    }
  };

}//end of ns std

#endif
