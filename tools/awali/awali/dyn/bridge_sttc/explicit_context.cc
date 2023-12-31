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

#ifndef DYN_EXPLICIT_CONTEXT_CC
#define DYN_EXPLICIT_CONTEXT_CC

#include <memory>
#include <list>
#include <string>
#include <sstream>

#include <awali/sttc/labelset/traits.hh>
#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/labelset/tupleset.hh>
#include <awali/sttc/ctx/lal_char.hh>
#include <awali/sttc/ctx/lal_int.hh>
#include <awali/sttc/labelset/wordset.hh>
#include <awali/sttc/ctx/lan.hh>
#include <awali/sttc/ctx/lao.hh>
#include <awali/sttc/ctx/lat.hh>
#include <awali/sttc/weightset/is_finite.hh>
#include <awali/dyn/core/context.hh>

#include <awali/dyn/core/any.cc>

#include <awali/common/to_value_of.hxx>

namespace awali { namespace dyn {

  template <typename Context>
  struct explicit_context_t : public dyn::abstract_context_t {
    std::shared_ptr<Context> context_ptr;
    
    typename Context::weightset_t::value_t w(dyn::any_t& a) const {
      return dyn::internal::extract_value<typename Context::weightset_t>(
          a, *(context_ptr->weightset()));
    }

    typename Context::labelset_t::value_t l(dyn::any_t& a) const {
      return dyn::internal::extract_value<typename Context::labelset_t>(
          a, *(context_ptr->labelset()));
    }

    explicit_context_t(const Context& c) :
    context_ptr(std::make_shared<Context>(c)) {}


    std::string vname(bool full) const override
    {
      return context_ptr->vname(full);
    }

    std::string sname() const override
    {
      return context_ptr->sname();
    }



    std::string labelset_name() const override 
    {
      return context_ptr->labelset()->sname();
    }

    std::string weightset_name(std::string format) const override {
      std::ostringstream os;
      context_ptr->weightset()->print_set(os, format);
      return os.str();
    }

    Context const& get_context() {
      return *context_ptr;
    }

    std::vector<dyn::any_t> alphabet() const override {
      std::vector<dyn::any_t> res;
      for(auto l : context_ptr->labelset()->genset())
        res.emplace_back(l); 
      return res;
    }

    any_t epsilon() const override
    {
      return epsilon_();
    }

    bool is_eps_allowed() const override
    {
      return Context::labelset_t::has_one();
    }
    
//     bool are_words_allowed() const override
//     {
//       return this->description()->labelset()->is_law();
//     }
    
//     size_t tape_number() const override 
//     {
//       return this->description()->labelset()->tape_number();
//     }
  


    static
    context::labelset_description
    ls_desc(sttc::ctx::lao const&) 
    { 
        context::labelset_description ls_desc 
          = context::make_labelset_description();
        ls_desc->type_= context::CTypes::ONESET;
        return ls_desc;
    }
    
    static
    context::labelset_description
    ls_desc(sttc::ctx::lal_char const& ls) 
    { 
        context::labelset_description ls_desc 
          = context::make_labelset_description();
        ls_desc->type_= context::CTypes::LETTERSET;
        for(char c : ls.genset())
          ls_desc->alphabet.push_back(std::string(1,c));
        return ls_desc;
    }


    static
    context::labelset_description
    ls_desc(sttc::ctx::lal_int const& ls) 
    { 
        context::labelset_description ls_desc 
          = context::make_labelset_description();
        ls_desc->type_= context::CTypes::INTLETTERSET;
        for(auto c : ls.genset())
          ls_desc->int_alphabet.push_back(c);
        return ls_desc;
    }
    
    template<typename T>
    static
    context::labelset_description
    ls_desc(sttc::ctx::lan<T> const& ls) 
    { 
        context::labelset_description ls_res 
          = context::make_labelset_description();
        ls_res->type_= context::CTypes::NULLABLE;
        ls_res->children_.push_back(ls_desc(*(ls.labelset())));
        return ls_res;
    }
    
    static
    context::labelset_description
    ls_desc(sttc::wordset<sttc::set_alphabet<sttc::char_letters>> const& ls) 
    { 
        context::labelset_description ls_res 
          = context::make_labelset_description();
        ls_res->type_= context::CTypes::WORDSET;
        for(char c : ls.genset())
          ls_res->alphabet.push_back(std::string(1,c));
        return ls_res;
    }

    static
    context::labelset_description
    ls_desc(sttc::wordset<sttc::set_alphabet<sttc::int_letters>> const& ls) 
    { 
        context::labelset_description ls_res 
          = context::make_labelset_description();
        ls_res->type_= context::CTypes::INTWORDSET;
        for(auto c : ls.genset())
          ls_res->int_alphabet.push_back(c);
        return ls_res;
    }

    
    template<typename... Ts, std::size_t... I>
    static
    context::labelset_description
    ls_desc_(sttc::ctx::lat<Ts...> const& ls, 
             awali::internal::index_sequence<I...>) 
    { 
        context::labelset_description ls_res 
          = context::make_labelset_description();
        ls_res->type_= context::CTypes::TUPLE;
        for (auto child: {ls_desc(ls.template set<I>())...})
          ls_res->children_.push_back(child);
        return ls_res;
    }

    template<typename... Ts>
    static
    context::labelset_description
    ls_desc(sttc::ctx::lat<Ts...> const& ls) 
    { 
        return ls_desc_(ls, ls.indices);
    }


    template<typename T>
    static
    context::weightset_description
    ws_desc(T const& ws)
    {
      std::stringstream ss;
      ws.print_set(ss);
      return context::weightset(ss.str());
    }

    

    context::context_description
    description() const override
    {
      static context::context_description cd
        = c_desc(ls_desc(*(this->context_ptr->labelset())), 
                 ws_desc(*(this->context_ptr->weightset())));
      return cd;
    }



  private:
    template <typename C = Context>
    typename std::enable_if<C::labelset_t::has_one(),any_t>::type 
    epsilon_() const {
      return context_ptr->labelset()->one();
    }
    template <typename C = Context>
    typename std::enable_if<!C::labelset_t::has_one(),any_t>::type 
    epsilon_() const {
      throw std::runtime_error("Labelset does not allows epsilon");
    }

    template<typename Labelset>
    void add_letter_(dyn::any_t label, const Labelset& lab) {
      typename Labelset::genset_t::letter_t val;
      try { 
        val = internal::any_cast<typename Labelset::genset_t::letter_t>
            (label); 
      }
      catch(any_cast_exception const& e1) { 
        try {
          std::stringstream ss;
          ss << label;
          val = awali::internal::to_letter_of(lab, ss.str());
        }
        catch (parse_exception const& e2) { 
          std::stringstream ss;
          ss << e1.what();
          ss << " \tAlso failed to parse string \"" << label 
             << "\" as a letter of " << lab.vname() << ": "
             << e2.what();
          throw any_cast_exception(ss.str().c_str());
        }
      }
      const_cast<typename Labelset::genset_t&>(lab.genset()).add_letter(val);
    }

    template<typename Labelset>
    bool has_letter_(dyn::any_t l, const Labelset& lab) const {
      return lab.genset().has((typename Labelset::genset_t::letter_t)l);
    }

    template<typename... T>
    void add_letter_(dyn::any_t, const sttc::tupleset<T...>&) {
      throw std::runtime_error("No genset in tupleset");
    }

    template<typename... T>
    bool has_letter_(dyn::any_t, const sttc::tupleset<T...>&) const {
      throw std::runtime_error("No genset in tupleset");
    }

    void add_letter_(dyn::any_t, const sttc::oneset&) {
      throw std::runtime_error("No genset in oneset");
    }

    bool has_letter_(dyn::any_t, const sttc::oneset&) const {
      return false;
    }

  public:
    void add_letter(dyn::any_t label) override {
      // In this case, we don't call function `l` because the labelset
      // does not know `label` yet
        return add_letter_(label, *(context_ptr->labelset()));
    }

    bool has_letter(dyn::any_t label) const override {
      try {
        return has_letter_(l(label), *(context_ptr->labelset()));
      }
      catch (any_cast_exception const& e) {
        return false;
      }
    }

    dyn::any_t weight_one() const override {
      return context_ptr->weightset()->one();
    }

    bool show_one() const override {
      return context_ptr->weightset()->show_one();
    }

    dyn::any_t weight_zero() const override {
      return context_ptr->weightset()->zero();
    }

    bool is_finite_weightset() const override {
      return is_finite(*context_ptr->weightset());
    }
    
    bool is_locally_finite_weightset() const override {
      return is_finite(*context_ptr->weightset());
    }

//     std::string get_weightset() const override {
//       return context_ptr->weightset()->vname();
//     }

    dyn::any_t get_weight(std::string w) const override {
        return awali::internal::to_value_of(*(context_ptr->weightset()),w);
    }

    dyn::any_t get_word(std::string w) const override {
        return this->get_word_(w,*(context_ptr->labelset()),priority::value);
    }

  private:
    
    virtual explicit_context_t<Context>* copy() override
    { 
      return new explicit_context_t(*this);
    }

    //FALLBACK
    template<size_t i, typename P, typename L>
    any_t get_label_(std::string, size_t, const L&, priority::ONE<P>) const {
      throw std::runtime_error("Not a transducer");
    }

    template<typename P, typename L>
    dyn::any_t get_word_(std::string, const L&, priority::ONE<P>) const {
      throw std::runtime_error("Words have no meaning when labels are one");
    }

    //ACTUAL FUNCTION
    template<size_t i, typename P, typename L>
    auto 
    get_label_(std::string l, size_t n, const L& labelset, priority::TWO<P> p) 
    const
    -> decltype((labelset->template set<i>()), any_t(true))
    {
      if (i == n) 
        return awali::internal::to_value_of(
          context_ptr->labelset()->template set<i>(), l);
      else if (i+1 >= Context::labelset_t::size())
        throw std::out_of_range(std::string("There is no tape n°")+std::to_string(n)+".");
      else
        return get_label_<(i+1<Context::labelset_t::size()?i+1:0)>
          (l,n, labelset, p);
    }

    template<typename P, typename L>
    auto
    get_word_(std::string w, const L& labelset, priority::TWO<P>) const
    -> decltype(typename L::genset_t(), dyn::any_t(true))
    {
        return awali::internal::to_word_of(labelset,w);
    }

  public:
    any_t get_label(std::string l,size_t i) const override
    {
      return get_label_<0>(l,i, context_ptr->labelset(), priority::value);
    }

    dyn::any_t get_label(std::string l) const override {
        return awali::internal::to_value_of(*(context_ptr->labelset()),l);
    }

    std::string weight_to_string(any_t weight) const override {
      std::ostringstream st;
      context_ptr->weightset()->print(w(weight), st);
      return st.str();
    }

    std::string 
    label_to_string(any_t label) const override 
    {
      std::ostringstream st;
      context_ptr->labelset()->print(l(label), st);
      return st.str();
    }

    std::list<any_t> expand_label(any_t label) const override {
      try {
        return ( internal::any_cast<std::list<any_t>> (label) );
      } catch (...) {}
      return internal::expand(label, *(context_ptr->labelset()));
    }

    dyn::any_t 
    add_weights(dyn::any_t lweight, dyn::any_t rweight) const override
    {
      return context_ptr->weightset()->add(w(lweight),w(rweight));
    }

    dyn::any_t 
    mul_weights(dyn::any_t lweight, dyn::any_t rweight) const override {
      return context_ptr->weightset()->mul(w(lweight),w(rweight));
    }

    dyn::any_t
    div_weights(dyn::any_t lweight, dyn::any_t rweight) const override 
    {
      return context_ptr->weightset()->rdiv(w(lweight),w(rweight));
    }

  };

  template<typename Context>
  const Context&
  get_stc_context(dyn::context_t ctx) {
    return dynamic_cast<explicit_context_t<Context>&>(*ctx).get_context();
  }
}}//end of ns awali::dyn

#include <awali/dyn/core/context_description.cc>

#endif
