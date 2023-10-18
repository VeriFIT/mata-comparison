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

foreach(F 
        linked_hashmap
        json_parser
        priority
        is_iterable
        enumerate_enums
       )
  add_executable(test-common-${F} EXCLUDE_FROM_ALL test-${F}.cc)

  add_test(build-common-${F} "${CMAKE_COMMAND}" 
                      --build ${CMAKE_BINARY_DIR} 
                      --target test-common-${F})

  set_tests_properties(build-common-${F} PROPERTIES FIXTURES_SETUP build-common-${F})

  IF (CHECK_MEMORY_LEAK)
    add_test(${F} ${MEMORY_CHECK_COMMAND} ./test-common-${F})
  ELSE()
    add_test(${F} ./test-common-${F})
  ENDIF()

  set_tests_properties(${F} PROPERTIES FIXTURES_REQUIRED build-common-${F})
endforeach()

add_executable(test-common-version EXCLUDE_FROM_ALL test-version.cc)


add_test(build-common-version "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target test-common-version)
set_tests_properties(build-common-version PROPERTIES FIXTURES_SETUP build-common-version)
add_test(common-version ./test-common-version "${VERSION_MAJOR}" "${VERSION_MINOR}" "${VERSION_PATCH}" "${VERSION_SUFFIX}")
set_tests_properties(common-version PROPERTIES FIXTURES_REQUIRED build-common-version)
# include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../..")





