# This file is part of Awali.
# Copyright 2016-2021 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
#
# Awali is a free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

cdef extern from "automaton.h" namespace "awali::py":
    cppclass simple_transducer_t(basic_automaton_t):
        unsigned num_tapes() except +
        transition_t get_transition(int src, int dst, vector[string] label) except +
        bool has_transition3(int src, int dst, vector[string]  label) except +
        void del_transition3(int src, int dst, vector[string] label) except +
        vector[string] label_of(transition_t t) except +
        transition_t set_transition4(int src, int dst, vector[string] label, string weight) except +
        transition_t set_transition3(int src, int dst, vector[string] label) except +
        string add_transition4(int src, int dst, vector[string] label, string weight) except +
        string add_transition3(int src, int dst, vector[string] label) except +
        vector[int] successors2(int src, vector[string] label) except +
        vector[int] predecessors2(int src, vector[string] label) except +
        vector[transition_t] incoming2(int s, vector[string] label) except +
        vector[transition_t] outgoing2(int s, vector[string] label) except +
        vector[transition_t] in2(int s, vector[string] label) except +
        vector[transition_t] out2(int s, vector[string] label) except +
        vector[string] alphabets() except +
        bool has_letter(string l, unsigned tape_num) except +
    simple_transducer_t make_simple_transducer1(vector[string] alphabets) except +
    simple_transducer_t make_simple_transducer2(vector[string] alphabets, string semiring) except +

    simple_transducer_t synchronize_(simple_transducer_t tdc) except +
    bool is_synchronizable_(simple_transducer_t tdc) except +

    simple_automaton_t domain_(simple_transducer_t tdc) except +
    simple_automaton_t image_(simple_transducer_t tdc) except +
    simple_transducer_t images_(simple_transducer_t tdc) except +
    simple_automaton_t projection_(simple_transducer_t tdc, unsigned i) except +
    simple_transducer_t inverse_(simple_transducer_t tdc) except +
    simple_transducer_t compose_(simple_transducer_t tdc1, simple_transducer_t tdc2) except +
    bool is_functional_(simple_transducer_t tdc) except +

    simple_automaton_t eval_tdc_(simple_automaton_t aut, simple_transducer_t tdc) except +
