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

#include <awali/sttc/algos/daut.hh>
#include <awali/sttc/algos/dot.hh>
#include <awali/sttc/algos/fsm.hh>
#include <awali/sttc/algos/grail.hh>
#include <awali/sttc/algos/js_print.hh>
#include <awali/sttc/weightset/b.hh>
#include <awali/sttc/ctx/lal_char.hh>
#include <awali/sttc/ctx/lan_char.hh>
#include <awali/dyn/bridge_sttc/explicit_automaton.cc>

#include<set-types.hh>

namespace awali {

  template<typename Labelset, typename Weightset>
  struct Filter {
    template<typename Aut, typename OStream>
    static OStream& fado(const Aut&, OStream&) {
      throw std::domain_error("Fado output not supported for this automaton");
    }
    template<typename Aut, typename OStream>
    static OStream& grail(const Aut&, OStream&) {
      throw std::domain_error("Grail output not supported for this automaton");
    }
    template<typename Aut, typename OStream>
    static OStream& efsm(const Aut&, OStream&) {
      throw std::domain_error("Efsm output not supported for this automaton");
    }
  };

  template<typename Labelset>
  struct Filter<Labelset, sttc::b> {
    template<typename Aut, typename OStream>
    static OStream& fado(const Aut&, OStream&) {
      throw std::domain_error("Grail output not supported for this automaton");
    }
    template<typename Aut, typename OStream>
    static OStream& grail(const Aut&, OStream&) {
      throw std::domain_error("Grail output not supported for this automaton");
    }
    template<typename Aut, typename OStream>
    static OStream& efsm(const Aut&, OStream&) {
      throw std::domain_error("Efsm output not supported for this automaton");
    }
  };

  template<typename Weightset>
  struct Filter<sttc::ctx::lal_char, Weightset> {
    template<typename Aut, typename OStream>
    static OStream& fado(const Aut&, OStream&) {
      throw std::domain_error("Fado output not supported for this automaton");
    }
    template<typename Aut, typename OStream>
    static OStream& grail(const Aut&, OStream&) {
      throw std::domain_error("Grail output not supported for this automaton");
    }
    template<typename Aut, typename OStream>
    static OStream& efsm(const Aut& a, OStream& o) {
      return sttc::efsm(a, o);
    }
  };

  template<>
  struct Filter<sttc::ctx::lal_char, sttc::b> {
    template<typename Aut, typename OStream>
    static OStream& fado(const Aut& a, OStream& o) {
      return sttc::fado(a, o);
    }
    template<typename Aut, typename OStream>
    static OStream& grail(const Aut& a, OStream& o) {
      return sttc::grail(a, o);
    }
    template<typename Aut, typename OStream>
    static OStream& efsm(const Aut& a, OStream& o) {
      return sttc::efsm(a, o);
    }
  };

  template<>
  struct Filter<sttc::ctx::lan_char, sttc::b> {
    template<typename Aut, typename OStream>
    static OStream& fado(const Aut& a, OStream& o) {
      return sttc::fado(a, o);
    }
    template<typename Aut, typename OStream>
    static OStream& grail(const Aut& a, OStream& o) {
      return sttc::grail(a, o);
    }
    template<typename Aut, typename OStream>
    static OStream& efsm(const Aut& a, OStream& o) {
      return sttc::efsm(a, o);
    }
  };

//   extern "C" std::ostream& jsonput(dyn::automaton_t aut, std::ostream& out) {
//     auto a = dyn::get_stc_automaton<context_t>(aut);
//     return sttc::js_print(a, out);
//   }

  extern "C" std::ostream& dot(dyn::automaton_t aut, std::ostream& out, bool history, bool horizontal) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return sttc::dot(a, out, false, history, horizontal);
  }

//   extern "C" std::ostream& daut(dyn::automaton_t aut, std::ostream& out) {
//     auto a = dyn::get_stc_automaton<context_t>(aut);
//     return sttc::daut(a, out);
//   }

  extern "C" std::ostream& fado(dyn::automaton_t aut, std::ostream& out) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return Filter<labelset_t,weightset_t>::fado(a, out);
  }

  extern "C" std::ostream& grail(dyn::automaton_t aut, std::ostream& out) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return Filter<labelset_t,weightset_t>::grail(a, out);
  }

  extern "C" std::ostream& efsm(dyn::automaton_t aut, std::ostream& out) {
    auto a = dyn::get_stc_automaton<context_t>(aut);
    return Filter<labelset_t,weightset_t>::efsm(a, out);
  }

  
  extern "C" json_ast_t to_json_ast(dyn::automaton_t aut, 
                                     json_ast_t extra_metadata) 
  {
    auto sttc_aut = dyn::get_stc_automaton<context_t>(aut);
    return sttc::aut_to_ast(sttc_aut, extra_metadata);
  }
}


