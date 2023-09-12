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

/* ---------------------------------------------------|
|  Called from print_out.cc as fake modularisation    |
|----------------------------------------------------*/

/* ---------------------------------------|
|  Name-desc list processing               |
|----------------------------------------*/

struct name_desc_pair_t {
  std::string name;
  std::string desc;
};

/* prints string 's' as title and underlines it with the character 'u'
   defined as a constant inside the function                                 */
void print_title(const std::string& s)
{
  char u = '=';

  std::cout << s + " :" << std::endl;
  std::cout << std::string(s.size()+2,u) << std::endl;
  return;
}

/* ---------------------------------------|
|  Name-desc list tagging                |
|----------------------------------------*/

/* T is a struct that has a 'name' and a 'desc' fields
   Input= a vector of T : list
   Output = a vector of name_desc_pair_t : tagged_cmd_list
   where the (name, desc) field of every entry of list is copied 
   and name is tagged with '*' if it appears in is_choices list and
   with '**' if it appears in double_type_cmds list.
   Such tagging actions are recorded in is_chc and dbl_tp respectively  */
template<typename T>
std::vector<name_desc_pair_t> tagged_cmd_list(std::vector<T>& list, 
											  bool& is_chc, bool& dbl_tp) 
{
  std::vector<name_desc_pair_t> v;
  static std::string item_name;

  for (T& item : list) {
    item_name = item.name;
    if ( found_in_list(item_name, is_choices) ) {
      item_name = item_name + "*";
      is_chc=true;
    }
    else {
      if ( found_in_list(item_name, double_type_cmds) ) {
        item_name = item_name + "**";
        dbl_tp=true;
      }
    }
    v.push_back({item_name,item.desc});
  }

  return v;
}

/* T is a struct that has a 'name' field
   Input= a vector of T : list
   Output = a vector of std::string : tagged_cmd_name_list
   where the "name" field of every entry of list is copied 
   and tagged with '*' if it appears in is_choices list and
   with '**' if it appears in double_type_cmds list.
   Such tagging actions are recorded in is_chc and dbl_tp respectively  */
template<typename T>
std::vector<std::string> tagged_cmd_name_list(std::vector<T>& list, 
											  bool& is_chc, bool& dbl_tp) 
{
  std::vector<std::string> v;
  std::string item_name;

  for (T& item : list) {
    item_name = item.name;
    if ( found_in_list(item_name, is_choices) ) {
      item_name = item_name + "*";
      is_chc=true;
    }
    else {
      if ( found_in_list(item_name, double_type_cmds) ) {
        item_name = item_name + "**";
        dbl_tp=true;
      }
    }
    v.push_back(item_name);
  }

  return v;
}

/* The same as above, but without record of tagging.       */
template<typename T>
std::vector<std::string> tagged_cmd_name_list(std::vector<T>& list) 
{
  std::vector<std::string> v;
  std::string item_name;

  for (T& item : list) {
    item_name = item.name;
    if ( found_in_list(item_name, is_choices) ) {
      item_name = item_name + "*";
    }
    else {
      if ( found_in_list(item_name, double_type_cmds) ) {
        item_name = item_name + "**";
      }
    }
    v.push_back(item_name);
  }

  return v;
}

// /* ---------------------------------------|
// |  Name-desc list building                |
// |----------------------------------------*/
// 
// /* Builds a name-desc list from the json file contained in a directory
//    pointed to by a dyn::loading function                                   */
// std::vector<name_desc_pair_t> 
//   list_directory_content(std::map<std::string, dyn::loading::file_loc_t>& dcl)
// {
//   std::vector<name_desc_pair_t> v;
//   for (auto p : dcl) {
//     std::string caption = "";
//     std::string name = p.second.name;
//     std::ifstream fic(p.second.dir+'/'+p.second.name+"."+p.second.ext);
//     js_object* jo = js_object::parse(fic);
//     if ( jo->has_field("metadata") )
//       if ( jo->object("metadata")->has_field("caption") )
//         caption = jo->object("metadata")->str("caption");
//     fic.close();
//     v.push_back({name,caption});
//   }
// 
//   return v;
// }

/* ---------------------------------------|
|  Name-desc list building                |
|----------------------------------------*/

/* Builds a name-desc list from the json file contained in a directory
   pointed to by a dyn::loading function                                   */
std::vector<name_desc_pair_t> 
  list_directory_content(std::map<std::string, dyn::loading::file_loc_t>& dcl)
{
  std::vector<name_desc_pair_t> v;
  for (auto p : dcl) {
    std::string caption = "";
    std::string name = p.second.name;
    std::ifstream fic(p.second.full());
    json_ast_t jo = json_ast::from(fic);
    if ( jo->has_path({"metadata","caption"}) )
      caption = jo->at("metadata")->at("caption")->to_string();
    fic.close();
    v.push_back({name,caption});
  }

  return v;
}

/* ---------------------------------------|
|  Name-desc list printing                |
|----------------------------------------*/

/* prints name list    */
template<typename T>
void print_name_list(std::vector<T> list) 
{
  for (T& item : list) {
    std::cout << item.name << std::endl; 
  }
  std::cout << std::endl;
  return;
}


/* computes the width of columns given the list and terminal width  (VM) */
template<typename T>
void name_desc_list_layout(std::vector<T>& list, size_t terminal_width, 
                           size_t& col_name, size_t& col_desc) 
{
  size_t sep_width = 1;     // size of separators between col_name and col_desc
                            // (cannot be computed because of UTF8 chars)
  size_t min_number_of_filler = 2; // min number of filler char
  size_t filler_sep = 1;           // nb of spaces before filler
  size_t right_margin_width = 1;   // size of right margin

  /* Computing the max length of names */
  size_t max = 0;                  
  for (T& item : list)       
    if ( item.name.size() > max )
      max = item.name.size();
 
 /* A line is 
     ________col_name_________       __col_desc_
    /                         \     /           \
    <name><spaces><filler char><sep><description><right_margin>
          \______/ 
     `filler_sep`  

    col_name= max(length of names) + filler_sep + min_number_of_filler
    col_desc= terminal_width - sep_width - col_name - right_margin_width
 */
  col_name = max+min_number_of_filler+filler_sep;
  int col_desc_tmp = terminal_width-col_name-sep_width-right_margin_width;

  if (col_desc_tmp < 12)   // If second column is less than 12 width, 
    col_desc_tmp = 100000; // we give up a nice display and every item 
                       // in one line
  col_desc = col_desc_tmp;

  return;
}

/* prints header line for name-desc list    (VM)      */
void print_name_desc_header(const size_t& col_name, const size_t& col_desc, 
                           std::string header_left = "Name:", 
                           std::string header_right = "Description:") 
{
  static size_t sep_width = 1;          // size of separators between columns
  static char underline = '-';

  
  std::cout << std::left <<  std::setw(col_name) << header_left;
  std::cout << std::string(sep_width, ' ');
  std::cout << std::left <<  std::setw(col_desc) << header_right;
  std::cout << std::endl;
  
  std::cout << std::string(col_name,underline) << std::string(sep_width, ' ') 
            << std::string(col_desc,underline);
  std::cout << std::endl;

  return;
}

/* prints name-desc list   (VM)   */
template<typename T>
void print_name_desc_list(std::vector<T> list, size_t col_name, size_t col_desc, 
                          bool cut_at_word = true) 
{
  char first_line_filler = '.';
  char following_line_filler = ' ';
  /* These four strings must have the same size <sep_width> defined in 
     name_desc_list_layout function                                       */
  std::string first_line_sep = " ";  // separator for 1st line of block
  std::string last_line_sep = " ";   // sep. for last line of block
  std::string middle_line_sep = " "; // sep. for other lines
  std::string one_line_sep = " ";    // sep. for the line if only one line
  size_t filler_sep = 1; // nb of spaces before filler 
                         // (defined in name_desc_list_layout() )

  for (T& item : list) {
    if (item.name == empty_name) { // takes care of place holders
      std::cout <<  std::endl;     // leaves an empty line
    } 
	else {
	  size_t size = item.desc.size();
      std::cout << std::left << std::setfill(first_line_filler) 
                << std::setw(col_name) << item.name+std::string(filler_sep,' ');
      size_t start = 0;
      std::string fill = "";
      while (start < size) {
        size_t len = col_desc;
        if (cut_at_word) {
          while ( len != 0 && (start+len) < size 
                  && item.desc[start+len] != ' ') 
            len--;
          if (len == 0) /* word is too long for one line */
            len = col_desc;
        }
        bool is_first_line = (start == 0),
             is_last_line = (start+len >= size);
        std::string sep;
        if (is_first_line && is_last_line)
          sep = one_line_sep;
        else if (is_first_line)
          sep = first_line_sep;
        else if (is_last_line)
          sep = last_line_sep;
        else
          sep = middle_line_sep;
        std::cout << fill << sep << item.desc.substr(start,len) << std::endl;
        if (!fill.compare(""))
          fill = std::string(col_name,following_line_filler);
        start+=len;
        while (start<size && item.desc[start] == ' ') 
          start++;
      }
	}
  }
  std::cout << std::endl;
  return;
}


/*  process name_desc list  without header printing  (for command lists) */
template<typename T>
void process_name_desc_list(std::vector<T>& list, std::string& title,
							bool cut_at_word = true) 
{
  size_t col_name;
  size_t col_desc;
  bool is_chc = false;
  bool dbl_tp = false;
  
  size_t terminal_width = balanced_term_width();

  std::vector<name_desc_pair_t> v = tagged_cmd_list(list, is_chc, dbl_tp);

  name_desc_list_layout(v, terminal_width, col_name, col_desc);
  
  std::cout << std::endl;
  print_title(title);
  std::cout << std::endl;

  print_name_desc_list(v, col_name, col_desc);
  std::cout << std::endl;
  
  if (is_chc) 
	variable_width_print(vw_doc::is_chc_str, terminal_width);
  if (dbl_tp) 
	variable_width_print(vw_doc::dbl_tp_str, terminal_width);
  
  return;
}

/*  process name_desc list  with header printing  
   (for options and  examples lists) no need for tagging.               */
template<typename T>
void process_name_desc_list_with_headers(std::vector<T>& list, std::string& title,
                            std::string header_left = "Name:", 
                            std::string header_right = "Description:",
							bool cut_at_word = true) 
{
  size_t col_name, col_desc;

  size_t terminal_width = balanced_term_width();

  name_desc_list_layout(list, terminal_width, col_name, col_desc);
  
  std::cout << std::endl;
  print_title(title);
  std::cout << std::endl;
  
  print_name_desc_header(col_name, col_desc, header_left, header_right);
  print_name_desc_list(list, col_name, col_desc);
  std::cout << std::endl;
  
  return;
}

// /*  process name list  without header printing  (for command lists) */
// template<typename T>
// void process_name_desc_list(std::vector<T>& list, std::string& title,
// 							bool cut_at_word = true) 
// {
//   size_t col_name;
//   size_t col_desc;
//   bool is_chc = false;
//   bool dbl_tp = false;
//   
//   size_t terminal_width = balanced_term_width();
// 
//   std::vector<name_desc_pair_t> v = tagged_cmd_list(list, is_chc, dbl_tp);
// 
//   name_desc_list_layout(v, terminal_width, col_name, col_desc);
//   
//   std::cout << std::endl;
//   print_title(title);
//   std::cout << std::endl;
// 
//   print_name_desc_list(v, col_name, col_desc);
//   std::cout << std::endl;
//   
//   if (is_chc) 
// 	variable_width_print(vw_doc::is_chc_str, terminal_width);
//   if (dbl_tp) 
// 	variable_width_print(vw_doc::dbl_tp_str, terminal_width);
//   
//   return;
// }


// // End of file name_desc_list_processing.cc --- return to print_out.cc
