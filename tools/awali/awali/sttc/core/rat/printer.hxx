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

#ifndef AWALI_CORE_RAT_PRINTER_HXX
# define AWALI_CORE_RAT_PRINTER_HXX

#include <awali/sttc/misc/escape.hh>
#include <awali/sttc/misc/indent.hh>
#include <awali/sttc/misc/raise.hh>

namespace awali { namespace sttc
{
  namespace rat
  {

    inline
    std::ostream&
    operator<<(std::ostream& o, type_t t)
    {
      switch (t)
        {
# define CASE(T) case type_t::T: o << #T; break
          CASE(zero);
          CASE(one);
          CASE(atom);
          CASE(sum);
          CASE(prod);
          CASE(ldiv);
          CASE(conjunction);
          CASE(shuffle);
          CASE(star);
          CASE(transposition);
          CASE(lweight);
          CASE(rweight);
          CASE(complement);
# undef CASE
        }
      return o;
    }

    template <typename RatExpSet>
    inline
    printer<RatExpSet>::printer(const ratexpset_t& rs,
                                std::ostream& out,
                                const bool debug)
      : out_(out)
      , ctx_(rs.context())
      , identities_(rs.identities())
      , debug_(debug)
    {}


# define DEFINE                                 \
    template <typename RatExpSet>               \
    inline                                      \
    auto                                        \
    printer<RatExpSet>

    DEFINE::operator()(const node_t& v)
      -> std::ostream&
    {
      static bool print = !! getenv("AWALI_PRINT");
      static bool debug = !! getenv("AWALI_DEBUG");
      if (print)
        out_ << '<' << v.type() << '>' << sttc::incendl;
      if (debug && format_ == "latex" && identities_ == identities_t::series)
        out_ << "{\\color{red}{";
      else if (debug && format_ == "latex" && identities_ == identities_t::trivial)
        out_ << "{\\color{blue}{";
      v.accept(*this);
      if (debug && format_ == "latex" && identities_ == identities_t::series)
        out_ << "}}";
      else if (debug && format_ == "latex" && identities_ == identities_t::trivial)
        out_ << "}}";
      if (print)
        out_ << sttc::decendl << "</" << v.type() << '>';
      return out_;
    }

    DEFINE::format(const std::string& format)
      -> void
    {
      format_ = format;
      if (format_ == "latex")
        {
          lgroup_       = "{";
          rgroup_       = "}";
          langle_       = " \\langle ";
          rangle_       = " \\rangle ";
          lparen_       = "\\left(";
          rparen_       = "\\right)";
          star_         = "^{*}";
          complement_   = "^{c}";
          transposition_ = "^{T}";
          conjunction_  = " \\& ";
          shuffle_      = " \\between ";
          product_      = " \\, ";
          sum_
            = (identities_ == identities_t::series) ? " \\oplus " : " + ";
          zero_         = "\\emptyset";
          one_          = "\\varepsilon";
          lmul_         = "\\,";
          rmul_         = "\\,";
          ldiv_         = " \\backslash ";
        }
      else if (format_ == "text")
        {
          lgroup_       = "";
          rgroup_       = "";
          langle_       = "<";
          rangle_       = ">";
          lparen_       = "(";
          rparen_       = ")";
          star_         = "*";
          complement_   = "{c}";
          transposition_ = "{T}";
          conjunction_  = "&";
          shuffle_      = ":";
          product_      = "";
          sum_          = "+";
          zero_         = "\\z";
          one_          = "\\e";
          lmul_         = "";
          rmul_         = "";
          ldiv_         = "{\\}";
        }
      else
        raise("invalid output format for ratexp: ", str_escape(format));
    }

    DEFINE::precedence(const node_t& v) const
      -> precedence_t
    {
      const atom_t* atom = dynamic_cast<const atom_t*>(&v);
      if (atom && ! ctx_.labelset()->is_letter(atom->value()))
        return precedence_t::word;
      else
        switch (v.type())
          {
# define CASE(Type)                             \
            case exp::type_t::Type:             \
              return precedence_t::Type;
            CASE(atom);
            CASE(complement);
            CASE(conjunction);
            CASE(ldiv);
            CASE(lweight);
            CASE(one);
            CASE(prod);
            CASE(rweight);
            CASE(shuffle);
            CASE(star);
            CASE(sum);
            CASE(transposition);
            CASE(zero);
# undef CASE
          }
      abort(); // Unreachable.
    }

# define VISIT(Type)                          \
    DEFINE::visit(const Type ## _t& v)        \
      -> void

    VISIT(lweight)
    {
      out_ << langle_;
      ctx_.weightset()->print(v.weight(), out_, format_);
      out_ << rangle_ << lmul_;
      print_child(*v.sub(), v);
    }

    VISIT(rweight)
    {
      print_child(*v.sub(), v);
      out_ << rmul_ << langle_;
      ctx_.weightset()->print(v.weight(), out_, format_);
      out_ << rangle_;
    }

    VISIT(zero)
    {
      (void) v;
      out_ << zero_;
    }

    VISIT(one)
    {
      (void) v;
      out_ << one_;
    }

    VISIT(atom)
    {
      ctx_.labelset()->print(v.value(), out_, format_);
    }

    DEFINE::print_child(const node_t& child, const node_t& parent)
      -> void
    {
      static bool force = !! getenv("AWALI_PARENS");
      bool parent_has_precedence = precedence(child) <= precedence(parent);
      bool needs_parens =
        (force
         || (parent_has_precedence
             && ! (parent.is_unary() && child.is_unary())));
      if (needs_parens)
        out_ << lparen_;
      else if (parent.is_unary())
        out_ << lgroup_;
      operator()(child);
      if (needs_parens)
        out_ << rparen_;
      else if (parent.is_unary())
        out_ << rgroup_;
    }

    template <typename RatExpSet>
    template <type_t Type>
    inline
    auto
    printer<RatExpSet>::print(const unary_t<Type>& v, const char* op)
      -> void
    {
      print_child(*v.sub(), v);
      out_ << op;
    }

    template <typename RatExpSet>
    template <type_t Type>
    inline
    auto
    printer<RatExpSet>::print(const variadic_t<Type>& n, const char* op)
      -> void
    {
      bool first = true;
      for (auto i: n)
        {
          if (! first)
            out_ << op;
          print_child(*i, n);
          first = false;
        }
    }

# undef VISIT
# undef DEFINE

  } // namespace rat
}}//end of ns awali::stc

#endif // !AWALI_CORE_RAT_PRINTER_HXX
