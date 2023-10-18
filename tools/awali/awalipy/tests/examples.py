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

import unittest, sys, os
cmd_folder=os.getcwd()
if cmd_folder not in sys.path:
    sys.path.insert(0, cmd_folder)

import awalipy

class TestExampleAutomata(unittest.TestCase):

    names = awalipy.list_examples()["automata"].keys()
    automata = []
    str_repr = []
    expressions = []
    def test_01_load(self):
        for name in self.names:
            self.automata.append(awalipy.load(name));

    def test_02_str(self):
        for automaton in self.automata:
            if (automaton.num_states() <= 100):
                self.str_repr.append(str(automaton));

    def test_03_toexp(self):
        for automaton in self.automata:
            if isinstance(automaton, awalipy.Automaton):
                if (automaton.num_states() <= 100):
                    self.expressions.append(automaton.aut_to_exp())



sys.stderr.write(
    "\n"
    +
    "============================================================\n"
    +
    "\tTesting example automata.\n"
    +
    "============================================================\n"
);
suite = unittest.TestLoader().loadTestsFromTestCase(TestExampleAutomata)
runner = unittest.TextTestRunner(verbosity=2,failfast=True)
result = runner.run(suite)
exit(not result.wasSuccessful())
