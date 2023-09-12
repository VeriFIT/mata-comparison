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

add_custom_target(check-cora DEPENDS copy-cora-tests cora cora-tester COMMAND ${CMAKE_CTEST_COMMAND} COMMENT "Launching test suite for 'cora' layer")

add_custom_target(copy-cora-tests COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

FILE(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/test-list.txt" count LIMIT_COUNT 1)

FOREACH(i RANGE 1 ${count})
  add_test(NAME cora-${i}
           COMMAND "${CMAKE_CURRENT_BINARY_DIR}/../extras/cora-tester"
                   "${CMAKE_CURRENT_BINARY_DIR}/../cora"
                   "${CMAKE_CURRENT_SOURCE_DIR}/test-list.txt"
                   "${i}"
           WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  )

ENDFOREACH(i)

# This allows tests to mostly run in parallel.
# To generate this list 
# `make cleanlib && make check-cora` and look at `~/.awali/otf`
add_dependencies(check-cora
  lal_char_b-automaton
  lal_char_b-context
  lal_char_b-derivation
  lal_char_b-determinize
  lal_char_b-eval
  lal_char_b_lal_char_b-are_equivalent
  lal_char_b-output
  lal_char_b-quotient
  lal_char_b-standard
  lan-lal_char-_b-context
  lan-lal_char-_b-proper
)

add_dependencies(check check-cora)

