
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

class RatExpMinimalAutomatonTests(unittest.TestCase): 

    aut_to_exp = ["default", "glushkov", "derived_term", "breaking", \
                  "thompson", "compact_thompson", "weighted_thompson", \
                  "standard_and_quotient"]
    quotient = ["default", "moore", "hopcroft"]
    minim = ["default", "determinize_quotient", "brzozowski"]

    def test(self):
        expressions = [awalipy.RatExp("(a+b)*abab(a+b)*"),\
                       awalipy.load("conway-exp"), \
                       awalipy.load("div3b2-exp")]
        for e in expressions: 
            automata = []
            for a in self.aut_to_exp:
                for q in self.quotient:
                    for m in self.minim:
                        automata.append((a,m,q,e.minimal_automaton(a,m,q)))
        for ((a1,m1,q1,aut1),(a2,m2,q2,aut2)) in zip(automata[:-1],automata[1:]):
            self.assertTrue(awalipy.are_isomorphic(aut1,aut2), \
                            "Problem with " + str(e) + " : "\
                            + str((a1,m1,q1)) + " and " + str((a2,m2,q2)))
        

sys.stderr.write(
    "\n"
    +
    "=======================================================================\n"
    +
    "\tTest equivalence of methods for computing the minimal automaton.\n"
    +
    "=======================================================================\n"
);
suite = unittest.TestLoader().loadTestsFromTestCase(RatExpMinimalAutomatonTests)
runner = unittest.TextTestRunner(verbosity=2,failfast=True)
result = runner.run(suite)
if len(result.failures) > 0:
    exit(1)
