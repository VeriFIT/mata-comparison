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

#ifndef AWALI_ALGOS_DERIVATION_HH
# define AWALI_ALGOS_DERIVATION_HH

#include <awali/sttc/algos/constant_term.hh>
#include <awali/sttc/algos/split.hh>
#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/core/rat/visitor.hh>
#include <awali/sttc/history/ratexp_history.hh>
#include <awali/sttc/ctx/fwd.hh>
#include <awali/sttc/misc/raise.hh>
#include <awali/sttc/weightset/polynomialset.hh>
#include <awali/sttc/labelset/traits.hh>
# include <stack>
# include <iostream>
# include <unordered_map>

# if DEBUG
#  define DEBUG_IFELSE(Then, Else) Then
# else
#  define DEBUG_IFELSE(Then, Else) Else
# endif

# define DEBUG_IF(Then) DEBUG_IFELSE(Then,)

namespace awali { namespace sttc {


  /*---------------------.
  | derivation(ratexp).  |
  `---------------------*/

  namespace rat
  {
    template <typename RatExpSet>
    class derivation_visitor
      : public RatExpSet::const_visitor
    {
    public:
      using ratexpset_t = RatExpSet;
      using context_t = context_t_of<ratexpset_t>;
      using labelset_t = labelset_t_of<context_t>;
      using label_t = label_t_of<context_t>;
      using ratexp_t = typename ratexpset_t::value_t;
      using weightset_t = weightset_t_of<ratexpset_t>;
      using weight_t = typename weightset_t::value_t;

      using polynomialset_t = ratexp_polynomialset_t<ratexpset_t>;
      using polynomial_t = typename polynomialset_t::value_t;

      using super_type = typename ratexpset_t::const_visitor;
      using node_t = typename super_type::node_t;

      constexpr static const char* me() { return "derivation"; }

      derivation_visitor(const ratexpset_t& rs)
        : rs_(rs)
      {}

      polynomial_t
      operator()(const ratexp_t& v, label_t var)
      {
        variable_ = var;
        v->accept(*this);
        return std::move(res_);
      }

      // FIXME: duplicate with expand.
      ratexp_t
      ratexp(const polynomial_t p)
      {
        ratexp_t res = rs_.zero();
        for (const auto& m: p)
          res = rs_.add(res, rs_.lmul(m.second, m.first));
         return res;
      }

      AWALI_RAT_UNSUPPORTED(ldiv)
      AWALI_RAT_UNSUPPORTED(transposition)

      AWALI_RAT_VISIT(zero, e)
      {
        res_ = ps_.zero();
        cst_ = ws_.zero();
      }

      AWALI_RAT_VISIT(one, e)
      {
        res_ = ps_.zero();
        cst_ = ws_.one();
      }

      AWALI_RAT_VISIT(atom, e)
      {
        if (e.value() == variable_)
          res_ = ps_.one();
        else
          res_ = ps_.zero();
        cst_ = ws_.zero();
      }

      AWALI_RAT_VISIT(sum, e)
      {
        polynomial_t res = ps_.zero();
        weight_t w = ws_.zero();
        for (const auto& v: e)
          {
            v->accept(*this);
            ps_.add_here(res, res_);
            w = ws_.add(w, cst_);
          }
        res_ = std::move(res);
        cst_ =w;
      }

      AWALI_RAT_VISIT(prod, e)
      {
        // We generate a sum.
        auto res = ps_.zero();
        // Accumulate the product of the constant terms of the
        // previous factors.
        weight_t w = ws_.one();
        for (unsigned i = 0, n = e.size(); i < n; ++i)
          {
            const auto& v = e[i];
            v->accept(*this);
            for (unsigned j = i + 1; j < n; ++j)
              res_ = ps_.rmul_letter(res_, e[j]);
            ps_.add_here(res, ps_.lmul(w, res_));
            w = ws_.mul(w, cst_);
          }
        res_ = std::move(res);
        cst_ = w;
      }
      
      AWALI_RAT_UNSUPPORTED(conjunction)
      AWALI_RAT_UNSUPPORTED(shuffle)
      AWALI_RAT_UNSUPPORTED(complement)

      AWALI_RAT_VISIT(star, e)
      {
        e.sub()->accept(*this);
        // We need a copy of e, but without its weights.
        cst_ = ws_.star(cst_);
        res_ = ps_.lmul(cst_,
                        ps_.rmul_letter(res_, e.shared_from_this()));
      }

      AWALI_RAT_VISIT(lweight, e)
      {
          e.sub()->accept(*this);
          res_ = ps_.lmul(e.weight(), res_);
          cst_ = ws_.mul( e.weight(), cst_);
      }
      
      AWALI_RAT_VISIT(rweight, e)
      {
        e.sub()->accept(*this);
        polynomial_t res;
        for (const auto& m: res_)
          ps_.add_here(res, rs_.rmul(m.first, e.weight()), m.second);
        res_ = res;
        cst_ = ws_.mul(cst_, e.weight());
      }

    private:
      ratexpset_t rs_;
      /// Shorthand to the weightset.
      weightset_t ws_ = *rs_.weightset();
      polynomialset_t ps_ = make_ratexp_polynomialset(rs_);
      /// The result.
      polynomial_t res_;
      /// The derivation variable.
      label_t variable_;
      weight_t cst_;
    };

  } // rat::

  /// Derive a ratexp wrt to a letter.
  template <typename RatExpSet>
  inline
  rat::ratexp_polynomial_t<RatExpSet>
  derivation(const RatExpSet& rs,
             const typename RatExpSet::ratexp_t& e,
             label_t_of<RatExpSet> a,
             bool breaking = false)
  {
    static_assert(RatExpSet::context_t::labelset_t::is_free(),
                  "requires free labelset");
    rat::derivation_visitor<RatExpSet> derivation{rs};
    auto res = derivation(e, a);
    if (breaking)
      res = split(rs, res);
    return res;
  }


  /// Derive a polynomial of ratexps wrt to a letter.
  template <typename RatExpSet>
  inline
  rat::ratexp_polynomial_t<RatExpSet>
  derivation(const RatExpSet& rs,
             const rat::ratexp_polynomial_t<RatExpSet>& p,
             label_t_of<RatExpSet> a,
             bool breaking = false)
  {
    auto ps = rat::make_ratexp_polynomialset(rs);
    using polynomial_t = rat::ratexp_polynomial_t<RatExpSet>;
    polynomial_t res;
    for (const auto& m: p)
      res = ps.add(res,
                   ps.lmul(m.second, derivation(rs, m.first, a, breaking)));
    return res;
  }


  /// Derive a ratexp wrt to a word.
  template <typename RatExpSet>
  inline
  rat::ratexp_polynomial_t<RatExpSet>
  derivation(const RatExpSet& rs,
             const typename RatExpSet::ratexp_t& e,
             const typename sttc::labelset_trait<typename RatExpSet::labelset_t>::wordset_t::word_t& word,
             bool breaking = false)
  {
    auto i = std::begin(word);
    auto end = std::end(word);
    require(i != end, "derivation: word cannot be empty");
    auto res = derivation(rs, e, *i, breaking);
    for (++i; i != end; ++i)
      res = derivation(rs, res, *i, breaking);
    return res;
  }

  /*-----------------------.
  | derived_term(ratexp).  |
  `-----------------------*/

  namespace internal
  {
    template <typename RatExpSet>
    struct derived_termer
    {
      using ratexpset_t = RatExpSet;
      using ratexp_t = typename ratexpset_t::value_t;

      using context_t = context_t_of<ratexpset_t>;
      using weightset_t = weightset_t_of<context_t>;

      using automaton_t = mutable_automaton<context_t>;

      /// Symbolic states: the derived terms are polynomials of ratexps.
      using polynomialset_t = rat::ratexp_polynomialset_t<ratexpset_t>;
      using polynomial_t = typename polynomialset_t::value_t;

      /// Symbolic states to state handlers.
      using smap = std::unordered_map<ratexp_t, state_t,
                                      utils::hash<ratexpset_t>,
                                      utils::equal_to<ratexpset_t>>;

      derived_termer(const ratexpset_t& rs, bool breaking = false, bool keep_history = true)
        : rs_(rs)
        , breaking_(breaking)        
        , res_{make_shared_ptr<automaton_t>(rs_.context())}
        , keep_history_(keep_history)
      {}

      /// The state for ratexp \a r.
      /// If this is a new state, schedule it for visit.
      state_t state(const ratexp_t& r)
      {
        // Benches show that the map_.emplace technique is slower, and
        // then that operator[] is faster than emplace.
        state_t res;
        auto i = map_.find(r);
        if (i == std::end(map_))
          {
            DEBUG_IF(std::cerr << "New state: "; rs_.print(r, std::cerr) << '\n'; );
            res = res_->add_state();
            map_[r] = res;
            todo_.push(r);
          }
        else
          res = i->second;
        return res;
      }

      automaton_t operator()(const ratexp_t& ratexp)
      {
        weightset_t ws = *rs_.weightset();
        // Turn the ratexp into a polynomial.
        {
          polynomial_t initial
            = breaking_ ? split(rs_, ratexp)
            : polynomial_t{{ratexp, ws.one()}};
          for (const auto& p: initial)
            // Also loads todo_.
            res_->set_initial(state(p.first), p.second);
        }

        const auto& ls = rs_.labelset()->genset();
        rat::derivation_visitor<RatExpSet> derivator{rs_};
        while (!todo_.empty())
          {
            ratexp_t src = todo_.top();
            auto s = map_[src];
            todo_.pop();
            res_->set_final(s, constant_term(rs_, src));
            for (auto l : ls) {
              auto der = derivator(src, l);
              if (breaking_)
                der = split(rs_, der);
 
              for (const auto& m: der)
                res_->add_transition(s, state(m.first), l, m.second);
            }
          }
          if(keep_history_) {
            auto history = std::make_shared<ratexp_history<ratexpset_t>>(rs_);
               res_->set_history(history);
               for (const auto& p: map_)
                {
                    history->add_state(p.second, p.first);
                 }
                  }
        return std::move(res_);
      }

    private:
      /// The ratexp's set.
      ratexpset_t rs_;
      /// Whether to perform a breaking derivation.
      bool breaking_ = false;
      //The map from rat exps into states
      smap map_;
      /// The resulting automaton.
      automaton_t res_;
      /// States to visit.
      std::stack<ratexp_t> todo_;

      bool keep_history_;
    };
  }

  /// Derive a ratexp wrt to a string.
  template <typename RatExpSet>
  inline
  mutable_automaton<typename RatExpSet::context_t>
  derived_term(const RatExpSet& rs, const typename RatExpSet::ratexp_t& r,
               bool breaking = false, bool keep_history = true)
  {
    internal::derived_termer<RatExpSet> dt{rs, breaking, keep_history};
    return dt(r);
  }


}}//end of ns awali::stc

#endif // !AWALI_ALGOS_DERIVATION_HH
