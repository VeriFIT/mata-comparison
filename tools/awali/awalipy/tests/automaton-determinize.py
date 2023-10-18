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
import awalipy as vr
import random;

global A,B,C,D
A = None
B = None
C = None
D = None

class AutomatonDeterminizeTests(unittest.TestCase):

  def test_01s_a1(self):
    A= vr.load("a1")
    B= vr.load("private::a1_determinization", internal_examples=True)
    self.assertTrue(vr.are_isomorphic(A.determinize(), B))

  def test_02_ladybird(self):
    ipow= 1
    for i in range(1,6):
      ipow*= 2
      A= vr.make_ladybird(i)
      A_det= A.determinize()
      A_det_comp= A_det.complete()
      self.assertEqual(ipow, A_det_comp.num_states())
      A_det_comp_min= A_det_comp.trim().minimal_automaton().complete()
      self.assertEqual(ipow, A_det_comp_min.num_states())

  def test_03_random(self):
    for i in range(100):
      n_states= int(random.uniform(1, 10))+2
      A=vr.make_random_DFA(n_states, "abc")
      self.assertTrue(A.is_deterministic())
      B= A.copy()
      B.add_transition(0,0,'a')
      B.add_transition(0,1,'a')
      B.add_transition(n_states-2, n_states-1,'b')
      B.add_transition(n_states-1, n_states-1,'b')
      self.assertFalse(B.is_deterministic())
      B_det=B.determinize()
      self.assertTrue(B_det.is_deterministic())
      self.assertTrue(vr.are_equivalent(B_det, B))


sys.stderr.write(
    "\n"
    +
    "============================================================\n"
    +
    "\tDeterminization tests for automata over weightset B.\n"
    +
    "============================================================\n"
);
suite = unittest.TestLoader().loadTestsFromTestCase(AutomatonDeterminizeTests)
runner = unittest.TextTestRunner(verbosity=2,failfast=True)
result = runner.run(suite)
exit(not result.wasSuccessful())
