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

foreach(VAL
        accessible
        add_path
        automaton
        compact
        derivation
        determinize
        eps
        eval
        factories
        filter
        global
        is_finite
        json
        lift
        minimize
        names
        output
        restriction
        tdc
        tuple
        weightsets
        lr_parse_number
       )

  #include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../..")
  # COMMENTED BY VM (26-01-2022)


  # ${CMAKE_CURRENT_SOURCE_DIR} equals  <smth>/awali/sttc/tests and we want to
  # include <smth>
  
  add_definitions(-DAWALI_AUTOMATA_DEPOSITORY="${AWALI_SHARE_PATH}/awali/automata/")
  # Changed by VM to be compatible with older versions of CMake (26-01-2022)
  
  add_executable(test-sttc-${VAL} EXCLUDE_FROM_ALL test-${VAL}.cc )

  add_test(build-sttc-${VAL} "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target test-sttc-${VAL})


  IF (CHECK_MEMORY_LEAK)
    add_test(${VAL} ${MEMORY_CHECK_COMMAND} ./test-sttc-${VAL})
  ELSE()
    add_test(${VAL} ./test-sttc-${VAL})
  ENDIF()

  set_tests_properties(${VAL} PROPERTIES FIXTURES_REQUIRED build-sttc-${VAL})
  set_tests_properties(build-sttc-${VAL} PROPERTIES FIXTURES_SETUP build-sttc-${VAL})

endforeach(VAL)
add_custom_target(check-sttc COMMAND "${CMAKE_CTEST_COMMAND}" COMMENT "Launching test suite for 'sttc' layer")


#add_dependencies(check check-sttc)

