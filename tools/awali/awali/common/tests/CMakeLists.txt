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

foreach(F 
        linked_hashmap
        json_parser
        priority
        is_iterable
       )
  add_executable(test-${F} EXCLUDE_FROM_ALL test-${F}.cc)

  add_test(build-${F} "${CMAKE_COMMAND}" 
                      --build ${CMAKE_BINARY_DIR} 
                      --target test-${F})

  set_tests_properties(build-${F} PROPERTIES FIXTURES_SETUP build-${F})

  add_test(${F} ./test-${F})

  set_tests_properties(${F} PROPERTIES FIXTURES_REQUIRED build-${F})
endforeach()

add_executable(test-version EXCLUDE_FROM_ALL test-version.cc)


add_test(build-version "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target test-version)
set_tests_properties(build-version PROPERTIES FIXTURES_SETUP build-version)
add_test(version ./test-version "${VERSION_MAJOR}" "${VERSION_MINOR}" "${VERSION_PATCH}" "${VERSION_SUFFIX}")
set_tests_properties(version PROPERTIES FIXTURES_REQUIRED build-version)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../..")





