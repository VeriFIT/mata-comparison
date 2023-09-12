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
    cppclass basic_automaton_t:
        size_t num_states() except + ### <- in test suite (its.)
        size_t num_initials() except + ### <- its.
        size_t num_finals() except + ### <- its.
        size_t num_transitions() except + ### <- its.
        bool has_state(int s) except + ### <- its.
        bool is_initial(int s) except +  ### <- its.
        bool is_final(int s) except + ### <- its.
        string get_initial_weight(int s) except + ### <- its.
        string get_final_weight(int s) except + ### <- its.
        bool has_transition1(transition_t t) except + ### <- its.
        int src_of(transition_t t) except + ### <- its.
        int dst_of(transition_t t) except + ### <- its.
        string weight_of(transition_t t) ### <- its.
        int null_state() except +
        int null_transition() except +
        int add_state0() except + ### <- its.
        int add_state1(string name) except +
        void del_state(int s) except + ### <- its.
        void set_initial2(int s, string w) except + ### <- its.
        void set_initial1(int s) except + ### <- its.
        string add_initial(int s, string w) except + ### <- its.
        void unset_initial(int s) except + ### <- its.
        void set_final2(int s, string w) except + ### <- its.
        void set_final1(int s) except + ### <- its.
        string add_final(int s, string w) except + ### <- its.
        void unset_final(int s) except + ### <- its.
        void del_transition1(transition_t tr) except + ### <- its.
        void del_transition2(int src, int dst) except + ### <- its.
        transition_t set_eps_transition2(int src, int dst) except + ### <- its.
        transition_t set_eps_transition3(int src, int dst, string weight) except + ### <- its.
        string add_eps_transition2(int src, int dst) except + ### <- its.
        string add_eps_transition3(int src, int dst, string weight) except + ### <- its.
        string set_weight(transition_t tr, string weight) except +
        string add_weight(transition_t tr, string weight) except +
        string lmul_weight(transition_t tr, string weight) except +
        string rmul_weight(transition_t tr, string weight) except +
        vector[int] states() except + ### <- its.
        vector[transition_t] transitions() except + ### <- its.
        vector[int] initial_states() except + ### <- its.
        #vector[transition_t] initial_transitions() except +
        vector[int] final_states() except + ### <- its.
        #vector[transition_t] final_transitions() except +
        vector[int] successors1(int src) except + ### <- its.
        vector[int] predecessors1(int src) except + ### <- its.
        vector[transition_t] outin(int src, int dst) except +
        vector[transition_t] incoming1(int s) except +
        vector[transition_t] outgoing1(int s) except +
        #vector[transition_t] in1(int s) except +
        #vector[transition_t] out1(int s) except +
        #int pre() except +
        #int post() except +
        #vector[int] all_states() except +
        #vector[transition_t] all_transitions() except +
        #vector[transition_t] all_out(int s) except +
        #vector[transition_t] all_in(int s) except +
        bool is_transducer() except +
        string get_static_context() except +
        void strip_history() except +
        string get_state_name(int s) except +
        string weight_one() except + ### <- its.
        string weight_zero() except + ### <- its.
        weightset_t get_weightset() except + 
        string get_weightset_name() except + ### <- its.
#          string json() except +
#          string fado() except +
#          string grail() except +
#          string dot(bool, bool) except +
        string svg(bool, bool) except +
        void release() except +
        void rewrap_into_me_(basic_automaton_t other)
    #basic_automaton_t make_basic_automaton1(string alphabet) except +
    #basic_automaton_t make_basic_automaton2(string alphabet,string semiring) except +
    void pdfdisplay(basic_automaton_t,bool,bool) except+
    vector[vector[string]] example_automata_() except+
    vector[vector[string]] example_ratexps_() except+
    basic_automaton_t load_automaton_(string s, string fmt) except+
    void save_automaton_(basic_automaton_t aut, string s, string fmt) except +
    basic_automaton_t copy_(basic_automaton_t aut) except +

    vector[int] accessible_states_(basic_automaton_t aut) except +  ### <- its.
    size_t num_accessible_states_(basic_automaton_t aut) except +  ### <- its.
    basic_automaton_t accessible_(basic_automaton_t aut) except +  ### <- its.
    void accessible_here_(basic_automaton_t aut) except +  ### <- its.
    bool is_accessible_(basic_automaton_t aut) except +  ### <- its.

    vector[int] coaccessible_states_(basic_automaton_t aut) except +  ### <- its.
    size_t num_coaccessible_states_(basic_automaton_t aut) except +  ### <- its.
    basic_automaton_t coaccessible_(basic_automaton_t aut) except + ### <- its.
    void coaccessible_here_(basic_automaton_t aut) except +  ### <- its.
    bool is_coaccessible_(basic_automaton_t aut) except +  ### <- its.

    vector[int] useful_states_(basic_automaton_t aut) except +   ### <- its.
    size_t num_useful_states_(basic_automaton_t aut) except +   ### <- its.
    basic_automaton_t trim_(basic_automaton_t aut) except +   ### <- its.
    void trim_here_(basic_automaton_t aut) except + ### <- its.
    bool is_trim_(basic_automaton_t aut) except + ### <- its.

    basic_automaton_t concatenate_(basic_automaton_t aut, basic_automaton_t aut2) except +

    basic_automaton_t lift_(basic_automaton_t aut) except +

    void transpose_here_(basic_automaton_t aut) except +
    basic_automaton_t transpose_(basic_automaton_t aut) except +
    basic_automaton_t condensation_(basic_automaton_t aut) except +

    vector[vector[state_t]] sccs_ (basic_automaton_t aut) except +
    vector[string] available_semirings() except +
    vector[state_t] scc_of_ (basic_automaton_t aut, state_t s) except +

