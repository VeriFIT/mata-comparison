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

#ifndef AWALI_ALGOS_TIKZ_HH
# define AWALI_ALGOS_TIKZ_HH

# include <algorithm>
# include <iostream>
# include <unordered_map>
# include <unordered_set>
# include <vector>

// FIXME: factor dot and tikz.
#include <awali/sttc/algos/grail.hh> // outputter
#include <awali/sttc/algos/dot.hh> // format_entry

namespace awali { namespace sttc {

  /*--------------------------.
  | tikz(automaton, stream).  |
  `--------------------------*/

  namespace internal
  {
    /// \brief Format automaton to TikZ format.
    ///
    /// \tparam Aut an automaton type, not a pointer type.
    template <typename Aut>
    class tikzer: public outputter<Aut>
    {
    public:
      using super_type = outputter<Aut>;
      using typename super_type::automaton_t;
      using typename super_type::state_t;
      using typename super_type::transition_t;
      using typename super_type::weight_t;

      using super_type::aut_;
      using super_type::format_entry_;
      using super_type::os_;
      using super_type::ws_;

      using super_type::super_type;

      /// Format a TikZ attribute.
      /// \param kind  the attribute name (e.g., "initial").
      /// \param w     the associated weight (e.g., initial weight).
      void format(const std::string& kind, const weight_t& w)
      {
        if (!ws_.is_zero(w))
          {
            os_ << ',' << kind;
            if (ws_.show_one() || !ws_.is_one(w))
              {
                os_ << ',' << kind << " text=$\\langle ";
                ws_.print(w, os_, "latex") << "\\rangle$";
              }
          }
      }

      void operator()()
      {
        os_ <<
          "\\documentclass{standalone}\n"
          "  \\usepackage{tikz}\n"
          "  \\usetikzlibrary{arrows, automata, positioning}\n"
          "  \\tikzstyle{automaton}=[shorten >=1pt, pos=.4,\n"
          "                         >=stealth', initial text=]\n"
          "  \\tikzstyle{accepting}=[accepting by arrow]\n"
          "\n"
          "\\begin{document}\n"
          "\\begin{tikzpicture}[automaton]\n"
          ;

        for (auto s : aut_->states())
          {
            os_ << "  \\node[state";
            format("initial", aut_->get_initial_weight(s));
            format("accepting", aut_->get_final_weight(s));
            os_ << "] (";
            aut_->print_state(s, os_);
            os_ << ')';
            if (2 < s)
              {
                os_ << " [right=of ";
                aut_->print_state(s - 1, os_);
                os_ << ']';
              }
            os_ << " {$";
            aut_->print_state(s, os_);
            os_ << "$};\n";
        }

        for (auto src : aut_->states())
          {
            std::set<state_t> ds;
            for (auto t: aut_->out(src))
              ds.insert(aut_->dst_of(t));
            for (auto dst: ds)
              {
                os_ << "  \\path[->] (";
                aut_->print_state(src, os_);
                os_ << ')'
                    << " edge"
                    << (src == dst ? "[loop above]" : "")
                    << " node[above]"
                    << " {$" << format_entry_(src, dst, "latex") << "$}"
                    << " (";
                aut_->print_state(dst, os_);
                os_ << ");\n";
              }
          }
        os_ <<
          "\\end{tikzpicture}\n"
          "\\end{document}";
      }
    };
  }

  /// \brief Format automaton to TikZ format.
  ///
  /// \tparam AutPtr an automaton type.
  template <typename AutPtr>
  std::ostream&
  tikz(const AutPtr& aut, std::ostream& out)
  {
    internal::tikzer<AutPtr> t{aut, out};
    t();
    return out;
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_TIKZ_HH
