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

import awali as vr
import sys

def key(set):
    #This is particularly inefficient.
    return str(sorted(set));




def determinize (A) :
    #B will be the DFA resulting from the power-set construction applied to A
    B = vr.Automaton(A.alphabet())

    #Maps a  (set of states of A)  to a  (state of B)
    set_to_state = dict()
# ================================================ Initial state computation ==
    init_state_of_B = B.add_state()
    B.set_initial(init_state_of_B)
    init_state_of_B_as_set = set()
    for p in A.initial_states() :
        init_state_of_B_as_set.add(p)
    set_to_state[key(init_state_of_B_as_set)]= init_state_of_B
# ================================================================ Main loop ==
    #The state-set of B as a list of (states of A)
    list_of_Bstates_as_sets = [init_state_of_B_as_set]
    for cur_Bstate_as_set in list_of_Bstates_as_sets:
        cur_Bstate = set_to_state[key(cur_Bstate_as_set)]
        for letter in A.alphabet() :
            new_Bstate_as_set = set()
            for cur_Astate in cur_Bstate_as_set :
                for q in A.successors(cur_Astate, letter) :
                    new_Bstate_as_set.add(q)

            #If the new state has never been reached before, it is added to B
            #and the appropriate variables are updated; otherwise the
            #corresponding state of B is computing using the map set_to_state.
            if new_Bstate_as_set not in list_of_Bstates_as_sets :
                new_Bstate = B.add_state()
                list_of_Bstates_as_sets.append(new_Bstate_as_set)
                set_to_state[key(new_Bstate_as_set)] = new_Bstate
            else :
                new_Bstate = set_to_state[key(new_Bstate_as_set)]

            B.set_transition(cur_Bstate, new_Bstate, letter)
# ================================================= Final states computation ==
    for cur_Bstate_as_set in list_of_Bstates_as_sets:
        for cur_Astate in cur_Bstate_as_set:
            if A.is_final(cur_Astate):
                B.set_final(set_to_state[key(cur_Bstate_as_set)])
                break;
    return B


class Determinizer(vr.Automaton):
    def __init__(self, source):
        vr.Automaton.__init__(self,source.alphabet())
        self.source = source
        self.states_as_sets = list()
        self.set_to_state = dict()
        self.states_to_visit_as_sets = list()
        self.remaining_letters = list()
        self.cur_state_as_set = None

        init_state_as_set = set()
        for q in self.source.states():
            if self.source.is_initial(q):
                init_state_as_set.add(q)
        self.states_as_sets.append(init_state_as_set)
        self.states_to_visit_as_sets.append(init_state_as_set)
        init_state = self.add_state(init_state_as_set)
        self.set_initial(init_state)
        self.set_to_state[key(init_state_as_set)] = init_state

    def add_state(self, new_state_as_set):
        name=''
        sep='{'
        for q in new_state_as_set:
            name+= sep+str(q)
            sep= ', '
        name+= '}'
        new_state= vr.Automaton.add_state(self,name)
        for q in new_state_as_set:
            if (self.source.is_final(q)):
                self.set_final(new_state)
                break;
        return new_state;

    def next(self):
        if (not self.has_work_left()):
            return
        if (len(self.remaining_letters) > 0):
            cur_letter= self.remaining_letters.pop()
        else:
            self.cur_state_as_set = self.states_to_visit_as_sets.pop()
            self.remaining_letters = list(self.alphabet())
            cur_letter= self.remaining_letters.pop(0)
        new_state_as_set = set()
        for cur_source_state in self.cur_state_as_set :
            for q in self.source.successors(cur_source_state, cur_letter) :
                new_state_as_set.add(q)
        if new_state_as_set not in self.states_as_sets:
            self.states_as_sets.append(new_state_as_set)
            new_state = self.add_state(new_state_as_set)
            self.states_to_visit_as_sets.append(new_state_as_set)
            self.set_to_state[key(new_state_as_set)] = new_state
        else:
            new_state = self.set_to_state[key(new_state_as_set)]
        cur_state = self.set_to_state[key(self.cur_state_as_set)]
        self.set_transition(cur_state, new_state, cur_letter)

    def has_work_left(self):
        return (len(self.states_to_visit_as_sets) + len(self.remaining_letters) > 0)

    def result(self):
        while (self.has_work_left()):
            self.next()
        return self;

if __name__ == '__main__':
    if (len(sys.argv) < 2) or (len(sys.argv) > 3) :
        print "Usage: determinize <in> [out]"
    else :
        A = vr.load(sys.argv[1])
        B = determinize(A)
        if (len(sys.argv) == 3) :
            B.save(sys.argv[2])
