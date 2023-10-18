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

# cmake_minimum_required(VERSION 2.8.12)
# project(Cora CXX)
# 
# include(CheckCXXCompilerFlag)
# check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
# if(COMPILER_SUPPORTS_CXX11)
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# else()
#         check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
#         if(COMPILER_SUPPORTS_CXX0X)
#                 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#         else()
#                 message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#         endif()
# endif()
# 
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I.")
# # set (CMAKE_SHARED_LINKER_FLAGS "-rdynamic")
# 
# SET(CMAKE_BUILD_TYPE DEBUG)
# SET(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O0")

find_package(Curses)

IF(NOT "${CURSES_FOUND}")
  MESSAGE(FATAL_ERROR "Could not find NCurses library, required to compile cora.  You may disable Cora compilation by adding option -DCORA=FALSE to the cmake command.")
ENDIF()

add_executable(cora 
  edit/area.cc 
  edit/bless.cc 
  edit/editter.cc 
  edit/input.cc
  edit/queued_ostream.cc
  edit/sentence.cc
  edit/tip.cc
  edit/token.cc 
  edit/transition_pack.cc 
  edit/validator.cc 
  cora.cc)
target_link_libraries(cora dl)
target_link_libraries(cora awalidyn)
target_link_libraries(cora ncurses)

add_executable(__cora_autocompletion autocompletion.cc)
target_link_libraries(__cora_autocompletion dl)
target_link_libraries(__cora_autocompletion awalidyn)
include_directories("..")

IF (TARGET check)
ELSE()
  add_custom_target(check)
ENDIF()

IF (NOT DEFINED FORCE_INSTALL_IN_PREFIX)
  SET(FORCE_INSTALL_IN_PREFIX "FALSE")
ENDIF()

add_subdirectory(extras)
add_subdirectory(tests)
  



################################################################################
##                           INSTALLATION COMMANDS                            ##
################################################################################


# CORA INSTALL PREFIX
IF (NOT DEFINED CORA_INSTALL_PREFIX)
  SET(CORA_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
ENDIF()


## BINARIES ####################################################################
IF (NOT DEFINED CORA_BINARY_INSTALL_DIR) 
  SET(CORA_BINARY_INSTALL_DIR "${CORA_INSTALL_PREFIX}/bin")
ENDIF()

install( TARGETS cora DESTINATION "${CORA_BINARY_INSTALL_DIR}" )
install( TARGETS __cora_autocompletion DESTINATION "${CORA_BINARY_INSTALL_DIR}" )


## BASH COMPLETION INSTALL #####################################################
IF (NOT DEFINED CORA_BASH_COMPL_INSTALL_DIR)
  IF ("${FORCE_INSTALL_IN_PREFIX}")
    SET(CORA_BASH_COMPL_INSTALL_DIR "IN_PREFIX")
  ELSE()
    SET (CORA_BASH_COMPL_INSTALL_DIR "AUTO")
  ENDIF()
ENDIF()
 
IF ("${CORA_BASH_COMPL_INSTALL_DIR}" STREQUAL "IN_PREFIX")
  SET (CORA_BASH_COMPL_INSTALL_DIR "${CORA_INSTALL_PREFIX}/etc/bash_completion.d")
ENDIF()

IF ( "${CORA_BASH_COMPL_INSTALL_DIR}" STREQUAL "AUTO")
  find_file( BASH_COMPLD_PATH
               NAMES "bash_completion.d"
               HINTS "/" "/opt" "/opt/local" "/usr/local" 
               PATH_SUFFIXES "etc" )
  set(CORA_BASH_COMPL_INSTALL_DIR "${BASH_COMPLD_PATH}")
ENDIF()

IF ( "${CORA_BASH_COMPL_INSTALL_DIR}" STREQUAL "BASH_COMPLD_PATH-NOTFOUND" )
  message( WARNING "Could not find 'bash_completion.d' directory. Bash-completion support for cora will not be installed. Use option -DCORA_BASH_COMPL_INSTALL_DIR=<path> to provide custom path." )
ELSE()
  install(
    FILES "${CMAKE_SOURCE_DIR}/cora/autocompletion-cora.bash" 
    DESTINATION "${CORA_BASH_COMPL_INSTALL_DIR}" )
ENDIF ()

## ZSH COMPLETION INSTALLATION #################################################
IF (NOT DEFINED CORA_ZSH_COMPL_INSTALL_DIR)
  IF ("${FORCE_INSTALL_IN_PREFIX}")
    SET(CORA_ZSH_COMPL_INSTALL_DIR "IN_PREFIX")
  ELSE()
    SET (CORA_ZSH_COMPL_INSTALL_DIR "AUTO")
  ENDIF()
ENDIF()
 
IF ("${CORA_ZSH_COMPL_INSTALL_DIR}" STREQUAL "IN_PREFIX")
  SET (CORA_ZSH_COMPL_INSTALL_DIR "${CORA_INSTALL_PREFIX}/share/zsh/site-functions")
ENDIF()



IF ("${CORA_ZSH_COMPL_INSTALL_DIR}" STREQUAL "AUTO")
  find_file( SITEFUNC_PATH
            NAMES "site-functions"
            HINTS "/etc" "/usr/local/share" "/usr/share" "/usr" "/opt" "/opt/local"
            PATH_SUFFIXES "zsh" )
  set(CORA_ZSH_COMPL_INSTALL_DIR "${SITEFUNC_PATH}")
ENDIF()

IF( "${CORA_ZSH_COMPL_INSTALL_DIR}" STREQUAL "INSTALL SITEFUNC_PATH-NOTFOUND" )
  message( WARNING "Could not find 'site-functions' directory.  Zsh-completion support for cora will not be installed. Use option -DCORA_ZSH_COMPL_INSTALL_DIR=<path> to provide custom path." )
else()
  install(
    FILES "${CMAKE_SOURCE_DIR}/cora/autocompletion-cora.zsh"
    DESTINATION "${CORA_ZSH_COMPL_INSTALL_DIR}"
    RENAME "_cora"
  )
#  install(CODE "execute_process(COMMAND ${CMAKE_COMMAND}
#      -D \"SOURCE=${CMAKE_SOURCE_DIR}/cora/autocompletion-cora.zsh\"
#      -D \"DESTINATION=${CORA_ZSH_COMPL_INSTALL_DIR}\"
#      -D \"MANIFEST=${CMAKE_CURRENT_BINARY_DIR}/tmp.txt\"
#      -P \"${CMAKE_CURRENT_SOURCE_DIR}/CMakeScripts/install_zsh_comp.cmake\")")

#  install(CODE "file(READ \"${CMAKE_CURRENT_BINARY_DIR}/tmp.txt\" CONTENT)")
#  install(CODE "SET (CMAKE_INSTALL_MANIFEST_FILES \"\${CMAKE_INSTALL_MANIFEST_FILES};\${CONTENT}\")")
endif ()


###############################################################################
##                          UNINSTALLATION COMMANDS                          ##
###############################################################################

if(TARGET uninstall)
else()
  add_custom_target ( uninstall
    COMMAND ${CMAKE_COMMAND}
        -D "DATA_TO_REMOVE=${CMAKE_BINARY_DIR}/install_manifest.txt"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeScripts/remove_data.cmake"
  )
endif()
