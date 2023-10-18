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

#include <awali/sttc/weightset/lr_parse_number.hh>

using namespace awali;
using namespace awali::sttc::internal;

void
test_involution() {

  std::cout << std::endl;
  std::cout << "involution" << std::endl;
  std::cout << "\tints" << std::endl;
  for (int x : {0,1,-2,3,45,-678,90,100000}) {
    std::string s = std::to_string(x);
    size_t p = s.length();
    int int_res = lr_parse_int(s,p);
    if (x != int_res)
      throw std::runtime_error("Error with: "+s+" (expected: "+std::to_string(x)+", got:"+std::to_string(int_res)+").");
    if (p != 0)
      throw std::runtime_error("Scan error with: "+s+" (expected position: 0, got:"+std::to_string(p)+").");
    std::cout << "\t\tSuccess (int) with \"" << s << "\"." << std::endl;
    
    // As q_fraction
    p = s.length();
    q_fraction_t qfraction_res = lr_parse_qfraction(s,p);
    if (q_fraction_t{x,1} != qfraction_res)
      throw std::runtime_error("Error with: "+s+" (expected: "+std::to_string(x)+", got: "+std::to_string(qfraction_res.num)+"/"+std::to_string(qfraction_res.den)+".");
    if (p != 0)
      throw std::runtime_error("Scan error with: "+s+" (expected position: 0, got:"+std::to_string(p)+").");
    std::cout << "\t\tSuccess (qfraction) with \"" << s << "\"." << std::endl;

   // As double
    p = s.length();
    double double_res = lr_parse_double(s,p);
    if (x != double_res)
      throw std::runtime_error("Error with: "+s+" (expected: "+std::to_string(x)+", got:"+std::to_string(double_res)+").");
    if (p != 0)
      throw std::runtime_error("Scan error with: "+s+" (expected position: 0, got:"+std::to_string(p)+").");
    std::cout << "\t\tSuccess (double) with \"" << s << "\"." << std::endl;
    
    // AS complex
    p = s.length();
    std::complex<double> complex_res = lr_parse_complex(s,p);
    if (std::complex<double>{(double)x,0} != complex_res)
      throw std::runtime_error("Error with: "+s+" (expected: "+std::to_string(x)+", got:"+std::to_string(complex_res.real())+"~"+std::to_string(complex_res.imag())+").");
    if (p != 0)
      throw std::runtime_error("Scan error with: "+s+" (expected position: 0, got:"+std::to_string(p)+").");
    std::cout << "\t\tSuccess (complex, real) with \"" << s << "\"." << std::endl;
    
    // AS imaginary
    std::complex<double> x2 = {0,(double)x};
    s += "i";
    p = s.length();
    std::complex<double> complex_res2 = lr_parse_complex(s,p);
    if (x2 != complex_res2)
      throw std::runtime_error("Error with: "+s+" (expected: "+std::to_string(x)+", got:"+std::to_string(complex_res.real())+"~"+std::to_string(complex_res.imag())+").");
    if (p != 0)
      throw std::runtime_error("Scan error with: "+s+" (expected position: 0, got:"+std::to_string(p)+").");
    std::cout << "\t\tSuccess (complex, imag) with \"" << s << "\"." << std::endl;
  }

  std::cout << "\tdoubles" << std::endl;
  for (double x: {0.,-.0,1.2,-.34e5,678e90,-100000.}) {
    std::string s = std::to_string(x);
    size_t p = s.length();
    double double_res = lr_parse_double(s,p);
    if (x != double_res)
      throw std::runtime_error("Error with: "+s+" (expected: "+std::to_string(x)+", got:"+std::to_string(double_res)+").");
    if (p != 0)
      throw std::runtime_error("Scan error with: "+s+" (expected position: 0, got:"+std::to_string(p)+").");
    std::cout << "\t\tSuccess (double) with \"" << s << "\"." << std::endl;
  }

}


void 
test_parse_qfraction() 
{
  std::cout << std::endl;
  std::cout << "lr_parse_qfraction" << std::endl;

  {
    q_fraction_t q{-10,1};
    for (std::string s: {"-10", "-10/1", "-20/2", "-10.", "-10.0", "-10.00", "-100/10"}) {
      size_t i = s.length();
      q_fraction_t r = lr_parse_qfraction(s,i);
      if (q != r) {
        std::stringstream ss;
        ss << "Error with \"" << s << "\".  Parsed: " 
           << r << ".  Expected: " << q << ".  " << "Stopped at position: "
           << i << "."
        ;
        throw std::runtime_error(ss.str());
      }
      else 
        std::cout << "\tSuccess with: " << s << std::endl;
    }
  }
  {
    q_fraction_t q{1,2};
    for (std::string s: {"1/2", "+1/2", ".5", "+.5", "+0.5", "0.5"}) {
      size_t i = s.length();
      q_fraction_t r = lr_parse_qfraction(s,i);
      if (q != r) {
        std::stringstream ss;
        ss << "Error with \"" << s << "\".  Parsed: " 
           << r << ".  Expected: " << q << ".  " << "Stopped at position: "
           << i << "."
        ;
        throw std::runtime_error(ss.str());
      }
      else 
        std::cout << "\tSuccess with: " << s << std::endl;
    }
  }
  {
    for (std::string s : {"+.", "12.+12", "13.+13", "1/+", "+/2", "3/-4", "5/+6", ".", "/", "7/", "/8", "a"}) {
      size_t i = s.length();
      try {
        q_fraction_t r = lr_parse_qfraction(s,i);
        std::stringstream ss;
        ss << "Error with \"" << s << "\".  Parsed: " 
         << r << ".  Expected an exception to be raised.";
        throw std::runtime_error(ss.str());
      } 
      catch (...) {
        std::cout << "\tSuccessfully raised an exception with: " << s <<  std::endl;
      }
    }
  }
}

void 
test_parse_double() 
{
  std::cout << std::endl;
  std::cout << "lr_parse_double" << std::endl;
  for (std::string s : {"23", "+23", "2.3e1", ".000000000023e12", "0.023e+3", "230e-1","230000000000e-10"}) {
    size_t i = s.length();
    double d = lr_parse_double(s,i);
    if (d != 23)
      throw std::runtime_error("Error with: "+s+"(parsed: "+std::to_string(d)+")");
    if (i != 0)
      throw std::runtime_error("Size error with: "+s);
    std::cout << "\t Success with: " << s << std::endl;
  }

  {
    std::cout << std::endl;
    std::string s = "23";
    for (int i = 1; i <= 5; i++) {
      s = s + ((char) ('a' + i -1));
      s = ((char) ('z' - i + 1)) + s;
      size_t p = s.length()-i;
      double d = lr_parse_double(s,p);
      if (d != 23)
        throw std::runtime_error("Error with: " + s + "(parsed: " 
                                 + std::to_string(d) + ")");
      if (p != i)
        throw std::runtime_error("Scan error with: "+s);
      std::cout << "\t Success with: " << s << std::endl;
    }
  }
}








void 
test_parse_complex() 
{
  std::cout << std::endl;
  std::cout << "lr_parse_complex" << std::endl;
  for (std::string s : {"i-98.765", "-98.765+i", "-98.765+1i", "+1i-98.765", "+i-98.765"}) 
  {
    size_t i = s.length();
    std::complex<double> c1 = lr_parse_complex(s,i);
    std::complex<double> c2 = {-98.765,1};
    if (c1 != c2)
      throw std::runtime_error("Error with: "+s+"(parsed: "+std::to_string(c1.imag())+"i+"+std::to_string(c1.real())+")");
    if (i != 0)
      throw std::runtime_error("Scan error with: "+s+". Found: "+std::to_string(i)+".");
    std::cout << "\t Success with: " << s << std::endl;
  }
  std::cout << std::endl;
  for (std::string s : {"-i+98.765e1", "98.765e1-i", "+98.765e1-i","-i+98.765e+1"}) 
  {
    size_t i = s.length();
    std::complex<double> c1 = lr_parse_complex(s,i);
    std::complex<double> c2 = {987.65,-1};
    if (c1 != c2)
      throw std::runtime_error("Error with: "+s+"(parsed: "+std::to_string(c1.imag())+"i+"+std::to_string(c1.real())+")");
    if (i != 0)
      throw std::runtime_error("Scan error with: "+s+". Found: "+std::to_string(i)+".");
    std::cout << "\t Success with: " << s << std::endl;
  }
  
  std::cout << std::endl;
  for (std::string s : {"+75i+0", "750000000000e-10i", "75i+0", "75i", "+75i", "0+75i","+0+75i"}) 
  {
    size_t i = s.length();
    std::complex<double> c1 = lr_parse_complex(s,i);
    std::complex<double> c2 = {0,75};
    if (c1 != c2)
      throw std::runtime_error("Error with: "+s+"(parsed: "+std::to_string(c1.imag())+"i+"+std::to_string(c1.real())+")");
    if (i != 0)
      throw std::runtime_error("Scan error with: "+s+". Found: "+std::to_string(i)+".");
    std::cout << "\t Success with: " << s << std::endl;
  }
  std::cout << std::endl;
  for (std::string s : {"+76.1+0i", "761000000000e-10", "76.1+0i", "76.1", "+76.1", "0i+76.1","+0i+76.1"}) 
  {
    size_t i = s.length();
    std::complex<double> c1 = lr_parse_complex(s,i);
    std::complex<double> c2 = {76.1,0};
    if (c1 != c2)
      throw std::runtime_error("Error with: "+s+" (parsed: "+std::to_string(c1.imag())+"i+"+std::to_string(c1.real())+")");
    if (i != 0)
      throw std::runtime_error("Scan error with: "+s+". Found: "+std::to_string(i)+".");
    std::cout << "\t Success with: " << s << std::endl;
  }
  {
    std::cout << std::endl;
    std::string s = "i+1";
    for (int i = 1; i <= 5; i++) {
      s = s + ((char) ('a' + i -1));
      s = ((char) ('z' - i + 1)) + s;
      size_t p = s.length()-i;
      std::complex<double> c1 = lr_parse_complex(s,p);
      std::complex<double> c2 = {1,1};
      if (c1 != c2)
        throw std::runtime_error("Error with: " + s + " (parsed: "+std::to_string(c1.imag())+"i+"+std::to_string(c1.real())+")");

      if (p != i)
        throw std::runtime_error("Scan error with: "+s);
      std::cout << "\t Success with: " << s << std::endl;
    }
  }
}



int 
main (int argc, char** argv) 
{
  test_involution();
  test_parse_double();
  test_parse_complex();
  test_parse_qfraction();

}



