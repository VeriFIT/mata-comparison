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

MESSAGE(STATUS "Awalipy (1/3) - Start")
MESSAGE(STATUS "Awalipy (2/3) - Defering to Python module 'distutils'")
IF("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
  EXECUTE_PROCESS( COMMAND "${PYTHON_BIN}" setup.py build_ext install --install-lib .
                   WORKING_DIRECTORY "${WORKING_DIR}"
                   RESULT_VARIABLE X
                   ERROR_VARIABLE Y)
  IF(X)
    MESSAGE(FATAL_ERROR ${Y})
  ENDIF()
ELSE()
  EXECUTE_PROCESS( COMMAND "${PYTHON_BIN}" setup.py build_ext install --install-lib .
                   WORKING_DIRECTORY "${WORKING_DIR}"
                   OUTPUT_FILE "cython.log"
                   ERROR_FILE "cython.log"
                   RESULT_VARIABLE X)
  IF(X)
    MESSAGE(FATAL_ERROR "Details in file awalipy/cython.log .")
  ENDIF()
ENDIF()

MESSAGE(STATUS "Awalipy (3/3) - Done")
