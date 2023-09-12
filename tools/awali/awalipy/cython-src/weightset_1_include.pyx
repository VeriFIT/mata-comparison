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
  cppclass weightset_t:
    string zero() except +
    string one() except +
    string mul_weights(string l, string r) except +
    string add_weights(string l, string r) except +
    string div_weights(string l, string r) except +
    string name() except +
  weightset_t make_simple_weightset(string s) except +
