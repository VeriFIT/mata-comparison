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

import importer # Imports awalipy in the correct manner
import awalipy
import sys, unittest


class StateNamesTests(unittest.TestCase):

    def div3b2(self):
        return awalipy.make_divkbaseb(3,2)

    def test_01_has_explicit_name(self):
        div3b2 = self.div3b2()
        for i in div3b2.states():
            self.assertFalse(div3b2.has_explicit_name(i))

    def test_02_set_name_different(self):
        div3b2 = self.div3b2()
        names = ["Hello", "World", "!"]
        states = div3b2.states()
        for i in [0,1,2]:
            div3b2.set_state_name(states[i],names[i])
            self.assertTrue(div3b2.has_explicit_name(states[i]))
            self.assertEqual(names[i], div3b2.get_state_name(states[i]))
            self.assertEqual(states[i], div3b2.get_state_by_name(names[i]))

    def test_03_set_name_same(self):
        div3b2 = self.div3b2()
        name = "Hello world"
        states = div3b2.states()
        div3b2.set_state_name(states[0], name)
        div3b2.set_state_name(states[1], name)
        self.assertTrue(type(div3b2.get_state_by_name(name)) is list)
        self.assertTrue(states[0] in div3b2.get_state_by_name(name))
        self.assertTrue(states[1] in div3b2.get_state_by_name(name))
        self.assertEqual(2, len(div3b2.get_state_by_name(name)))
        div3b2.set_state_name(states[2], name)
        self.assertTrue(type(div3b2.get_state_by_name(name)) is list)
        self.assertTrue(states[0] in div3b2.get_state_by_name(name))
        self.assertTrue(states[1] in div3b2.get_state_by_name(name))
        self.assertTrue(states[2] in div3b2.get_state_by_name(name))
        self.assertEqual(3, len(div3b2.get_state_by_name(name)))

    def test_04_rename_different(self):
        div3b2 = self.div3b2()
        name1 = "Hello"
        name2 = "World"
        states = div3b2.states()
        div3b2.set_state_name(states[0], name1)
        div3b2.set_state_name(states[0], name2)        
        with self.assertRaises(KeyError):
            div3b2.get_state_by_name(name1)
        self.assertEqual(states[0], div3b2.get_state_by_name(name2))

    def test_05_rename_same_source(self):
        div3b2 = self.div3b2()
        name1 = "Hello"
        name2 = "World"
        states = div3b2.states()
        div3b2.set_state_name(states[0], name1)
        div3b2.set_state_name(states[1], name1)
        div3b2.set_state_name(states[0], name2)
        self.assertEqual(states[0], div3b2.get_state_by_name(name2))
        self.assertEqual(states[1], div3b2.get_state_by_name(name1))

    def test_06_rename_same_target(self):
        div3b2 = self.div3b2()
        name1 = "Hello"
        name2 = "World"
        states = div3b2.states()
        div3b2.set_state_name(states[0], name1)
        div3b2.set_state_name(states[1], name2)
        div3b2.set_state_name(states[0], name2)
        with self.assertRaises(KeyError):
            div3b2.get_state_by_name(name1)
        self.assertTrue(type(div3b2.get_state_by_name(name2)) is list)
        self.assertTrue(states[0] in div3b2.get_state_by_name(name2))
        self.assertTrue(states[1] in div3b2.get_state_by_name(name2))
        self.assertEqual(2, len(div3b2.get_state_by_name(name2)))

    def test_07_delete_state_different(self):
        div3b2 = self.div3b2()
        names = ["Hello", "World", "!"]
        states = div3b2.states()
        div3b2.set_state_name(states[0], names[0])
        div3b2.set_state_name(states[1], names[1])
        div3b2.set_state_name(states[2], names[2])
        for i in [0,1,2]:
            div3b2.del_state(states[i])
            with self.assertRaises(KeyError):
                div3b2.get_state_by_name(names[i])

    def test_08_delete_state_same(self):
        div3b2 = self.div3b2()
        name = "Hello world !"
        states = div3b2.states()
        div3b2.set_state_name(states[0], name)
        div3b2.set_state_name(states[1], name)
        div3b2.set_state_name(states[2], name)
            
        div3b2.del_state(states[0])
        self.assertTrue(type(div3b2.get_state_by_name(name)) is list)
        self.assertTrue(states[1] in div3b2.get_state_by_name(name))
        self.assertTrue(states[2] in div3b2.get_state_by_name(name))
        self.assertEqual(2, len(div3b2.get_state_by_name(name)))

        div3b2.del_state(states[1])
        self.assertTrue(type(div3b2.get_state_by_name(name)) is int)
        self.assertEqual(states[2], div3b2.get_state_by_name(name))

        div3b2.del_state(states[2])
        with self.assertRaises(KeyError):
            div3b2.get_state_by_name(name)

    def test_09_rewrap_product(self):
        div3b2 = self.div3b2()        
        names = ["Hello", "World", "!"]
        states = div3b2.states()
        div3b2.set_state_name(states[0], names[0])
        div3b2.set_state_name(states[1], names[1])
        div3b2.set_state_name(states[2], names[2])
        div3b2.product_here(div3b2)

        with self.assertRaises(KeyError):
            div3b2.get_state_by_name(names[0])
        with self.assertRaises(KeyError):
            div3b2.get_state_by_name(names[1])
        with self.assertRaises(KeyError):
            div3b2.get_state_by_name(names[2])

    def test_10_copy(self):
        div3b2 = self.div3b2()        
        names = ["Hello", "World", "!"]
        states = div3b2.states()
        for i in [0,1,2]:
            div3b2.set_state_name(states[i], names[i])
        copy = div3b2.copy()
        states = copy.states()
        for i in [0,1,2]:
            self.assertEqual(names[i], copy.get_state_name(states[i]))

    def test_11_allow_eps_here(self):
        div3b2 = self.div3b2()        
        names = ["Hello", "World", "!"]
        states = div3b2.states()
        for i in [0,1,2]:
            div3b2.set_state_name(states[i], names[i])
        div3b2.allow_eps_transition_here()
        states = div3b2.states()
        for i in [0,1,2]:
            self.assertEqual(names[i], div3b2.get_state_name(states[i]))

    def test_12_id_or_name_int(self):
        div3b2 = self.div3b2()    
        for i in div3b2.states():  
            self.assertEqual(i,div3b2._id_or_name(i))

    def test_13_id_or_name_str_correct(self):
        div3b2 = self.div3b2()        
        names = ["Hello", "World", "!"]
        states = div3b2.states()
        for i in [0,1,2]:
            div3b2.set_state_name(states[i], names[i])
        for i in [0,1,2]:
            self.assertEqual(states[i],div3b2._id_or_name(names[i]))

    def test_14_id_or_name_str_absent(self):
        div3b2 = self.div3b2()        
        names = ["Hello", "World", "!"]
        states = div3b2.states()
        for i in [0,1,2]:
            div3b2.set_state_name(states[i], names[i])
        wrong_names = ["olleH", "dlroW"]
        for i in [0,1]:
            with self.assertRaises(KeyError):
                div3b2._id_or_name(wrong_names[i])

    def test_15_id_or_name_str_multiple(self):
        div3b2 = self.div3b2()    
        name = "Hello world !"
        states = div3b2.states()
        
        div3b2.set_state_name(states[0], name)
        div3b2.set_state_name(states[1], name)
        with self.assertRaises(ValueError):
            div3b2._id_or_name(name)

        div3b2.set_state_name(states[2], name)
        with self.assertRaises(ValueError):
            div3b2._id_or_name(name)

    def test_16_id_or_name_str_wrong_type(self):
        div3b2 = self.div3b2()        
        names = ["Hello", "World", "!"]
        states = div3b2.states()
        for i in [0,1,2]:
            div3b2.set_state_name(states[i], names[i])
        wong_names = ["olleH", "dlroW"]
        for i in [0.2,div3b2,[],{"Hello":"World"}]:
            with self.assertRaises(TypeError):
                div3b2._id_or_name(i)

    def test_17_div3b2_builtbyname(self):
        target = self.div3b2()
        built = awalipy.Automaton("01")
        built.add_state('0 mod 3')
        built.add_state('1 mod 3')
        built.add_state('2 mod 3')
        built.add_transition('0 mod 3','0 mod 3','0')
        built.set_transition('0 mod 3','1 mod 3','1')
        built.add_transition('1 mod 3','0 mod 3','1')
        built.set_transition('1 mod 3','2 mod 3','0')
        built.add_transition('2 mod 3','1 mod 3','0')
        built.set_transition('2 mod 3','2 mod 3','1')
        built.set_initial('0 mod 3')
        built.add_final('0 mod 3')
        self.assertTrue(awalipy.are_isomorphic(target,built))
        built.add_state('3 mod 3')
        built.del_state('3 mod 3')
        self.assertTrue(awalipy.are_isomorphic(target,built))
        
    def test_18_minblocka_builtbyname(self):
        target = awalipy.load("minblocka")
        built = awalipy.Automaton("ab","Z-min-plus")
        built.add_state('before')
        built.add_state('count a')
        built.add_state('after')
        built.set_transition('before','before','a','0')
        built.add_transition('before','before','b','0')
        built.set_transition('before','count a','b','0')
        built.add_transition("count a","count a",'a','2')
        built.add_transition("count a","count a",'a','1')
        built.set_transition('count a','after','b','0')
        built.add_transition('after','after','a','0')
        built.set_transition('after','after','b','0')
        built.set_initial('before', '0')
        built.add_initial('count a', '0')
        built.set_final('count a', '0')
        built.add_final('after', '0')
        self.assertTrue(awalipy.are_isomorphic(target,built))

    def test_19_gray_builtbyname(self):
        target = awalipy.load("gray")
        built = awalipy.Transducer(["01","01"])
        built.add_state('i')
        built.add_state('p')
        built.add_state('q')
        built.add_state('r')
        built.add_state('t')
        built.add_initial('i')
        built.set_initial('p')
        built.add_final('q')
        built.set_final('t')
        built.set_transition('i','i',['0','0'])
        built.set_transition('i','p',['1','1'])
        built.set_transition('p','t',['','1'])
        built.set_transition('p','q',['0','1'])
        built.set_transition('p','r',['1','0'])
        built.set_transition('q','r',['1','1'])
        built.set_transition('r','q',['1','1'])
        built.set_transition('q','q',['0','0'])
        built.set_transition('r','r',['0','0'])
        self.assertTrue(awalipy.are_isomorphic(target,built))

sys.stderr.write(
    "\n"
    +
    "=======================================================================\n"
    +
    "\tTest equivalence of methods for computing the minimal automaton.\n"
    +
    "=======================================================================\n"
);
suite = unittest.TestLoader().loadTestsFromTestCase(StateNamesTests)
runner = unittest.TextTestRunner(verbosity=2,failfast=True)
result = runner.run(suite)
if len(result.failures) > 0:
    exit(1)
