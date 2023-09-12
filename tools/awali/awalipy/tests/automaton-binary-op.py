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

import unittest, sys
import semiring
import awalipy
import random

def random_copy(aut):
        states = aut.states();
        n = len(states)
        new = aut.copy()
        for s in aut.states():
            new.del_state(s)
        states_new = []
        for i in range(n):
            states_new.append(new.add_state())
        random.shuffle(states_new);
        states_map = {}
        for i in range(n):
            states_map[states[i]] = states_new[i]
        for i in range(n):
            for tr in aut.outgoing(states[i]): 
                src = aut.src_of(tr)
                dst = aut.dst_of(tr)
                weight = aut.weight_of(tr)
                label = aut.label_of(tr)
                new.set_transition(states_map[src], states_map[dst], 
                                   label, weight)
            new.set_final(states_map[states[i]],
                          aut.get_final_weight(states[i]))
            new.set_initial(states_map[states[i]],
                          aut.get_initial_weight(states[i]))
        return new


class AutomatonBinaryTests(unittest.TestCase):

    def test_are_isomorphic(self):
        automata = []
        for s in ["a1","b1","c1","d1","e1","gray","minab"]:
            automata.append(awalipy.load("a1"));
        for i in range(28):
            automata.append(awalipy.make_random_DFA(i+2, "abc"))
        for A in automata:
            B = random_copy(A)
            self.assertEqual (True, awalipy.are_isomorphic(A,B))
            self.assertEqual (True, B.are_isomorphic(A))

    def test_are_equivalent(self):
        automata = []
        for s in ["a1","b1","c1","d1","e1","gray","minab"]:
            automata.append(awalipy.load("a1"));
        for i in range(8):
            automata.append(awalipy.make_random_DFA(i+2, "abc"))
        for A in automata:
            C = A.aut_to_exp().exp_to_aut()
            B = random_copy(A)
            self.assertEqual (True, awalipy.are_equivalent(B,C))
            self.assertEqual (True, C.are_equivalent(B))

    def test_sum(self):
        automata = []
        for s in ["a1","b1","c1","d1","e1","gray","minab"]:
            automata.append(awalipy.load("a1"));
        for i in range(8):
            automata.append(awalipy.make_random_DFA(i+12, "abc"));
        for A in automata:
            for B in automata:
                C = awalipy.sum(A,B)
                self.assertEqual(len(A.states())+len(B.states()),len(C.states()))
                D = B.sum(A)
                self.assertEqual(len(A.states())+len(B.states()),len(D.states()))
                self.assertEqual(True, awalipy.are_isomorphic(C,D))



    def test_product(self):
        automata = []
        X = awalipy.Automaton(awalipy.RatExp("(a+b)*"))
        words = X.enumerate(6)
        for s in ["a1","oddb","evena","slowcerny"]:
            automata.append(awalipy.load("a1"))
        for i in range(1):
            X = awalipy.make_random_DFA((i+1)*8+1, "ab")
            X.set_initial(0)
            X.set_final(1)
            automata.append(X)
        d = {}
        for A in automata:
            for w in words:
                d[A,w] = A(w)
        for A in automata:
            for B in automata:
                if A is not B:
                    C = awalipy.product(A,B)
                    D = B.product(A)
                    for w in words:
                        bA = ('1' == d[A,w])
                        bB = ('1' == d[B,w])
                        bC = ('1' == C(w))
                        bD = ('1' == D(w))
                        self.assertEqual(bA and bB, bC)
                        self.assertEqual(bA and bB, bD)




sys.stderr.write(
        "\n"
        +
        "============================================================\n"
        +
        "\tBinary algorithm tests.\n"
        +
        "============================================================\n"
    );
suite = unittest.TestLoader().loadTestsFromTestCase(AutomatonBinaryTests)
runner = unittest.TextTestRunner(verbosity=2,failfast=True)
result = runner.run(suite)
if len(result.failures):
    exit(1)
