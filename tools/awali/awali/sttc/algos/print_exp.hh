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

#ifndef AWALI_ALGOS_PRINT_EXP_HH
# define AWALI_ALGOS_PRINT_EXP_HH

#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/misc/raise.hh>
# include <stack>
# include <iostream>

# if DEBUG
#  define DEBUG_IFELSE(Then, Else) Then
# else
#  define DEBUG_IFELSE(Then, Else) Else
# endif

# define DEBUG_IF(Then) DEBUG_IFELSE(Then,)

namespace awali {
  namespace sttc {


    /*,----------------.
      | print(ratexp). |
      `----------------'*/

  namespace rat
  {
    template <typename RatExpSet>
    class print_visitor
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

      constexpr static const char* me() { return "print-exp"; }

      print_visitor(const ratexpset_t& rs, std::ostream& o, bool with_dot)
        : rs_(rs), stream_(o), with_dot_(with_dot)
      {}

      std::ostream& operator()(const ratexp_t& v)
      {
        v->accept(*this);
        return stream_;
      }

      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(transposition)

      AWALI_RAT_VISIT(zero,)
      {
        stream_ << "\\z";
      }

      AWALI_RAT_VISIT(one,)
      {
        stream_ << "\\e";
      }

      AWALI_RAT_VISIT(atom, e)
      {
        ls_.print(e.value(), stream_);
      }

      AWALI_RAT_VISIT(sum, e)
      {
        for (unsigned i = 0, n = e.size(); i < n; ++i) {
          if(i>0)
            stream_ << '+';
          const auto& v = e[i];
          if(i>0 && v->type()==type_t::sum)
            stream_ << '(';
          v->accept(*this);
          if(i>0 && v->type()==type_t::sum)
            stream_ << ')';
        }
      }

      AWALI_RAT_VISIT(prod, e)
      {
        for (unsigned i = 0, n = e.size(); i < n; ++i) {
          const auto& v = e[i];
          if(i>0 && with_dot_)
            stream_ << '.';
          if(v->type()==type_t::sum || (i>0 && v->type()==type_t::prod))
            stream_ << '(';
          v->accept(*this);
          if(v->type()==type_t::sum || (i>0 && v->type()==type_t::prod))
            stream_ << ')';
        }
      }

      AWALI_RAT_VISIT(conjunction, e)
      {
        char c='(';
        for (unsigned i = 0, n = e.size(); i < n; ++i) {
          stream_ << c;
          c='&';
          const auto& v = e[i];
          v->accept(*this);
        }
        stream_ << ')';
      }

      AWALI_RAT_VISIT(shuffle, e)
      {
        char c='(';
        for (unsigned i = 0, n = e.size(); i < n; ++i) {
          stream_ << c;
          c='$';
          const auto& v = e[i];
          v->accept(*this);
        }
        stream_ << ')';
      }

      AWALI_RAT_VISIT(complement, e)
      {
        stream_ << '(';
        e.sub()->accept(*this);
        stream_ << "){c}";
      }

      AWALI_RAT_VISIT(star, e)
      {
        const auto& v = e.sub();
        if(v->type()==type_t::sum || v->type()==type_t::prod)
          stream_ << '(';
        v->accept(*this);
        if(v->type()==type_t::sum || v->type()==type_t::prod)
          stream_ << ')';
        stream_ << '*';
      }

      AWALI_RAT_VISIT(lweight, e)
      {
        const auto& v = e.sub();
        stream_ << '<';
        ws_.print(e.weight(), stream_);
        stream_ << '>';
        if(    v->type()==type_t::sum 
            || v->type()==type_t::prod 
            || v->type()==type_t::star )
          stream_ << '(';
        v->accept(*this);
        if(    v->type()==type_t::sum 
            || v->type()==type_t::prod 
            || v->type()==type_t::star )
          stream_ << ')';
      }

      AWALI_RAT_VISIT(rweight, e)
      {
        const auto& v = e.sub();
        if(v->type()==type_t::sum || v->type()==type_t::prod || v->type()==type_t::star)
          stream_ << '(';
        v->accept(*this);
        if(v->type()==type_t::sum || v->type()==type_t::prod || v->type()==type_t::star)
          stream_ << ')';
        stream_ << '<';
        ws_.print(e.weight(), stream_);
        stream_ << '>';
      }

    private:
      ratexpset_t rs_;
      std::ostream& stream_;
      bool with_dot_;
      /// Shorthand to the weightset.
      const weightset_t& ws_ = *rs_.weightset();
      const labelset_t& ls_ = *rs_.labelset();
    };

  } // rat::

  template <typename RatExpSet>
  inline
  std::ostream&
  print(const RatExpSet& rs,
             const typename RatExpSet::ratexp_t& e,
        std::ostream& o,
        bool with_dot=false)
  {
    rat::print_visitor<RatExpSet> printer{rs,o,with_dot};
    return printer(e);
  }


}}//end of ns awali::stc

#endif // !AWALI_ALGOS_PRINT_EXP_HH
