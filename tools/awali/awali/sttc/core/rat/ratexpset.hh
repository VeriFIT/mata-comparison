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

#ifndef AWALI_CORE_RAT_RATEXPSET_HH
# define AWALI_CORE_RAT_RATEXPSET_HH

# include <set>
# include <string>

#include <awali/sttc/core/rat/identities.hh>
#include <awali/sttc/core/rat/ratexp.hh>
#include <awali/sttc/core/rat/json_visitor.hxx>
#include <awali/sttc/ctx/context.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/common/enums.hh>
#include <awali/sttc/labelset/oneset.hh>
#include <awali/sttc/labelset/labelset.hh>
#include <awali/sttc/labelset/letterset.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/weightset/z.hh>
#include <awali/sttc/weightset/q.hh>
#include <awali/sttc/weightset/r.hh>
#include <awali/sttc/algos/exp_parser.hh>
#include <awali/sttc/algos/js_parser.hh>

namespace awali {
  namespace sttc {
    namespace rat {
      /// A typed ratexp set.
      /// \tparam Context  the LabelSet and WeightSet types.
      template <typename Context>
      class ratexpset_impl
      {
      public:
        using self_type = ratexpset<Context>;
        using context_t = Context;
        using labelset_t = labelset_t_of<context_t>;
        using weightset_t = weightset_t_of<context_t>;
        using kind_t = labels_are_ratexps;
        using labelset_ptr = typename context_t::labelset_ptr;
        using weightset_ptr = typename context_t::weightset_ptr;
        using label_t = label_t_of<context_t>;
        using weight_t = typename weightset_t::value_t;
        using identities_t = rat::identities;
        using const_visitor = rat::const_visitor<label_t, weight_t>;
        /// Type of ratexps.
        //
        // See http://stackoverflow.com/questions/15537023 to know why we
        // add the sttc::rat:: part: GCC wants it, Clang does not care,
        // both are right.
# define DEFINE(Type)                                           \
        using Type ## _t = ::awali::sttc::rat::Type<label_t, weight_t>
        DEFINE(atom);
        DEFINE(complement);
        DEFINE(inner);
        DEFINE(conjunction);
        DEFINE(leaf);
        DEFINE(ldiv);
        DEFINE(lweight);
        DEFINE(node);
        DEFINE(one);
        DEFINE(prod);
        DEFINE(rweight);
        DEFINE(shuffle);
        DEFINE(star);
        DEFINE(maybe);
        DEFINE(plus);
        DEFINE(sum);
        DEFINE(transposition);
        DEFINE(zero);
# undef DEFINE
        template <exp::type_t Type>
        using unary_t = unary<Type, label_t, weight_t>;
        template <exp::type_t Type>
        using variadic_t = variadic<Type, label_t, weight_t>;
        using ratexp_t = std::shared_ptr<const node_t>;

        using type_t = typename node_t::type_t;
        using ratexps_t = typename node_t::ratexps_t;

        /// The value this is a set of: typeful shared pointers.
        using value_t = typename node_t::value_t;

        /// A value sequence.
        using values_t = std::vector<value_t>;

        using word_t = self_type;
        using letter_t = self_type;

      public:
        /// Static description key.
        static std::string sname();
        /// Dynamic description key.
        std::string vname(bool full = true) const;
        /// Build from the description in \a is.
        static self_type make(std::istream& is);

        /// Constructor.
        /// \param ctx        the generator set for the labels, and the weight set.
        /// \param identities the identities to guarantee
        ratexpset_impl(const context_t& ctx,
                       identities_t identities); // FIXME: make this optional again?

        /// Whether unknown letters should be added, or rejected.
        /// \param o   whether to accept unknown letters
        /// \returns   the previous status.
        bool open(bool o) const;

        const context_t& context() const;


        identities_t identities() const;
        bool is_series() const;

        const labelset_ptr& labelset() const;
        const weightset_ptr& weightset() const;

        static auto atom(const label_t& v)
          -> value_t;

        /// When used as a LabelSet for automata.
        static value_t special()
        {
          return atom(labelset_t::special());
        }

        /// When used as a LabelSet for automata.
        /// When used as WeightSet for automata.
        static bool is_special(value_t v)
        {
          return equals(special(), v);
        }

        bool is_letter(value_t) const
        {
          return false;
        }

        bool is_zero(value_t v) const ATTRIBUTE_PURE;
        static bool is_one(value_t v) ATTRIBUTE_PURE;

        static constexpr bool is_commutative_semiring()
        {
          return false;
        }

        static constexpr bool show_one()
        {
          return false;
        }

        static constexpr bool has_one()
        {
          return true;
        }

        static constexpr bool is_ratexpset()
        {
          return true;
        }

        static constexpr bool is_free()
        {
          return false;
        }

        static constexpr star_status_t star_status()
        {
          return star_status_t::STARRABLE;
        }

        value_t conv(std::istream& is) const;
        template <typename GenSet>
        value_t conv(const letterset<GenSet>& ls,
                     typename letterset<GenSet>::value_t v) const;
        value_t conv(b, typename b::value_t v) const;
        value_t conv(const z& ws, typename z::value_t v) const;
        value_t conv(const q& ws, typename q::value_t v) const;
        value_t conv(const r& ws, typename r::value_t v) const;
        template <typename Ctx2>
        value_t conv(const ratexpset_impl<Ctx2>& ws,
                     typename ratexpset_impl<Ctx2>::value_t v) const;

        value_t conv(self_type, value_t v) const;

        std::set<value_t> convs(std::istream&) const
        {
          raise(vname(), ": ranges not implemented");
        }

        std::ostream& print(const value_t v, std::ostream& o,
                            const std::string& format = "text") const;

        std::ostream&
        print_set(std::ostream& o, const std::string& format = "text") const
        {
          if (format == "latex")
            {
              o << "\\mathsf{";
              switch (identities())
                {
                case identities_t::trivial:
                case identities_t::associativity:
                  o << "RatE";
                  break;
                case identities_t::series:
                  o << "Series";
                  break;
                default:
                  assert(false);
                };
              o << "}[";
              context().print_set(o, format);
              o << ']';
            }
          else if (format == "text") {
            switch (identities())
              {
              case identities_t::trivial:
              case identities_t::associativity:
                o << "RatExp";
                break;
              case identities_t::series:
                o << "Series";
                break;
              default:
                assert(false);
              };
            o << "[";
            context().print_set(o, format);
            o << ']';
          }
          else
            raise("invalid format: ", format);
          return o;
        }



    template<unsigned version = version::fsm_json>
    json::node_t* to_json() 
    const 
    {
      version::check_fsmjson<version>();
      switch (version) {
        default:
          json::object_t* obj 
            = context().template to_json<version>()->object();
          json::object_t* res = new json::object_t();
          switch (identities())
            {
            case identities_t::trivial:
            case identities_t::associativity:
              res->push_back("Rational Expression",obj);
              break;
            case identities_t::series:
              res->push_back("Series",obj);
              break;
            default:
              assert(false);
            };
          return res;
      }
    }



    template<unsigned version = version::fsm_json>
    json::node_t* 
    value_to_json(value_t v)
    const 
    {
      switch (version) {
        case 0:
          throw parse_exception("[ratexpset] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          rat::json_visitor<ratexpset_impl<Context>, version> jsonner(*this);
          return jsonner(v);
      }
    }

    template<unsigned version = version::fsm_json>
    value_t
    value_from_json(json::node_t const* p)
    const 
    {
      switch (version) {
        case 0:
          throw parse_exception("[ratexpset] Unsupported fsm-json version:"
                                + std::to_string(version));
        case 1:
        default:
          return js_parse_exp_content(*this,p);
      }
    }


    //value_t
    //js_parse(json::node_t* p) const;

    value_t
    parse(const std::string & s, size_t& p) const {
      auto parser = awali::sttc::internal::exp_parser<ratexpset_impl>(*this, s, p);
      value_t v=parser.parseE();
      p=parser.p_;
      return v;
    }

      public:
        /// Whether \a l < \a r.
        static bool less_than(value_t l, value_t r);

        /// Whether \a l == \a r.
        static bool equals(value_t l, value_t r);

        /// Hash \a l.
        static size_t hash(const value_t& l);

        // Concrete type implementation.
        value_t zero() const;
        static value_t one();
        value_t add(value_t l, value_t r) const;
        value_t mul(value_t l, value_t r) const;
        value_t concat(value_t l, value_t r) const;
        value_t conjunction(value_t l, value_t r) const;
        value_t shuffle(value_t l, value_t r) const;
        value_t ldiv(value_t l, value_t r) const;
        value_t rdiv(value_t l, value_t r) const;
        value_t star(value_t e) const;
        value_t maybe(value_t e) const;
        value_t plus(value_t e) const;
        /// Add a complement operator.
        value_t complement(value_t e) const;
        /// Add a transposition operator.
        value_t transposition(value_t e) const;
        /// Right-multiplication by a weight.
        value_t rmul(value_t e, const weight_t& w) const;
        /// Left-multiplication by a weight.
        value_t lmul(const weight_t& w, value_t e) const;
        /// The transposed of this rational expression.
        value_t transpose(value_t e) const;

        /// Make a `word' out of a ratexp
        word_t word(label_t l) const
        {
          return l;
        }

        /// A ratexp matching one character amongst \a chars.
        template <typename... Args>
        value_t letter_class(Args&&... chars) const;

        std::string to_string(value_t e) const; // Mostly for internal convenience.

      private:
        void require_weightset_commutativity() const;
        bool less_than_ignoring_weight_(value_t l, value_t r) const;
        value_t remove_from_sum_series_(ratexps_t addends,
                                        typename ratexps_t::iterator i) const;
        value_t insert_in_sum_series_(const sum_t& addends, value_t r) const;
        value_t merge_sum_series_(const sum_t& addends1, value_t aa2) const;
        value_t add_nonzero_series_(value_t l, value_t r) const;
        exp::type_t type_ignoring_lweight_(value_t e) const;
        weight_t possibly_implicit_lweight_(value_t e) const;
        value_t unwrap_possible_lweight_(value_t e) const;
        value_t mul_expressions_(value_t l, value_t r) const;
        value_t mul_series_(value_t l, value_t r) const;
        value_t mul_(value_t l, value_t r, bool series) const;
        bool is_unweighted_nonsum_(value_t v) const;
        bool is_nonsum_(value_t v) const;
        value_t mul_atoms_(const label_t& l, const label_t& r) const;
        value_t mul_atoms_(const label_t& l, const label_t& r, std::true_type) const; // law.
        value_t mul_atoms_(const label_t& l, const label_t& r, std::false_type) const; // ! law.
        value_t mul_unweighted_nontrivial_products_(value_t a, value_t b) const;
        value_t mul_products_(value_t a, value_t b) const;
        value_t nontrivial_mul_expressions_(value_t l, value_t r) const;
        value_t nontrivial_mul_series_(value_t l, value_t r) const;
        value_t nontrivial_lmul_expression_(const weight_t& w, value_t s) const;
        value_t nontrivial_lmul_series_(const weight_t& w, value_t s) const;
        value_t nontrivial_rmul_expression_(value_t e, const weight_t& w) const;
        value_t nontrivial_rmul_series_(value_t e, const weight_t& w) const;

        /// Push \a v in \a res, applying associativity if possible.
        /// \tparam Type  the kind of ratexps on which to apply associativity.
        ///               Must be sum, conjunction, shuffle, or prod.
        template <exp::type_t Type>
        void gather(ratexps_t& res, value_t v) const;

        /// A list denoting the gathering of \a l and \a r, applying
        /// associativity if possible.
        /// \tparam Type  the kind of ratexps on which to apply associativity.
        ///               Must be SUM or PROD.
        template <exp::type_t Type>
        ratexps_t gather(value_t l, value_t r) const;

        /// If Context is LAW.
        value_t concat_(value_t l, value_t r, std::true_type) const;
        /// If Context is not LAW.
        value_t concat_(value_t l, value_t r, std::false_type) const;

        /// If context is oneset.
        template <typename LabelSet_, typename... Args>
        value_t letter_class_(const Args&&... chars, std::true_type) const;

        /// If context is not oneset.
        template <typename LabelSet_>
        value_t
        letter_class_(std::set<std::pair<typename LabelSet_::letter_t,
                      typename LabelSet_::letter_t>> chars,
                      bool accept,
                      std::false_type) const;

      private:
        context_t ctx_;
        const identities_t identities_;
      };
    } // rat::

    /// The meet of two ratexpsets.
    template <typename Ctx1, typename Ctx2>
    inline
    auto
    meet(const ratexpset<Ctx1>& a, const ratexpset<Ctx2>& b)
      -> ratexpset<meet_t<Ctx1, Ctx2>>
    {
      return {meet(a.context(), b.context()),
          meet(a.identities(), b.identities())};
    }

    /// The union of two ratexpsets.
    template <typename Ctx1, typename Ctx2>
    inline
    auto
    join(const ratexpset<Ctx1>& a, const ratexpset<Ctx2>& b)
      -> ratexpset<join_t<Ctx1, Ctx2>>
    {
      return {join(a.context(), b.context()),
          join(a.identities(), b.identities())};
    }

    // Used as a labelset.
    template <typename GenSet1,  typename Ctx2>
    inline
    auto
    join(const letterset<GenSet1>& a, const ratexpset<Ctx2>& b)
      -> ratexpset<context<join_t<letterset<GenSet1>, labelset_t_of<Ctx2>>,
      weightset_t_of<Ctx2>>>
    {
      using ctx_t = context<join_t<letterset<GenSet1>, labelset_t_of<Ctx2>>,
                            weightset_t_of<Ctx2>>;
      return {ctx_t{join(a, *b.labelset()), *b.weightset()},
          b.identities()};
    }

    template <typename Ctx1,  typename GenSet2>
    inline
    auto
    join(const ratexpset<Ctx1>& a, const letterset<GenSet2>& b)
      -> decltype(join(b, a))
    {
      return join(b, a);
    }

    // B.  FIXME: screams for refactoring!

    template <typename Context>
    inline
    ratexpset<Context>
    join(const ratexpset<Context>& a, const b&)
    {
      return a;
    }

    template <typename Context>
    inline
    ratexpset<Context>
    join(const b& a, const ratexpset<Context>& b)
    {
      return join(b, a);
    }

    // Z.
    template <typename Context>
    inline
    auto
    join(const ratexpset<Context>& rs, const z& ws)
      -> ratexpset<context<labelset_t_of<Context>,
      join_t<weightset_t_of<Context>, z>>>
    {
      using ctx_t = context<labelset_t_of<Context>,
                            join_t<weightset_t_of<Context>, z>>;
      return {ctx_t{*rs.labelset(), join(*rs.weightset(), ws)},
          rs.identities()};
    }

    template <typename Context>
    inline
    auto
    join(const z& ws, const ratexpset<Context>& rs)
      -> join_t<ratexpset<Context>, z>
    {
      return join(rs, ws);
    }

    // Q.
    template <typename Context>
    inline
    auto
    join(const ratexpset<Context>& rs, const q& ws)
      -> ratexpset<context<labelset_t_of<Context>,
      join_t<weightset_t_of<Context>, q>>>
    {
      using ctx_t = context<labelset_t_of<Context>,
                            join_t<weightset_t_of<Context>, q>>;
      return {ctx_t{*rs.labelset(), join(*rs.weightset(), ws)},
          rs.identities()};
    }

    template <typename Context>
    inline
    auto
    join(const q& ws, const ratexpset<Context>& rs)
      -> join_t<ratexpset<Context>, q>
    {
      return join(rs, ws);
    }

    // R.
    template <typename Context>
    inline
    auto
    join(const ratexpset<Context>& rs, const r& ws)
      -> ratexpset<context<labelset_t_of<Context>,
      join_t<weightset_t_of<Context>, r>>>
    {
      using ctx_t = context<labelset_t_of<Context>,
                            join_t<weightset_t_of<Context>, r>>;
      return {ctx_t{*rs.labelset(), join(*rs.weightset(), ws)},
          rs.identities()};
    }

    template <typename Context>
    inline
    auto
    join(const r& ws, const ratexpset<Context>& rs)
    // FIXME: loops if wrong order.
      -> join_t<ratexpset<Context>, r>
    {
      return join(rs, ws);
    }
  }
}//end of ns awali::stc

#include <awali/sttc/core/rat/ratexpset.hxx>

#endif // !AWALI_CORE_RAT_RATEXPSET_HH

#include <awali/sttc/algos/is_valid.hh>
