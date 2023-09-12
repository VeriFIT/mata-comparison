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

#ifndef AWALI_ALGOS_DOT_HH
# define AWALI_ALGOS_DOT_HH

# include <algorithm>
# include <iostream>

#include <awali/sttc/algos/grail.hh>
#include <awali/sttc/algos/accessible.hh> // useful_states
#include <awali/sttc/misc/escape.hh>
#include <awali/sttc/misc/set.hh>

namespace awali { namespace sttc {


  namespace internal
  {
    /*-------------------------.
    | dot(automaton, stream).  |
    `-------------------------*/

    /// \brief Format an automaton into Dot.
    ///
    /// \tparam Aut an automaton type.
    template <typename Aut>
    class dotter: public outputter<Aut>
    {
    private:
      using super_type = outputter<Aut>;
      using typename super_type::automaton_t;
      using typename super_type::weightset_t;
      using typename super_type::weight_t;

      using super_type::aut_;
      using super_type::finals_;
      using super_type::format_entry_;
      using super_type::initials_;
      using super_type::os_;
      using super_type::ws_;

      using super_type::super_type;

      // Dot, by default, uses the X11 color naming scheme, whose "gray"
      // is really light (it looks almost blue in some cases).
      const char* gray = "color = DimGray";

    public:
      dotter(const automaton_t& aut, std::ostream& out,
             bool dot2tex, bool print_history, bool horizontal=true)
        : super_type(aut, out)
        , dot2tex_(dot2tex)
        , print_history_(print_history)
        , horizontal_(horizontal)
      {}

      /// Format a TikZ attribute.
      /// \param sep   the separator to print before (if we print something).
      /// \param kind  the attribute name (e.g., "initial").
      /// \param w     the associated weight (e.g., initial weight).
      bool format(const std::string& sep,
                  const std::string& kind, const weight_t& w)
      {
        if (ws_.is_zero(w))
          return false;
        else
          {
            os_ << sep << kind;
            if (ws_.show_one() || !ws_.is_one(w))
              {
                os_ << ", " << kind << " text={";
                ws_.print(w, os_) << '}';
              }
            return true;
          }
      }

      /// Pretty-print state \a s for both dot and dot2tex.
      void
      print_state_(state_t s)
      {
        os_ << s;
        if (dot2tex_)
          {
            os_ << " [";
            std::string style;
            std::string sep;
            std::string close;
            // I hate this piece of code.  There must be means to be
            // better looking...
            os_ << "label = \"";
            aut_->print_state_name(s, os_, "latex");
            static bool debug = getenv("AWALI_DEBUG");
            if (debug)
              os_ << " (" << s << ')';
            os_ << "\", style = \"named";
            sep = ", ";
            close = "\"";
            if (format(sep, "initial", aut_->get_initial_weight(s)))
              {
                sep = ", ";
                close = "\"";
              }
            if (format(sep, "accepting", aut_->get_final_weight(s)))
              close = "\"";
            os_ << close << ']';
          }
        else
          {
            // Dot format.
            os_ << " [label = \"";
            if(print_history_ && aut_->has_history(s))
                aut_->print_state_history(s, os_);
            else
              aut_->print_state_name(s, os_, "text");
            static bool debug = getenv("AWALI_DEBUG");
            if (debug)
              os_ << " (" << s << ')';
            os_ << "\", shape = box, style = rounded]";
          }
      }

      std::ostream& operator()()
      {
        auto useful = useful_states(aut_, true);

        os_ <<
          "digraph\n"
          "{\n"
          "  vcsn_context = \"" << aut_->context().vname() << "\"\n";
        if(horizontal_)
          os_ << "  rankdir = LR\n";

        if (dot2tex_)
          os_ <<
            "  d2toptions = \"--format tikz --tikzedgelabels --graphstyle=automaton --crop --nominsize --autosize\"\n"
            "  d2tdocpreamble = \""
            "    \\usepackage{amssymb}"
            "    \\usetikzlibrary{arrows, automata}"
            "    \\tikzstyle{automaton}=[shorten >=1pt, pos=.4, >=stealth', initial text=]"
            "    \\tikzstyle{named}=[rectangle, rounded corners]"
            "    \\tikzstyle{initial}=[initial by arrow]"
            "    \\tikzstyle{accepting}=[accepting by arrow]"
            "  \"\n";
        else
          {
            // Output the pre-initial and post-final states.
            if (!aut_->initial_transitions().empty()
                || !aut_->final_transitions().empty())
              {
                os_ <<
                  "  {\n"
                  "    node [shape = point, width = 0]\n";
                for (auto s : initials_())
                  {
                    os_ << "    I" << s;
                    os_ << '\n';
                  }
                for (auto s : finals_())
                  {
                    os_ << "    F" << s;
                    os_ << '\n';
                  }
                os_ << "  }\n";
              }
          }

        // Output all the states to make "print | read" idempotent.
        //
        // Put the useless ones in gray.  This does not work:
        //
        // { 0 1 2 }
        // { node [color = gray] 2 }
        //
        // because 2 was already "declared", and dot does not associate
        // "color = gray" to it.
        if (!aut_->states().empty())
          {
            os_ << "  {\n"
                << "    node ["
                << (dot2tex_
                    ? "texmode = math, style = state"
                    : "shape = circle")
                << "]\n";
            for (auto s : aut_->states())
              {
                os_ << "    ";
                print_state_(s);
                if (useful.find(s)==useful.end())
                  os_ << " [" << gray << ']';
                os_ << '\n';
              }
            os_ << "  }\n";
          }
        if (dot2tex_)
          os_ << "  edge [texmode = math, lblstyle = auto]\n";
        for (auto src : dot2tex_ ? aut_->states() : aut_->all_states())
          {
            // Sort by destination state.
            std::set<state_t> ds;
            if (dot2tex_)
              for (auto t: aut_->out(src))
                ds.insert(aut_->dst_of(t));
            else
              for (auto t: aut_->all_out(src))
                ds.insert(aut_->dst_of(t));
            for (auto dst: ds)
              {
                os_ << "  ";
                if (src == aut_->pre())
                  {
                    os_ << 'I' << dst;
                    os_ << " -> " << dst;
                  }
                else if (dst == aut_->post())
                  {
                    os_ << src << " -> ";
                    os_ << 'F' << src;
                  }
                else
                  {
                    os_ << src << " -> " << dst;
                  }
                std::string s = format_entry_(src, dst,
                                              dot2tex_ ? "latex" : "text");
                bool useless = useful.find(src)==useful.end()
                  || useful.find(dst)==useful.end();
                if (!s.empty() || useless)
                  {
                    os_ << " [";
                    const char* sep = "";
                    if (!s.empty())
                      {
                        os_ << "label = \"" << str_escape(s) << "\"";
                        sep = ", ";
                      }
                    if (useless)
                      os_ << sep << gray;
                    os_ << ']';
                  }
                os_ << '\n';
              }
          }
        return os_ << '}';
      }

      /// Whether to format for dot2tex.
      bool dot2tex_ = false;
      bool print_history_ = false;
      bool horizontal_ = true;
    };
  }

  template <typename Aut>
  std::ostream&
  dot(const Aut& aut, std::ostream& out, bool dot2tex = false, bool keep_history = true, bool horizontal = true)
  {
    internal::dotter<Aut> dot(aut, out, dot2tex, keep_history, horizontal);
    return dot();
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_DOT_HH
