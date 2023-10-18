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

from __future__ import print_function
include "common.pyx"
include "printing_callback_setup.pyx"
include "weightset_1_include.pyx"
include "weightset_2_class.pyx"
include "basic_automaton_1_include.pyx"
include "basic_automaton_2_class.pyx"
include "basic_automaton_3_function.pyx"
include "automaton_1_include.pyx"
include "automaton_2_class.pyx"
include "automaton_3_function.pyx"
include "transducer_1_include.pyx"
include "transducer_2_class.pyx"
include "transducer_3_function.pyx"
include "ratexp_1_include.pyx"
include "ratexp_2_class.pyx"
include "ratexp_3_function.pyx"
include "fused_Automaton_or_Transducer.pyx"

_print_warning("The python module awalipy relies on compilation executed \"on-the-fly\" depending on the context (type of weights, of labels, etc.). As a result, the very first call to a given function in a given context may take up to one minute. ")

_setup_printing_stream()
