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
import awalipy as vr

global A,B,C,D
A = None
B = None
C = None
D = None


class TransducerEditionTests(unittest.TestCase):
    def assertAutomatonSynctacticEquality(self, X, Y):
        self.assertEqual(X.states(), Y.states())
        self.assertEqual(sorted(X.initial_states()), sorted(Y.initial_states()))
        self.assertEqual(sorted(X.final_states()), sorted(Y.final_states()))
        self.assertEqual(X.transitions(), Y.transitions())
        for i in Y.transitions():
            self.assertEqual(X.src_of(i), Y.src_of(i))
            self.assertEqual(X.dst_of(i), Y.dst_of(i))
            self.assertEqual(X.label_of(i), Y.label_of(i))

    def test_01_creation(self):
        global A,B
        A = vr.Transducer(['ab','bc'], semiring.name)
        B = vr.make_transducer(['ab','bc'], semiring.name)
        #self.assertEqual(A.get_static_context(), 'lat<lan<lal_char>,lan<lal_char>>_'+semiring.name.lower())
        #self.assertEqual(vr.get_static_context(B), 'lat<lan<lal_char>,lan<lal_char>>_'+semiring.name.lower())

    def test_02_alphabet(self):
        self.assertEqual(A.alphabets(), ['ab','bc'])
        self.assertEqual(vr.alphabets(B), ['ab','bc'])
        for c in [['a','b'], ['b','c'] ]:
            self.assertTrue(A.has_letter(c[0], 0))
            self.assertTrue(A.has_letter(c[1], 1))
            self.assertTrue(vr.has_letter(B, c[0], 0))
            self.assertTrue(vr.has_letter(B, c[1], 1))

    def test_03_weightset(self):
        self.assertEqual(A.get_weightset().one(), semiring.values[1])
        self.assertEqual(A.get_weightset().zero(), semiring.values[0])

    def test_04_states(self):
        A.add_state()
        A.add_state()
        A.add_state()
        self.assertEqual(A.states(), [0,1,2])
        self.assertEqual(A.num_states(), 3)
        for i in range(3):
            self.assertTrue(A.has_state(i))
        for i in range(3,10):
            self.assertFalse(A.has_state(i))
        vr.add_state(B)
        vr.add_state(B)
        vr.add_state(B)
        self.assertEqual(vr.states(B), [0,1,2])
        self.assertEqual(vr.num_states(B), 3)
        for i in range(3):
            self.assertTrue(vr.has_state(B,i))
        for i in range(3,10):
            self.assertFalse(vr.has_state(B,i))

    def test_05_final_initial(self):
        A.set_initial(0)
        A.add_initial(2, semiring.values[4])
        A.add_initial(2, semiring.values[3])
        A.add_initial(1, semiring.values[1])
        A.unset_initial(0)
        A.add_initial(1, semiring.values[0])
        A.set_final(0)
        A.add_final(1)
        A.set_final(1, semiring.values[2])
        A.unset_final(2)
        if (semiring.name.lower() == 'f2'):
            self.assertFalse(A.is_initial(2))
            A.set_initial(2)
        self.assertEqual(A.initial_states(), [1,2])
        self.assertEqual(A.final_states(), [0,1])
        self.assertEqual(A.num_initials(), 2)
        self.assertEqual(A.num_finals(), 2)
        vr.set_initial(B, 0)
        vr.add_initial(B, 2, semiring.values[4])
        vr.add_initial(B, 2, semiring.values[3])
        vr.add_initial(B, 1, semiring.values[1])
        vr.unset_initial(B, 0)
        vr.add_initial(B, 1, semiring.values[0])
        vr.set_final(B, 0)
        vr.add_final(B, 1)
        vr.set_final(B, 1, semiring.values[2])
        vr.unset_final(B, 2)
        if (semiring.name.lower() == 'f2'):
            self.assertFalse(vr.is_initial(B,2))
            vr.set_initial(B, 2)
        self.assertEqual(vr.initial_states(B), [1,2])
        self.assertEqual(vr.final_states(B), [0,1])
        self.assertEqual(vr.num_initials(B), 2)
        self.assertEqual(vr.num_finals(B), 2)
        for i in [1,2]:
            self.assertTrue(A.is_initial(i))
            self.assertTrue(vr.is_initial(B,i))
        self.assertEqual(A.get_initial_weight(1), semiring.values[1])
        self.assertEqual(vr.get_initial_weight(B,1), semiring.values[1])
        self.assertEqual(A.get_initial_weight(2), semiring.added)
        self.assertEqual(vr.get_initial_weight(B,2), semiring.added)
        self.assertFalse(A.is_initial(0))
        self.assertFalse(vr.is_initial(B,0))
        self.assertEqual(A.get_initial_weight(0), semiring.values[0])
        self.assertEqual(vr.get_initial_weight(B,0), semiring.values[0])
        for i in [0,1]:
            self.assertTrue(A.is_final(i))
            self.assertTrue(vr.is_final(B, i))
        self.assertEqual(A.get_final_weight(0), semiring.values[1])
        self.assertEqual(vr.get_final_weight(B,0), semiring.values[1])
        self.assertEqual(A.get_final_weight(1), semiring.values[2])
        self.assertEqual(vr.get_final_weight(B,1), semiring.values[2])
        self.assertFalse(A.is_final(2))
        self.assertFalse(vr.is_final(B, 2))
        self.assertEqual(A.get_final_weight(2), semiring.values[0])
        self.assertEqual(vr.get_final_weight(B,2), semiring.values[0])

    def test_06_transitions(self):
        L = [ [1, 1, ['b','b']], [0,1,['a','c']], [0,0,['a','c']], [0,0,['b','b']],\
            [0,2,['a','c']], [1, 1, ['b','b']],  \
            [1,2,['b','b']], [2,2,['a','c']], [0,2,['a','c']], ]
        #last transition is redundant hence not semiring.added
        j=0
        for i in L:
            A.add_transition(i[0], i[1], i[2], semiring.values[j])
            vr.add_transition(B, i[0], i[1], i[2], semiring.values[j])
            j=(j+1)%5
        if (semiring.name.lower() == 'f2'):
            self.assertFalse(A.has_transition(0, 2, ['a','c']))
            self.assertFalse(vr.has_transition(B, 0, 2, ['a','c']))
            A.add_transition(0,2,['a','c'])
            vr.add_transition(B, 0,2,['a','c'])
        L.pop(5);
        L.pop(0);
        j=0
        for i in A.transitions():
            self.assertTrue(A.has_transition(i))
            self.assertEqual(A.src_of(i), L[j][0])
            self.assertEqual(A.dst_of(i), L[j][1])
            self.assertEqual(A.label_of(i), L[j][2])
            self.assertTrue(A.has_transition(L[j][0], L[j][1], L[j][2]))
            self.assertEqual(A.get_transition(L[j][0], L[j][1], L[j][2]), i)
            if (j < 3):
                self.assertEqual(A.weight_of(i), semiring.values[j+1]) #+1 since we deleted 0
            elif (j == 3):
                self.assertEqual(A.weight_of(i), semiring.added)
            else:
                self.assertEqual(A.weight_of(i), semiring.values[j+2-5])
            j=j+1

        for i in range(10,10):
            self.assertFalse(A.has_transition(i))
        with self.assertRaises(TypeError):
            A.has_transition(0,1)
        self.assertEqual(A.predecessors(1), [0])
        self.assertEqual(A.predecessors(1, ['b','b']), [])
        self.assertEqual(sorted(A.predecessors(2, ['a','c'])), [0,2])
        self.assertEqual(sorted(A.successors(2)), [2])
        self.assertEqual(sorted(A.successors(0)), [0,0,1,2])
        self.assertEqual(sorted(A.successors(0, ['a','c'])), [0,1,2])

        j=0
        for i in vr.transitions(B):
            self.assertTrue(vr.has_transition(B, i))
            self.assertEqual(vr.src_of(B, i), L[j][0])
            self.assertEqual(vr.dst_of(B, i), L[j][1])
            self.assertEqual(vr.label_of(B, i), L[j][2])
            self.assertTrue(vr.has_transition(B, L[j][0], L[j][1], L[j][2]))
            self.assertEqual(vr.get_transition(B, L[j][0], L[j][1], L[j][2]), i)
            j=j+1
        for i in range(10,10):
            self.assertFalse(vr.has_transition(B, i))
        self.assertFalse(vr.has_transition(B,2,2, ['b','b']))
        with self.assertRaises(TypeError):
            vr.has_transition(B,0,1)
        self.assertEqual(vr.predecessors(B, 1), [0])
        self.assertEqual(vr.predecessors(B, 1, ['b','b']), [])
        self.assertEqual(sorted(vr.predecessors(B, 2, ['a','c'])), [0,2])
        self.assertEqual(sorted(vr.successors(B, 2)), [2])
        self.assertEqual(sorted(vr.successors(B, 0)), [0,0,1,2])
        self.assertEqual(sorted(vr.successors(B, 0, ['a','c'])), [0,1,2])

        self.assertAutomatonSynctacticEquality(A,B)

    def test_07_copy(self):
        global C, D
        C = A.copy()
        self.assertAutomatonSynctacticEquality(A,C)
        D = vr.copy(B)
        self.assertAutomatonSynctacticEquality(B,D)
        self.assertAutomatonSynctacticEquality(C,D)

    def test_08_del(self):
        A.del_state(1)
        self.assertEqual(A.states(), [0,2])
        for i in A.transitions():
            self.assertNotEqual(A.src_of(i),1)
            self.assertNotEqual(A.dst_of(i),1)
        A.del_transition(9)
        self.assertFalse(A.has_transition(9))
        A.del_transition(0,2,['a','c'])
        self.assertFalse(A.has_transition(0,2,['a','c']))
        self.assertTrue(A.has_transition(0,0,['a','c']))
        self.assertTrue(A.has_transition(0,0,['b','b']))
        A.del_transition(0,0)
        self.assertFalse(A.has_transition(0,0,['a','c']))
        self.assertFalse(A.has_transition(0,0,['b','b']))
        with self.assertRaises(ValueError):
            A.del_state(1)
        with self.assertRaises(ValueError):
            A.del_transition(1,1,['a','c'])
        with self.assertRaises(ValueError):
            A.del_transition(2,1,['a','c'])
        self.assertEqual(A.transitions(), [])
        A.del_transition(2,2,['b','b'])
        self.assertEqual(A.transitions(), [])

        vr.del_state(B, 1)
        self.assertEqual(vr.states(B), [0,2])
        for i in vr.transitions(B):
            self.assertNotEqual(vr.src_of(B, i), 1)
            self.assertNotEqual(vr.dst_of(B, i), 1)
        vr.del_transition(B, 9)
        self.assertFalse(vr.has_transition(B, 9))
        vr.del_transition(B, 0, 2, ['a','c'])
        self.assertFalse(vr.has_transition(B, 0, 2, ['a','c']))
        self.assertTrue(vr.has_transition(B, 0, 0, ['a','c']))
        self.assertTrue(vr.has_transition(B, 0, 0, ['b','b']))
        vr.del_transition(B, 0, 0)
        self.assertFalse(vr.has_transition(B, 0, 0, ['a','c']))
        self.assertFalse(vr.has_transition(B, 0, 0, ['b','b']))
        with self.assertRaises(ValueError):
            vr.del_state(B, 1)
        with self.assertRaises(ValueError):
            vr.del_transition(B, 1, 1, ['a','c'])
        with self.assertRaises(ValueError):
            vr.del_transition(B, 2, 1, ['a','c'])
        self.assertEqual(vr.transitions(B), [])
        vr.del_transition(B, 2, 2, ['b','b'])
        self.assertEqual(vr.transitions(B), [])
        self.assertAutomatonSynctacticEquality(A,B)


    #def test_99_rerun_all_with_lowercase_weightset(self):
        #global A, B
        #semiring.name = semiring.name.lower()
        #self.test_01_creation()
        #self.test_02_alphabet()
        #self.test_03_weightset()
        #self.test_04_states()
        #self.test_05_final_initial()
        #self.test_06_transitions()
        #self.test_07_copy()
        #self.test_08_del()




while(semiring.next()):
    exclude_list= []
    sys.stderr.write(
        "\n"
        +
        "============================================================\n"
        +
        "\tEdition tests for transducers over weight-set " + semiring.name + ".\n"
        +
        "============================================================\n"
    );
    if (semiring.name not in exclude_list):
        suite = unittest.TestLoader().loadTestsFromTestCase(TransducerEditionTests)
        runner = unittest.TextTestRunner(verbosity=2,failfast=True)
        result = runner.run(suite)
        if len(result.failures):
            exit(1)
    else:
        sys.stderr.write("Test ignored: weigh-set '"+semiring.name+"' belongs to the exclude list.\n");
