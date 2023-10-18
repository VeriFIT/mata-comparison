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

#ifndef AWALI_CORE_RAT_RATEXP_HH
# define AWALI_CORE_RAT_RATEXP_HH

# include <vector>
# include <string>

//# include <boost/range.hpp> // make_iterator_range
#include <awali/sttc/misc/cont_filter.hh> // it_cont
#include <awali/sttc/core/rat/fwd.hh>
#include <awali/sttc/core/rat/visitor.hh>

namespace awali { namespace sttc
{
  namespace rat
  {

    /// The abstract, non-parameterized, root for all rational
    /// expression node types.
    class exp
    {
    public:
#ifndef COVERAGE
      virtual ~exp() = default;
#endif

      /// The possible types of ratexps.
      using type_t = rat::type_t;

      /// The type of this node.
      virtual type_t type() const = 0;

      /// Whether star, complement.
      bool is_unary() const
      {
        return sttc::rat::is_unary(type());
      }

      /// Whether a leaf of the ratexp tree.
      bool is_leaf() const
      {
        type_t t = type();
        return (sttc::rat::is_constant(t)
                || t == type_t::atom);
      }
    };


    /*-------.
    | node.  |
    `-------*/

    /// The abstract parameterized, root for all rational expression
    /// types.
    template <typename Label, typename Weight>
    class node
      : public std::enable_shared_from_this<node<Label, Weight>>
      , public exp
    {
    public:
      using label_t = Label;
      using weight_t = Weight;
      using node_t = rat::node<label_t, weight_t>;
      using value_t = std::shared_ptr<const node_t>;
      /// Same as value_t, but writable.  Use with care.
      using wvalue_t = std::shared_ptr<node_t>;
      using ratexps_t = std::vector<value_t>;
      using const_visitor = sttc::rat::const_visitor<label_t, weight_t>;

      virtual void accept(const_visitor &v) const = 0;
    };

    /*--------.
    | inner.  |
    `--------*/

    /// An inner node.
    template <typename Label, typename Weight>
    class inner
      : public node<Label, Weight>
    {
    public:
      using label_t = Label;
      using weight_t = Weight;
      using super_type = node<label_t, weight_t>;
      using value_t = typename super_type::value_t;
    };


    /*-----------.
    | variadic.  |
    `-----------*/

    /// An inner node with multiple children.
    ///
    /// Implements the Composite Design Pattern.
    template <exp::type_t Type, typename Label, typename Weight>
    class variadic
      : public inner<Label, Weight>
    {
    public:
      static_assert(sttc::rat::is_variadic(Type), "invalid type");

      using label_t = Label;
      using weight_t = Weight;
      using super_type = inner<label_t, weight_t>;
      using node_t = node<label_t, weight_t>;
      using value_t = typename super_type::value_t;
      using ratexps_t = typename super_type::ratexps_t;

      using const_iterator = typename ratexps_t::const_iterator;
      // Needed by boost::make_iterator_range, but since we iterate
      // over const value (well, shared_ptr to const values), make it
      // a const_iterator anyway.  Otherwise, clang won't compile.
      using iterator = const_iterator;
      using const_reverse_iterator = typename ratexps_t::const_reverse_iterator;
      using reverse_iterator = const_reverse_iterator;

      virtual type_t type() const override { return Type; };

      const_iterator begin() const;
      const_iterator end() const;
      const_reverse_iterator rbegin() const;
      const_reverse_iterator rend() const;
      size_t size() const;

      /// Access the n-th element.
      /// precondition 0 <= n < size.
      const value_t operator[](size_t n) const;

      /// The first item of this variadic.
      const value_t head() const;
      /// The last item of this variadic.
      const value_t back() const;

      /// The non-first items.
      auto tail() const -> internal::it_cont<const_iterator>;

      variadic(const ratexps_t& ns = ratexps_t());
      variadic(const variadic& that)
        : super_type(that)
        , sub_(that.sub_)
      {}

      /// Return a copy of children.
      ratexps_t subs() const;

      virtual void accept(typename node_t::const_visitor &v) const override;

    private:
      ratexps_t sub_;
    };

    /*--------.
    | unary.  |
    `--------*/

    template <exp::type_t Type, typename Label, typename Weight>
    class unary
      : public inner<Label, Weight>
    {
    public:
      static_assert(is_unary(Type), "invalid type");

      using label_t = Label;
      using weight_t = Weight;
      using super_type = inner<label_t, weight_t>;
      using node_t = node<label_t, weight_t>;
      using value_t = typename node_t::value_t;

      unary(value_t exp);
      virtual type_t type() const override { return Type; };

      const value_t sub() const;

      virtual void accept(typename node_t::const_visitor &v) const override;

    private:
      value_t sub_;
    };


    /*--------.
    | weight. |
    `--------*/

    /// An inner node implementing a weight.
    ///
    /// Implements the Composite Design Pattern.
    template <exp::type_t Type, typename Label, typename Weight>
    class weight_node
      : public inner<Label, Weight>
    {
    public:
      static_assert(Type == type_t::lweight
                    || Type == type_t::rweight,
                    "invalid type");

      using label_t = Label;
      using weight_t = Weight;
      using super_type = inner<label_t, weight_t>;
      using node_t = node<label_t, weight_t>;
      using value_t = typename super_type::value_t;

      virtual type_t type() const override { return Type; };

      const value_t sub() const;
      const weight_t& weight() const;
      void set_weight(const weight_t& w);

      weight_node(const weight_t& w, value_t exp);
      weight_node(const weight_node& that)
        : sub_(that.sub_)
        , weight_(that.weight_)
      {}

      virtual void accept(typename node_t::const_visitor &v) const override;

    private:
      value_t sub_;
      weight_t weight_;
    };


    /*-------.
    | leaf.  |
    `-------*/

    /// The root from which to derive the final node types.
    template <typename Label, typename Weight>
    class leaf
      : public node<Label, Weight>
    {
    public:
      using label_t = Label;
      using weight_t = Weight;
      using node_t = node<label_t, weight_t>;
      using type_t = typename node_t::type_t;
      using value_t = typename node_t::value_t;
      using super_type = node_t;
    };


    template <exp::type_t Type, typename Label, typename Weight>
    class constant
      : public leaf<Label, Weight>
    {
    public:
      static_assert(is_constant(Type), "invalid type");
      using label_t = Label;
      using weight_t = Weight;
      using super_type = leaf<label_t, weight_t>;
      using node_t = node<label_t, weight_t>;
      using value_t = typename node_t::value_t;

      virtual type_t type() const override { return Type; };

      virtual void accept(typename node_t::const_visitor &v) const override;
    };


    template <typename Label, typename Weight>
    class atom
      : public leaf<Label, Weight>
    {
    public:
      using label_t = Label;
      using weight_t = Weight;
      using super_type = leaf<label_t, weight_t>;
      using node_t = node<label_t, weight_t>;
      using type_t = typename node_t::type_t;
      using value_t = typename node_t::value_t;

      atom(const label_t& value);

      virtual type_t type() const override { return type_t::atom; };

      virtual void accept(typename node_t::const_visitor &v) const override;
      const label_t& value() const;

    private:
      label_t value_;
    };

  } // namespace rat
}}//end of ns awali::stc

#include <awali/sttc/core/rat/ratexp.hxx>

#endif // !AWALI_CORE_RAT_RATEXP_HH
