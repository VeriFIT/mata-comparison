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

#ifndef DYN_EXPLICIT_RATEXP_CC
#define DYN_EXPLICIT_RATEXP_CC

#include <awali/dyn/core/ratexp.hh>
#include <awali/sttc/core/rat/ratexpset.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/algos/print_exp.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/algos/js_print.hh>
#include<awali/sttc/algos/exp_stats.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/common/priority.hh>

namespace awali { namespace dyn {
    
  template <typename Context>
  struct explicit_ratexp_t : public dyn::abstract_ratexp_t, public sttc::ratexpset<Context>::const_visitor
 {
   using ratexpset_t = sttc::ratexpset<Context>;
   using ratexp_t = typename ratexpset_t::ratexp_t;
   using weightset_t = typename Context::weightset_t;
   using labelset_t = typename Context::labelset_t;

   ratexp_t exp_;
   std::shared_ptr<ratexpset_t> expset_;
   mutable sttc::exp_stats_t *global_stats;  
   
   explicit_ratexp_t(ratexp_t exp, std::shared_ptr<ratexpset_t> expset) :
     exp_(exp), expset_(expset), global_stats(nullptr) {}

   ~explicit_ratexp_t() {
     if(global_stats!=nullptr)
       delete global_stats;
   }

    std::vector<dyn::any_t> 
    alphabet() const override 
    {
      std::vector<dyn::any_t> res;
      for(auto l : expset_->labelset()->genset())
        res.emplace_back(l);
      return res;
    }

    dyn::context_t get_context() const override {
      return std::make_shared<dyn::explicit_context_t<Context>>(expset_->context());
    }
//   
//     template<typename P>
//     std::ostream& print_(ratexpset_t const& expset, ratexp_t const&, 
//       std::ostream& o, priority::ONE<P>)
//     const
//     {
//        return sttc::print(expset, exp, o);
//     }
//     
//     template<typename P>
//     auto print_(ratexpset_t const& expset, ratexp_t const&, 
//       std::ostream& o, priority::TWO<P>) 
//     const
//     -> typename std::enable_if< 
//           (std::is_same<typename labelset_t::value_t,int>::value), 
//           std::ostream& >::type
//     {
//        return sttc::print(*expset_, exp_, o,true);
//     }

      
    std::ostream& 
    print(std::ostream& o) const override {
      return sttc::print(*expset_,exp_,o,std::is_same<typename
        labelset_t::value_t,int>::value);
    }

//     std::ostream& json(std::ostream& o) const {
//       return js_print(*expset_, exp_, o);
//     }

    ratexp_t get_ratexp() {
      return exp_;
    }

    ratexpset_t get_ratexpset() {
      return *expset_;
    }

    dyn::ratexp_t lmul(dyn::weight_t w) const override {
      typename ratexpset_t::weight_t ws=(typename ratexpset_t::weight_t) w;
      return std::make_shared<explicit_ratexp_t<Context>>(expset_->lmul(ws, exp_), expset_);
    }

    dyn::ratexp_t rmul(dyn::weight_t w) const override {
      typename ratexpset_t::weight_t ws= (typename ratexpset_t::weight_t) w;
      return std::make_shared<explicit_ratexp_t<Context>>(expset_->rmul(exp_, ws), expset_);
    }

   dyn::ratexp_t add(dyn::ratexp_t e) const override {
      auto ex=dynamic_cast<explicit_ratexp_t<Context>&>(*e).get_ratexp();
      return std::make_shared<explicit_ratexp_t<Context>>(expset_->add(exp_, ex), expset_);
    }

    dyn::ratexp_t mult(dyn::ratexp_t e) const override {
      auto ex=dynamic_cast<explicit_ratexp_t<Context>&>(*e).get_ratexp();
      return std::make_shared<explicit_ratexp_t<Context>>(expset_->mul(exp_, ex), expset_);
    }

 private:
   void set_global_stats() const {
     global_stats = new sttc::exp_stats_t();
     *global_stats = sttc::exp_stats(*expset_, exp_);
   }
   
 public:
   
   unsigned size() const override {
     if(global_stats==nullptr)
       set_global_stats();
     return global_stats->size;
   }

   unsigned length() const override {
     if(global_stats==nullptr)
       set_global_stats();
     return global_stats->length;
   }

   unsigned height() const override {
     if(global_stats==nullptr)
       set_global_stats();
     return global_stats->height;
   }
   
   unsigned star_height() const override {
     if(global_stats==nullptr)
       set_global_stats();
     return global_stats->star_height;
   }

   dyn::ratexp_t star() const override {
      return std::make_shared<explicit_ratexp_t<Context>>(expset_->star(exp_), expset_);
    }

   unsigned arity() const override {
     if(!stats) {
       stats=true;
       exp_->accept(const_cast<explicit_ratexp_t&>(*this));
     }
     return arity_;
   }

   const std::vector<dyn::ratexp_t>& children() const override {
     if(!stats) {
       stats=true;
       exp_->accept(const_cast<explicit_ratexp_t&>(*this));
     }
     return children_;
   }

   dyn::ExpKind get_kind() const override {
     if(!stats) {
       stats=true;
       exp_->accept(const_cast<explicit_ratexp_t&>(*this));
     }
     return kind_;
   }

   dyn::weight_t lweight() const override {
     if(!stats) {
       stats=true;
       exp_->accept(const_cast<explicit_ratexp_t&>(*this));
     }
     return lweight_;
   }

   dyn::weight_t rweight() const override {
     if(!stats) {
       stats=true;
       exp_->accept(const_cast<explicit_ratexp_t&>(*this));
     }
     return rweight_;
   }

   dyn::label_t value() const override {
     if(!stats) {
       stats=true;
       exp_->accept(const_cast<explicit_ratexp_t&>(*this));
     }
     return value_;
   }
   using super_type = typename Context::const_visitor;
   constexpr static const char* me() { return "dyn-ratexp"; }

   AWALI_RAT_UNSUPPORTED(ldiv)
   AWALI_RAT_UNSUPPORTED(transposition)
   AWALI_RAT_UNSUPPORTED(conjunction)
   AWALI_RAT_UNSUPPORTED(shuffle)
   AWALI_RAT_UNSUPPORTED(complement)

   AWALI_RAT_VISIT(zero,)
   {
     arity_=0;
     kind_=dyn::ExpKind::ZERO;
   }
   AWALI_RAT_VISIT(one,)
   {
     arity_=0;
     kind_=dyn::ExpKind::ONE;
   }
   AWALI_RAT_VISIT(atom,e)
   {
     arity_=0;
     kind_=dyn::ExpKind::ATOM;
     value_=e.value();
   }
   AWALI_RAT_VISIT(sum,e)
   {
     arity_=e.size();
     kind_=dyn::ExpKind::SUM;
     for(auto c:e)
       children_.emplace_back(std::make_shared<explicit_ratexp_t>(c, expset_));
   }
   AWALI_RAT_VISIT(prod,e)
   {
     arity_=e.size();
     kind_=dyn::ExpKind::PROD;
     for(auto c:e)
       children_.emplace_back(std::make_shared<explicit_ratexp_t>(c, expset_));
   }
   AWALI_RAT_VISIT(star,e)
   {
     arity_=1;
     kind_=dyn::ExpKind::STAR;
     children_.emplace_back(std::make_shared<explicit_ratexp_t>(e.sub(), expset_));
   }
   AWALI_RAT_VISIT(lweight,e)
   {
     lweight_ = e.weight();
     e.sub()->accept(*this);
   }
   AWALI_RAT_VISIT(rweight,e)
   {
     rweight_ = e.weight();
     e.sub()->accept(*this);
   }

 private:
   mutable bool stats = false;
   unsigned arity_;
   std::vector<dyn::ratexp_t> children_;
   dyn::ExpKind kind_;
   dyn::weight_t lweight_=expset_->weightset()->one();
   dyn::weight_t rweight_=expset_->weightset()->one();
   dyn::label_t value_=expset_->weightset()->one();
  };
    
}//end of ns dyn

  template<typename Context>
  typename sttc::ratexpset_of<Context>::ratexp_t
  get_stc_ratexp(dyn::ratexp_t e) {
    using context_t = sttc::ratexp_context_of<Context>;
    return dynamic_cast<dyn::explicit_ratexp_t<context_t>&>(*e).get_ratexp();
  }

  template<typename Context>
  sttc::ratexpset_of<Context>
  get_stc_ratexpset(dyn::ratexp_t e) {
    using context_t = sttc::ratexp_context_of<Context>;
    return dynamic_cast<dyn::explicit_ratexp_t<context_t>&>(*e).get_ratexpset();
  }

  template<typename Ratexpset, typename Context= typename Ratexpset::context_t>
      dyn::ratexp_t make_ratexp(typename Ratexpset::ratexp_t exp,
                                std::shared_ptr<Ratexpset> expset) {
    return std::make_shared<dyn::explicit_ratexp_t<Context>>(exp,expset);
  }

}//end of ns awali
#endif
