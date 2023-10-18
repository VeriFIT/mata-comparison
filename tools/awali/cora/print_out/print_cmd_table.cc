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


/* ---------------------------------------------------|
|  Called from print_out.cc as fake modularisation    |
|----------------------------------------------------*/


/* ---------------------------------------|
|  Prints the array of commands           |
|----------------------------------------*/


/* outputs the string margin + s padded with spaces up to a total width n */
std::string padding(const std::string& margin, const std::string& s, 
                    const unsigned n)
{
  std::string output;
  output = margin + s;
  output = output + std::string(n-output.size(),' ');
  return output;
}

/* outputs the string s centered with spaces in a width n string  */
std::string centering(const std::string& s, const unsigned n)
{
  std::string output;
  unsigned d = n - s.size();
  output = std::string(d/2,' ') + s + std::string(d-(d/2),' ');
  return output;
}


/* ---------------------------------------|
|  Drawing a 3 column table               |
|----------------------------------------*/

//// 1. Framing

// horizontal table line for 3 columns of width n1, n2, and n3
void hor_line_print(const unsigned n1, const unsigned n2, const unsigned n3,	  
                    const std::string left, const std::string middle, 
                    const std::string right)
{
  std::string h ="\u2500"; // UTF8 character for horizontal line
  std::string u1 ="", u2 ="", u3 ="";

  for(unsigned i= 0; i<n1; ++i) u1 = u1 + h;
  for(unsigned i= 0; i<n2; ++i) u2 = u2 + h;
  for(unsigned i= 0; i<n3; ++i) u3 = u3 + h;
  std::cout << left + u1 + middle + u2 + middle + u3 + right << std::endl;
  return;
}

// top line for 3 columns of width n
void top_line_print(const unsigned n)
{
  std::string left ="\u250C", middle ="\u252C", right ="\u2510";
  hor_line_print( n, n, n, left, middle, right);
  return ;
}

// mid line for 3 columns of width n
void mid_line_print(const unsigned n)
{
  std::string left ="\u251C", middle ="\u253C", right ="\u2524";
  hor_line_print( n, n, n, left, middle, right);
  return ;
}

// bottom line for 3 columns of width n1, n2, and n3
void bot_line_print(unsigned n)
{
  std::string left ="\u2514", middle ="\u2534", right ="\u2518";
  hor_line_print( n, n, n, left, middle, right) ;
  return ;
}

//// 2. Line printing

void centered_line_print(const std::string& s1, const std::string& s2, 
                         const std::string& s3,
                         unsigned n)
{
  std::string v ="\u2502"; // UTF8 character for vertical line

  std::cout << v + centering(s1,n) + v + centering(s2,n) + v + centering(s3,n) 
             + v << std::endl;
  return;
}

void padded_line_print(const std::string& s1, const std::string& s2, 
                       const std::string& s3,
                       unsigned n)
{
  std::string v ="\u2502"; // UTF8 character for vertical line
  std::string margin = " ";

  std::cout << v + padding(margin,s1,n) + v + padding(margin,s2,n) + 
               v + padding(margin,s3,n)+ v << std::endl;
  return;
}

//// 3. Table printing

void block_print(const std::vector<std::string>& v1, 
                 const std::vector<std::string>& v2,
                 const std::vector<std::string>& v3,
                 unsigned n)
{
  std::string s1, s2, s3;
  unsigned max;

  max= std::max(std::max(v1.size(), v2.size()), v3.size());

  for(unsigned j= 0; j<max; ++j) {
    s1 = "";
    s2 = "";
    s3 = "";
    if (j < v1.size())
      s1 = v1[j];
    if (j < v2.size())
      s2 = v2[j];
    if (j < v3.size())
      s3 = v3[j];
    padded_line_print( s1, s2, s3, n);
  }

  return;
}

void print_table()
{
  unsigned col_width=25;
  std::string s1, s2, s3, u;
  std::vector<std::string> v1, v2, v3;

  s1 = "basic-cmds"; //Basic commands
  s2 = "generic-cmds"; //Generic commands
  s3 = "wfa-cmds"; //Wfa commands
  v1 = tagged_cmd_name_list(commands_basic);
  v2 = tagged_cmd_name_list(commands_generic);
  v3 = tagged_cmd_name_list(commands_aut);

  std::cout << std::endl;
  top_line_print( col_width);
  centered_line_print( s1, s2, s3, col_width);
  mid_line_print( col_width);
  block_print( v1, v2, v3, col_width);
  bot_line_print( col_width);

  s1="ratexp-cmds"; //Ratexp commands
  s2="nfa-cmds"; //Nfa commands
  s3="transducer-cmds"; //Transducer commands
  v1 = tagged_cmd_name_list(commands_exp);
  v2 = tagged_cmd_name_list(commands_nfa);
  v3 = tagged_cmd_name_list(commands_tdc);

  top_line_print( col_width);
  centered_line_print( s1, s2, s3, col_width);
  mid_line_print( col_width);
  block_print( v1, v2, v3, col_width);
  bot_line_print( col_width);
  std::cout << std::endl;

  std::cout << doc::is_chc_str << std::endl;
  std::cout << doc::dbl_tp_str << std::endl;
  
  return;
} // End of print_table

// // End of file --- return to print_out.cc
