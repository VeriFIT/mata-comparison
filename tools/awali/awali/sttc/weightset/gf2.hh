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

#ifndef AWALI_WEIGHTSET_GF2_HH
# define AWALI_WEIGHTSET_GF2_HH

# include <cassert>
# include <ostream>

#include <awali/sttc/misc/escape.hh>
#include <awali/utils/hash.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/sttc/weightset/weightset.hh>

namespace awali { namespace sttc {

  namespace internal
  {
    class gf2_impl
    {
    public:
      using self_type = gf2;

      static std::string sname()
      {
        return "gf2";
      }

      std::string vname(bool = true) const
      {
        return sname();
      }

      enum gf2_value {
        Z, O,  X, X_O, X2, X2_O, X2_X, X2_X_O
      }

      gf2_value[8][8] addition_table = {
      //       0        1        x      x+1       x²     x²+1     x²+x   x²+x+1
        {      Z,       O,       X,     X_O,      X2,    X2_O,    X2_X,  X2_X_O } // +0
        {      O,       Z,     X_O,       X,    X2_O,      X2,  X2_X_O,    X2_X } // +1
        {      X,     X_O,       Z,       O,    X2_X,  X2_X_O,      X2,    X2_O } // +x
        {    X_O,       X,       O,       Z,  X2_X_O,    X2_X,    X2_O,      X2 } // +x+1
        {     X2,    X2_O,    X2_X,  X2_X_O,       Z,       O,       X,     X_O } // +x²
        {   X2_O,      X2,  X2_X_O,    X2_X,       O,       Z,     X_O,       X } // +x²+1
        {   X2_X,  X2_X_O,      X2,    X2_O,       X,     X_O,       Z,       O } // +x²+x
        { X2_X_O,    X2_X,    X2_O,      X2,     X_O,       X,       O,       Z } // +x²+x+1
      };

      using value_t = gf2_value;
    };
  }
}}//end of ns awali::stc
