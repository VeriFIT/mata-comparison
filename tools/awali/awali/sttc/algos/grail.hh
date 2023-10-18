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

#ifndef AWALI_ALGOS_GRAIL_HH
# define AWALI_ALGOS_GRAIL_HH

# include <iostream>
# include <map>

#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/misc/escape.hh>
#include <awali/sttc/weightset/fwd.hh> // b
#include <awali/sttc/weightset/polynomialset.hh>

namespace awali { namespace sttc {


  namespace internal
  {

    /*------------.
    | outputter.  |
    `------------*/

    /// \brief Factor common bits in automaton formatting.
    ///
    /// \tparam Aut an automaton type.
    template <typename Aut>
    class outputter
    {
    protected:
      using automaton_t = Aut;

    public:
      outputter(const automaton_t& aut, std::ostream& out)
        : aut_(aut)
        , os_(out)
      {}

      virtual ~outputter() = default;

      // Should not be public, but needed by GCC 4.8.1.
      // http://gcc.gnu.org/bugzilla/show_bug.cgi?id=58972

    protected:
      using context_t = context_t_of<automaton_t>;
      using label_t = label_t_of<automaton_t>;
      using weightset_t = weightset_t_of<automaton_t>;
      using weight_t = weight_t_of<automaton_t>;

      /// A list of states.
      using states_t = std::vector<state_t>;

      /// Convert a label to its representation.
      virtual std::string
      label_(const label_t& l)
      {
        return ls_.is_one(l) ? "@epsilon" : format(ls_, l);
      }

      /// Output the transition \a t.  Do not insert eol.
      /// "Src Label Dst".
      virtual void output_transition_(transition_t t)
      {
        aut_->print_state(aut_->src_of(t), os_);
        os_ << ' ' << label_(aut_->label_of(t)) << ' ';
        aut_->print_state(aut_->dst_of(t), os_);
      }

      /// The labels and weights of transitions from \a src to \a dst.
      ///
      /// The main advantage of using polynomials instead of directly
      /// iterating over aut_->outin(src, dst) is to get a result which
      /// is sorted (hence more deterministic).
      std::string format_entry_(state_t src, state_t dst,
                                const std::string& fmt = "text")
      {
        auto entry = get_entry(aut_, src, dst);
        return ps_.format(entry, ", ", fmt);
      }

      /// Output transitions, sorted lexicographically on (Label, Dest).
      void output_state_(const state_t s)
      {
        std::vector<transition_t> ts;
        for (auto t : aut_->out(s))
          ts.push_back(t);
        std::sort
          (begin(ts), end(ts),
           [this](transition_t l, transition_t r)
           {
             return (std::forward_as_tuple(aut_->label_of(l), aut_->dst_of(l))
                     < std::forward_as_tuple(aut_->label_of(r), aut_->dst_of(r)));
           });
        for (auto t : ts)
          {
            os_ << '\n';
            output_transition_(t);
          }
      }

      /// Output transitions, sorted lexicographically.
      /// "Src Label Dst\n".
      void output_transitions_()
      {
        for (auto s: aut_->states())
          output_state_(s);
      }

      /// List names of states in \a ss, preceded by ' '.
      void list_states_(const states_t& ss)
      {
        for (auto s: ss)
          {
            os_ << ' ';
            aut_->print_state(s, os_);
          }
      }

      /// The list of initial states, sorted.
      states_t initials_()
      {
        states_t res;
        for (auto t: aut_->initial_transitions())
          res.emplace_back(aut_->dst_of(t));
        std::sort(begin(res), end(res));
        return res;
      }

      /// The list of final states, sorted.
      states_t finals_()
      {
        states_t res;
        for (auto t: aut_->final_transitions())
          res.emplace_back(aut_->src_of(t));
        std::sort(begin(res), end(res));
        return res;
      }

      /// The automaton we have to output.
      const automaton_t& aut_;
      /// Output stream.
      std::ostream& os_;
      /// Short-hand to the labelset.
      const labelset_t_of<automaton_t>& ls_ = *aut_->labelset();
      /// Short-hand to the weightset.
      const weightset_t& ws_ = *aut_->weightset();
      /// Short-hand to the polynomialset used to print the entries.
      const polynomialset<context_t_of<automaton_t>> ps_{aut_->context()};
    };

  }


  /*---------.
  | fadoer.  |
  `---------*/

  namespace internal
  {

    /// \brief Format an automaton into Fado.
    ///
    /// \tparam Aut an automaton type, not a pointer type.
    template <typename Aut>
    class fadoer: public outputter<Aut>
    {
      static_assert(context_t_of<Aut>::is_lal || context_t_of<Aut>::is_lan,
                    "requires labels_are_(letters|nullable)");
      // FIXME: Not right: F2 is also using bool, but it is not bool.
      static_assert(std::is_same<weightset_t_of<Aut>, b>::value,
                    "requires Boolean weights");

      using super_type = outputter<Aut>;

      using super_type::aut_;
      using super_type::finals_;
      using super_type::initials_;
      using super_type::list_states_;
      using super_type::os_;
      using super_type::output_transitions_;
      using super_type::ws_;

      using super_type::super_type;

    public:
      /// Actually output \a aut_ on \a os_.
      // http://www.dcc.fc.up.pt/~rvr/FAdoDoc/_modules/fa.html#saveToFile
      void operator()()
      {
        bool is_deter = is_deterministic_(aut_);
        os_ << (is_deter ? "@DFA" : "@NFA");
        list_states_(finals_());
        if (!is_deter)
          {
            os_ << " *";
            list_states_(initials_());
          }
        output_transitions_();
      }

    private:
      template <typename A>
      typename std::enable_if<labelset_t_of<A>::is_free(),
                              bool>::type
      is_deterministic_(const A& a)
      {
        return is_deterministic(a);
      }

      template <typename A>
      typename std::enable_if<!labelset_t_of<A>::is_free(),
                              bool>::type
      is_deterministic_(const A&)
      {
        return false;
      }
    };

  }

  template <typename Aut>
  std::ostream&
  fado(const Aut& aut, std::ostream& out)
  {
    internal::fadoer<Aut> fado{aut, out};
    fado();
    return out;
  }

  /*----------.
  | grailer.  |
  `----------*/

  namespace internal
  {
    /// \brief Format an automaton into Fado.
    ///
    /// \tparam Aut an automaton type, not a pointer type.
    ///
    /// See https://cs.uwaterloo.ca/research/tr/1993/01/93-01.pdf.
    template <typename Aut>
    class grailer: public outputter<Aut>
    {
      static_assert(context_t_of<Aut>::is_lal || context_t_of<Aut>::is_lan,
                    "requires labels_are_(letters|nullable)");
      // FIXME: Not right: F2 is also using bool, but it is not bool.
      static_assert(std::is_same<weightset_t_of<Aut>, b>::value,
                    "requires Boolean weights");

      using super_type = outputter<Aut>;

      using typename super_type::automaton_t;

      using super_type::aut_;
      using super_type::finals_;
      using super_type::initials_;
      using super_type::os_;
      using super_type::output_transitions_;
      using super_type::ws_;

      using super_type::super_type;

    public:
      /// Actually output \a aut_ on \a os_.
      void operator()()
      {
        // Don't end with a \n.
        const char* sep = "";
        for (auto s: initials_())
          {
            os_ << sep
                << "(START) |- ";
            aut_->print_state(s, os_);
            sep = "\n";
          }
        output_transitions_();
        for (auto s: finals_())
          {
            os_ << '\n';
            aut_->print_state(s, os_) <<  " -| (FINAL)";
          }
      }
    };
  }

  template <typename Aut>
  std::ostream&
  grail(const Aut& aut, std::ostream& out)
  {
    internal::grailer<Aut> grail{aut, out};
    grail();
    return out;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_GRAIL_HH
