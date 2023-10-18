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


ctypedef void (*CallBackFunctionType)(string);

cdef extern from "automaton.h" namespace "awali::py":
    void setup_callback_stream(CallBackFunctionType warning, CallBackFunctionType error);

def py_callback_function(s):
    sys.stdout.flush() # I have no clue why I need that line (VM)
    _print_warning(s)    
    sys.stdout.flush()

cdef void c_callback_function(string s):
    py_callback_function(s);

def _setup_printing_stream():
    setup_callback_stream(&c_callback_function, &c_callback_function)

