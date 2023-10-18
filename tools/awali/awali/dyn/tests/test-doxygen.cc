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

using namespace awali;
using namespace awali::dyn;

/* This file contains the examples that are given in Doxygen.  
 * If it fails to compile/execute.  Please modify the doxygen doc accordingly.
 */

int main(int argc, char**argv) {


  /*
  These are examples given in class ratexp_t; if you have to change them,
  please change the doxygen doc!
  */
  {
    ratexp_t exp = ratexp_t::from("(a+b)*a(a+b)*");   
  }{
    ratexp_t exp = ratexp_t::from("(a+b)*a(a+b)*", "B", "abc"); 
  }{
    ratexp_t exp = ratexp_t::from("(<1>a+<-1>b)*","Z-min-plus"); 
  }{
    ratexp_t exp = ratexp_t::with_tuple_labels::from("([a,a]+[b,b])*",2);
    transducer_t tdc = exp_to_aut(exp);
  }{
    ratexp_t exp = ratexp_t::with_int_labels::from("(0+1)*1(<2>0+<2>1)*","Z");
  }



  /*
  These are examples given in class automaton_t if you have to change them,
  please change the doxygen doc!
  */
  {
    std::vector<context::labelset_description> v = 
      {
        context::letterset("ab"),
        context::nullableset(context::intletterset(12,14)),
        context::wordset("bcd")
      };
    automaton_t aut = automaton_t(context::ltupleset(v),
                                  context::weightset("Z"));
  }


  /*
  These are examples given in class automaton_t::with_int_labels if you have to
  change them, please change the doxygen doc!
  */
  {
    automaton_t aut = automaton_t::with_int_labels::from_size(5); 
  }{
    automaton_t aut = automaton_t::with_int_labels::from_range(2,5); 
  }{
    automaton_t aut = automaton_t::with_int_labels::from_range(2, 5, "Z"); 
  }


  /* 
   These are examples given in class automaton_t.
   */
  {
    automaton_t aut = automaton_t::from("ab"); // Builds an empty automaton over the alphabet 
    
    state_t s = aut->add_state(); // adding two states
    state_t t = aut->add_state(); 

    aut->set_initial(s);  // making state `s` initial
    aut->set_final(t);    //making state `t` final

    aut->set_transition(s,s,'b'); // A loop labelled by 'b' on both states
    aut->set_transition(t,t,'b');

    aut->set_transition(s,t,'a'); // Reading an 'a' changes the states
    aut->set_transition(t,s,'a'); 

    //aut now recognizes the words over {a,b} with an odd number of 'a's.
    
    //pdfdisplay(aut); // Outputs `aut` to a temporary file as a pdf image
                     // and opens it with the default pdf viewer.
  }{
     ratexp_t exp = ratexp_t::from("(a+b)*bb(a+b)*");
     automaton_t aut1 = exp_to_aut(exp);
     aut1 = minimal_automaton(aut1); // determinizes and minimizes `aut`

     // The same result may be obtained directly from the expression.
     automaton_t aut2 = minimal_automaton(exp);

     bool test =  are_isomorphic(aut1,aut2); // `test` equals `true`
  }{
    // Builds an automaton with transitions labelled in {a,b} and weighted in Z.
    automaton_t aut = automaton_t::from("ab","Z", false); 
    
    state_t s = aut->add_state();
    state_t t = aut->add_state();

    aut->set_initial(s, 1); // Set initial weight of `s` to 1
    aut->set_final(t, 1); // Set final weight of `t` to 1

    aut->set_transition(s,s,'a',1);
    aut->set_transition(s,s,'b',1);
    aut->set_transition(t,t,'a',1);
    aut->set_transition(t,t,'b',1);
    aut->set_transition(s,t,'b',1);

    // `aut` is an automaton that associate with every word the number of b' in it.
    int i = (int) eval(aut,"bbbaaabbaaab"); // `i` equals 6

    save(aut,"count-b.json"); // Saves `aut` in JSON format to file "count-b.json"
  }


  /* 
   These are examples given in class options_t.
   */
  {
    ratexp_t exp = ratexp_t::from("(aa)*a(aa)*");
    // Computing the Thompson automaton of exp
    automaton_t aut1 = exp_to_aut(exp, {EXP_TO_AUT_ALGO=THOMPSON} );
    // Compute the automaton resulting from the "Derived Term" algorithm
    automaton_t aut2 = exp_to_aut(exp, {EXP_TO_AUT_ALGO=DERIVED_TERM} );
  }{
    ratexp_t exp = ratexp_t::from("(aa)*aa(aa)*");
    // Next line computes the minimal automaton accepting the language
    // of `exp`.
    // It basically calls in order: 
    //   1. exp_to_aut, 2. proper, 3. determinize, 4. min_quotient
    // Optional parameters for all these functions may be provided.
    automaton_t aut = minimal_automaton(exp, 
        { EXP_TO_AUT_ALGO=THOMPSON,
          MINIM_ALGO=DETERMINIZE_QUOTIENT,
          QUOTIENT_ALGO=MOORE              } );
  }{
    options_t opt = { IN_PLACE=true, 
                      KEEP_HISTORY=true, //This is useless since KEEP_HISTORY
                                         //default to `true` anyway
                      EXP_TO_AUT_ALGO=THOMPSON
                    };
    ratexp_t exp = ratexp_t::from("(a+b)*aba(a+b)*");
    automaton_t aut = exp_to_aut(exp, opt); // Only EXP_TO_AUT_ALGO is 
                                            // meaningful, Thompson's 
                                            // algorithm is used.
    proper(aut, opt); // IN_PLACE is meaningful, hence this modifies `aut`
  }
  
  
  /* 
   These are examples given in dyn/core/typedefs.hh
   */
  {
    automaton_t aut = load("a1");
    // In a1, labels are chars so we may cast it directly:
    char label = (char) aut->label_of(aut->transitions()[0]) ; 
    // NB: the previous line takes the label of the first transition in an
    // arbitrary order; its meaning is dubious.
  }{
    automaton_t aut = load("binary");
    // In `binary`, weights are integers so we may cast it as such:
    int weight = (int) eval(aut, "101010");
  }
}
