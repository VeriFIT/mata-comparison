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



foreach(VAL io-file io-stream make-url-validator)

  #include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../..")
  # ${CMAKE_CURRENT_SOURCE_DIR} equals  <smth>/awali/dyn/tests and we want to
  # include <smth>

#   add_executable(test-example-${VAL} ${VAL}.cc )
#   target_link_libraries(test-example-${VAL} dl)
#   target_link_libraries(test-example-${VAL} awalidyn)

  add_test(NAME make-example-${VAL} 
           COMMAND make "${VAL}" "DST_DIR=${CMAKE_CURRENT_BINARY_DIR}" "CMAKE_PATH=${CMAKE_BINARY_DIR}"
           WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
          )

   set_tests_properties(make-example-${VAL} PROPERTIES FIXTURES_SETUP make-example-${VAL})
endforeach(VAL)
  
  
#Example programs that are run without arguments 
foreach(VAL make-url-validator)
 add_test(example-${VAL} ./${VAL})
 set_tests_properties(example-${VAL} PROPERTIES FIXTURES_REQUIRED make-example-${VAL})
endforeach(VAL)

#Example programs that are run with one argument of type automata.
foreach(VAL io-file io-stream )
  add_test(example-${VAL} ./${VAL} "${CMAKE_SOURCE_DIR}/awali/automata/a1.json")
  set_tests_properties(example-${VAL} PROPERTIES FIXTURES_REQUIRED make-example-${VAL})
endforeach(VAL)
