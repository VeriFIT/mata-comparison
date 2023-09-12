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

#ifndef DYN_BASIC_AUTOMATON_HH
#define DYN_BASIC_AUTOMATON_HH

#include<sstream>
#include<awalipy/bridge-to-dyn/utils.hh>

namespace awali { namespace py {


  struct basic_automaton_t  {
  protected:
    dyn::automaton_t aut_;

  public:
    //Initialisation and conversion
    basic_automaton_t() : aut_() {}

    basic_automaton_t(const dyn::automaton_t& a) : aut_(a) {}

    void rewrap_into_me_(basic_automaton_t other) {
        if (!((bool) aut_))
          release();
        aut_= ((dyn::automaton_t) other);
    }

    operator dyn::automaton_t() const {
      return dyn::automaton_t(aut_);
    }

    std::string label_to_string(dyn::any_t a) const 
    { return awali::py::label_to_string(a,aut_->get_context()); }

    std::string weight_to_string(dyn::any_t a) const 
    { return awali::py::weight_to_string(a,aut_->get_context()); }

    dyn::state_t null_state() const {
      return aut_->null_state();
    }
    
    dyn::transition_t null_transition() const {
      return aut_->null_transition();
    }

    void verify_existence_of_state(int state, std::string comment="") const {
      if (!has_state(state)) {
        std::stringstream ss;
        if (comment.compare("") ==  0)
          ss << comment << " state does not exist: ";
        else
          ss << "No such state exists: ";
        ss << state << std::endl;
        std::string msg;
        std::getline(ss, msg);
        throw std::invalid_argument(msg);
      }
    }

    
    void 
    verify_existence_of_transition (dyn::transition_t trans, 
                                    std::string comment="") 
    const 
    {
      if (!has_transition1(trans)) {
        std::stringstream ss;
        if (comment.compare("") ==  0)
          ss << comment << " transition does not exist: ";
        else
          ss << "No such transition exists: ";
        ss << trans << std::endl;
        std::string msg;
        std::getline(ss, msg);
        throw std::invalid_argument(msg);
      }
    }

    size_t num_states() const {
      return aut_->num_states();
    }

    size_t num_initials() const {
      return aut_->num_initials();
    }

    size_t num_finals() const {
      return aut_->num_finals();
    }

    size_t num_transitions() const {
      return aut_->num_transitions();
    }

    bool has_state(int s) const {
      return aut_->has_state(s+2);
    }

    bool is_initial(int s) const {
      verify_existence_of_state(s);
      return aut_->is_initial(s+2);
    }

    bool is_final(int s) const {
      verify_existence_of_state(s);
      return aut_->is_final(s+2);
    }

    std::string get_initial_weight(int s) const {
      verify_existence_of_state(s);
      return weight_to_string(aut_->get_initial_weight(s+2));
    }

    std::string  get_final_weight(int s) const {
      verify_existence_of_state(s);
      return weight_to_string(aut_->get_final_weight(s+2));
    }

    bool has_transition1(dyn::transition_t t) const {
      return aut_->has_transition(t);
    }

    int src_of(dyn::transition_t t) const {
      verify_existence_of_transition(t);
      return aut_->src_of(t)-2;
    }

    int dst_of(dyn::transition_t t) const {
      verify_existence_of_transition(t);
      return aut_->dst_of(t)-2;
    }

    std::string weight_of(dyn::transition_t t) const {
      verify_existence_of_transition(t);
      return weight_to_string(aut_->weight_of(t));
    }

    int add_state1(const std::string& name) {
      return aut_->add_state(name)-2;
    }

    int add_state0() {
      return aut_->add_state()-2;
    }

    void del_state(int s) {
      verify_existence_of_state(s);
      aut_->del_state(s+2);
    }

    void set_initial2(int s, const std::string& w) {
      verify_existence_of_state(s);
      aut_->set_initial(s+2,aut_->get_context()->get_weight(w));
    }

    void set_initial1(int s) {
      verify_existence_of_state(s);
      aut_->set_initial(s+2);
    }


    std::string add_initial(int s, const std::string& w) {
      verify_existence_of_state(s);
      return weight_to_string(
        aut_->add_initial(s+2, aut_->get_context()->get_weight(w)));
    }

    void unset_initial(int s) {
      verify_existence_of_state(s);
      aut_->unset_initial(s+2);
    }

    void set_final2(int s, const std::string& w) {
      verify_existence_of_state(s);
      aut_->set_final(s+2, aut_->get_context()->get_weight(w));
    }

    void set_final1(int s) {
      verify_existence_of_state(s);
      aut_->set_final(s+2);
    }

    std::string add_final(int s, const std::string& w) {
      verify_existence_of_state(s);
      return weight_to_string(
        aut_->add_final(s+2, aut_->get_context()->get_weight(w)));
    }

    void unset_final(int s) {
      verify_existence_of_state(s);
      aut_->unset_final(s+2);
    }

    void del_transition1(dyn::transition_t tr) {
      verify_existence_of_transition(tr);
      aut_->del_transition(tr);
    }

    void del_transition2(int src, int dst) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      aut_->del_transition(src+2, dst+2);
    }

    dyn::transition_t set_eps_transition2(int src, int dst) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return aut_->set_eps_transition(src+2, dst+2);
    }

    dyn::transition_t set_eps_transition3(int src, int dst, const std::string& weight) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return aut_->set_eps_transition(src+2, dst+2, aut_->get_context()->get_weight(weight));
    }

    std::string add_eps_transition2(int src, int dst) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return weight_to_string(aut_->add_eps_transition(src+2, dst+2));
    }

    std::string add_eps_transition3(int src, int dst, const std::string& weight) {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(dst, "Destination");
      return weight_to_string(
        aut_->add_eps_transition(src+2, dst+2, 
                                 aut_->get_context()->get_weight(weight))
      );
    }

    std::string set_weight(dyn::transition_t tr, const std::string& weight) {
      verify_existence_of_transition(tr);
      return weight_to_string(
        aut_->set_weight(tr, aut_->get_context()->get_weight(weight)));
    }

    std::string add_weight(dyn::transition_t tr, const std::string& weight) {
      verify_existence_of_transition(tr);
      return weight_to_string(
        aut_->add_weight(tr, aut_->get_context()->get_weight(weight))
      );
    }

    std::string lmul_weight(dyn::transition_t tr, const std::string& weight) {
      verify_existence_of_transition(tr);
      return weight_to_string(aut_->lmul_weight(tr, aut_->get_context()->get_weight(weight)));
    }

    std::string rmul_weight(dyn::transition_t tr, const std::string& weight) {
      verify_existence_of_transition(tr);
      return weight_to_string(aut_->rmul_weight(tr, aut_->get_context()->get_weight(weight)));
    }

    std::vector<int> states() const {
      std::vector<int> r;
      for(auto s: aut_->states())
	r.emplace_back(s-2);
      return r;
    }

    std::vector<dyn::transition_t> transitions() const {
      return aut_->transitions();
    }

    std::vector<int> initial_states() const {
      std::vector<int> r;
      for(auto s: aut_->initial_states())
	r.emplace_back(s-2);
      return r;
    }

//     std::vector<dyn::transition_t> initial_transitions() const {
//       return aut_->initial_transitions();
//     }

    std::vector<int> final_states() const {
      std::vector<int> r;
      for(auto s: aut_->final_states())
	r.emplace_back(s-2);
      return r;
    }

//     std::vector<dyn::transition_t> final_transitions() const {
//       return aut_->final_transitions();
//     }

    std::vector<int> successors1(int src) const {
      verify_existence_of_state(src);
      std::vector<int> r;
      for(auto s: aut_->successors(src+2))
	r.emplace_back(s-2);
      return r;
    }

    std::vector<int> predecessors1(int src) const {
      verify_existence_of_state(src);
      std::vector<int> r;
      for(auto s: aut_->predecessors(src+2))
	r.emplace_back(s-2);
      return r;
    }

    std::vector<dyn::transition_t> outin(int src, int dst) const {
      verify_existence_of_state(src, "Source");
      verify_existence_of_state(src, "Destination");
      return aut_->outin(src+2, dst+2);
    }

    std::vector<dyn::transition_t> incoming1(int s) const {
      verify_existence_of_state(s);
      return aut_->incoming(s+2);
    }

    std::vector<dyn::transition_t> outgoing1(int s) const {
      verify_existence_of_state(s);
      return aut_->outgoing(s+2);
    }

    std::vector<dyn::transition_t> in1(int s) const {
      verify_existence_of_state(s);
      return aut_->in(s+2);
    }

    std::vector<dyn::transition_t> out1(int s) const {
      verify_existence_of_state(s);
      return aut_->out(s+2);
    }

//     int pre() const {
//       return aut_->pre()-2;
//     }
// 
//     int post() const {
//       return aut_->post()-2;
//     }

//     std::vector<int> all_states() const {//including pre() and post()
//       std::vector<int> r;
//       for(auto s: aut_->all_states())
//   r.emplace_back(s-2);
//       return r;
//     }
// 
//     std::vector<dyn::transition_t> all_transitions() const { // including initial and final
//       return aut_->all_transitions();
//     }
// 
//     std::vector<dyn::transition_t> all_out(int s) const { // including final
//       verify_existence_of_state(s);
//       return aut_->all_out(s+2);
//     }

//     std::vector<dyn::transition_t> all_in(int s) const { // including initial
//       verify_existence_of_state(s);
//       return aut_->all_in(s+2);
//     }

    bool is_transducer() const {
      return aut_->is_transducer();
    }

    std::string get_static_context() const {
      return aut_->get_context()->sname();
    }

    void strip_history() {
      aut_->strip_history();
    }

    std::string get_state_name(int s) const {
      verify_existence_of_state(s);
      return aut_->get_state_name(s+2);
    }

    std::string  weight_one() const {
      return weight_to_string(aut_->get_context()->weight_one());
    }

    std::string  weight_zero() const {
      return weight_to_string(aut_->get_context()->weight_zero());
    }

    std::string get_weightset_name(std::string format = "text") const {
      return aut_->get_context()->weightset_name(format);
    }

    weightset_t get_weightset() {
      return weightset_t(aut_->get_context());
    }

//     std::string dot(bool history=false, bool horizontal = true) {
//       std::ostringstream os;
//       dyn::dot(((dyn::automaton_t) aut_),os, history, horizontal) << std::endl;
//       return os.str();
//     }
// 
    std::string svg(bool history=false, bool horizontal = true) const {
      std::ostringstream os;
      dyn::put( ((dyn::automaton_t) aut_), 
                os, 
                { dyn::KEEP_HISTORY = history, 
                  dyn::LAYOUT = (horizontal?HORIZONTAL:VERTICAL), 
                  dyn::IO_FORMAT = SVG}
      ) << std::endl;
      return os.str();
    }
// 
//     std::string fado() {
//       std::ostringstream os;
//       dyn::fado((dyn::automaton_t) aut_, os) << std::endl;
//       return os.str();
//     }
// 
//     std::string grail() {
//       std::ostringstream os;
//       dyn::grail((dyn::automaton_t) aut_, os) << std::endl;
//       return os.str();
//     }
// 
//     std::string json() {
//       std::ostringstream os;
//       dyn::json((dyn::automaton_t) aut_, os) << std::endl;
//       return os.str();
//     }

    

//     void display() {
//       std::string s = dyn::loading::get_lib_directory()+"/tmp.gv";
//       std::ofstream o(s);
//       o << this->dot() << std::endl;
//       o.close();
//       (void) system(("dotty "+s).c_str());
//     }

    void release() {
      aut_.reset();
    }
  };

  basic_automaton_t make_basic_automaton2(std::string alphabet, std::string semiring) {
    return dyn::automaton_t::from(alphabet, semiring);
  }

  void pdfdisplay(basic_automaton_t aut, bool horizontal, bool history) {
    dyn::pdfdisplay((dyn::automaton_t) aut, {dyn::LAYOUT = (horizontal?HORIZONTAL:VERTICAL), dyn::KEEP_HISTORY=history});
  }

  basic_automaton_t make_basic_automaton1(std::string alphabet) {
    return dyn::automaton_t::from(alphabet);
  }

  void verify_existence_of_state(basic_automaton_t& aut, int state, std::string comment=""){
    aut.verify_existence_of_state(state, comment);
  }

}}// end of namespace awali::py

#endif
