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

#define NO_EDIT
#include <awalipy/bridge-to-dyn/automaton.hh>
#include <awalipy/bridge-to-dyn/ratexp.hh>
#include <awali/dyn.hh>

#include <fstream>
#include <sstream>

namespace awali { namespace py {

  namespace internal {
    

    /** This class allows to load the dynamic library with dlopen once.
     * It makes all symbols of the dynamic library available in the static
     * modules.
     */
    struct dynamic_load_of_dyn {
      dynamic_load_of_dyn() {
        std::vector<std::string> dirs = dyn::loading::get_dynlib_directory();
        void* open = nullptr;
        for (std::string dir : dirs) {
          std::string path = std::string(dir+"/"+"libawalidyn"+std::string(CMAKE_SHARED_LIBRARY_SUFFIX));
          open = dlopen( path.c_str(), RTLD_NOW|RTLD_GLOBAL );
          if (open != nullptr)
            break;
        }
        if (open == nullptr)
          throw std::runtime_error("dlopen of Awali C++ dynamic library failed.");
      }
    }
    static dynamic_load_of_dyn;
  }


   

  //accessible
  std::vector<int> accessible_states_(basic_automaton_t aut) {
    auto a = (dyn::automaton_t)aut;
    auto v= dyn::accessible_states(a);
    std::vector<int> r;
    for(auto s : v) {
      r.emplace_back(s-2);
    }
    return r;
  }

  std::vector<int> coaccessible_states_(basic_automaton_t aut) {
    auto a = (dyn::automaton_t)aut;
    auto v= dyn::coaccessible_states(a);
    std::vector<int> r;
    for(auto s : v) {
      r.emplace_back(s-2);
    }
    return r;
  }

  std::vector<int> useful_states_(basic_automaton_t aut) {
    auto a = (dyn::automaton_t)aut;
    auto v= dyn::useful_states(a);
    std::vector<int> r;
    for(auto s : v) {
      r.emplace_back(s-2);
    }
    return r;
  }

  size_t num_accessible_states_(basic_automaton_t aut) {
    return dyn::num_accessible_states((dyn::automaton_t)aut);
  }

  size_t num_coaccessible_states_(basic_automaton_t aut) {
    return dyn::num_coaccessible_states((dyn::automaton_t)aut);
  }

  size_t num_useful_states_(basic_automaton_t aut) {
    return useful_states_(aut).size();
  }

  basic_automaton_t accessible_(basic_automaton_t aut) {
    return basic_automaton_t(dyn::accessible((dyn::automaton_t)aut));
  }

  basic_automaton_t coaccessible_(basic_automaton_t aut) {
    return basic_automaton_t(dyn::coaccessible((dyn::automaton_t)aut));
  }

  basic_automaton_t trim_(basic_automaton_t aut) {
    return basic_automaton_t(dyn::trim((dyn::automaton_t)aut));
  }

  void accessible_here_(basic_automaton_t aut) {
    dyn::accessible((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  void coaccessible_here_(basic_automaton_t aut) {
    dyn::coaccessible((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  void trim_here_(basic_automaton_t aut) {
    dyn::trim((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  bool is_trim_(basic_automaton_t aut) {
    return dyn::is_trim((dyn::automaton_t)aut);
  }

  basic_automaton_t concatenate_
    (basic_automaton_t aut1, basic_automaton_t aut2)
  {
    return basic_automaton_t(dyn::concatenate((dyn::automaton_t) aut1, (dyn::automaton_t) aut2));
  }



  bool is_accessible_(basic_automaton_t aut) {
    return dyn::is_accessible((dyn::automaton_t)aut);
  }

  bool is_coaccessible_(basic_automaton_t aut) {
    return dyn::is_coaccessible((dyn::automaton_t)aut);
  }


  bool is_useless_(simple_automaton_t aut) {
    return dyn::is_useless((dyn::automaton_t)aut);
  }

  bool is_empty_(simple_automaton_t aut) {
    return dyn::is_empty((dyn::automaton_t)aut);
  }

  //automaton
  /*
  simple_automaton_t make_automaton_from_context(context_t ctx) {
    return simple_automaton_t(make_automaton_from_context(ctx));
  }

  simple_automaton_t make_automaton_from_description(context_description cd) {
    return simple_automaton_t(make_automaton_from_context(cd));
  }

  simple_automaton_t make_automaton_from_label_and_weight_description(labelset_description ls, weightset_description ws) {
    return simple_automaton_t(make_automaton_from_context(ls,ws));
  }

  context_t make_context(context_description cd);
  */

//   simple_automaton_t make_weighted_automaton_(std::string alphabet, std::string semiring) {
//     try {
//       return simple_automaton_t(make_automaton(alphabet, semiring));
//     } catch(const std::runtime_error& e) {
//       error_no=WRONG_SEMIRING;
//       return simple_automaton_t(make_automaton(""));
//     }
//   }

//   simple_automaton_t make_NFA_(std::string alphabet) {
//     return simple_automaton_t(make_automaton(alphabet));
//   }
//
//   simple_automaton_t make_weighted_automaton_with_eps_(std::string alphabet, std::string semiring) {
//     try {
//       return simple_automaton_t(make_automaton_with_eps(alphabet, semiring));
//     } catch(const std::runtime_error& e) {
//       error_no=WRONG_SEMIRING;
//       return simple_automaton_t(make_automaton(""));
//     }
//   }

  simple_automaton_t make_NFA_with_eps2(std::string alphabet, std::string semiring) {
    return simple_automaton_t(dyn::automaton_t::from(alphabet, semiring, true));
  }

  simple_automaton_t make_NFA_with_eps1(std::string alphabet) {
    return simple_automaton_t(dyn::automaton_t::from(alphabet, true));
  }

//   simple_automaton_t parse_json_automaton_(const std::string& s) {
//     try {
//       std::istringstream is(s);
//       return simple_automaton_t(parse_automaton(is));
//     } catch(const std::runtime_error& e) {
//       error_no=PARSE_ERROR;
//     }
//     return simple_automaton_t(make_automaton(""));
//   }

  basic_automaton_t copy_(basic_automaton_t aut) {
    return basic_automaton_t(dyn::copy((dyn::automaton_t)aut));
  }

  simple_automaton_t support_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::support((dyn::automaton_t)aut));
  }

  simple_automaton_t characteristic_(simple_automaton_t aut, std::string const& semiring) {
//     try {
    return simple_automaton_t(dyn::characteristic((dyn::automaton_t)aut, semiring));
//     } catch(const std::runtime_error& e) {
//       error_no=WRONG_SEMIRING;
//       return simple_automaton_t(make_automaton(""));
//     }
  }

//   int add_state_(simple_automaton_t aut, std::string s) {
//     return add_state((dyn::automaton_t)aut, s)+2;
//   }

  //derivation

  static bool operator< (const simple_ratexp_t& exp, const simple_ratexp_t& exp2) {
    return (dyn::ratexp_t)exp < (dyn::ratexp_t)exp2;
  }

  std::map<simple_ratexp_t, std::string> 
  derivation_(simple_ratexp_t exp, std::string label, bool breaking = false) {
    auto res= dyn::internal::derivation((dyn::ratexp_t) exp, ((dyn::ratexp_t)exp)->get_context()->get_label(label), breaking);
    std::map<simple_ratexp_t, std::string> simple_res;
    for(auto p : res)
      simple_res.emplace( simple_ratexp_t(p.first), 
                          exp.weight_to_string(p.second) );
    return simple_res;
  }

//   simple_automaton_t derived_term_(simple_ratexp_t exp, bool breaking = false, bool keep_history = true) {
//     return simple_automaton_t(dyn::derived_term((dyn::ratexp_t) exp, breaking, keep_history));
//   }
  //determinize
  simple_automaton_t determinize_(simple_automaton_t aut, bool history = true) {
    return simple_automaton_t(dyn::determinize((dyn::automaton_t)aut,
      {dyn::KEEP_HISTORY=history}));
  }

  bool are_equivalent_(simple_automaton_t aut, simple_automaton_t aut2) {
    return dyn::are_equivalent((dyn::automaton_t)aut, (dyn::automaton_t)aut2);
  }

  simple_automaton_t complement_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::complement((dyn::automaton_t)aut));
  }

  void complement_here_(simple_automaton_t aut) {
    dyn::complement((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  simple_automaton_t complete_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::complete((dyn::automaton_t)aut));
  }

  void complete_here_(simple_automaton_t aut) {
    dyn::complete((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  simple_automaton_t reduce_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::reduce((dyn::automaton_t)aut));
  }

  simple_automaton_t left_reduce_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::left_reduce((dyn::automaton_t)aut));
  }

  simple_automaton_t right_reduce_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::right_reduce((dyn::automaton_t)aut));
  }

  bool is_deterministic_(simple_automaton_t aut) {
    return dyn::is_deterministic((dyn::automaton_t)aut);
  }

  bool is_complete_(simple_automaton_t aut) {
    return dyn::is_complete((dyn::automaton_t)aut);
  }

  bool is_ambiguous_(simple_automaton_t aut) {
    return dyn::is_ambiguous((dyn::automaton_t)aut);
  }

  //edit
  /*
  void tdc_edit(simple_automaton_t tdc);
  void edit(simple_automaton_t aut);
  //eliminate
  void eliminate_state(simple_automaton_t aut, state_t s);
  void eliminate_state(simple_automaton_t aut, std::string st);
  */
  //eval
  std::string eval_(simple_automaton_t aut, const std::string& word) {
    std::ostringstream ws;
    ws << dyn::eval((dyn::automaton_t) aut, word);
    return ws.str();
  }

  std::map<std::string,std::string> enumerate_(simple_automaton_t aut, unsigned max) {
    auto res = dyn::enumerate((dyn::automaton_t)aut, max);
    std::map<std::string,std::string> simple_res;
    for(auto p : res) {
      std::ostringstream words, ws;
      words << p.first;
      ws << p.second;
      simple_res.emplace(words.str(), ws.str());
    }
    return simple_res;
  }

  std::map<std::string,std::string> shortest_(simple_automaton_t aut, unsigned max) {
    auto res = dyn::shortest((dyn::automaton_t)aut, max);
    std::map<std::string,std::string> simple_res;
    for(auto p : res) {
      std::ostringstream words, ws;
      words << p.first;
      ws << p.second;
      simple_res.emplace(words.str(), ws.str());
    }
    return simple_res;
  }

  //factor
  void prefix_here_(simple_automaton_t aut) {
    dyn::prefix((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  simple_automaton_t prefix_(simple_automaton_t aut) {
    return dyn::prefix((dyn::automaton_t)aut);
  }

  void suffix_here_(simple_automaton_t aut) {
    dyn::suffix((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  simple_automaton_t suffix_(simple_automaton_t aut) {
    return dyn::suffix((dyn::automaton_t)aut);
  }

  void factor_here_(simple_automaton_t aut) {
    dyn::factor((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  simple_automaton_t factor_(simple_automaton_t aut) {
    return dyn::factor((dyn::automaton_t)aut);
  }

  //factories
  simple_automaton_t make_n_ultimate_(unsigned n, std::string alphabet) {
    return simple_automaton_t(dyn::factory::n_ultimate(n, alphabet));
  }

  simple_automaton_t make_divkbaseb_(unsigned k, unsigned b, std::string alphabet) {
    return simple_automaton_t(dyn::factory::divkbaseb(k, b, alphabet));
  }

  simple_automaton_t make_double_ring_(unsigned n, const std::vector<unsigned>& finals, std::string alphabet) {
    return simple_automaton_t(dyn::factory::double_ring(n, finals, alphabet));
  }

  simple_automaton_t make_ladybird_(unsigned n, std::string alphabet) {
    return simple_automaton_t(dyn::factory::ladybird(n, alphabet));
  }

  simple_automaton_t make_cerny_(unsigned n) {
    return simple_automaton_t(dyn::factory::cerny(n, "ab"));
  }

  simple_automaton_t make_witness_(unsigned n, std::string alphabet) {
    return simple_automaton_t(dyn::factory::witness(n, alphabet));
  }

//minimize
//    simple_automaton_t minimal_brzozowski_(simple_automaton_t aut) {
//      return simple_automaton_t(dyn::minimal_automaton((dyn::automaton_t)aut, BRZOZOWSKI));
//   }

  simple_automaton_t minimal_automaton_(simple_automaton_t aut, std::string method) {
    awali::dyn::options_t opts = {};
    if (method == "moore") 
      opts += ( dyn::QUOTIENT_ALGO=MOORE );
    else if (method =="hopcroft")
      opts += ( dyn::QUOTIENT_ALGO=HOPCROFT);
    else if (method == "brzozowski")
      opts += ( dyn::MINIM_ALGO=BRZOZOWSKI);
    else
      throw std::domain_error("Valid values for minimal_automaton are: \"hopcroft\", \"moore\" and \"brzozowski\"");

    return simple_automaton_t(dyn::minimal_automaton((dyn::automaton_t)aut,
                              opts));
  }

  simple_automaton_t min_quotient_(simple_automaton_t aut, std::string method ="default") {
    return simple_automaton_t(dyn::min_quotient((dyn::automaton_t)aut,
      {dyn::QUOTIENT_ALGO=method}));
  }
  simple_automaton_t min_coquotient_(simple_automaton_t aut, std::string method="default") {
    return simple_automaton_t(dyn::min_coquotient((dyn::automaton_t)aut,
      {dyn::QUOTIENT_ALGO=method}));
  }

  simple_automaton_t quotient_(simple_automaton_t aut, std::vector<std::vector<int>>& equiv) {
    auto a=(dyn::automaton_t)aut;
    std::vector<std::vector<dyn::state_t>> eqs;
    for(auto v : equiv) {
      std::vector<dyn::state_t> part;
      for(auto s: v)
	part.emplace_back(s-2);
      eqs.emplace_back(part);
    }
    return simple_automaton_t(dyn::quotient(a,eqs));
  }
  simple_automaton_t coquotient_(simple_automaton_t aut, std::vector<std::vector<int>>& equiv) {
    auto a=(dyn::automaton_t)aut;
    std::vector<std::vector<dyn::state_t>> eqs;
    for(auto v : equiv) {
      std::vector<dyn::state_t> part;
      for(auto s: v)
        part.emplace_back(s-2);
      eqs.emplace_back(part);
    }
    return simple_automaton_t(dyn::coquotient(a,eqs));
  }

  //output
//   std::string dot_(simple_automaton_t aut, bool history=false, bool dot2tex = false) {
//     std::ostringstream os;
//     dot((dyn::automaton_t) aut, os, history, dot2tex) << std::endl;
//     return os.str();
//   }
//
//   std::string fado_(simple_automaton_t aut) {
//     std::ostringstream os;
//     fado((dyn::automaton_t) aut, os) << std::endl;
//     return os.str();
//   }
//
//   std::string grail_(simple_automaton_t aut) {
//     std::ostringstream os;
//     grail((dyn::automaton_t) aut, os) << std::endl;
//     return os.str();
//   }
//
//   std::string json_(simple_automaton_t aut) {
//     std::ostringstream os;
//     json((dyn::automaton_t) aut, os) << std::endl;
//     return os.str();
//   }

//   simple_automaton_t fado_parse_(const std::string& s) {
//     std::istringstream in(s);
//     return simple_automaton_t(fado(in));
//   }
//
//   simple_automaton_t grail_parse_(const std::string& s) {
//     std::istringstream in(s);
//     return simple_automaton_t(grail(in));
//   }

  //product
  simple_automaton_t product_(simple_automaton_t aut1, simple_automaton_t aut2) {
    return simple_automaton_t(dyn::product((dyn::automaton_t)aut1, (dyn::automaton_t)aut2));
  }

  simple_automaton_t power_(simple_automaton_t aut, unsigned n) {
    return simple_automaton_t(dyn::power((dyn::automaton_t)aut, n));
  }

  simple_automaton_t shuffle_(simple_automaton_t aut1, simple_automaton_t aut2) {
    return simple_automaton_t(dyn::shuffle((dyn::automaton_t)aut1, (dyn::automaton_t)aut2));
  }

  simple_automaton_t infiltration_(simple_automaton_t aut1, simple_automaton_t aut2) {
    return simple_automaton_t(dyn::infiltration((dyn::automaton_t)aut1, (dyn::automaton_t)aut2));
  }

  simple_automaton_t sum_(simple_automaton_t aut1, simple_automaton_t aut2) {
    return simple_automaton_t(dyn::sum((dyn::automaton_t)aut1, (dyn::automaton_t)aut2));
  }

  bool are_isomorphic_(simple_automaton_t aut1, simple_automaton_t aut2) {
    return dyn::are_isomorphic((dyn::automaton_t)aut1, (dyn::automaton_t)aut2);
  }

  //proper
  void proper_here_(simple_automaton_t aut, bool backward=true, bool prune=true) {
    dyn::proper((dyn::automaton_t)aut, 
      {dyn::IN_PLACE=true, 
       dyn::DIRECTION= (backward?BACKWARD:FORWARD), dyn::PRUNE=prune});
  }

  simple_automaton_t proper_(simple_automaton_t aut, bool backward=true, bool prune=true) {
    return simple_automaton_t(dyn::proper((dyn::automaton_t)aut, {dyn::DIRECTION= (backward?BACKWARD:FORWARD), dyn::PRUNE=prune}));
  }

  bool is_proper_(simple_automaton_t aut) {
    return dyn::is_proper((dyn::automaton_t)aut);
  }

  bool is_valid_(simple_automaton_t aut) {
    return dyn::is_valid((dyn::automaton_t)aut);
  }

  simple_automaton_t allow_eps_transition_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::allow_eps_transition((dyn::automaton_t)aut));
  }

  simple_automaton_t exp_to_aut_( simple_ratexp_t exp,
                                  std::string const& method="default") 
  {
    return simple_automaton_t( 
      dyn::exp_to_aut((dyn::ratexp_t)exp, {dyn::EXP_TO_AUT_ALGO=method}));
  }

//   simple_automaton_t thompson_(simple_ratexp_t exp) {
//     return simple_automaton_t(dyn::thompson((dyn::ratexp_t)exp));
//   }

  //randon
  simple_automaton_t random_DFA_(unsigned size, const std::string& alphabet) {
    return simple_automaton_t(dyn::factory::randomDFA(size,alphabet));
  }

  //ratexp
  /*
  ratexp_t make_ratexp_with_context(const std::string& exp, context_description cd);
  ratexp_t make_ratexp_with_context(const std::string& exp, labelset_description ls, weightset_description ws);
  */
//   simple_ratexp_t make_weighted_ratexp_with_alphabet_(const std::string& exp, const std::string& alph, const std::string& sr) {
//     return simple_ratexp_t(make_ratexp_with_alphabet(exp, alph, sr));
//   }
//
//   simple_ratexp_t make_regexp_with_alphabet_(const std::string& exp, const std::string& alph) {
//     return simple_ratexp_t(make_ratexp_with_alphabet(exp, alph));
//   }
//
//   simple_ratexp_t make_weighted_ratexp_(const std::string& exp, const std::string& sr) {
//     return simple_ratexp_t(make_ratexp(exp, sr));
//   }
//
//   simple_ratexp_t make_regexp_(const std::string& exp) {
//     return simple_ratexp_t(make_ratexp(exp));
//   }

//  simple_ratexp_t aut_to_exp_(simple_automaton_t aut) {
//    return simple_ratexp_t(dyn::aut_to_exp((dyn::automaton_t)aut));
//  }.   mod. js  200316

  simple_ratexp_t aut_to_exp_(simple_automaton_t aut, const std::string& order="default") {
    return simple_ratexp_t(dyn::aut_to_exp((dyn::automaton_t)aut, {dyn::STATE_ELIM_ORDER=order}));
  }

  basic_automaton_t lift_(basic_automaton_t aut) {
    return basic_automaton_t(dyn::lift::lift((dyn::automaton_t)aut));
  }

  simple_ratexp_t expand_(simple_ratexp_t exp) {
    return simple_ratexp_t(dyn::expand((dyn::ratexp_t)exp));
  }

  simple_ratexp_t star_normal_form_(simple_ratexp_t exp) {
    return simple_ratexp_t(dyn::star_normal_form((dyn::ratexp_t)exp));
  }

  unsigned star_height_(simple_ratexp_t exp) {
    return dyn::star_height((dyn::ratexp_t)exp);
  }

//   simple_ratexp_t parse_json_ratexp_(const std::string& s) {
//     std::istringstream i(s);
//     return simple_ratexp_t(parse_ratexp(i));
//   }



//   simple_automaton_t standard_(simple_ratexp_t exp) {
//     return simple_automaton_t(dyn::standard((simple_ratexp_t)exp));
//   }

  //synchronize
  simple_transducer_t synchronize_(simple_transducer_t tdc) {
    return simple_transducer_t(dyn::synchronize(tdc));
  }

  bool is_synchronizable_(simple_transducer_t tdc) {
    return dyn::is_synchronizable(tdc);
  }

//   //Transducers
//   simple_automaton_t make_transducer_(std::vector<std::string> alphabets) {
//    return simple_automaton_t(make_transducer(alphabets));
//   }
//
//   void set_tdc_transition_(simple_automaton_t tdc, state_t src, state_t dst, const std::vector<std::string>& labels) {
//     set_tdc_transition((dyn::automaton_t)tdc, src, dst, labels);
//   }
//
//   void add_tdc_transition_(simple_automaton_t tdc, state_t src, state_t dst, const std::vector<std::string>& labels) {
//     add_tdc_transition((dyn::automaton_t)tdc, src, dst, labels);
//   }
//
//   void del_tdc_transition_(simple_automaton_t tdc, state_t src, state_t dst, const std::vector<std::string>& labels) {
//     del_tdc_transition((dyn::automaton_t)tdc, src, dst, labels);
//   }
//
//   void set_tdc_wtransition_(simple_automaton_t tdc, state_t src, state_t dst, const std::vector<std::string>& labels, const std::string& w) {
//     set_tdc_transition((dyn::automaton_t)tdc, src, dst, labels, w);
//   }
//
//   void add_tdc_wtransition_(simple_automaton_t tdc, state_t src, state_t dst, const std::vector<std::string>& labels, weight_t w) {
//     add_tdc_transition((dyn::automaton_t)tdc, src, dst, labels, w);
//   }

//   std::vector<std::string> get_tdc_label_(simple_automaton_t tdc, transition_t tr) {
//     return get_tdc_label((dyn::automaton_t)tdc, tr);
//   }

  simple_automaton_t domain_(simple_transducer_t tdc) {
    return dyn::domain(tdc);
  }

  simple_automaton_t image_(simple_transducer_t tdc) {
    return dyn::image(tdc);
  }

  simple_transducer_t images_(simple_transducer_t tdc) {
    return dyn::images(tdc);
  }

  simple_automaton_t projection_(simple_transducer_t tdc, unsigned i) {
    return dyn::projection(tdc, i);
  }

  simple_transducer_t inverse_(simple_transducer_t tdc) {
    return dyn::inverse(tdc);
  }

  simple_transducer_t compose_(simple_transducer_t tdc1, simple_transducer_t tdc2) {
    return dyn::compose(tdc1, tdc2);
  }

  simple_transducer_t partial_identity_(simple_automaton_t aut){
    return dyn::partial_identity((dyn::automaton_t)aut);
  }

  bool is_functional_(simple_transducer_t tdc) {
    return dyn::is_functional(tdc);
  }

  //simple_automaton_t lift_tdc(simple_automaton_t tdc);

  //transpose
  void transpose_here_(basic_automaton_t aut) {
    dyn::transpose((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  basic_automaton_t transpose_(basic_automaton_t aut) {
    return simple_automaton_t(dyn::transpose((dyn::automaton_t)aut));
  }

//   void display_(simple_automaton_t aut) {
//     std::string s = dyn::loading::get_lib_directory()+"/tmp.gv";
//     std::ofstream o(s);
//     o << dot_(aut) << std::endl;
//     o.close();
//     system(("dotty "+s).c_str());
//   }

  basic_automaton_t load_automaton_(const std::string& s, const std::string& fmt) {
    return basic_automaton_t(dyn::load(s, {dyn::IO_FORMAT=fmt}));
  }

  void save_automaton_(basic_automaton_t aut, const std::string& s, const std::string& fmt) {
    dyn::save(aut,s, {dyn::IO_FORMAT=fmt});
  }




  simple_automaton_t eval_tdc_(simple_automaton_t aut, simple_transducer_t tdc) {
    return dyn::eval_tdc(aut, tdc);
  }



  //standard
  bool is_standard_(simple_automaton_t aut) {
    return dyn::is_standard((dyn::automaton_t)aut);
  }

  simple_automaton_t standard_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::standard((dyn::automaton_t)aut));
  }

  void standard_here_(simple_automaton_t aut) {
    dyn::standard((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  simple_automaton_t concatenate_(simple_automaton_t aut, simple_automaton_t aut2) {
    return simple_automaton_t(dyn::concatenate((dyn::automaton_t)aut, (dyn::automaton_t)aut2));
  }

  void concatenate_here_(simple_automaton_t aut, simple_automaton_t aut2) {
    dyn::concatenate((dyn::automaton_t)aut, (dyn::automaton_t)aut2,
        {dyn::IN_PLACE=true});
  }

  simple_automaton_t sum_of_standad_(simple_automaton_t aut, simple_automaton_t aut2) {
    return simple_automaton_t(dyn::sum_of_standard((dyn::automaton_t)aut, (dyn::automaton_t)aut2));

}

  void sum_of_standard_here_(simple_automaton_t aut, simple_automaton_t aut2) {
    dyn::sum_of_standard((dyn::automaton_t)aut, (dyn::automaton_t)aut2, {dyn::IN_PLACE=true});
  }

  simple_automaton_t star_(simple_automaton_t aut) {
    return simple_automaton_t(dyn::star((dyn::automaton_t)aut));
  }

  void star_here_(simple_automaton_t aut) {
    dyn::star((dyn::automaton_t)aut, {dyn::IN_PLACE=true});
  }

  std::vector<std::vector<std::string>> example_ratexps_() {
    std::map<std::string,dyn::loading::file_loc_t> automata 
      = dyn::loading::example_ratexps();
    std::vector<std::vector<std::string>> res;
    res.reserve(automata.size());

    for (auto a : automata) {
      json_ast_t jo = json_ast::from_file(a.second.full());
      std::string desc;
      if(jo->has_path({"metadata","caption"}))
        desc = jo->at_path({"metadata","caption"})->to_string();
      else
        desc = "";
      res.push_back({a.first, desc});
    }
    return res;
  }
  std::vector<std::vector<std::string>> example_automata_() {
    std::map<std::string,dyn::loading::file_loc_t> automata 
      = dyn::loading::example_automata();
    std::vector<std::vector<std::string>> res;
    res.reserve(automata.size());

    for (auto a : automata) {
      json_ast_t jo = json_ast::from_file(a.second.full());
      std::string desc;
      if(jo->has_path({"metadata","caption"}))
        desc = jo->at_path({"metadata","caption"})->to_string();
      else
        desc = "";
      res.push_back({a.first, desc});
    }
    return res;
  }

  basic_automaton_t display_rat_(simple_ratexp_t r) {
      return basic_automaton_t(dyn::draw_exp(r));
  }

  basic_automaton_t condensation_(basic_automaton_t a) {
      return basic_automaton_t(dyn::condensation(a));
  }

  std::vector<std::vector<dyn::state_t>> sccs_ (basic_automaton_t aut) {
    auto r = dyn::strongly_connected_components((dyn::automaton_t)aut);
    std::vector<std::vector<dyn::state_t>> result;
    result.reserve(r.partition.size());
    for (auto v : r.partition) {
      std::vector<dyn::state_t> res;
      res.reserve(v.size());
      for (auto s : v)
        res.push_back(s-2);
      result.push_back(res);
    }
    return result;
  }

  std::vector<dyn::state_t> scc_of_ (basic_automaton_t aut, dyn::state_t s) {
    auto l = dyn::scc_of((dyn::automaton_t)aut, s+2);
    std::vector<dyn::state_t> res;
    res.reserve(l.size());
    for (auto s : l)
      res.push_back(s-2);
    return res;
  }

  std::vector<std::string> available_semirings() {
    return dyn::context::all_weightset_public_static_names();
  }

}}// end of namespace awali::py
