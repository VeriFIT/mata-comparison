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

#ifndef AWALI_ALGOS_INFO_HH
# define AWALI_ALGOS_INFO_HH

# include <iostream>

#include <awali/sttc/algos/accessible.hh>
#include <awali/sttc/algos/is_ambiguous.hh>
#include <awali/sttc/algos/is_complete.hh>
#include <awali/sttc/algos/determinize.hh>
#include <awali/sttc/algos/is_eps_acyclic.hh>
#include <awali/sttc/algos/is_normalized.hh>
#include <awali/sttc/algos/is_valid.hh>
#include <awali/sttc/algos/standard.hh>
#include <awali/sttc/algos/synchronizing_word.hh>
#include <awali/sttc/core/rat/info.hh>
#include <awali/sttc/core/rat/size.hh>

namespace awali { namespace sttc {


  namespace detail_info
  {
    /*---------------.
    | is-ambiguous.  |
    `---------------*/
    template <typename Aut>
    typename std::enable_if<labelset_t_of<Aut>::is_free(),
                            bool>::type
    is_ambiguous(const Aut& a)
    {
      return sttc::is_ambiguous(a);
    }

    template <typename Aut>
    typename std::enable_if<!labelset_t_of<Aut>::is_free(),
                            std::string>::type
    is_ambiguous(const Aut&)
    {
      return "N/A";
    }

    /*--------------.
    | is-complete.  |
    `--------------*/
    template <typename Aut>
    typename std::enable_if<labelset_t_of<Aut>::is_free(),
                            bool>::type
    is_complete(const Aut& a)
    {
      return sttc::is_complete(a);
    }

    template <typename Aut>
    typename std::enable_if<!labelset_t_of<Aut>::is_free(),
                            std::string>::type
    is_complete(const Aut&)
    {
      return "N/A";
    }

    /*-------------------.
    | is_deterministic.  |
    `-------------------*/
    template <typename Aut>
    typename std::enable_if<labelset_t_of<Aut>::is_free(),
                            bool>::type
    is_deterministic(const Aut& a)
    {
      return sttc::is_deterministic(a);
    }

    template <typename Aut>
    typename std::enable_if<!labelset_t_of<Aut>::is_free(),
                            std::string>::type
    is_deterministic(const Aut&)
    {
      return "N/A";
    }

    /*-------------------.
    | is_synchronizing.  |
    `-------------------*/
    template <typename Aut>
    typename std::enable_if<labelset_t_of<Aut>::is_free(),
                            bool>::type
    is_synchronizing(const Aut& a)
    {
      return sttc::is_synchronizing(a);
    }

    template <typename Aut>
    typename std::enable_if<!labelset_t_of<Aut>::is_free(),
                            std::string>::type
    is_synchronizing(const Aut&)
    {
      return "N/A";
    }

    /*---------------------------.
    | num_deterministic_states.  |
    `---------------------------*/
    template <typename Aut>
    typename std::enable_if<labelset_t_of<Aut>::is_free(),
                            size_t>::type
    num_deterministic_states(const Aut& a)
    {
      return sttc::num_deterministic_states(a);
    }

    template <typename Aut>
    typename std::enable_if<!labelset_t_of<Aut>::is_free(),
                            std::string>::type
    num_deterministic_states(const Aut&)
    {
      return "N/A";
    }

    /*----------------------.
    | num_eps_transitions.  |
    `----------------------*/

    template <typename Aut>
    ATTRIBUTE_CONST
    typename std::enable_if<!labelset_t_of<Aut>::has_one(),
                            size_t>::type
    num_eps_transitions_(const Aut&)
    {
      return 0;
    }

    template <typename Aut>
    typename std::enable_if<labelset_t_of<Aut>::has_one(),
                            size_t>::type
    num_eps_transitions_(const Aut& aut)
    {
      size_t res = 0;
      for (auto t : aut->transitions())
        res += aut->labelset()->is_one(aut->label_of(t));
      return res;
    }

    template <typename Aut>
    size_t
    num_eps_transitions(const Aut& aut)
    {
      return num_eps_transitions_(aut);
    }
  }

  /*--------------------------.
  | info(automaton, stream).  |
  `--------------------------*/

  template <class A>
  std::ostream&
  info(const A& aut, std::ostream& out, bool detailed = false)
  {
#define ECHO(Name, Value)                               \
    out << Name ": " << Value << '\n'
    ECHO("type", aut->vname(true));
    ECHO("number of states", aut->num_states());
    ECHO("number of initial states", aut->num_initials());
    ECHO("number of final states", aut->num_finals());
    ECHO("number of accessible states", num_accessible_states(aut));
    ECHO("number of coaccessible states", num_coaccessible_states(aut));
    ECHO("number of useful states", num_useful_states(aut));
    ECHO("number of transitions", aut->num_transitions());
    ECHO("number of deterministic states",
         detail_info::num_deterministic_states(aut));
    ECHO("number of eps transitions", detail_info::num_eps_transitions(aut));
    if (detailed)
      ECHO("is ambiguous", detail_info::is_ambiguous(aut));
    ECHO("is complete", detail_info::is_complete(aut));
    ECHO("is deterministic", detail_info::is_deterministic(aut));
    ECHO("is empty", is_empty(aut));
    ECHO("is eps-acyclic", is_eps_acyclic(aut));
    ECHO("is normalized", is_normalized(aut));
    ECHO("is proper", is_proper(aut));
    ECHO("is standard", is_standard(aut));
    if (detailed)
      ECHO("is synchronizing", detail_info::is_synchronizing(aut));
    ECHO("is trim", is_trim(aut));
    ECHO("is useless", is_useless(aut));
#undef ECHO
    // No eol for the last one.
    out << "is valid: " << is_valid(aut);
    return out;
  }

  /*-----------------------.
  | info(ratexp, stream).  |
  `-----------------------*/

  template <class RatExpSet>
  void
  info(const RatExpSet& rs, const typename RatExpSet::ratexp_t& e,
       std::ostream& o)
  {
    rat::size<RatExpSet> sizer;
    sttc::rat::info<RatExpSet> nfo;
    nfo(*e);

# define DEFINE(Type)                            \
    << "\n" #Type ": " << nfo.Type
    o
      << "type: " << rs.vname(true)
      << "\nsize: " << sizer(e)
      DEFINE(sum)
      DEFINE(shuffle)
      DEFINE(conjunction)
      DEFINE(prod)
      DEFINE(star)
      DEFINE(maybe)
      DEFINE(plus)
      DEFINE(complement)
      DEFINE(zero)
      DEFINE(one)
      DEFINE(atom)
      DEFINE(lweight)
      DEFINE(rweight)
      ;
# undef DEFINE
  }

}}//end of ns awali::stc

#endif // !AWALI_ALGOS_INFO_HH
