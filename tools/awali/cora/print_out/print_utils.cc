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

/* -----------------------------------------|
|  Adjust size printing to terminal width   |
|------------------------------------------*/

/* computes and returns term width  (written by VM)  */
size_t compute_term_width() 
{
  size_t term_width = term_width_std;
#ifdef TIOCGSIZE
  struct ttysize ts;
  int ret = ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
  if (ret>=0)
    term_width = ts.ts_cols;
#elif defined(TIOCGWINSZ)
  struct winsize ts;
  int ret = ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
  if (ret>=0)
    term_width = ts.ws_col;
#endif
  return term_width;
}

/* Functions derived from compute_term_width() that takes into account  
   the term width constants                                            */
size_t balanced_term_width()
{
  size_t term_width = compute_term_width();
  /* Bounding the line length */
  if ( term_width > term_width_max )
    term_width = term_width_max;
  return term_width;
}

size_t balanced_term_width(bool& alarm)
{
  size_t term_width = compute_term_width();
  /* Bounding the line length */
  if ( term_width > term_width_max )
    term_width = term_width_max;
  /* If term_width is too small, an alarm is sent */
  if ( term_width < term_width_min )
    alarm=false;
  return term_width;
}

/* ---------------------------------------|
|  Adaptative printing                    |
|----------------------------------------*/

/* prints string <s> cutting at <width>, or at closest to <width> end of word
   accepts newline characters and restart a new line after them
   ends with a new line
   (original cutting algorithm by VM)                                        */
void variable_width_print(const std::string& s, 
						  const size_t width = term_width_std,
					      const bool cut_at_word = true) 
{
  size_t size = s.size();
  size_t start = 0;
  
  while (start < size) {
    bool found_new_line = false;
    size_t len = 0;
    while ( len < width && (start+len) < size && s[start+len] != '\n')
      len++;
    if ( s[start+len] == '\n' ) {
      len++;
      found_new_line = true;
      std::cout << s.substr(start,len); // the last character read is \n
    } 
	else {
      len = width;
      if (cut_at_word) {
        while ( len != 0 && (start+len) < size && s[start+len] != ' ')
          len--;
        if (len == 0) /* word is too long for one line */
          len = width;
      }
      std::cout << s.substr(start,len) << std::endl;
	}
    start+=len;
    if ( !found_new_line ) // spaces after \n are significant
      while (start<size && s[start] == ' ') 
        start++;
  }

  std::cout << std::endl;
  return;
}

/* the same as above, but outputs on warning stream in warning color 
   no newline in <s> 
   begins and ends with a new line; restores normal color.           */
void variable_width_warning(const std::string& s, 
						    const size_t width = term_width_std,
					        const bool cut_at_word = true) 
{
  size_t size = s.size();
  size_t start = 0;
  *dyn::warning_stream << warning_clr + "\n";
  
  while (start < size) {
    size_t len = width;
    if (cut_at_word) {
      while ( len != 0 && (start+len) < size && s[start+len] != ' ')
        len--;
      if (len == 0) /* word is too long for one line */
        len = width;
      }
    *dyn::warning_stream << s.substr(start,len)<< std::endl;

    start+=len;
    while (start<size && s[start] == ' ') 
      start++;
    }

  *dyn::warning_stream << reset_clr << std::endl;
  return;
}

/* test the width of the terminal and call variable_width_print() */
void adaptative_print(const std::string& s, const bool cut_at_word = true) 
{
  size_t width = balanced_term_width();
  variable_width_print( s, width, cut_at_word);
  return;
}
  
/* test the width of the terminal and call variable_width_warning() */
void adaptative_warning(const std::string& s, const bool cut_at_word = true) 
{
  size_t width = balanced_term_width();
  variable_width_warning( s, width, cut_at_word);
  return;
}

/* warning before documentation set up for a 'term_width_std' character wide terminal */
void width_warning()
{
  size_t width = compute_term_width();
  if (width < term_width_std) 
	variable_width_warning( vw_doc::warning_term_width, width);
  return;
}

/*  Centralized call to warning_stream in order to unformize layout.    
    The message 'msg' is supposed NOT to end with a newline character. 
    Multiple space characters are not significant but if they follow 
    a newline character.                                              */
void warning_vw_print(const std::string& msg) 
{
  size_t size = msg.size();
  size_t start = 0;
  std::string warn_str = "[Warning] ";
  size_t left_mrg = warn_str.size();
  size_t right_mrg = 1;

  size_t width = compute_term_width();
  width = width - left_mrg - right_mrg;

  
  // preparation: skips a line, sets stream to warning colour, prints heading
  *dyn::warning_stream  << std::endl << warning_clr << warn_str;
 
  if ( size < width+1 ) {
    *dyn::warning_stream << msg << std::endl;
  } 
  else {
    while (start < size) {
      bool found_new_line = false;
      size_t len = 0;
      while ( len < width && (start+len) < size && msg[start+len] != '\n')
        len++;
      if ( msg[start+len] == '\n' ) {
        len++;
        found_new_line = true;
        *dyn::warning_stream  << msg.substr(start,len); // last character read is \n
      } 
	  else {
        len = width;
        while ( len != 0 && (start+len) < size && msg[start+len] != ' ')
          len--;
        if (len == 0) /* word is too long for one line */
          len = width;
        *dyn::warning_stream  << msg.substr(start,len) << std::endl << std::string(left_mrg, ' ');
	  }
      start+=len;
      if ( !found_new_line ) // spaces after \n are significant
        while (start<size && msg[start] == ' ') 
          start++;
    }
  }
  // end: resets stream to normal colour and skips a line
  *dyn::warning_stream << reset_clr << std::endl;
  return;
}


/*  Centralized call to error_stream in order to unformize layout.    
    The message msg is supposed NOT to end with a newline character. */
void error_print(const std::string& msg) 
{
  *dyn::error_stream  << std::endl << error_clr
                      << "[Error] " << msg << std::endl
                      << reset_clr << std::endl;
  return;
}

void error_print(const std::string& msg1, const std::string& msg2) 
{
  *dyn::error_stream  << std::endl << error_clr
                      << "[Error] " << msg1 << std::endl
                      << "        " << msg2 << std::endl
                      << reset_clr << std::endl;
  return;
}

void x_error_print(const std::string& msg, const std::string& err_typ) 
{
  *dyn::error_stream  << std::endl << error_clr
                      << "[" << err_typ << "] " << msg << std::endl
                      << reset_clr << std::endl;
  return;
}

void deprecated_command(std::string name, std::string version, std::string new_name = "") {
  std::stringstream s;
  s << "Command " << name << " is deprecated since version " << version << ".";
  if (new_name != "")
    s << " Use " << new_name << " instead.";
  warning_vw_print(s.str());
}

// // End of file print_utils.cc --- return to print_out.cc
