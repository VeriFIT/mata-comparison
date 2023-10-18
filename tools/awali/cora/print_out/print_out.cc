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


/* ----------------------------------------------|
|  Called from cora.cc as fake modularisation    |
|                                                |
|  Contains constants, and calls to various      |
|  modules that contain printing functions.      |
|-----------------------------------------------*/


#include<awali/common/docstring/doc_entries.hh>
#include<awali/dyn/core/context_description/description_types.hh>

#include <sys/ioctl.h>
#include <unistd.h>

/* constants related to printing on terminal              */

const size_t term_width_std = 80; // standard line width (used in verbatim doc)
const size_t term_width_max = 96; // maximal line width (in 'adaptative' printing)
const size_t term_width_min = 45; // threshold line width that triggers 
                                  // alternative behaviour (to be fixed)


/* -----------------------------------------|
|  Calls to printing modules                |
|------------------------------------------*/

//// Functions used in other modules          ////
#include <cora/print_out/print_utils.cc> 

//// Functions for processing name-desc lists ////
#include <cora/print_out/name_desc_list_processing.cc> 

//// Functions for printing table of commands ////
#include <cora/print_out/print_cmd_table.cc> 

//// Printing documentation ////
#include <cora/print_out/print_doc.cc> 

//// Printing help ////
#include <cora/print_out/help.cc> 

//// Printing lists ////
#include <cora/print_out/list.cc> 

//// Printing info ////
#include <cora/print_out/print_info.cc> 

// // End of file print_out.cc --- return to cora.cc
