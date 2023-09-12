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


/*****************************************************************************
 /!\ Beware, the string below is used as a source for generating a page in 
     Doxygen in a md.
     
     A preprocessing is done that escape any <, >, [, ] or backslash
     that appears outside of a block of code.
     This exclude lines starting with 4 spaces and strings between backquote
     and the character > if it is the very first character of a line.

     This process will fail if there is an odd number of backquotes on any 
     given line !  Also backquotes are doubled for technical reasons, 
     so do not use double backquotes.
  
     Please duplicate this warning if you use this file as a template.
******************************************************************************/

#ifndef AWALI_COMMON_DOCSTRING_TEMPLATEDOCFILE_HH
#define AWALI_COMMON_DOCSTRING_TEMPLATEDOCFILE_HH

#include <awali/common/docstring/entry.hh>

namespace awali { namespace docstring {

static entry_t templatedocfile = {
/* Name:  */ "",
/* Description: */ "",
/* Title: */ "",
/* Content: */ 
R"---doxytoken---(  

)---doxytoken---"
};

}} //End of namespaces awali::docstring and awali


#endif

