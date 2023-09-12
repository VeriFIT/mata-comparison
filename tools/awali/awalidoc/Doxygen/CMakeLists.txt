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

find_package(Doxygen)

add_executable(awalidoc-escape-md-bin EXCLUDE_FROM_ALL escape-md.cc)


SET(CMAKE_PUREMD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  
  add_custom_target(
    "doxygen_puremd"
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/awalidoc-escape-md-bin 
            "${CMAKE_PUREMD_DIR}"
    DEPENDS awalidoc-escape-md-bin
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating pure markdown Doxygen files."
  )
  
  add_custom_target(
      doxygen
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      DEPENDS doxygen_puremd
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
    )

   add_custom_target(
      upload-doxygen
      DEPENDS doxygen
      COMMAND "${SCRIPT_DIR}/upload_doxygen.sh" "${VERSION_MAJOR}.${VERSION_MINOR}"
    )

ELSE()
  MESSAGE(WARNING "Could not find Doxygen. Possibility to generate html documentation will be disable.")
ENDIF(DOXYGEN_FOUND)

