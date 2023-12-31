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

#ifndef AWALI_CORE_RAT_PRINTER_HH
# define AWALI_CORE_RAT_PRINTER_HH

# include <iostream>

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/core/rat/identities.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/misc/cast.hh>

namespace awali { namespace sttc
{
  namespace rat
  {

    template <typename RatExpSet>
    class printer
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using identities_t = typename ratexpset_t::identities_t;
      using weight_t = typename context_t::weightset_t::value_t;
      using super_type = typename ratexpset_t::const_visitor;
      using node_t = typename super_type::node_t;
      using inner_t = typename super_type::inner_t;
      template <type_t Type>
      using unary_t = typename super_type::template unary_t<Type>;
      template <type_t Type>
      using variadic_t = typename super_type::template variadic_t<Type>;
      using leaf_t = typename super_type::leaf_t;

      printer(const ratexpset_t& rs,
              std::ostream& out,
              const bool debug = !!getenv("AWALI_PARENS"));

      /// Set output format.
      void format(const std::string& format);

      /// Entry point: print \a v.
      std::ostream& operator()(const node_t& v);

      /// Entry point: print \a v.
      std::ostream&
      operator()(const std::shared_ptr<const node_t>& v)
      {
        return operator()(*v);
      }

    private:

# define DEFINE(Type)                                           \
      using Type ## _t = typename super_type::Type ## _t;       \
      virtual void visit(const Type ## _t& v)

      DEFINE(atom);
      DEFINE(complement)    { print(v, complement_); }
      DEFINE(conjunction)   { print(v, conjunction_); }
      DEFINE(ldiv)          { print(v, ldiv_); }
      DEFINE(lweight);
      DEFINE(one);
      DEFINE(prod)          { print(v, product_); }
      DEFINE(rweight);
      DEFINE(shuffle)       { print(v, shuffle_); }
      DEFINE(star)          { print(v, star_); }
      DEFINE(maybe)          { print(v, maybe_); }
      DEFINE(plus)          { print(v, plus_); }
      DEFINE(sum)           { print(v, sum_); }
      DEFINE(transposition) { print(v, transposition_); }
      DEFINE(zero);

# undef DEFINE

      /// The possible node precedence levels, increasing.
      ///
      /// When printing a word (i.e., a label with several letters),
      /// beware that it may require parens.  Think of
      /// star(atom(ab)): if we print it as 'ab*', it actually means
      /// 'a(b*)'.  Then give words a precedence lower than that of
      /// star.  This is the role of 'word' below.
      enum class precedence_t
      {
        sum,
        shuffle,
        conjunction,
        ldiv,
        prod,
        word = prod, // Multi-letter atoms.
        lweight,
        rweight,
        star,
        maybe,
        plus,
        complement,
        transposition,
        zero,
        one,
        atom,
      };

      /// The precedence of \a v (to decide when to print parens).
      precedence_t precedence(const node_t& v) const;

      /// Print the given child node, also knowing its parent.
      void print_child(const node_t& child, const node_t& parent);

      /// Print a unary node.
      template <rat::exp::type_t Type>
      void print(const unary_t<Type>& n, const char* op);

      /// Print an n-ary node.
      template <rat::exp::type_t Type>
      void print(const variadic_t<Type>& n, const char* op);

      /// Whether the left weight shows.
      ATTRIBUTE_PURE
      bool shows_left_weight_(const node_t& n)
      {
        return n.type() == rat::type_t::lweight;
      }

      /// Output stream.
      std::ostream& out_;
      /// Output format.
      std::string format_;
      /// Context to decode labels and weights.
      const context_t& ctx_;
      const identities_t identities_;
      /// Whether to be overly verbose.
      const bool debug_;

      /// Left and right boundaries (typically braces for LaTeX).
      const char* lgroup_ = nullptr;
      const char* rgroup_ = nullptr;
      /// Left and right angle brackets for weights.
      const char* langle_ = nullptr;
      const char* rangle_ = nullptr;
      /// Left and right parentheses.
      const char* lparen_ = nullptr;
      const char* rparen_ = nullptr;
      /// External product.
      const char* lmul_ = nullptr;
      const char* rmul_ = nullptr;
      /// Quotient.
      const char* ldiv_ = nullptr;
      /// The ratexp operators.
      const char* star_ = nullptr;
      const char* maybe_ = nullptr;
      const char* plus_ = nullptr;
      const char* complement_ = nullptr;
      const char* transposition_ = nullptr;
      const char* conjunction_ = nullptr;
      const char* shuffle_ = nullptr;
      const char* product_ = nullptr;
      const char* sum_ = nullptr;
      /// The constants.
      const char* zero_ = nullptr;
      const char* one_ = nullptr;
    };

  } // namespace rat
}}//end of ns awali::stc

#include <awali/sttc/core/rat/printer.hxx>

#endif // !AWALI_CORE_RAT_PRINTER_HH
