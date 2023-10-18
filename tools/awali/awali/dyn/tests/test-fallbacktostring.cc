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

#include <awali/dyn.hh>

#include <string>

using namespace awali;
using namespace awali::dyn;

struct myword {
  std::string content;
  myword(std::string c) { content=c; }
  bool operator==(myword const& other) const { return true; } 
  bool operator<(myword const& other) const { return false; } 
};

std::ostream& operator<<(std::ostream& o, myword const& m) {
  return o << m.content;
}

int main() {

/* This highlights the "fallback to std::string" mechanism suggested for
   the cast of an `any_t` to the expected type.
   It requires an intermediary function to call for each method in explicit-automaton
   so it is only implemented currently in the method:
       automaton_t::set_transition(state_t,state_t,weight_t,label_t)
   but it works for the label and the weight
*/

  automaton_t a = automaton_t::from("ab","Q",true);
  state_t s0 = a->add_state();
  state_t s1 = a->add_state();
  state_t s2 = a->add_state();
  state_t s3 = a->add_state();
  state_t s4 = a->add_state();
  state_t s5 = a->add_state();
  state_t s6 = a->add_state();
  state_t s7 = a->add_state();
  state_t s8 = a->add_state();
  state_t s9 = a->add_state();
  state_t s10 = a->add_state();

  a->set_transition(s0,s0,'a', "1");
  a->set_transition(s1,s1,"a", 2u);

//   a->set_transition(s3,s3, "", 1); // Fails because "" is actually not a valid
                                      //   repr for epsimp,
  a->set_transition(s3,s3, "\\e", 1);

  transition_t w = a->set_transition(s2,s2,"a", "-2/5");
  a->set_transition(s4,s4,"b", true); // works because the "standard" way these
  a->set_transition(s5,s5,"b", 2.0);  // are put in ostream gives a valid string
                                      // for Q ("1" and "2", resp.)

//   a->set_transition(s3,s3,'a', 2.5);// fails because Q does not autorise this
                                       // representation format.

  // Fallback to std::string allows to use types that were not really available 
  // before: 
  automaton_t b = lift::lift(a);
  b->set_transition(s4,s5,"","(a+b)*");
  std::cerr << JSON << b << std::endl;

  std::cerr << std::endl << "****  Testing all label functions *******************************" << std::endl << std::endl;

  a->get_transition(s4,s4,"b");
  a->has_transition(s5,s6,"a");
  a->set_transition(s6,s6,"a");
  a->add_transition(s7,s7,"a");
  a->add_transition(s8,s8,"b", true);
  a->del_transition(s8,s8,"b");
  a->successors(s7, "a");
  a->predecessors(s7, "b");
  a->out(s7, "a");
  a->in(s7, "a");

  std::cerr << "Add letter" << std::endl;
  a->get_context()->add_letter("d");
  for (const any_t letter : a->alphabet())
    std::cerr << letter;
  std::cerr << std::endl;
  try {  
    a->get_context()->add_letter("de"); 
    throw std::runtime_error("add_letter was successfull on a string of length >1");
  }
  catch (any_cast_exception e) { 
    std::cerr << "Any_cast exception: " << e.what() << std::endl;
  }


  std::cerr << "Has letter" << std::endl;
  a->get_context()->has_letter("e");
  std::cerr << "Label to string" << std::endl;
  a->get_context()->label_to_string("d");


  automaton_t d = automaton_t::with_int_labels::from_size(12, "B");
  std::cerr << "Has letter (int)" << std::endl;
  d->get_context()->has_letter("12");
  d->get_context()->has_letter("15");
  std::cerr << "Add letter (int)" << std::endl;
  d->get_context()->add_letter("123");
  for (const any_t letter : d->alphabet())
    std::cerr << letter << " ";
  std::cerr << std::endl;

  try {  
    d->get_context()->add_letter("d"); 
    for (const any_t letter : d->alphabet())
      std::cerr << letter << " ";
    std::cerr << std::endl;
    throw std::runtime_error("add_letter was success with a non-integer");
  }
  catch (any_cast_exception e) { 
    std::cerr << "Any_cast exception: " << e.what() << std::endl;
  }



  std::cerr << std::endl << "****  Testing all weight functions *******************************" << std::endl << std::endl;

  a->set_initial(s4,"-1/12");
  a->set_final(s5,"-13");
  a->add_initial(s4,"-3/12");
  a->add_final(s5,"14");
  a->add_transition(s9,s9,'a',"5/15");
  a->set_eps_transition(s10,s10,"5/15");
  a->add_eps_transition(s10,s10,"10/15");

  a->set_weight(w, "7/3");
  a->add_weight(w, "1/3");
  a->lmul_weight(w, "4/3");
  a->rmul_weight(w, "18/3");

  a->get_context()->add_weights("7/3","7/2");
  a->get_context()->mul_weights("7/3","7/2");
  a->get_context()->div_weights("7/3","7/2");

  a->get_context()->weight_to_string("91/3");

  // expand_label(any_t) label_to_string(any_t,unsigned)

  std::cerr << std::endl << "****  TRANSDUCER *******************************" << std::endl << std::endl;

  // With some extra machinery, it also works with transducers
  // (See any.hh)
  automaton_t t = transducer_t::from({"ab","bc"});
  state_t t0 = t->add_state();
  state_t t1 = t->add_state();
  state_t t2 = t->add_state();
  state_t t3 = t->add_state();
  context_t c = t->get_context();
  
  // These highlight the new function get_label(std::string,unsigned) for transducers
  std::cerr << "Trying label a on tape 0:" << std::endl;
  c->get_label("a",0);
  
  std::cerr << "Trying label a on tape 1:" << std::endl;
  try { c->get_label("a",1); throw std::runtime_error("Wrong label on tape 1 not detected."); } 
  catch (const std::domain_error& e) { std::cerr << e.what() << std::endl; }

  std::cerr << "Trying label a on tape 2:" << std::endl;
  try { c->get_label("a",2); throw std::runtime_error("Index out of range not detected"); } 
  catch (const std::out_of_range& e) { std::cerr << e.what() << std::endl; }

  //These highlight the support in any_t of false tuples (in fact std::list<any_t>)
  any_t label('a','b');
  std::cerr << "Successfully created label: " << label << std::endl;

  //This only works due to fallback on string: the `tuple` any_t displays itself
  //as the string "(a,b)" and is parsed again at the sttc level.
  // Btw, these parentheses seem dubious.
  t->set_transition(t0,t0,label,1);
  transition_t x = t->set_transition(t1,t1,any_t('b',"\\e"),1);

  // In this instance, the real tuple is boxed into any_t and no translation to
  // and from std::string is paid.
  t->set_transition(t2,t2,t->label_of(x),true);

  std::cerr << JSON << t << std::endl;
  
  // This now fails because of sttc representation of epsilon
  std::cerr << "Trying to use \"\" as label for tape 0" << std::endl;
  try { t->set_transition(t3,t3, any_t("","a"), true ); 
        throw std::runtime_error("Empty string allow as epsilon in transducers.");}
  catch (any_cast_exception const& e) { std::cerr << "Any_cast exception: " << e.what() << std::endl; }


  std::cerr << std::endl << "****  LIVING THE DREAM ? *******************************" << std::endl << std::endl;
  std::vector<context::labelset_description> v;
  v.emplace_back(context::letterset("ab"));
  v.emplace_back(context::nullableset(context::intletterset(12,14)));
  v.emplace_back(context::wordset("isitworking?!"));
  // r is a lat< lal_char, lan<lal_int>, law_char> automaton
  automaton_t r 
  = automaton_t(context::ltupleset(v),context::weightset("B"));
  state_t r0 = r->add_state();
  state_t r1 = r->add_state();
//   state_t r2 = r->add_state();
//   state_t r3 = r->add_state();
  transition_t y = r->set_transition(r0,r0,{'a',13,"itworks"},true);

  any_t lab2 = r->label_of(y, 2); // the label on tape 2 of transition y
  r->set_transition(r1,r1,any_t("b",14,lab2),1);
  
  any_t lab_tot = r->label_of(y); //
  std::list<any_t> lab_as_list = r->get_context()->expand_label(lab_tot);

  std::cerr << JSON << r << std::endl;
  std::list<any_t> l = r->labels_of(y);
  std::vector<any_t> vect;
  for (any_t a : l)
    vect.push_back(a);
  for (any_t a : vect)
    std::cerr << a << std::endl;

  std::cerr << "Trying to ask labels on a simple automaton:" << std::endl;
  try { std::list<any_t> l = a->labels_of(w); throw std::runtime_error("Labels_of in automaton did not fail"); }
  catch (any_cast_exception const& e) 
  { std::cerr << "Any_cast exception: " <<  e.what() << std::endl; }
  
  try { std::list<any_t> l = a->get_context()->expand_label(a->label_of(w)); 
        throw std::runtime_error("expand_label on automaton did not fail");}
  catch (any_cast_exception e) 
  { std::cerr << "Any_cast exception: " <<  e.what() << std::endl; }

  std::cerr << "Trying eval" << std::endl;
  
  automaton_t evala = load("evena");
  any_t evalw = eval(evala,"aa");
  evalw = eval(evala,'a');
  evalw = eval(evala,myword{"aba"});
  try {
    evalw = eval(evala,myword{"abc"});
    throw std::runtime_error("eval did not fail on a word with forbidden letter.");
  }
  catch (any_cast_exception const& e) {
    std::cerr << "Any_cast exception: " << e.what() << std::endl; 
  }
}

