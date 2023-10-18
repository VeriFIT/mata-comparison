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

#ifndef DYN_SIMPLE_RATEXP_HH
#define DYN_SIMPLE_RATEXP_HH

#include<sstream>
#include<awalipy/bridge-to-dyn/utils.hh>

namespace awali { namespace py {
    const int KIND_ZERO = (int) dyn::ExpKind::ZERO, 
              KIND_ONE = (int) dyn::ExpKind::ONE, 
              KIND_ATOM = (int) dyn::ExpKind::ATOM, 
              KIND_SUM = (int) dyn::ExpKind::SUM, 
              KIND_PROD = (int) dyn::ExpKind::PROD, 
              KIND_STAR = (int) dyn::ExpKind::STAR;

  struct simple_ratexp_t {

  protected:
    dyn::ratexp_t exp_;
    
  public:
    //Initialisation and conversion
    simple_ratexp_t() : exp_() {}

    simple_ratexp_t(const dyn::ratexp_t& a) : exp_((dyn::ratexp_t)a) {}

    void rewrap_into_me_(simple_ratexp_t other) {
        exp_= other;
    }

    std::string alphabet() const {
      std::string alph= "";
      for(auto l : exp_->alphabet())
	alph+= this->label_to_string(l);
      return alph;
    }

    operator dyn::ratexp_t() const {
      return dyn::ratexp_t(exp_);
    }

    dyn::abstract_ratexp_t& to_dyn_ratexp() {
      return (*exp_);
    }


    std::string weight_to_string(dyn::any_t a) const
    { 
      return awali::py::weight_to_string(a,exp_->get_context()); 
    }

    std::string label_to_string(dyn::any_t a) const
    { 
      return awali::py::label_to_string(a,exp_->get_context()); 
    }

    simple_ratexp_t add(const simple_ratexp_t e) const {
      return simple_ratexp_t(exp_->add((dyn::ratexp_t)e));
    }

    simple_ratexp_t mult(const simple_ratexp_t e) const {
      return simple_ratexp_t(exp_->mult((dyn::ratexp_t)e));
    }

    simple_ratexp_t star() const {
      return simple_ratexp_t(exp_->star());
    }

    std::string print_() const {
      std::ostringstream os;
      exp_->print(os);
      return os.str();
    }

    std::string json() const{
      std::ostringstream os;
      os << awali::JSON << exp_ << std::endl;
      return os.str();
    }

    std::string get_weightset(std::string format = "text") const {
        return exp_->get_context()->weightset_name(format);
    }

    std::string get_static_context() const {
        return exp_->get_context()->sname();
    }

    simple_ratexp_t copy() {
      return simple_ratexp_t(exp_);
    }

    bool is_valid() {
      return dyn::is_valid(exp_);
    }

    std::string constant_term() {
      return exp_->get_context()->weight_to_string(dyn::constant_term(exp_));
    }

    std::vector<simple_ratexp_t> children() {
      std::vector<simple_ratexp_t> res;
      for (auto c: exp_->children())
        res.push_back(c);
      return res;
    }

    int get_kind() {
      return (int) exp_->get_kind();
    }

    std::string lweight() {
      return weight_to_string(exp_->lweight());
    }
    std::string rweight() {
      return weight_to_string(exp_->rweight());
    }
    std::string value() {
      return label_to_string(exp_->value());
    }

//     unsigned size() {
//       return exp_->size();
//     } 
//     
//     unsigned height() {
//       return exp_->size();
//     } 

  };
  simple_ratexp_t make_simple_ratexp1(std::string str) {
    return dyn::ratexp_t::from(str);
  }
  simple_ratexp_t make_simple_ratexp2(std::string str, std::string semiring) {
    return dyn::ratexp_t::from(str, semiring);
  }

  simple_ratexp_t make_simple_ratexp3
  (const std::string& exp, const std::string& weightset, const std::string& alphabet) 
  {
    return dyn::ratexp_t::from(exp, weightset, alphabet);
  }

  simple_ratexp_t make_simple_ratexp2bis(const std::string& exp, const std::string& alph) {
    return dyn::ratexp_t::from(exp, "B", alph);
  }









}}// end of namespace awali::py

#endif
