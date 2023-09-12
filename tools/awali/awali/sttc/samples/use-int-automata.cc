#include <awali/sttc/automaton.hh>
#include <awali/sttc/alphabets/int.hh>
#include <awali/sttc/algos/eval.hh>
#include <awali/sttc/algos/enumerate.hh>
#include <awali/sttc/algos/allow_words.hh>
#include <awali/sttc/algos/aut_to_exp.hh>
#include <awali/sttc/algos/derivation.hh>

//Static features are defined in the awali namespace
using namespace awali::sttc;

int main() {
  using letterset_t=letterset<set_alphabet<int_letters>>;
  using context_t=context<letterset_t,b>;
  context_t ctx{letterset_t{-1,0,1}, b()};
  auto aut=make_mutable_automaton(ctx);  
  unsigned s[3];
  for(int i=0; i<3; i++)
    s[i]=aut->add_state();
  /* a mutable_automaton is actually a (shared) pointer;
     to call the method, one have to use ->
  */
  aut->set_initial(s[0]);
  aut->set_final(s[2]);
  aut->set_transition(s[0], s[0], 0);
  aut->set_transition(s[0], s[0], 1);
  aut->set_transition(s[0], s[1], 0);
  aut->set_transition(s[1], s[2], -1);
  aut->set_transition(s[2], s[2], 1);
  aut->set_transition(s[2], s[2], 0);

  js_print(aut, std::cout) << std::endl;

  auto autcp = compact(aut);
  js_print(autcp, std::cout) << std::endl;

  
  auto wlbs = get_wordset(*aut->labelset());
  //Print a word:
  wlbs.print(std::basic_string<int>{1,0,-1}, std::cout) << std::endl;  
  std::cout << "Evaluation of two words:" << std::endl;
  std::cout << "1:0:-1 :" << eval(aut,wlbs.parse("1:0:-1")) << std::endl;
  std::cout << "1:1 :" << eval(aut,std::basic_string<int>{1,1}) << std::endl;
  std::cout << "Enumeration up to length 3:" << std::endl;
  for(auto p : enumerate(aut, 3))
    wlbs.print(p.first,std::cout) << std::endl;

  auto ratexpset=ratexpset_of<context_t>(get_rat_context(ctx),rat::identities::trivial);
  auto exp=aut_to_exp(aut);

  auto autb=derived_term(ratexpset, exp);
  js_print(autb, std::cout) << std::endl;

  std::ostringstream oss;
  ratexpset.print(exp,oss);
  std::cout << oss.str() << std::endl;
  auto expb = parse_exp(ratexpset, oss.str());
  ratexpset.print(expb, std::cout) << std::endl;  
  return 0;
}
