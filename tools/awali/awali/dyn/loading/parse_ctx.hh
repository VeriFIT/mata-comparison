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

#ifndef AWALI_DYN_LOADING_PARSE_HH
#define AWALI_DYN_LOADING_PARSE_HH

#include<list>
#include<string>
#include<set>

namespace awali { namespace  dyn {
  namespace loading {


    /* Transforms the string representing a weighset into the string of
    * the file (without ".hh") where it is defined.
    */
    std::string filename_of_ws(std::string w);

    /* Transforms the string representing a weighset into the string of
    * its type.
    */
    std::string classname_of_ws(std::string w);
    
    /*
     * C  -> L_W
     * L  -> lao | lal_char lal_int | law_char | lan<L> | lat<LL>
     * LL -> L | L,LL
     * W  -> b | z | q | r | c | f2 | zmin | zmax | pmax | zzN | nnK
     *     | ratexpset<C> | series<C> | product<WW>
     * WW -> W | W,WW
     *
     * where N is a positive number.
     */

    void rec_parse_labelset(const std::string& s, size_t& p,
                            std::set<std::string>& labelsets, std::string& ls);

    void rec_parse_context(const std::string& s, size_t& p,
                           std::set<std::string>& weightsets, std::set<std::string>& labelsets,
                           std::string& ws, std::string& ls);

}}}//end of ns awali::dyn

#endif
