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

# distutils: language = c++
import os, tempfile, subprocess
import sys, operator
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp cimport bool
from libcpp.map cimport map
from awalipy_purepython_extra import run_from_jupyter_notebook, _deprecated

cdef extern from "automaton.h" namespace "awali::version":
    string full

__version__ = full

cdef extern from "automaton.h" namespace "awali::version":
    string full

__version__ = full

cpdef dict _from_map_s_s(map[string, string] m):
    pymap = {}
    for it in m: #Iterate through the c++ map
        pymap[str(it.first)] = str(it.second)
    return pymap

cdef char _str_to_char(str len1_str):
    if len(len1_str) != 1:
        raise ValueError("Expected str of length 1: "+ len1_str)
    else:
        return ord(len1_str[0])

cdef vector[unsigned] _list_to_vector_of_unsigned(list l):
    cdef vector[unsigned] v;
    for i in l:
        v.push_back(i)
    return v

cdef extern from "<set>" namespace "std" nogil:
    cdef cppclass set[T]:
        set() except +
        set(set&) except +

#cdef extern from "<memory>" namespace "std":
    #cdef cppclass shared_ptr[T]:
        #shared_ptr()
        #shared_ptr(T*p)
        #shared_ptr(const shared_ptr&)
        #long use_count()
        #T operator*()

#cdef extern from "<iostream>" namespace "std":
    #cdef cppclass ostream:
        #ostream& write(const char*, int) except +

#cdef extern from "<sstream>" namespace "std":
    #cdef cppclass ostringstream:
        #ostream& operator<<(any&) except +
        #ostream& operator<<(ostream& (*pf)(ostream&)) except +
        #string str()

#cdef extern from "<iostream>" namespace "std::ios_base":
    #cdef cppclass open_mode:
        #pass
    #cdef open_mode binary

#cdef extern from "<memory>" namespace "std":
    #cdef cppclass shared_ptr[T]:
        #shared_ptr()
        #shared_ptr(T*p)
        #shared_ptr(const shared_ptr&)
        #long use_count()
        #T operator*()

#cdef extern from "<fstream>" namespace "std":
    #cdef cppclass ofstream(ostream):
        #ofstream(const char*) except +
        #ofstream(const char*, open_mode) except +


cdef extern from "automaton.h" namespace "awali::dyn":
    ctypedef unsigned int size_t
    ctypedef unsigned int transition_t
    ctypedef unsigned int state_t

    #cdef cppclass any[T]:
        #any(any)
        #any(string)
        #any(T)
        #ostream& operator<<(ostream& o, const any& a)


def _bad_arg_number_(str class_name, str method_name, list arg_nums):
    msg= ""
    if (class_name != ""):
        msg+= class_name+'.'
    msg+= method_name+'(...) takes exactly '
    sep=''
    for i in arg_nums[:-1]:
        msg+= sep
        sep= ', '
        msg+=str(i)
    if (len(arg_nums) >1):
        msg+= ' or '
    msg+= str(arg_nums[-1])+' arguments.'
    raise TypeError(msg)



ctypedef fused RatExp_or_Str_or_Aut:
    RatExp
    str
    Automaton



def _print_warning(s):
    print("[Warning]", s, file=sys.stderr, flush=True)
