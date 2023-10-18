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


#ifndef AWALI_STTC_CORE_RAT_JSON_VISITOR_HXX
#define AWALI_STTC_CORE_RAT_JSON_VISITOR_HXX

#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/ctx/traits.hh>

#include <awali/common/version.hh>
#include <awali/common/json/node.cc>

namespace awali { namespace sttc {


  /*---------------------.
  | Json export(ratexp).  |
  `---------------------*/

  namespace rat
  {
    template <typename RatExpSet, unsigned version=version::fsm_json>
    class json_visitor
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using labelset_t = labelset_t_of<context_t>;
      using ratexp_t = typename ratexpset_t::value_t;
      using weightset_t = weightset_t_of<ratexpset_t>;
      using super_type = typename ratexpset_t::const_visitor;
      using node_t = typename super_type::node_t;
      

     constexpr static const char* me() { return "json_visitor"; }

      json_visitor(const ratexpset_t& rs)
        : rs_(rs)
      {}
      
      /* Getter and setter might be made more safe */
      json::object_t* ptr() { return _ptr;}
      void set_ptr(json::object_t* x) { _ptr =x;}

      json::object_t* operator()(const ratexp_t& v)
      {
        v->accept(*this);
        return ptr();
      }

      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(transposition)
      AWALI_RAT_UNSUPPORTED(complement)
      AWALI_RAT_UNSUPPORTED(conjunction)

      AWALI_RAT_VISIT(zero,)
      {
        set_ptr(new json::object_t("zero",new json::null_t()));
      }

      AWALI_RAT_VISIT(one,)
      {
        set_ptr(new json::object_t("one",new json::null_t()));
      }

      AWALI_RAT_VISIT(atom, e)
      {
        set_ptr(new json::object_t("label", ls_.value_to_json(e.value())));
      }

      template <typename T>
      json::object_t* array_from_children(T e, std::string const& str) 
      {
        json::array_t* arr= new json::array_t();
        for (unsigned i = 0, n = e.size(); i < n; ++i) {
          const auto& v = e[i];
          v->accept(*this);
          arr->push_back(ptr());
        }
        return new json::object_t(str, arr);
      }

      AWALI_RAT_VISIT(sum, e)
      {
        set_ptr(array_from_children(e,"sum"));
      }

      AWALI_RAT_VISIT(prod, e)
      {
        set_ptr(array_from_children(e,"prod"));
      }

      AWALI_RAT_VISIT(shuffle, e)
      {
        set_ptr(array_from_children(e,"shuffle"));
      }

      AWALI_RAT_VISIT(star, e)
      {
        e.sub()->accept(*this);
        set_ptr(new json::object_t("star",ptr()));
      }

      AWALI_RAT_VISIT(maybe, e)
      {
        e.sub()->accept(*this);
        set_ptr(new json::object_t("maybe",ptr()));
      }

      AWALI_RAT_VISIT(plus, e)
      {
        e.sub()->accept(*this);
        set_ptr(new json::object_t("plus",ptr()));
      }

      AWALI_RAT_VISIT(lweight, e)
      {
        e.sub()->accept(*this);
        json::object_t* o = ptr();
        o->push_front("lweight",ws_.value_to_json(e.weight()));
      }

      AWALI_RAT_VISIT(rweight, e)
      {
        e.sub()->accept(*this);
        json::object_t* o = ptr();
        o->push_front("rweight",ws_.value_to_json(e.weight()));
      }

    private:
      ratexpset_t rs_;
      /// Shorthand to the weightset.
      json::object_t* _ptr = nullptr;
      weightset_t ws_ = *rs_.weightset();
      labelset_t ls_ = *rs_.labelset();
    };



}//end of namespace awali::sttc::rat
}//end of namespace awali::sttc
}//end of namespace awali

#endif
