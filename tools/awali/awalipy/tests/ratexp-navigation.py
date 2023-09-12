
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

class RatexpTests(unittest.TestCase): 

    def exptoaut(self,exp,alph): 
        kind = exp.get_kind()
        ws= exp.get_weightset()
        aut = awalipy.Automaton(alph, allow_eps=True, weightset=ws);
        if (kind == awalipy.RatExp.ZERO):
            return aut
        elif (kind == awalipy.RatExp.ONE):
            s = aut.add_state()
            aut.set_initial(s)
            aut.set_final(s)
        elif (kind == awalipy.RatExp.ATOM):
            l = exp.children();
            i = aut.add_state()
            f = aut.add_state()
            aut.set_initial(i)
            aut.set_final(f)
            aut.set_transition(i,f,l[0])
        elif (kind == awalipy.RatExp.SUM):
            for subexp in exp.children():
                aut.sum_here(self.exptoaut(subexp,alph))
        elif (kind == awalipy.RatExp.PROD):
            old_final = []
            first = True
            for subexp in exp.children():
                X = self.exptoaut(subexp,alph)
                smap = {}
                for s in X.states():
                    smap[s] = aut.add_state();
                final = []
                for s in X.final_states():
                    final.append((smap[s],X.get_final_weight(s)))
                initial = []
                for s in X.initial_states():
                    sw = X.get_initial_weight(s)
                    if first:
                        aut.set_initial(smap[s],sw)
                    for (f,w) in old_final:
                        aut.set_transition(f,smap[s],"\\e",
                            ws.mul(w,sw))
                for i in X.transitions():
                    aut.set_transition(smap[X.src_of(i)], 
                                       smap[X.dst_of(i)], 
                                       X.label_of(i),
                                       X.weight_of(i)
                                       )
                old_final = final
                first = False
            for (f,w) in old_final:
                aut.set_final(f,w)
        elif (kind == awalipy.RatExp.STAR):
            aut = self.exptoaut(exp.children()[0],alph);
            for i in aut.initial_states():
                for f in aut.final_states(): 
                    aut.set_transition(f,i,"\\e",
                      ws.mul(aut.get_final_weight(f),aut.get_initial_weight(i))
                    )
            s = aut.add_state()
            aut.set_initial(s)
            aut.set_final(s)
        [l,r] = exp.weight()
        if l != ws.one():
            for i in aut.initial_states():
                aut.set_initial(i, ws.mul(l,aut.get_initial_weight(i)));
        if r != ws.one():
            for f in aut.final_states():
                aut.set_final(f, ws.mul(aut.get_final_weight(f),r));
        return aut
    
    def test_01_exptoaut_B(self):
        for exp in ["\\e", "a+b","a+b+c","ab","abc","a*","(a*)*", "ab+cd","(a+b)*","(a+b+c)*(ac+d)*+dabc+((a+ab)*+abc)*"]:
            E = awalipy.RatExp(exp,alphabet="abcd")
            aut1 = E.exp_to_aut()
            aut2 = self.exptoaut(E,"abcd")
            aut2p = awalipy.proper(aut2)
            #aut1.save("/tmp/aut1.json") #uncomment to debug
            #aut2.save("/tmp/aut2.json")
            self.assertEqual(True, awalipy.are_equivalent(aut1,aut2p),"Problem with ratexp: "+str(exp)+" ("+str(E.get_kind())+")")

    def test_02_exptoaut_Z(self):
        for exp in ["<2>\\e", "<3>a+b", "(a<4>)+(<5>b)", "<7>(a+b+c)", "(a+b+c)<7>","<8>ab<9>","a<10>bc","<11>(a*)","(<11>a)*","<12>(a+<13>b+c<14>)*(ac+d)*+<-1>dabc+((a+<66>ab)*a<-7>+abc)*<-2>"]:
            E = awalipy.RatExp(exp,alphabet="abcd",weightset="Z")
            aut1 = E.exp_to_aut()
            aut2 = self.exptoaut(E,"abcd")
            aut2p = awalipy.proper(aut2)
            #aut1.save("/tmp/aut1.json") #uncomment to debug
            #aut2.save("/tmp/aut2.json")
            self.assertEqual(True, awalipy.are_equivalent(aut1,aut2p),"Problem with ratexp: "+str(exp)+" ("+str(E.get_kind())+")")



sys.stderr.write(
    "\n"
    +
    "=======================================================================\n"
    +
    "\tExp navigation tests.\n"
    +
    "=======================================================================\n"
);
suite = unittest.TestLoader().loadTestsFromTestCase(RatexpTests)
runner = unittest.TextTestRunner(verbosity=2,failfast=True)
result = runner.run(suite)
if len(result.failures):
    exit(1)
