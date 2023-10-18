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

#include<awali/sttc/automaton.hh>
#include<awali/sttc/algos/js_print.hh> // js_print
#include<awali/sttc/algos/derivation.hh> //derived_term
#include<awali/sttc/algos/standard.hh> //standard
#include<awali/sttc/algos/compact_thompson.hh> //compact_thompson
#include<awali/sttc/algos/flat.hh> //"flat" automaton
#include<awali/sttc/algos/draw_exp.hh> //"flat" automaton
#include<awali/sttc/weightset/z.hh>
#include<awali/sttc/ctx/lat.hh>
#include<awali/sttc/ctx/lan_char.hh>
#include<awali/sttc/ctx/lal_int.hh>
#include<awali/sttc/algos/exp_stats.hh>

//Static features are defined in the awali::sttc namespace
using namespace awali::sttc;


int main() {
  /* In the static library, rational expressions are just trees.
   * To handle them, a context is required: this is the ``ratexpset''
   * To create a simple context where letters are chars,
   * one can use the make_ratexpset factory, that can be templated with
   * a weightset, here we consider rational expressions over Z.
   *
   * Notice that C++11 allows to use 'auto' to avoid the writing of
   * pretty complicated types.
   */
  auto expset = make_ratexpset<z>();
  auto exp = make_ratexp(expset,"(a*<2>b)*");
  std::cout << "Expression:" << std::endl;
  /* Every function on rational expressions requires a context.
   * A rational expression can be printed as the expression itself...
   */ 
  print(expset, exp, std::cout) << std::endl;
  // or as a json files that also contains the description of the ratexpset
  js_print(expset, exp, std::cout) << std::endl;

  /* There exists several algorithms to convert a rational expression into
   * an automaton.
   * For expressions over letters, one can use the derived term automaton.
   */
  auto aut=derived_term(expset, exp);
  std::cout << "Automate (derrived term):" << std::endl;
  /* The standard format for Awali automata is the json files.
   */
  js_print(aut, std::cout) << std::endl;
  /*
   * Access to some statistics on the rational expression
   */
  auto stats = exp_stats(expset, exp);
  std::cout << "length: " << stats.length  //number of leaves
            << " size: " << stats.size    //number of nodes
            << " height: " << stats.height  
            << " star height: " << stats.star_height << std::endl;

  /* Another example
   * We want to deal with expressions over pairs (representing transductions).
   */
  std::cout << "Another expression:" << std::endl;
  auto tdc_expset=make_tdc_ratexpset();
  auto tdc_exp = make_ratexp(tdc_expset,"[a,\\e]*[b,y]");
  auto tdc_dexp = draw_exp(tdc_expset, tdc_exp);
  //js_print(tdc_dexp, std::cout) << std::endl;
  js_print(tdc_expset, tdc_exp, std::cout) << std::endl;
  /* Since labels are not letters, one can not use ther derivation.
   * The standard automaton is a construction that applies on
   * any type of expression.
   */
  auto tdc = standard(tdc_expset, tdc_exp);
  js_print(tdc, std::cout) << std::endl;

  /* An advanced example
   * To deal with letters that are int
   *
   * There is no helper to build contexts where letters are int.
   * We must first declare the context of automata :
   * The first parameter is the type of ``labelset'' : here Labels Are Letters (lal) which are implemented by the type int
   * The second parameter is the type of ``weighset'' : here there is no weights, thus the weights are Boolean : b
   */
  using int_context_t = context<ctx::lal_int,b>;
  int_context_t int_ctx{ctx::lal_int{},b()};
  /* Next, the ratexpset (type and value) is derived from the context.
   * We specify that we only use trivial identities on expressions 
   * (no associativity nor distributivity)
   */
  using int_expset_t = ratexpset_of<int_context_t>;
  int_expset_t int_expset{get_rat_context(int_ctx), int_expset_t::identities_t::trivial};
  auto int_exp = make_ratexp(int_expset,"0*4.12+5");
  js_print(int_expset, int_exp, std::cout) << std::endl;
  /* We use here another construction,
   * compact_thompson builds a Thompson like automaton
   * with less epsilon transitions.
   */
  //auto aut_int = compact_thompson(int_expset, int_exp);
  //js_print(aut_int, std::cout) << std::endl;
  //Another variant of Thompson automaton
  auto aut_flat = compact_thompson(int_expset, int_exp);
  js_print(aut_flat, std::cout) << std::endl;  
  dot(aut_flat, std::cerr);
}
