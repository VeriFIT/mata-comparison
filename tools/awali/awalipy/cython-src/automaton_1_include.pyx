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
    cppclass simple_automaton_t(basic_automaton_t):
        transition_t get_transition(int src, int dst, string label)  except + ### <- in test suite (its.)
        bool has_transition3(int src, int dst, string label)  except + ### <- its.
        void del_transition3(int src, int dst, string label) except + ### <- its.
        string label_of(transition_t t)  except + ### <- its.
        transition_t set_transition4(int src, int dst, string label, string weight)  except +
        transition_t set_transition3(int src, int dst, string label)  except +
        string add_transition4(int src, int dst, string label, string weight)  except +
        string add_transition3(int src, int dst, string label)  except + ### <- its.
        #set_tdc_transition3(state_t src, state_t dst, vector[string] labels)  except +
        vector[int] successors2(int src, string label)  except + ### <- its.
        vector[int] predecessors2(int src, string label)  except + ### <- its.
        vector[transition_t] incoming2(int s, string label)  except +
        vector[transition_t] outgoing2(int s, string label)  except +
        vector[transition_t] in2(int s, string label)  except +
        vector[transition_t] out2(int s, string label)  except +
        string alphabet() except + ### <- its.
        bool has_letter(string l) except + ### <- its.
    simple_automaton_t make_simple_automaton2(string alphabet, string semiring) except + ### <- its.
    simple_automaton_t make_simple_automaton1(string alphabet) except + ### <- its.
    simple_automaton_t make_NFA_with_eps1(string alphabet)
    simple_automaton_t make_NFA_with_eps2(string alphabet, string semiring)

    simple_automaton_t random_DFA_(unsigned size, string alphabet) except +
##    simple_automaton_t make_de_n_ultimate_(unsigned n, string alphabet) except +  #a
    simple_automaton_t make_divkbaseb_(unsigned k, unsigned b, string alphabet) except + #a
    simple_automaton_t make_double_ring_(unsigned n, vector[unsigned] finals, string alphabet) except +  #a
    simple_automaton_t make_ladybird_(unsigned n, string alphabet) except + #a
    simple_automaton_t make_n_ultimate_(unsigned n, string alphabet) except + #a
    simple_automaton_t make_witness_(unsigned n, string alphabet) except + #a
    simple_automaton_t make_cerny_(unsigned n) except + #a

    simple_automaton_t support_(simple_automaton_t aut) except +
    simple_automaton_t characteristic_(simple_automaton_t aut, string semiring) except +
    simple_ratexp_t aut_to_exp_(simple_automaton_t aut) except +

    simple_transducer_t partial_identity_(simple_automaton_t aut) except +
    simple_automaton_t allow_eps_transition_(simple_automaton_t aut) except +

    simple_automaton_t determinize_(simple_automaton_t aut, bool history) except +
    bool are_equivalent_(simple_automaton_t aut, simple_automaton_t aut2) except +
    simple_automaton_t complement_(simple_automaton_t aut) except +
    void complement_here_(simple_automaton_t aut) except +
    simple_automaton_t complete_(simple_automaton_t aut) except +
    void complete_here_(simple_automaton_t aut) except +
    simple_automaton_t reduce_(simple_automaton_t aut) except +
    simple_automaton_t left_reduce_(simple_automaton_t aut) except +
    simple_automaton_t right_reduce_(simple_automaton_t aut) except +
    bool is_deterministic_(simple_automaton_t aut) except +
    bool is_complete_(simple_automaton_t aut) except +
    bool is_ambiguous_(simple_automaton_t aut) except +

    string eval_ (simple_automaton_t aut, string word) except +
    map[string,string] shortest_(simple_automaton_t aut, unsigned max) except +
    map[string,string] enumerate_(simple_automaton_t aut, unsigned max) except +

    simple_automaton_t product_(simple_automaton_t aut1, simple_automaton_t aut2) except +
    simple_automaton_t power_(simple_automaton_t aut, unsigned n) except +
    simple_automaton_t shuffle_(simple_automaton_t aut1, simple_automaton_t aut2) except +
    simple_automaton_t infiltration_(simple_automaton_t aut1, simple_automaton_t aut2) except +
    simple_automaton_t sum_(simple_automaton_t aut1, simple_automaton_t aut2) except +
    bool are_isomorphic_(simple_automaton_t aut1, simple_automaton_t aut2) except +
    void proper_here_(simple_automaton_t aut, bool backward, bool prune)  except +
    simple_automaton_t proper_(simple_automaton_t aut, bool backward, bool prune)  except +
    bool is_proper_(simple_automaton_t aut) except +
    bool is_valid_(simple_automaton_t aut) except +

    simple_automaton_t prefix_(simple_automaton_t aut) except +
    void prefix_here_(simple_automaton_t aut) except +
    simple_automaton_t suffix_(simple_automaton_t aut) except +
    void suffix_here_(simple_automaton_t aut) except +
    simple_automaton_t factor_(simple_automaton_t aut) except +
    void factor_here_(simple_automaton_t aut) except +

#      simple_automaton_t minimal_brzozowski_(simple_automaton_t aut) except +
    simple_automaton_t minimal_automaton_(simple_automaton_t aut, string method) except +
    simple_automaton_t minimal_automaton_(simple_automaton_t aut) except +
    simple_automaton_t min_quotient_(simple_automaton_t aut, string method) except +
    simple_automaton_t min_quotient_(simple_automaton_t aut) except +
    simple_automaton_t min_coquotient_(simple_automaton_t aut, string) except +
    simple_automaton_t min_coquotient_(simple_automaton_t aut) except +
    simple_automaton_t quotient_(simple_automaton_t aut, vector[vector[int]] equiv) except +
    simple_automaton_t coquotient_(simple_automaton_t aut, vector[vector[int]] equiv) except +
