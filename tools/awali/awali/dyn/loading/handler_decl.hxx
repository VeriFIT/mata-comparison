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

#include <set>
#include <awali/dyn/core/context_description.hh>
#include <awali/dyn/core/abstract_ratexp.hh>

#ifndef HANDLER_EXTERN
#define HANDLER_EXTERN 
#define UNDEF_ON_EXIT
#endif


#define INSTANCIATE_call_0_1(r,a1) HANDLER_EXTERN template r call0<r,a1>(std::string,std::string, std::string, a1)

#define INSTANCIATE_call_0_2(r,a1,a2) HANDLER_EXTERN template r call0<r,a1,a2>(std::string,std::string, std::string, a1, a2)

#define INSTANCIATE_call_0_3(r,a1,a2,a3) HANDLER_EXTERN template r call0<r,a1,a2,a3>(std::string,std::string, std::string, a1, a2)


#define INSTANCIATE_call_1_0(r,a1) HANDLER_EXTERN template r call1<r,a1>(std::string,std::string, a1)

#define INSTANCIATE_call_1_1(r,a1,a2) HANDLER_EXTERN template r call1<r,a1,a2>(std::string,std::string, a1, a2)

#define INSTANCIATE_call_1_2(r,a1,a2,a3) HANDLER_EXTERN template r call1<r,a1,a2,a3>(std::string,std::string, a1, a2, a3)

#define INSTANCIATE_call_1_3(r,a1,a2,a3,a4) HANDLER_EXTERN template r call1<r,a1,a2,a3,a4>(std::string,std::string, a1, a2, a3, a4)

#define INSTANCIATE_call_1_4(r,a1,a2,a3,a4,a5) HANDLER_EXTERN template r call1<r,a1,a2,a3,a4,a5>(std::string,std::string, a1, a2, a3, a4, a5)


#define INSTANCIATE_call_2_0(r,a1,a2) HANDLER_EXTERN template r call2<r,a1,a2>(std::string,std::string, a1, a2)

#define INSTANCIATE_call_2_1(r,a1,a2,a3) HANDLER_EXTERN template r call2<r,a1,a2,a3>(std::string,std::string, a1, a2, a3)


namespace awali { namespace dyn { namespace loading {

//In order to know which one to instanciate here, comment instanciator below and execute:
// make -j7 awalidyn && nm -gC awali/CMakeFiles/awalidyn.dir/dyn/modules[>.cc.o | sed 's/std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >/std::string/g' | sed 's/awali::dyn:://g' | grep -i ' *W .*call[0-9]' | sort | uniq --count | sort
// and find lines that start with number greater than one.

INSTANCIATE_call_1_0(automaton_t, automaton_t);
INSTANCIATE_call_1_0(void, automaton_t);
INSTANCIATE_call_1_0(bool, automaton_t);

INSTANCIATE_call_1_1(bool, automaton_t,automaton_t);
INSTANCIATE_call_1_1(automaton_t, automaton_t,automaton_t);
INSTANCIATE_call_1_1(automaton_t, automaton_t, bool);

INSTANCIATE_call_2_0(bool, automaton_t,automaton_t);

}}}


#undef INSTANCIATE_call_0_1
#undef INSTANCIATE_call_1_0
#undef INSTANCIATE_call_1_1
#undef INSTANCIATE_call_1_2
#undef INSTANCIATE_call_1_3
#undef INSTANCIATE_call_1_4
#undef INSTANCIATE_call_2_0
#undef INSTANCIATE_call_2_1
#ifdef UNDEF_ON_EXIT
#undef HANDLER_EXTERN
#undef UNDEF_ON_EXIT
#endif
