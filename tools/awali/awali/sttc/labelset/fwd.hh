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

#ifndef AWALI_LABELSET_FWD_HH
# define AWALI_LABELSET_FWD_HH

namespace awali { namespace sttc {


  // letterset.hh.
  template <typename GenSet>
  class letterset;

  // nullableset.hh.
  template <typename LabelSet>
  class nullableset;

  // oneset.hh.
  class oneset;

  // tupleset.hh.
  template <typename... LabelSets>
  class tupleset;

  // wordset.hh.
  template <typename GenSet>
  class wordset;

}}//end of ns awali::stc

#endif // !AWALI_LABELSET_FWD_HH
