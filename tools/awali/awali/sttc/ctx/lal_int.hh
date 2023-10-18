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

#ifndef AWALI_CTX_LAL_INT_HH
# define AWALI_CTX_LAL_INT_HH

#include <awali/sttc/alphabets/int.hh>
#include <awali/sttc/alphabets/setalpha.hh>
#include <awali/sttc/labelset/letterset.hh>

namespace awali {
  namespace sttc {
    namespace ctx
    {
      using lal_int = letterset<set_alphabet<int_letters>>;
    }
  }
}//end of ns awali::stc

#endif // !AWALI_CTX_LAL_INT_HH
