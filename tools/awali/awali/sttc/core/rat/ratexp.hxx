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

#ifndef AWALI_CORE_RAT_RATEXP_HXX_
# define AWALI_CORE_RAT_RATEXP_HXX_

#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/core/rat/visitor.hh>

namespace awali { namespace sttc
{
  namespace rat
  {

#define DEFINE_CTOR(Node)                       \
    template <typename Label, typename Weight>  \
    inline                                      \
    Node<Label, Weight>::Node

#define DEFINE(Node)                            \
    template <typename Label, typename Weight>  \
    inline                                      \
    auto                                        \
    Node<Label, Weight>

    /*-------.
    | atom.  |
    `-------*/

    DEFINE_CTOR(atom)(const label_t& value)
      : value_(value)
    {}

    DEFINE(atom)::accept(typename node_t::const_visitor& v) const
      -> void
    {
      v.visit(*this);
    }

    DEFINE(atom)::value() const
      -> const label_t&
    {
      return value_;
    }

# undef DEFINE_CTOR
# undef DEFINE


#define DEFINE_CTOR(Node)                                       \
    template <type_t Type, typename Label, typename Weight>     \
    inline                                                      \
    Node<Type, Label, Weight>::Node

#define DEFINE(Node)                                         \
    template <type_t Type, typename Label, typename Weight>  \
    inline                                                   \
    auto                                                     \
    Node<Type, Label, Weight>


    /*-----------.
    | variadic.  |
    `-----------*/

    DEFINE_CTOR(variadic)(const ratexps_t& ns)
      : sub_(ns)
    {}


    DEFINE(variadic)::begin() const
      -> const_iterator
    {
      return sub_.begin();
    }

    DEFINE(variadic)::end() const
      -> const_iterator
    {
      return sub_.end();
    }

    DEFINE(variadic)::rbegin() const
      -> const_reverse_iterator
    {
      return sub_.rbegin();
    }

    DEFINE(variadic)::rend() const
      -> const_reverse_iterator
    {
      return sub_.rend();
    }

    DEFINE(variadic)::size() const
      -> size_t
    {
      return sub_.size();
    }

    DEFINE(variadic)::operator[](size_t n) const
      -> const value_t
    {
      return sub_[n];
    }

    DEFINE(variadic)::head() const
      -> const value_t
    {
      return *begin();
    }

    DEFINE(variadic)::back() const
      -> const value_t
    {
      return sub_.back();
    }

    DEFINE(variadic)::tail() const
      -> internal::it_cont<const_iterator>
    {
        auto it=begin();
      return internal::it_cont<const_iterator>(++it,end());
    }

    DEFINE(variadic)::subs() const
      -> ratexps_t
    {
      return sub_;
    }

    DEFINE(variadic)::accept(typename node_t::const_visitor& v) const
      -> void
    {
      v.visit(*this);
    }

    /*---------.
    | weight.  |
    `---------*/

    DEFINE_CTOR(weight_node)(const weight_t& weight, value_t sub)
      : sub_(sub)
      , weight_(weight)
    {}

    DEFINE(weight_node)::sub() const
      -> const value_t
    {
      return sub_;
    }

    DEFINE(weight_node)::weight() const
      -> const weight_t&
    {
      return weight_;
    }

    DEFINE(weight_node)::set_weight(const weight_t& w)
      -> void
    {
      weight_ = w;
    }

    DEFINE(weight_node)::accept(typename node_t::const_visitor& v) const
      -> void
    {
      v.visit(*this);
    }

    /*--------.
    | unary.  |
    `--------*/

    DEFINE_CTOR(unary)(value_t sub)
      : sub_(sub)
    {}

    DEFINE(unary)::sub() const
      -> const value_t
    {
      return sub_;
    }

    DEFINE(unary)::accept(typename node_t::const_visitor& v) const
      -> void
    {
      v.visit(*this);
    }

    /*-----------.
    | constant.  |
    `-----------*/

    DEFINE(constant)::accept(typename node_t::const_visitor& v) const
      -> void
    {
      v.visit(*this);
    }

# undef DEFINE_CTOR
# undef DEFINE

  } // namespace exp
}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_RATEXP_HXX_
