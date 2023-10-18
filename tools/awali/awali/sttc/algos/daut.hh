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

#ifndef AWALI_ALGOS_DAUT_HH
# define AWALI_ALGOS_DAUT_HH

# include <algorithm>
# include <iostream>

#include <awali/sttc/algos/grail.hh>
#include <awali/sttc/misc/escape.hh>
#include <awali/sttc/misc/set.hh>

namespace awali { namespace sttc {

  namespace internal
  {
    /*-------------------------.
    | daut(automaton, stream).  |
    `-------------------------*/

    /// \brief Format an automaton into daut vcsn format.
    ///
    /// \tparam Aut an automaton type.
    template <typename Aut>
    class dautter: public outputter<Aut>
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

    public:
      dautter(const automaton_t& aut, std::ostream& out)
        : super_type(aut, out)
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

      void
      print_state_(state_t s)
      {
        os_ << (s-2);
        os_ << " [label = \"";
        aut_->print_state_name(s, os_, "text");
        os_ << "\"]";
      }

      std::ostream& operator()() {
        os_ <<
          "context = \"" << aut_->context().vname() << "\"\n";
        if (!aut_->states().empty()) {
          os_ << "{\n";
          for (auto s : aut_->states())
            {
              os_ << "  ";
              print_state_(s);
              os_ << '\n';
            }
          os_ << "}\n";
        }
        for (auto src : aut_->all_states()) {
          // Sort by destination state.
          std::set<state_t> ds;
          for (auto t: aut_->all_out(src))
            ds.insert(aut_->dst_of(t));
          for (auto dst: ds) {
            if (src == aut_->pre())
              os_ << '$' << " -> " << (dst-2);
            else if (dst == aut_->post())
              os_ << (src-2) << " -> " << '$';
            else
              os_ << (src-2) << " -> " << (dst-2);
            std::string s = format_entry_(src, dst, "text");
            if (!s.empty()) {
              os_ << " [";
              os_ << "label = \"" << str_escape(s) << "\"";
              os_ << ']';
            }
            os_ << '\n';
          }
        }
        return os_;
      }
    };
  }
    /** Output in 'daut' format
     *
     * An extension of dot for automata
     */
  template <typename Aut>
  std::ostream&
  daut(const Aut& aut, std::ostream& out)
  {
    internal::dautter<Aut> daut(aut, out);
    return daut();
  }
}}//end of ns awali::stc

#endif // !AWALI_ALGOS_DAUT_HH
