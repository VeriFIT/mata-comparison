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

#ifndef DYN_EXPLICIT_AUTOMATON_CC
#define DYN_EXPLICIT_AUTOMATON_CC

#include<sstream>
#include<stdexcept>


#include <awali/sttc/core/transpose_view.hh>
#include <awali/sttc/ctx/lan.hh>
#include <awali/sttc/algos/js_print.hh>
#include <awali/sttc/misc/add_epsilon_trans.hh>

#include <awali/dyn/core/automaton.hh>
#include <awali/dyn/options/options.hh>
#include <awali/dyn/bridge_sttc/explicit_context.cc>
#include <awali/sttc/core/mutable_automaton.hh>

#include <awali/common/priority.hh>

namespace awali { namespace dyn {

  template <typename Context>
  struct explicit_automaton_t : public dyn::abstract_automaton_t {

    typename Context::weightset_t::value_t w(dyn::any_t& a) const {
      return dyn::internal::extract_value<typename Context::weightset_t>(
          a, *(aut_->context().weightset()));
    }

    typename Context::labelset_t::value_t l(dyn::any_t& a) const {
      return dyn::internal::extract_value<typename Context::labelset_t>(
          a, *(aut_->context().labelset()));
    }

    sttc::mutable_automaton<Context> aut_;

    explicit_automaton_t(sttc::mutable_automaton<Context> aut) :
      aut_(aut) {}

    state_t add_state(std::string s) override {
      state_t i= aut_->add_state();
      aut_->set_state_name(i,std::move(s));
      return i;
    }

    #define GEN_METH(TYPERET, NAME,CONST)       \
    TYPERET NAME() CONST override {             \
      return aut_->NAME();                      \
    }

    GEN_METH(size_t, num_states, const)
    GEN_METH(size_t, num_initials, const)
    GEN_METH(size_t, num_finals, const)
    GEN_METH(size_t, num_transitions, const)
    GEN_METH(const std::string&, get_name, const)
    GEN_METH(const std::string&, get_desc, const)
    GEN_METH(unsigned, add_state, )
    GEN_METH(unsigned, pre, const)
    GEN_METH(unsigned, post, const)
    #undef GEN_METH

    bool is_final(unsigned s) const override {
      return !outin(s,post()).empty();
    }

    void unset_final(unsigned s) override {
      for(auto tr : outin(s,post()))
        aut_->del_transition(tr);
    }

    #define GEN_METH(TYPERET, NAME, T1, CONST)  \
    TYPERET NAME(T1 x) CONST override {         \
      return aut_->NAME(x);                     \
    }
    GEN_METH(bool, has_state, unsigned, const)
    GEN_METH(bool, is_initial, unsigned, const)
    GEN_METH(dyn::any_t, get_initial_weight, unsigned, const)
    GEN_METH(dyn::any_t, get_final_weight, unsigned, const)
    GEN_METH(bool, has_transition, unsigned, const)
    GEN_METH(unsigned, src_of, unsigned, const)
    GEN_METH(unsigned, dst_of, unsigned, const)
    GEN_METH(dyn::any_t, label_of, unsigned, const)
    GEN_METH(dyn::any_t, weight_of, unsigned, const)
    GEN_METH(unsigned, get_state_by_name, std::string, const)
    #undef GEN_METH

    any_t label_of (unsigned tr, unsigned i) const override {
      any_t lab = label_of(tr);
      return awali::dyn::internal::extract(lab, i, 
        *(aut_->context().labelset()));
    }

    std::list<any_t> 
    labels_of(unsigned tr) const override
    {
      return internal::expand( label_of(tr), *(aut_->context().labelset()) );
    }

    #define GEN_METH(NAME, T1, CONST)           \
    void NAME(T1 x) CONST override {            \
      aut_->NAME(x);                            \
    }
    GEN_METH(del_state, unsigned, )
    GEN_METH(set_initial, unsigned, )
    GEN_METH(unset_initial, unsigned, )
    GEN_METH(set_final, unsigned, )
    GEN_METH(del_transition, unsigned, )
    GEN_METH(set_name, const std::string&, )
    GEN_METH(set_desc, const std::string&, )
    #undef GEN_METH

    unsigned 
    get_transition(unsigned src, unsigned dst, any_t label) const override {
      return aut_->get_transition(src, dst, l(label));
    }

    bool 
    has_transition(unsigned src, unsigned dst, dyn::any_t label) 
    const override {
      return aut_->has_transition(src, dst, l(label));
    }


    history_kind_t history_kind() const override {
      return aut_->history()->get_nature();
    }

    state_t origin_of(state_t s) const override {
      if (aut_->history()->has_history(s))
        return aut_->history()->get_state(s);
      else 
        /* the -2 is for avoid pre and post states */
        return (std::numeric_limits<unsigned>::max()-2);
    }

    std::vector<state_t> origins_of(state_t s) const override {
      return aut_->history()->get_state_set(s);
    }


    void strip_history() override {
      aut_->strip_history();
    }

    bool is_eps_transition(unsigned tr) const override {
     return sttc::is_epsilon<typename Context::labelset_t>(aut_->label_of(tr));
    }


    bool has_explicit_name(unsigned s) const override 
    {
      return aut_->has_explicit_name(s);
    }

    std::string get_state_name(unsigned s) const override {
      return aut_->get_state_name(s);
    }

    void set_state_name(unsigned s, std::string name) override {
      aut_->set_state_name(s, name);
    }
    
    void set_state_names_from_history() override {
      aut_->set_state_names_from_history();
    }
    
    void set_initial(unsigned s, dyn::any_t weight) override {
      aut_->set_initial(s, w(weight));
    }
    
    dyn::any_t add_initial(unsigned s, dyn::any_t weight) override {
      return aut_->add_initial(s, w(weight));
    }
    void set_final(unsigned s, dyn::any_t weight) override {
      aut_->set_final(s, w(weight));
    }
    dyn::any_t add_final(unsigned s, dyn::any_t weight) override {
      return aut_->add_final(s, w(weight));
    }

    transition_t 
    del_transition(unsigned src, unsigned dst, dyn::any_t label) override {
      return aut_->del_transition(src, dst, l(label));
    }

    void del_transition(unsigned src, unsigned dst) override {
      aut_->del_transition(src, dst);
    }

    void del_eps_transition(unsigned src, unsigned dst) override {
      sttc::del_epsilon_trans(aut_, src, dst);
    }


    unsigned 
    set_transition(unsigned src, unsigned dst, dyn::any_t label, 
        dyn::any_t weight) override 
    {
      return aut_->set_transition(src, dst, l(label), w(weight));
    }

    unsigned 
    set_transition(unsigned src, unsigned dst, dyn::any_t label) override {
      return aut_->set_transition(src, dst, l(label));
    }

    unsigned 
    set_eps_transition(unsigned src, unsigned dst, dyn::any_t weight) override {
      return sttc::set_epsilon_trans(aut_, src, dst, w(weight)) ;
    }

    unsigned set_eps_transition(unsigned src, unsigned dst) override {
      return sttc::set_epsilon_trans(aut_, src, dst);
    }


    dyn::any_t 
    add_transition(unsigned src, unsigned dst, dyn::any_t label, 
      dyn::any_t weight)override
    {
      return aut_->add_transition(src, dst, l(label), w(weight));
    }

    dyn::any_t 
    add_transition(unsigned src, unsigned dst, dyn::any_t label) override {
      return aut_->add_transition(src, dst, l(label));
    }

    dyn::any_t 
    add_eps_transition(unsigned src, unsigned dst, dyn::any_t weight) override {
      return sttc::add_epsilon_trans(aut_,src, dst, w(weight));
    }

    dyn::any_t add_eps_transition(unsigned src, unsigned dst) override {
      return sttc::add_epsilon_trans(aut_,src, dst);
    }

    dyn::any_t set_weight(unsigned tr, dyn::any_t weight) override {
      return aut_->set_weight(tr, w(weight));
    }

    dyn::any_t add_weight(unsigned tr, dyn::any_t weight) override {
      return aut_->add_weight(tr, w(weight));
    }

    dyn::any_t lmul_weight(unsigned tr, dyn::any_t weight) override {
      return aut_->lmul_weight(tr, w(weight));
    }

    dyn::any_t rmul_weight(unsigned tr, dyn::any_t weight) override {
      return aut_->rmul_weight(tr, w(weight));
    }

    std::vector<dyn::transition_t> 
    transitions(bool all) const
    {
      std::vector<dyn::transition_t> transitions;
      for (dyn::transition_t t : all ? aut_->all_transitions()
                                     : aut_->transitions()     )
        transitions.push_back(t);
      return transitions;
    }

    std::vector<dyn::state_t> states(bool all) const {
      std::vector<dyn::state_t> states;
      for (dyn::state_t s : all ? aut_->all_states() : aut_->states())
        states.push_back(s);
      return states;
    }

//     std::vector<dyn::transition_t> 
//     out(state_t s, dyn::options_t opt) const 
//     {
//       std::vector<dyn::state_t> transitions;
//       if (opt[PREPOST_PARADIGM])
//         for (dyn::transition_t t : aut_->all_out(s))
//           transitions.push_back(t);
//       else
//         for (dyn::transition_t t : aut_->out(s))
//           transitions.push_back(t);
//       return transitions;
//     }
// 
//     std::vector<dyn::transition_t> in(state_t s, dyn::options_t opt) const {
//       std::vector<dyn::state_t> transitions;
//       if (opt[PREPOST_PARADIGM])
//         for (dyn::transition_t t :  aut_->all_in(s))
//           transitions.push_back(t);
//       else
//         for (dyn::transition_t t : aut_->in(s))
//           transitions.push_back(t);
//       return transitions;
//     }

    std::vector<unsigned> outgoing(unsigned s, bool all) const {
      std::vector<dyn::state_t> transitions;
      if (all)
        for (dyn::transition_t t : aut_->all_out(s))
          transitions.push_back(t);
      else
        for (dyn::transition_t t : aut_->out(s))
          transitions.push_back(t);
      return transitions;
    }

    std::vector<unsigned> incoming(unsigned s, bool all) const {
      std::vector<dyn::state_t> transitions;
      if (all)
        for (dyn::transition_t t : aut_->all_in(s))
          transitions.push_back(t);
      else
        for (dyn::transition_t t : aut_->in(s))
          transitions.push_back(t);
      return transitions;
    }

    std::vector<unsigned> initial_states() const override {
      std::vector<unsigned> res;
      for(unsigned i : aut_->initial_transitions())
        res.emplace_back(dst_of(i));
      return res;
    }

    std::vector<unsigned> final_states() const override {
      std::vector<unsigned> res;
      for(unsigned i : aut_->final_transitions())
        res.emplace_back(src_of(i));
      return res;
    }

    std::vector<unsigned> successors(unsigned s) const override {
      std::vector<unsigned> res;
      for(unsigned i : aut_->out(s))
        res.emplace_back(aut_->dst_of(i));
      return res;
    }

    std::vector<unsigned> successors(unsigned s, dyn::any_t label) const override {
      std::vector<unsigned> res;
      auto lab = l(label);
      for(unsigned i : aut_->out(s,lab))
        res.emplace_back(aut_->dst_of(i));
      return res;
    }

    std::vector<unsigned> predecessors(unsigned s) const override {
      std::vector<unsigned> res;
      for(unsigned i : aut_->in(s))
        res.emplace_back(aut_->src_of(i));
      return res;
    }

    std::vector<unsigned> predecessors(unsigned s, dyn::any_t label) const override {
      std::vector<unsigned> res;
      auto lab = l(label);
      for(unsigned i : aut_->in(s,lab))
        res.emplace_back(aut_->src_of(i));
      return res;
    }

//     std::vector<unsigned> out(unsigned s, dyn::any_t label) const overr {
//       std::vector<unsigned> res;
//       auto lab = l(label);
//       for(unsigned i : aut_->out(s, lab))
//         res.emplace_back(i);
//       return res;
//     }

    std::vector<unsigned> 
    outgoing(unsigned s, dyn::any_t label) const override {
      std::vector<unsigned> res;
      auto lab = l(label);
      for(unsigned i : aut_->out(s, lab))
        res.emplace_back(i);
      return res;
    }

//     std::vector<unsigned> in(unsigned s, dyn::any_t label) const override {
//       std::vector<unsigned> res;
//       auto lab = l(label);
//       for(unsigned i : aut_->in(s, lab))
//         res.emplace_back(i);
//       return res;
//     }

    std::vector<unsigned> 
    incoming(unsigned s, dyn::any_t label) const override {
      std::vector<unsigned> res;
      auto lab = l(label);
      for(unsigned i : aut_->in(s, lab))
        res.emplace_back(i);
      return res;
    }

    std::vector<unsigned> outin(unsigned s, unsigned d) const override {
      std::vector<unsigned> res;
      for(unsigned i : aut_->outin(s, d))
        res.emplace_back(i);
      return res;
    }

    std::vector<dyn::any_t> alphabet() const override {
      std::vector<dyn::any_t> res;
      for(auto l : aut_->labelset()->genset())
        res.emplace_back(l);
      return res;
    }

    private:
    template<typename T>
    bool is_transducer(const T&) const {
      return false;
    }

    template<typename... T>
    bool is_transducer(const sttc::tupleset<T...>&) const {
      return true;
    }

    template<typename T>
    bool is_int_automaton(const T&) const {
      return false;
    }

    template<typename... T>
    bool is_int_automaton(const sttc::tupleset<T...>&) const {
      return false;
    }

    template<typename T>
    typename std::enable_if<std::is_same<typename T::letter_t,int>::value,bool>
    ::type 
    is_int_automaton(const sttc::letterset<T>&) const{
      return true;
    }

    template<typename T>
    bool is_int_automaton(const sttc::nullableset<T>& ns) const {
      return is_int_automaton(ns.labelset());
    }


    
    template<typename T>
    bool is_word_automaton(const T&) const {
      return false;
    }

    template<typename T>
    bool is_word_automaton(const sttc::wordset<T>&) const{
      return true;
    }

  public:
    unsigned null_state () const override
    {
      return sttc::mutable_automaton<Context>::element_type::null_state();
    }

    unsigned null_transition () const override {
      return sttc::mutable_automaton<Context>::element_type::null_state();
    }


    bool is_transducer() const override {
      return is_transducer(*(aut_->context().labelset()));
    }

    bool is_int_automaton() const override {
      return is_int_automaton(*(aut_->context().labelset()));
    }

    bool are_words_allowed() const override {
      return is_word_automaton(*(aut_->context().labelset()));
    }

    bool is_eps_allowed() const override {
      return this->get_context()->is_eps_allowed();
//       return Context::labelset_t::has_one();
    }

    //       std::ostream& json(std::ostream& o) const {
    //         return sttc::js_print(aut_, o, true);
    //       }

    dyn::context_t get_context() const override {
    aut_->context();
      return std::make_shared<dyn::explicit_context_t<Context>>(aut_->context());
    }

    sttc::mutable_automaton<Context> get_automaton() {
      return aut_;
    }
  };


  template<typename Context>
  sttc::mutable_automaton<Context>
  get_stc_automaton(dyn::automaton_t aut) {
    return dynamic_cast<explicit_automaton_t<Context>&>(*aut).get_automaton();
  }

  template<typename Context>
  dyn::automaton_t make_automaton(sttc::mutable_automaton<Context> a) {
    return dyn::automaton_t(std::make_shared<explicit_automaton_t<Context>>(a));
  }

  template<typename Aut>
  dyn::automaton_t make_automaton(std::shared_ptr<sttc::internal::transpose_view_impl<Aut>> a) {
    throw std::runtime_error("No dynamic encapsulation of transpose view");
  }

}}//end of ns awali::dyn


/* 
  VM: This seems to work for python but breaks cora and sometimes dyn(dual free).
*/
// #include <awali/dyn/options/option.cc>

#endif
