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

foreach(VAL 
        is_finite 
        restriction 
        tuple
       )

  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../..")
  # ${CMAKE_CURRENT_SOURCE_DIR} equals  <smth>/awali/dyn/tests and we want to
  # include <smth>

  add_executable(test-${VAL} EXCLUDE_FROM_ALL test-${VAL}.cc )
  #target_link_libraries(test-${VAL} dl)

  add_test(build-${VAL} "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target test-${VAL})

  add_test(${VAL} ./test-${VAL})


  set_tests_properties(${VAL} PROPERTIES FIXTURES_REQUIRED build-${VAL})
  set_tests_properties(build-${VAL} PROPERTIES FIXTURES_SETUP build-${VAL})

endforeach(VAL)
add_custom_target(check-sttc COMMAND "${CMAKE_CTEST_COMMAND}" COMMENT "Launching test suite for 'sttc' layer")


#add_dependencies(check check-sttc)

