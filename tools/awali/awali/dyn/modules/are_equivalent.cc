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

#ifndef DYN_MODULES_AREEQUIVALENT_CC
#define DYN_MODULES_AREEQUIVALENT_CC

#include <awali/dyn/modules/are_equivalent.hh>
#include <awali/dyn/modules/promotion.hh>
#include <awali/dyn/loading/handler.hh>
#include <awali/dyn/algos/sys.hh>

namespace awali {
  namespace dyn {

    bool
    are_equivalent (automaton_t aut1, automaton_t aut2)
    {
      if (aut1->get_context()->weightset_name() 
          != aut2->get_context()->weightset_name()) {
        *warning_stream << "Two automata with different weightset are never "
                        << "equivalent: "
                        << aut1->get_context()->weightset_name()
                        << " != "
                        << aut2->get_context()->weightset_name()
                        << " ." << std::endl;
        return false;
      }
      std::string ls1 = aut1->get_context()->labelset_name();
      std::string ls2 = aut2->get_context()->labelset_name();
      if (ls1 != ls2) 
        if ((ls1 != "lan<"+ls2+">") && (ls2 != "lan<"+ls1+">")) {
          *warning_stream << "The two automata have incompatible labelset, "
                          << "hence they cannot be equivalent: "
                          << "equivalent: "
                          << aut1->get_context()->labelset_name()
                          << " and "
                          << aut2->get_context()->labelset_name()
                          << " ." << std::endl;
          return false;
        }
      if (aut1->is_transducer() || aut2->is_transducer())
        throw std::domain_error("Function are_equivalent is not supported "
                                    "for transducers");
      if (aut1->get_context()->weightset_name()=="N" ||
	  aut1->get_context()->weightset_name()=="Z") {
	aut1=promote_automaton(aut1,"Q"); 
	aut2=promote_automaton(aut2,"Q"); 
      }
      return loading::call2<bool>("are_equivalent", "are_equivalent", aut1, 
                                  aut2);
    }

  }
}//end of ns awali::dyn

#endif
