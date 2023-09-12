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

#ifndef AWALI_ALGOS_COPY_HH
# define AWALI_ALGOS_COPY_HH

# include <unordered_map>

#include <awali/sttc/core/mutable_automaton.hh>
#include <awali/sttc/core/rat/copy.hh>
#include <awali/sttc/misc/attributes.hh>
#include <awali/sttc/misc/set.hh>
#include <awali/sttc/history/single_history.hh>

namespace awali { namespace sttc {


  /*------------------.
  | copy(automaton).  |
  `------------------*/

  namespace internal
  {
    /// Copy an automaton.
    /// \pre AutIn <: AutOut.
    template <typename AutIn, typename AutOut, typename InOutMap=std::unordered_map<state_t, state_t>>
    struct copier
    {
      copier(const AutIn& in, AutOut& out)
        : in_(in)
        , out_(out)
      {}

      template <typename Pred>
      void operator()(Pred keep_state, bool transpose=false)
      {
        // Copy the states.  We cannot iterate on the transitions
        // only, as we would lose the states without transitions.
        out_state[in_->pre()] = out_->pre();
        out_state[in_->post()] = out_->post();
        for (auto s: in_->states())
          if (keep_state(s))
            out_state[s] = out_->add_state();

        for (auto t : in_->all_transitions())
          {
            auto src = out_state.find(in_->src_of(t));
            auto dst = out_state.find(in_->dst_of(t));
            if (src != out_state.end() && dst != out_state.end())
              out_->new_transition_copy(src->second, dst->second,
                                        in_, t, transpose);
          }
      }

      template <typename Pred>
      void no_weight(Pred keep_state)
      {
        // Copy the states.  We cannot iterate on the transitions
        // only, as we would lose the states without transitions.
        out_state[in_->pre()] = out_->pre();
        out_state[in_->post()] = out_->post();
        for (auto s: in_->states())
          if (keep_state(s))
            out_state[s] = out_->add_state();

        for (auto t : in_->all_transitions())
          {
            auto src = out_state.find(in_->src_of(t));
            auto dst = out_state.find(in_->dst_of(t));
            if (src != out_state.end() && dst != out_state.end())
              out_->new_transition(src->second, dst->second,
                                   in_->label_of(t));
          }
      }

      /// A map from result state to original state.
      using origins_t = std::map<state_t, state_t>;
      origins_t
      origins() const
      {
        origins_t res;
        for (const auto& p: out_state)
          res[p.second] = p.first;
        return res;
      }

      /// Print the origins.
      static
      std::ostream&
      print(const origins_t& orig, std::ostream& o)
      {
        o << "/* Origins.\n"
          << "    node [shape = box, style = rounded]\n";
        for (auto p : orig)
          if (2 <= p.first)
            o << "    " << p.first - 2
              << " [label = \"" << p.second - 2 << "\"]\n";
        o << "*/\n";
        return o;
      }

      void set_history() {
        auto history = std::make_shared<single_history<AutIn>>(in_);
        out_->set_history(history);
        for (auto p: in_->all_states())
            history->add_state(out_state[p], p);
      }

      const InOutMap& in_out_map() const {
        return out_state;
      }
      
      /// Input automaton.
      const AutIn& in_;
      /// Output automaton.
      AutOut& out_;
      /// input state -> output state.
      InOutMap out_state;
    };
  }

  /// Copy an automaton.
  /// \pre AutIn <: AutOut.
  template <typename AutIn, typename AutOut, typename Pred>
  inline
  void
  copy_into(const AutIn& in, AutOut& out, Pred keep_state, bool keep_history=true, bool transpose=false)
  {
    internal::copier<AutIn, AutOut> copy(in, out);
    copy(keep_state, transpose);
    if(keep_history)
      copy.set_history();
  }

  template <typename AutIn, typename AutOut>
  inline
  void
  copy_into(const AutIn& in, AutOut& out, bool keep_history=true, bool transpose=false)
  {
    copy_into(in, out, [](state_t) { return true; }, keep_history, transpose);
  }

  template <typename AutIn, typename AutOut, typename Pred>
  inline
  void
  copy_support(const AutIn& in, AutOut& out, Pred keep_state, bool keep_history=true)
  {
    internal::copier<AutIn, AutOut> copy(in, out);
    copy.no_weight(keep_state);
    if(keep_history)
      copy.set_history();
  }

  template <typename AutIn, typename AutOut>
  inline
  void
  copy_support(const AutIn& in, AutOut& out, bool keep_history=true)
  {
    internal::copier<AutIn, AutOut> copy(in, out);
    copy.no_weight([](state_t) { return true; });
    if(keep_history)
      copy.set_history();
  }

  /// A copy of \a input keeping only its states that are accepted by
  /// \a keep_state.
  template <typename AutIn,
            typename AutOut = typename AutIn::element_type::automaton_nocv_t,
            typename Pred>
  inline
  AutOut
  copy(const AutIn& input, Pred keep_state, bool keep_history=true, bool transpose=false)
  {
    // FIXME: here, we need a means to convert the given input context
    // (e.g. letter -> B) into the destination one (e.g., letter ->
    // Q).  The automaton constructor wants the exact context type.
    auto res = make_mutable_automaton(input->context());
    sttc::copy_into(input, res, keep_state, keep_history, transpose);
    res->set_name(input->get_name());
    res->set_desc(input->get_desc());
    return res;
  }

  /// A copy of \a input.
  template <typename AutIn,
            typename AutOut = typename AutIn::element_type::automaton_nocv_t>
  inline
  AutOut
  copy(const AutIn& input, bool keep_history=true, bool transpose=false)
  {
    return sttc::copy<AutIn, AutOut>(input,
                                        [](state_t) { return true; }, keep_history, transpose);
  }

  /// A copy of \a input keeping only its states that are members of
  /// \a keep.
  template <typename AutIn,
            typename AutOut = typename AutIn::element_type::automaton_nocv_t>
  inline
  AutOut
  copy(const AutIn& input, const std::set<state_t>& keep, bool keep_history=true, bool transpose=false)
  {
    return sttc::copy<AutIn, AutOut>
      (input,
       [&keep](state_t s) { return keep.find(s)!=keep.end(); }, keep_history, transpose);
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_COPY_HH
