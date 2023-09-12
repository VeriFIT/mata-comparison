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

#ifndef DYN_EXPLICIT_CONTEXT_CC
#define DYN_EXPLICIT_CONTEXT_CC

#include <memory>
#include <list>
#include <string>
#include <sstream>

#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/labelset/tupleset.hh>
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

    std::string vname(bool full) const
    {
      return context_ptr->vname(full);
    }

    std::string sname() const
    {
      return context_ptr->sname();
    }



    std::string labelset_name() const {
      return context_ptr->labelset()->sname();
    }

    std::string weightset_name(std::string format) const {
      std::ostringstream os;
      context_ptr->weightset()->print_set(os, format);
      return os.str();
    }

    const Context& get_context() {
      return *context_ptr;
    }

    std::vector<dyn::any_t> alphabet() const {
      std::vector<dyn::any_t> res;
      for(auto l : context_ptr->labelset()->genset())
        res.emplace_back(l); 
      return res;
    }

    any_t epsilon() const {
      return epsilon_();
    }

    bool is_eps_allowed() const {
      return Context::labelset_t::has_one();
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
      catch(any_cast_exception const& e) { 
        std::stringstream ss;
        ss << label;
        try {
          val = awali::internal::to_letter_of(lab, ss.str());
        } 
        catch (parse_exception const& e ) { 
          std::stringstream ss;
          ss << e.what();
          ss << " \tAlso failed to parse string \"" << label 
             << "\" as a letter of " << lab.vname() << ".";
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
    void add_letter_(dyn::any_t l, const sttc::tupleset<T...>& lab) {
      throw std::runtime_error("No genset in tupleset");
    }

    template<typename... T>
    bool has_letter_(dyn::any_t l, const sttc::tupleset<T...>& lab) const {
      throw std::runtime_error("No genset in tupleset");
    }

    void add_letter_(dyn::any_t l, const sttc::oneset& lab) {
      throw std::runtime_error("No genset in oneset");
    }

    bool has_letter_(dyn::any_t l, const sttc::oneset& lab) const {
      return false;
    }

  public:
    void add_letter(dyn::any_t label) {
      // In this case, we don't call function `l` because the labelset
      // does not know `label` yet
        return add_letter_(label, *(context_ptr->labelset()));
    }

    bool has_letter(dyn::any_t label) const {
      try {
        return has_letter_(l(label), *(context_ptr->labelset()));
      }
      catch (any_cast_exception const& e) {
        return false;
      }
    }

    dyn::any_t weight_one() const {
      return context_ptr->weightset()->one();
    }

    bool show_one() const {
      return context_ptr->weightset()->show_one();
    }

    dyn::any_t weight_zero() const {
      return context_ptr->weightset()->zero();
    }

    bool is_finite_weightset() const {
      return is_finite(*context_ptr->weightset());
    }
    
    bool is_locally_finite_weightset() const {
      return is_finite(*context_ptr->weightset());
    }

    std::string get_weightset() const {
      return context_ptr->weightset()->vname();
    }

    dyn::any_t get_weight(std::string w) const {
        return awali::internal::to_value_of(*(context_ptr->weightset()),w);
    }


  private:
    //FALLBACK
    template<size_t i, typename P, typename L>
    any_t get_label_(std::string l, size_t n, const L& labelset, priority::ONE<P>) const {
      throw std::runtime_error("Not a transducer");
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

  public:
    any_t get_label(std::string l,size_t i) const
    {
      return get_label_<0>(l,i, context_ptr->labelset(), priority::value);
    }

    dyn::any_t get_label(std::string l) const {
        return awali::internal::to_value_of(*(context_ptr->labelset()),l);
    }

    std::string weight_to_string(any_t weight) const {
      std::ostringstream st;
      context_ptr->weightset()->print(w(weight), st);
      return st.str();
    }

    std::string label_to_string(any_t label) const {
      std::ostringstream st;
      context_ptr->labelset()->print(l(label), st);
      return st.str();
    }

    std::list<any_t> expand_label(any_t label) const {
      try {
        return ( internal::any_cast<std::list<any_t>> (label) );
      } catch (...) {}
      return internal::expand(label, *(context_ptr->labelset()));
    }

    dyn::any_t add_weights(dyn::any_t lweight, dyn::any_t rweight) const {
      return context_ptr->weightset()->add(w(lweight),w(rweight));
    }

    dyn::any_t mul_weights(dyn::any_t lweight, dyn::any_t rweight) const {
      return context_ptr->weightset()->mul(w(lweight),w(rweight));
    }

    dyn::any_t div_weights(dyn::any_t lweight, dyn::any_t rweight) const {
      return context_ptr->weightset()->rdiv(w(lweight),w(rweight));
    }


  };

  template<typename Context>
  const Context&
  get_stc_context(dyn::context_t ctx) {
    return dynamic_cast<explicit_context_t<Context>&>(*ctx).get_context();
  }
}}//end of ns awali::dyn

#endif
