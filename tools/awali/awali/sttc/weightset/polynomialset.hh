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

#ifndef AWALI_WEIGHTSET_POLYNOMIALSET_HH
# define AWALI_WEIGHTSET_POLYNOMIALSET_HH

# include <algorithm>
# include <iostream>
# include <sstream>
# include <type_traits>
# include <vector>

#include <awali/sttc/ctx/context.hh> // We need context to define join.
#include <awali/sttc/weightset/fwd.hh>

#include <awali/sttc/ctx/traits.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/utils/hash.hh>
#include <awali/sttc/misc/map.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/misc/stream.hh>
#include <awali/common/types.hh>

namespace awali { namespace sttc {
  // http://llvm.org/bugs/show_bug.cgi?id=18571
# if defined __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-value"
# endif
  template <typename LabelSet>
  auto label_is_zero(const LabelSet& ls, const typename LabelSet::value_t* l)
    -> decltype(ls.is_zero(l), bool())
  {
    return ls.is_zero(*l);
  }

# if defined __clang__
# pragma clang diagnostic pop
# endif

  template <typename LabelSet>
  bool label_is_zero(const LabelSet&, ...)
  ATTRIBUTE_CONST;

  template <typename LabelSet>
  bool label_is_zero(const LabelSet&, ...)
  {
    return false;
  }

  /// Linear combination of labels: map labels to weights.
  /// \tparam Context  the LabelSet and WeightSet types.
  template <class Context>
  class polynomialset
  {
  public:
    using self_type = polynomialset<Context>;
    using context_t = Context;
    using labelset_t = labelset_t_of<context_t>;
    using weightset_t = weightset_t_of<context_t>;
    using polynomialset_t = polynomialset<context_t>;

    using labelset_ptr = typename context_t::labelset_ptr;
    using weightset_ptr = typename context_t::weightset_ptr;
    /// Polynomials over labels.
    using label_t = typename labelset_t::value_t;
    using weight_t = weight_t_of<context_t>;

    using value_t = std::map<label_t, weight_t, internal::less<labelset_t>>;
    /// A pair <label, weight>.
    using monomial_t = typename value_t::value_type;

    polynomialset() = delete;
    polynomialset(const polynomialset&) = default;
    polynomialset(const context_t& ctx)
      : ctx_{ctx}
    {}

    /// The static name.
    static std::string sname()
    {
      return "polynomialset<" + context_t::sname() + ">";
    }

    /// The dynamic name.
    std::string vname(bool full = true) const
    {
      return "polynomialset<" + context().vname(full) + ">";
    }

    const context_t& context() const { return ctx_; }
    const labelset_ptr& labelset() const { return ctx_.labelset(); }
    const weightset_ptr& weightset() const { return ctx_.weightset(); }

    /// Remove the monomial of \a w in \a v.
    value_t&
    del_weight(value_t& v, const label_t& w) const
    {
      v.erase(w);
      return v;
    }

    /// Set the monomial of \a w in \a v to weight \a k.
    value_t&
    set_weight(value_t& v, const label_t& w, const weight_t k) const
    {
      if (weightset()->is_zero(k))
        del_weight(v, w);
      else
        v[w] = k;
      return v;
    }

    /// v += p.
    value_t&
    add_here(value_t& v, const value_t& p) const
    {
      for (const auto& m: p)
        add_here(v, m);
      return v;
    }

    /// v += m.
    value_t&
    add_here(value_t& v, const monomial_t& p) const
    {
      return add_here(v, p.first, p.second);
    }

    // FIXME: rename at least this version.
    /// v += (l, k).
    value_t&
    add_here(value_t& v, const label_t& l, const weight_t k) const
    {
      if (!label_is_zero(*labelset(), &l))
        {
          auto i = v.find(l);
          if (i == v.end())
            {
              set_weight(v, l, k);
            }
          else
            {
              // Do not use set_weight() because it would lookup w
              // again and we already have the right iterator.
              auto w2 = weightset()->add(i->second, k);
              if (weightset()->is_zero(w2))
                v.erase(i);
              else
                i->second = w2;
            }
        }
      return v;
    }

    const weight_t
    get_weight(const value_t& v, const label_t& w) const ATTRIBUTE_PURE
    {
      auto i = v.find(w);
      if (i == v.end())
        return weightset()->zero();
      else
        return i->second;
    }

    /// The sum of polynomials \a l and \a r.
    value_t
    add(const value_t& l, const value_t& r) const
    {
      value_t res = l;
      for (auto& i : r)
        add_here(res, i.first, i.second);
      return res;
    }

    /// The product of polynomials \a l and \a r.
    value_t
    mul(const value_t& l, const value_t& r) const
    {
      value_t res;
      for (auto i: l)
        for (auto j: r)
          add_here(res,
                     labelset()->concat(i.first, j.first),
                     weightset()->mul(i.second, j.second));
      return res;
    }

    /// The conjunction of polynomials \a l and \a r.
    /// Not valid for all the labelsets.
    value_t
    conjunction(const value_t& l, const value_t& r) const
    {
      value_t res;
      for (auto i: l)
        for (auto j: r)
          add_here(res,
                     labelset()->conjunction(i.first, j.first),
                     weightset()->mul(i.second, j.second));
      return res;
    }

    /// The star of polynomial \a v.
    value_t
    star(const value_t& v) const
    {
      // The only starrable polynomialsets are scalars (if they are
      // starrable too).
      auto s = v.size();
      if (s == 0)
        return one();
      else if (s == 1)
        {
          auto i = v.find(labelset()->one());
          if (i != v.end())
            {
              value_t res;
              add_here(res, i->first, weightset()->star(i->second));
              return res;
            }
        }
      raise(sname(), ": star: invalid value: ", format(*this, v));
    }

    /// The star of polynomial \a v.
    value_t
    plus(const value_t& v) const
    {
      // The only starrable polynomialsets are scalars (if they are
      // starrable too).
      auto s = v.size();
      if (s == 0)
        return zero();
      else if (s == 1)
        {
          auto i = v.find(labelset()->one());
          if (i != v.end())
            {
              value_t res;
              add_here(res, i->first, weightset()->plus(i->second));
              return res;
            }
        }
      raise(sname(), ": star: invalid value: ", format(*this, v));
    }

    /// Left exterior product.
    value_t
    lmul(const weight_t& w, const value_t& v) const
    {
      value_t res;
      if (!weightset()->is_zero(w))
        // FIXME: What if there are divisors of 0?
        for (const auto& m: v)
          add_here(res, m.first, weightset()->mul(w, m.second));
      return res;
    }

    /// Left product by a label.
    value_t
    lmul_letter(const label_t& lhs, const value_t& v) const
    {
      value_t res;
      for (auto i: v)
        add_here(res,
                   // FIXME: This is wrong, it should be mul, not concat.
                   labelset()->concat(lhs, i.first),
                   i.second);
      return res;
    }

    /// Right exterior product.
    value_t
    rmul(const value_t& v, const weight_t& w) const
    {
      value_t res;
      if (!weightset()->is_zero(w))
        for (const auto& m: v)
          add_here(res, m.first, weightset()->mul(m.second, w));
      return res;
    }

    /// Right product.
    value_t
    rmul_letter(const value_t& v, const label_t& rhs) const
    {
      value_t res;
      for (auto i: v)
        add_here(res,
                   labelset()->concat(i.first, rhs),
                   i.second);
      return res;
    }

    static value_t
    rdiv(const value_t&, const value_t&)
    {
      throw std::runtime_error("not implemented for polynomials");
    }

    static value_t
    ldiv(const value_t& l, const value_t& r)
    {
      return rdiv(r, l);
    }

    /// Left exterior division.
    value_t&
    ldiv_here(const weight_t& w, value_t& v) const
    {
      for (auto& m: v)
        m.second = weightset()->ldiv(w, m.second);
      return v;
    }

    /// Right exterior division.
    value_t&
    rdiv_here(value_t& v, const weight_t& w) const
    {
      for (auto& m: v)
        m.second = weightset()->rdiv(m.second, w);
      return v;
    }

    static bool
    equals(const value_t& l, const value_t& r) ATTRIBUTE_PURE
    {
      return l.size() == r.size()
        // FIXME: this is wrong, it uses operator== instead of equals().
        && std::equal(l.begin(), l.end(),
                      r.begin());
    }

    const value_t&
    one() const
    {
      static value_t one_{monomial_one()};
      return one_;
    }

    const monomial_t&
    monomial_one() const
    {
      // Singleton.
      static monomial_t res{labelset()->one(), weightset()->one()};
      return res;
    }

    bool
    is_one(const value_t& v) const ATTRIBUTE_PURE
    {
      if (v.size() != 1)
        return false;
      auto i = v.find(labelset()->one());
      if (i == v.end())
        return false;
      return weightset()->is_one(i->second);
    }

    const value_t&
    zero() const
    {
      static value_t zero_;
      return zero_;
    }

    bool
    is_zero(const value_t& v) const
    {
      return v.empty();
    }

    static constexpr bool show_one() { return true; }
    static constexpr star_status_t star_status()
    {
      return weightset_t::star_status();
    }

    /// Conversion from (this and) other weightsets.
    static value_t
    conv(self_type, value_t v)
    {
      return v;
    }

  private:
    value_t
    conv_from_this_weightset(const typename weightset_t::value_t& v) const
    {
      monomial_t m{labelset()->one(),
                   weightset()->mul(v, weightset()->one())};
      return value_t{m};
    }

  public:
    /// FIXME: use enable_if to prevent this from being instantiated
    /// when WS is a polynomialset.  Then use this same technique for
    /// ratexps.
    template <typename WS>
    value_t
    conv(const WS& ws, const typename WS::value_t& v) const
    {
      return conv_from_this_weightset(weightset()->conv(ws, v));
    }

    /// Convert from another polynomialset to type_t.
    template <typename C>
    value_t
    conv(const polynomialset<C>& sps,
         const typename polynomialset<C>::value_t& v) const
    {
      value_t res;
      typename C::labelset_t sls = * sps.labelset();
      typename C::weightset_t sws = * sps.weightset();
      labelset_t tls = * labelset();
      weightset_t tws = * weightset();
      for (const auto& m: v)
        // FIXME: rename this version of add_here.
        add_here(res, tls.conv(sls, m.first), tws.conv(sws, m.second));
      return res;
    }


    ATTRIBUTE_PURE
    static bool monomial_less_than(const monomial_t& lhs,
                                   const monomial_t& rhs)
    {
      if (labelset_t::less_than(lhs.first, rhs.first))
        return true;
      else if (labelset_t::less_than(rhs.first, lhs.first))
        return false;
      else
        return weightset_t::less_than(lhs.second, rhs.second);
    }

    static bool less_than(const value_t& lhs,
                          const value_t& rhs)
    {
      return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                          rhs.begin(), rhs.end(),
                                          monomial_less_than);
    }

    value_t
    transpose(const value_t& v) const
    {
      value_t res;
      for (const auto& i: v)
        res[labelset()->transpose(i.first)] = weightset()->transpose(i.second);
      return res;
    }

    static size_t hash(const monomial_t& m)
    {
      size_t res = 0;
      std::hash_combine(res, labelset_t::hash(m.first));
      std::hash_combine(res, weightset_t::hash(m.second));
      return res;
    }

    static size_t hash(const value_t& v)
    {
      size_t res = 0;
      for (const auto& m: v)
        {
          std::hash_combine(res, labelset_t::hash(m.first));
          std::hash_combine(res, weightset_t::hash(m.second));
        }
      return res;
    }

    /// Build from the description in \a is.
    static self_type make(std::istream& is)
    {
      // name is, for instance, "polynomialset<lal_char(abcd)_z>".
      eat(is, "polynomialset<");
      auto ctx = Context::make(is);
      eat(is, '>');
      return {ctx};
    }

    std::ostream&
    print_set(std::ostream& o, const std::string& format = "text") const
    {
      if (format == "latex")
        {
          o << "\\mathsf{Poly}[";
          labelset()->print_set(o, format);
          o << " \\to ";
          weightset()->print_set(o, format);
          o << "]";
        }
      else
        {
          o << "polynomialset<";
          labelset()->print_set(o, format);
          o << "_";
          weightset()->print_set(o, format);
          o << ">";
        }
      return o;
    }

    /// Construct from a string.
    ///
    /// Somewhat more general than a mere reversal of "format",
    /// in particular "a+a" is properly understood as "<2>a" in
    /// char_z.
    ///
    /// \param i    the stream to parse
    /// \param sep  the separator between monomials.
    value_t
    conv(std::istream& i, const char sep = '+') const
    {
      value_t res;
#define SKIP_SPACES()                           \
      while (isspace(i.peek()))                 \
        i.ignore()

      do
        {
          // Possibly a weight in braces.
          SKIP_SPACES();
          weight_t w = weightset()->one();
          bool default_w = true;
          if (i.peek() == langle)
            {
              // FIXME: convert to use conv(std::istream).
              w = conv(*weightset(), bracketed(i, langle, rangle));
              default_w = false;
            }

          // Possibly, a label.
          SKIP_SPACES();
          // Whether the label is \z.
          bool is_zero = false;
          if (i.peek() == '\\')
            {
              i.ignore();
              if (i.peek() == 'z')
                {
                  is_zero = true;
                  i.ignore();
                }
              else
                i.unget();
            }

          if (!is_zero)
            {
              // The label is not \z.

              // Handle ranges
              if (i.peek() == '[')
                for (auto c : labelset()->convs(i))
                  add_here(res, c, w);
              else
                {

                  // Register the current position in the stream, so that
                  // we reject inputs such as "a++a" in LAW (where the
                  // labelset::conv would accept the empty string between
                  // the two "+").
                  std::streampos p = i.tellg();
                  label_t label = labelset()->special();
                  // Accept an implicit label (which can be an error,
                  // e.g., for LAL) if there is an explicit weight.
                  try
                    {
                      label = labelset()->conv(i);
                    }
                  catch (const std::domain_error&)
                    {}
                  // We must have at least a weight or a label.
                  if (default_w && p == i.tellg())
                    raise(sname(), ": conv: invalid value: ",
                          str_escape(i.peek()),
                          " contains an empty label"
                          " (did you mean \\e or \\z?)");
                  add_here(res, label, w);
                }
            }

          // sep (e.g., '+'), or stop parsing.
          SKIP_SPACES();
          if (i.peek() == sep)
            i.ignore();
          else
            break;
        }
      while (true);
#undef SKIP_SPACES

      return res;
    }

    /// Print a monomial.
    std::ostream&
    print(const monomial_t& m, std::ostream& out,
          const std::string& format = "text") const
    {
      static bool parens = getenv("AWALI_PARENS");
      print_weight_(m.second, out, format);
      if (parens)
        out << (format == "latex" ? "\\left(" : "(");
      labelset()->print(m.first, out, format);
      if (parens)
        out << (format == "latex" ? "\\right)" : ")");
      return out;
    }

    /// Print a value (a polynomial).
    std::ostream&
    print(const value_t& v, std::ostream& out,
          const std::string& format = "text",
          const std::string& sep = " + ") const
    {
      bool latex = format == "latex";
      if (v.empty())
        out << (latex ? "\\emptyset" : "\\z");
      else
        print_<context_t>(v, out, format,
                          latex && sep == " + " ? " \\oplus " : sep);
      return out;
    }

    std::string
    format(const value_t& v, const std::string& sep = " + ",
           const std::string& fmt = "text") const
    {
      std::ostringstream o;
      print(v, o, fmt, sep);
      return o.str();
    }

    /// Format a monomial.
    std::string
    format(const monomial_t& m) const
    {
      std::ostringstream o;
      print(m, o, "text");
      return o.str();
    }

  private:
    /// Print a weight.
    std::ostream&
    print_weight_(const weight_t& w, std::ostream& out,
                  const std::string& format = "text") const
    {
      static bool parens = getenv("AWALI_PARENS");
      if (parens || weightset()->show_one() || !weightset()->is_one(w))
        {
          out << (format == "latex" ? "\\langle " : std::string{langle});
          weightset()->print(w, out, format);
          out << (format == "latex" ? "\\rangle " : std::string{rangle});
        }
      return out;
    }

    /// Print a polynomial value without ranges.
    std::ostream&
    print_without_ranges_(const value_t& v, std::ostream& out,
                          const std::string& format = "text",
                          const std::string& sep = " + ") const
    {
      bool first = true;
      for (const auto& m: v)
        {
          if (!first)
            out << sep;
          first = false;
          print(m, out, format);
        }
      return out;
    }

    /// Print a polynomial value with ranges.
    std::ostream&
    print_with_ranges_(const value_t& v, std::ostream& out,
                       const std::string& format = "text",
                       const std::string& sep = " + ") const
    {
      if (sep == " + " || v.size() <= 3)
        return print_without_ranges_(v, out, format, sep);

      // No ranges if the weights aren't all the same.
      std::vector<label_t> letters;
//       weight_t first_w = weightset()->zero(); 
//       commented out by VM on 2022-02-14
      for (const auto& m: v)
        {
          if(labelset()->is_one(m.first))
            continue;
          /*
          if (weightset()->is_zero(first_w))
            first_w = m.second;
          else if (!weightset()->equals(m.second, first_w))
            return print_without_ranges_(v, out, format, sep);
          */
          letters.push_back(m.first);
        }

      // Print with ranges.  First, the constant-term.
      if (labelset()->is_one(std::begin(v)->first))
        {
          print(*std::begin(v), out, format);
          if (1 < v.size())
            out << sep;
        }
      
      label_t left,last;
      bool start=true;
      for(const auto &l : letters)
        if(start) {
          start=false;
          left=l;
          last=l;
        }
        else {
          if(l == last+1 &&
             weightset()->equals(get_weight(v,left),get_weight(v,l))) {
            last=l;
          }
          else {
            if(last-left>1) {
              print_weight_(get_weight(v, left), out, format);
              out << '[';
              labelset()->print(left, out, format);
              out << '-';
              labelset()->print(last, out, format);
              out << ']' << sep;
            }
            else
              for(auto le = left; le<=last; ++le) {
                print_weight_(get_weight(v, le), out, format);
                labelset()->print(le, out, format);
                out << sep;
              }
            left = last = l;
          }
        }
      if(!start) {
        if (last-left>2) {
          print_weight_(get_weight(v, left), out, format);
          out << '[';
          labelset()->print(left, out, format);
          out << '-';
          labelset()->print(last, out, format);
          out << ']';
        }
        else
          for(auto le = left; le<=last; ++le) {
            print_weight_(get_weight(v, le), out, format);
            labelset()->print(le, out, format);
            if(le!=last)
              out << sep;
          }
      }
      return out;
    }

    /// Print a non-null value for neither LAL nor LAN.
    template <typename context>
    typename std::enable_if<!(context::is_lal || context::is_lan),
                            std::ostream&>::type
    print_(const value_t& v, std::ostream& out,
           const std::string& format = "text",
           const std::string& sep = " + ") const
    {
      return print_without_ranges_(v, out, format, sep);
    }

    /// Print a non-null value for LAL or LAN.
    template <typename Ctx>
    typename std::enable_if<Ctx::is_lal || Ctx::is_lan,
                            std::ostream&>::type
    print_(const value_t& v, std::ostream& out,
           const std::string& format = "text",
           const std::string& sep = " + ") const
    {
      return print_with_ranges_(v, out, format, sep);
    }


  private:
    context_t ctx_;

    /// Left marker for weight in concrete syntax.
    constexpr static char langle = '<';
    /// Right marker for weight in concrete syntax.
    constexpr static char rangle = '>';
  };

  /// The entry between two states of an automaton.
  template <typename Aut>
  typename polynomialset<context_t_of<Aut>>::value_t
  get_entry(const Aut& aut,
            state_t s, state_t d)
  {
    using automaton_t = Aut;
    using context_t = context_t_of<automaton_t>;
    using polynomialset_t = polynomialset<context_t>;
    using polynomial_t = typename polynomialset_t::value_t;

    polynomial_t res;
    for (auto t : aut->outin(s, d))
      // Bypass set_weight(), because we know that the weight is
      // nonzero, and that there is only one weight per letter.
      res[aut->label_of(t)] = aut->weight_of(t);
    return res;
  }

  // FIXME: this works perfectly well, but I'd like a two-parameter version.
  template <typename PLS1, typename PWS1,
            typename PLS2, typename PWS2>
  inline
  auto
  join(const polynomialset<context<PLS1, PWS1>>& p1,
       const polynomialset<context<PLS2, PWS2>>& p2)
    -> polynomialset<context<join_t<PLS1, PLS2>,
                             join_t<PWS1, PWS2>>>
  {
    return {join(p1.context(), p2.context())};
  }

  template <typename WS1,
            typename PLS2, typename PWS2>
  inline
  auto
  join(const WS1& w1,
       const polynomialset<context<PLS2, PWS2>>& p2)
    -> polynomialset<context<PLS2, join_t<WS1, PWS2>>>
  {
    using ctx_t = context<PLS2, join_t<WS1, PWS2>>;
    return ctx_t{* p2.labelset(), join(w1, * p2.weightset())};
  }

  template <typename PLS1, typename PWS1,
            typename WS2>
  inline
  auto
  join(const polynomialset<context<PLS1, PWS1>>& p1,
       const WS2& w2)
    -> polynomialset<context<PLS1, join_t<PWS1, WS2>>>
  {
    return join(w2, p1);
  }

}}//end of ns awali::stc

#endif // !AWALI_WEIGHTSET_POLYNOMIALSET_HH
