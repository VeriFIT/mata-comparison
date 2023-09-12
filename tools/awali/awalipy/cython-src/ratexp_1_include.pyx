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
    cppclass simple_ratexp_t:
        simple_ratexp_t add(const simple_ratexp_t e) except +
        simple_ratexp_t mult(const simple_ratexp_t e) except +
        simple_ratexp_t star() except +
        string print_() except +
        string json() except +
        void rewrap_into_me_(simple_ratexp_t other) except+
        string get_weightset() except+
        simple_ratexp_t copy() except+
        bool is_valid() except+
        string constant_term() except+
        int get_kind() except+
        vector[simple_ratexp_t] children() except+
        string rweight() except+
        string lweight() except+
        string value() except+
        string alphabet() except+

    int KIND_ZERO, KIND_ONE, KIND_ATOM, KIND_SUM, KIND_PROD, KIND_STAR
    simple_ratexp_t make_simple_ratexp1(string str) except +
    simple_ratexp_t make_simple_ratexp2(string str, string semiring) except +
    simple_ratexp_t make_simple_ratexp2bis(string exp, string alph) except +
    simple_ratexp_t make_simple_ratexp3(string exp, string alph, string sr) except +

    map[simple_ratexp_t, string] derivation_(simple_ratexp_t exp, string word, bool breaking) except +
#      simple_automaton_t derived_term_(simple_ratexp_t exp, bool breaking, bool keep_history) except +
#      simple_automaton_t thompson_(simple_ratexp_t exp) except +
    simple_automaton_t exp_to_aut_(simple_ratexp_t exp) except +
    simple_automaton_t exp_to_aut_(simple_ratexp_t exp, string method) except+
    simple_ratexp_t expand_(simple_ratexp_t exp) except +
    simple_ratexp_t star_normal_form_(simple_ratexp_t exp) except +
    unsigned star_height_(simple_ratexp_t exp) except +
#      simple_automaton_t standard_(simple_ratexp_t exp) except +
    basic_automaton_t display_rat_(simple_ratexp_t exp) except +

cdef dict _from_map_ratexp_s(map[simple_ratexp_t, string] m):
    pymap = {}
    for it in m:
        pymap[_RatExp(it.first)] = it.second
    return pymap
