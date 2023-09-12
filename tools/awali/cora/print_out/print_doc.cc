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



/* ---------------------------------------------------------------------|
|  On-line documentation processing                                     |
|                                                                       |
|  In addition to the documentation of every command <cmd>, available   |
|  with  cora help <cmd>, a number of subjects have been documented.    |
|                                                                       |
|  These documentation pages are stored in  awali/common/docstrings.    |
|  They appear in the Doxygen pages and can be called with              |
|  the command  cora doc <subject> .                                    |
|----------------------------------------------------------------------*/


/* list the documentation pages                             */
void print_docstring_list()
{
  size_t col_name, col_desc;

  size_t terminal_width = balanced_term_width();

  name_desc_list_layout(awali::docstring::entries, terminal_width, col_name, col_desc);
  print_name_desc_list(awali::docstring::entries, col_name, col_desc);
  
  std::cout << std::endl;
  return;
}

/* function activated by the command  cora doc <s>
   (cannot be called 'doc' because of the existence of a namespace doc.   */
void doc_(std::string s)
{
  for (auto entry : awali::docstring::entries) {
    if (s == entry.name) {
      // We could display the entry title, contained in `entry.title`
      std::cout << "\n" << title_clr << entry.title  << reset_clr << std::endl;
      std::cout << entry.content << std::endl;
      return;
    }
  }
  std::string error_msg1 = "No documentation entry available for '" + s + "'.";
  std::string error_msg2 = "Available entries are listed with the command "
                           " 'cora help doc' .";
  error_print(error_msg1, error_msg2);
  return;
}


// // End of file print_doc.cc --- return to print_out.cc
