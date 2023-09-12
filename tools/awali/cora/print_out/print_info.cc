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


//// Printing info of automata or expressions ////

// function grammatical agreement: put an "s" at the end of string s if n>1
std::string gram_agree(unsigned n, std::string s)
{
  if (n>1) return s+"s";
  else return s;
}

// Prints alphabet of char, list between braces, char separated by commas
void print_char_alphabet(std::vector<char>& alpha)
{
  char sep='{';
  for(auto l: alpha) {
    std::cout << sep << ' ' << l;
    sep=',';
  }
  std::cout << '}';
  return;
}



// function info(<aut>)
void info(dyn::automaton_t aut)
{
  if (aut->is_transducer()) {    // Case  aut  is a transducer
    dyn::transducer_t tdc(aut);
    unsigned n = tdc.num_tapes();
    if ( n != 2 ) {
      std::string error_msg1 = "cora does not handle transducers with " 
                             + std::to_string(n) + " tapes.";
      error_print(error_msg1);
      return;
    }

    std::cout << std::endl; // An empty line, to begin with
    std::cout << " Transducer ";
    if (!(tdc->get_name().size() < 1))
      std::cout << ": " << tdc->get_name();
    std::cout << std::endl << std::endl;
    std::cout << " Weightset : " << tdc->get_context()->weightset_name() 
              << std::endl;

    auto alphabets = tdc.alphabets();
    unsigned lim_size = 5;
    std::cout << " Input alphabet : ";
    print_char_alphabet(alphabets[0]);

    if (alphabets[0].size() < lim_size)
      std::cout << "\t";
    else
      std::cout << std::endl;
    std::cout << " Output alphabet : ";
    print_char_alphabet(alphabets[1]);
    std::cout << std::endl;
  }
  else {                           // Case  aut  is an automaton
    std::cout << std::endl; // An empty line, to begin with
    std::cout << " Automaton ";
    if (!(aut->get_name().size() < 1))
      std::cout << ": " << aut->get_name();
    std::cout << std::endl << std::endl;
    std::cout << " Weightset : " << aut->get_context()->weightset_name()
              << std::endl;

	std::string label_name = aut->get_context()->labelset_name();
    std::string label_tp, letter_tp;
    if (label_name.find("char") < label_name.size())
      letter_tp = "Character";
    else {
      if (label_name.find("int") < label_name.size())
        letter_tp = "Integer";
      else {
        std::string error_msg1 = "Unknown label type";
        error_print(error_msg1);
        return;
      }
    }

    if (label_name.find("lal") < label_name.size())
      label_tp = "Letters";
    else {
      if (label_name.find("law") < label_name.size())
        label_tp = "Words";
      else {
        std::string error_msg1 = "Unknown letter type";
        error_print(error_msg1);
        return;
      }
    }

    if (label_name.find("lan") < label_name.size())
      label_tp = label_tp + " + epsilon";

    std::cout << std::endl; 
    std::cout << " Label type : " << label_tp << "\t\t"
              << "Letter type : " << letter_tp << std::endl;
    std::cout << " Alphabet : ";
    char sep = '{';
    for(auto l: aut->alphabet()) {
      std::cout << sep << ' ' << l;
      sep=',';
    }
    std::cout << " }" << std::endl;
  }

  std::cout <<  std::endl;

  unsigned n = aut->num_states();
  std::cout << " " << n << " " << gram_agree(n, "state")+ "        " << "\t\t" ;
  n = aut->num_transitions();
  std::cout << " " << n << " " << gram_agree(n, "transition") << std::endl;
  n = aut->num_initials();
  std::cout << " " << n << " initial " << gram_agree(n, "state") << "\t\t" ;
  n = aut->num_finals();
  std::cout << " " << n << " final " << gram_agree(n, "state") <<  std::endl;

  std::cout <<  std::endl;

} // end function info(<aut>)

// function info(<exp>)
void info(dyn::ratexp_t exp)
{
  std::cout << std::endl; // An empty line, to begin with
  std::cout << " Ratexp ";
//     if (!(exp->get_name().size() < 1))
//       std::cout << ": " << exp->get_name();
//     std::cout << std::endl << std::endl;
  std::cout << std::endl;
  std::cout << std::endl; 
  std::cout << " Weightset : " << exp->get_context()->weightset_name()
            << std::endl;

  std::string label_name = exp->get_context()->labelset_name();
  std::string label_tp, letter_tp;
  if (label_name.find("char") < label_name.size())
    letter_tp = "Character";
  else {
    if (label_name.find("int") < label_name.size())
      letter_tp = "Integer";
    else {
      std::string error_msg1 = "Unknown label type";
      error_print(error_msg1);
      return;
    }
  }

  if (label_name.find("lal") < label_name.size())
    label_tp = "Letters";
  else {
    if (label_name.find("law") < label_name.size())
      label_tp = "Words";
    else {
      std::string error_msg1 = "Unknown letter type";
      error_print(error_msg1);
      return;
    }
  }

  std::cout << std::endl; 
  std::cout << " Label type : " << label_tp << "\t\t"
            << "Letter type : " << letter_tp << std::endl;
  std::cout << " Alphabet : ";
  char sep = '{';
  for(auto l: exp->alphabet()) {
    std::cout << sep << ' ' << l;
    sep=',';
  }
  std::cout << " }" << std::endl;

  std::cout <<  std::endl;
  std::cout << " size : "  << exp->size()   << "        \t";
  std::cout << "length : " << exp->length() << std::endl;
  std::cout << " depth : " << exp->height() << "        \t";
  std::cout << "star_height : " << exp->star_height() << std::endl;
  std::cout <<  std::endl;
} // end function info(<exp>)

// // End of file --- return to print_out.cc
