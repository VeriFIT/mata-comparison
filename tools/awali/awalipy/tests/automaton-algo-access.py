# This file is part of Awali.
# Copyright 2016-2023 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch
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


global A,B,C,D,E,F,G,H,I,J
A = None
B = None
C = None
D = None
E = None
F = None
G = None
H = None
I = None
J = None


class AutomatonAccessTests(unittest.TestCase):

    def assertAutomatonSynctacticEquality(self, X, Y):
        self.assertEqual(X.states(), Y.states())
        self.assertEqual(sorted(X.initial_states()), sorted(Y.initial_states()))
        self.assertEqual(sorted(X.final_states()), sorted(Y.final_states()))
        self.assertEqual(X.transitions(), Y.transitions())
        for i in Y.transitions():
            self.assertEqual(X.src_of(i), Y.src_of(i))
            self.assertEqual(X.dst_of(i), Y.dst_of(i))
            self.assertEqual(X.label_of(i), Y.label_of(i))

    def test_accessibility_coaccessibility(self):
        A= awalipy.Automaton('abc', semiring.name)
        B= awalipy.make_automaton('abc', semiring.name)
        for i in range(12):
            A.add_state()
            awalipy.add_state(B)
        for i in range(2):
            A.set_transition(i+1, i+2, 'a', semiring.values[2])
            A.set_transition(i+4, i+5, 'b', semiring.values[3])
            A.set_transition(i+7, i+8, 'c', semiring.values[4])
            awalipy.set_transition(B, i+1, i+2, 'a', semiring.values[2])
            awalipy.set_transition(B, i+4, i+5, 'b', semiring.values[3])
            awalipy.set_transition(B, i+7, i+8, 'c', semiring.values[4])
        for i in [0,1,4]:
            A.set_initial(i)
            awalipy.set_initial(B, i)
        for i in [6,9,10,11]:
            A.set_final(i)
            awalipy.set_final(B, i)
        self.assertEqual(A.num_accessible_states(), 7)
        self.assertEqual(awalipy.num_accessible_states(B), 7)
        self.assertEqual(A.num_coaccessible_states(), 8)
        self.assertEqual(awalipy.num_coaccessible_states(B), 8)
        self.assertEqual(A.num_useful_states(), 3)
        self.assertEqual(awalipy.num_useful_states(B), 3)
        self.assertEqual(A.accessible_states(), [0,1,2,3,4,5,6])
        self.assertEqual(awalipy.accessible_states(B), [0,1,2,3,4,5,6])
        self.assertEqual(A.coaccessible_states(), [4,5,6,7,8,9,10,11])
        self.assertEqual(awalipy.coaccessible_states(B), [4,5,6,7,8,9,10,11])
        C = A.accessible();
        self.assertEqual(C.states(), [0,1,2,3,4,5,6])
        D = A.coaccessible();
        self.assertEqual(D.states(), [4,5,6,7,8,9,10,11])
        E = A.trim();
        self.assertEqual(E.states(), [4,5,6])
        for i in range(2):
            self.assertTrue(C.has_transition(i+1, i+2, 'a'))
            self.assertTrue(C.has_transition(i+4, i+5, 'b'))
            self.assertTrue(D.has_transition(i+4, i+5, 'b'))
            self.assertTrue(D.has_transition(i+7, i+8, 'c'))
            self.assertTrue(E.has_transition(i+4, i+5, 'b'))
        self.assertAutomatonSynctacticEquality(E, C.coaccessible());
        self.assertAutomatonSynctacticEquality(C.coaccessible(), D.accessible());
        for Z in [A, C, D]:
            self.assertFalse(Z.is_trim())
        self.assertTrue(E)
        for Z in [A, C]:
            self.assertFalse(Z.is_coaccessible())
        for Z in [D, E]:
            self.assertTrue(Z.is_coaccessible())
        for Z in [A, D]:
            self.assertFalse(Z.is_accessible())
        for Z in [C, E]:
            self.assertTrue(Z.is_accessible())





while(semiring.next()):
    sys.stderr.write(
        "\n"
        +
        "============================================================\n"
        +
        "\tAccessibility tests for automata over weightset " + semiring.name + ".\n"
        +
        "============================================================\n"
    );
    suite = unittest.TestLoader().loadTestsFromTestCase(AutomatonAccessTests)
    runner = unittest.TextTestRunner(verbosity=2,failfast=True)
    result = runner.run(suite)
    if not result.wasSuccessful():
        exit(1)
