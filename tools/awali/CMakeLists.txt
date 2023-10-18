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

cmake_minimum_required(VERSION 3.1.0)
set(VRNAME "awali")
set(NAME_DISPLAY "Awali")
set(NAME_DESC "a library for weighted automata and transducers")


# Standard CMake variable
set(VERSION_MAJOR "2")
set(VERSION_MINOR "3")
set(VERSION_PATCH "0")
set(VERSION_SUFFIX "")
if ("" STREQUAL "${VERSION_SUFFIX}")
  set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
else()
  set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}-${VERSION_SUFFIX}")
endif() 

message(STATUS "CMake configuration for ${NAME_DISPLAY}, version ${VERSION}")

project("${NAME_DISPLAY}" LANGUAGES CXX)

# Extra variables 
set(AUTHORS "Sylvain Lombardy" "Victor Marsault" "Jacques Sakarovitch")
string(REPLACE ";" "\",\"" AUTHOR_CPP_LIST "\"${AUTHORS}\"")
set(LICENSE "GPLv3")
set(COPYRIGHT_YEARS "2016" "2022")

string(REPLACE ";" "\",\"" YEARS_CPP_LIST "\"${COPYRIGHT_YEARS}\"")
string(REPLACE ";" " and " AUTHOR_BIB_LIST "${AUTHORS}")
list(GET COPYRIGHT_YEARS 1 YEAR_BIB)


# Awalipy variables
set(AWALIPY_AUTHORS "${AUTHOR_BIB_LIST}")
set(AWALIPY_HOMEPAGE "http://vaucanson-project.org/Awali/")
set(AWALIPY_SUMMARY "Python frontend for ${NAME_DISPLAY}, ${NAME_DESC}")

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
	check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()
endif()

if (NOT DEFINED MEMORY_CHECK_COMMAND)
  set (MEMORY_CHECK_COMMAND valgrind --leak-check=full --error-exitcode=1 --log-file=/tmp/valgrind-%p-log.txt --)
endif()

if (NOT DEFINED CHECK_MEMORY_LEAK)
  SET(CHECK_MEMORY_LEAK "FALSE")
endif()


# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I.")
# set (CMAKE_SHARED_LINKER_FLAGS "-rdynamic")

SET(SCRIPT_DIR "${CMAKE_SOURCE_DIR}/scripts")


#set(CMAKE_CONFIGURATION_TYPES "Debug" "RelWithDebInfo" "Release")
# We cannot use CMAKE_CONFIGURATION_TYPES due to PY
IF (CMAKE_CONFIGURATION_TYPES)
  set(FATAL_ERROR "Due to the way this CMakeLists.txt is made to handle multiple package (awali, cora, awalipy) it cannot handle configuration types")
ENDIF()

# We use only CMAKE_CXX_FLAGS to be able to give it to other sources
SET(CMAKE_CXX_FLAGS_DEBUG "")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
SET(CMAKE_CXX_FLAGS_RELEASE "")

IF (NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
  IF(DEBUG)
    SET(CMAKE_BUILD_TYPE "Debug")
  ENDIF()
  IF(TESTING)
    SET(CMAKE_BUILD_TYPE "RelWithDebInfo")
  ENDIF()
ENDIF()

IF (NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()

STRING(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)

set(WARNINGS "-Wnull-dereference -Wnon-virtual-dtor -Wpedantic -Wall -Wextra -Wsuggest-override -Wdisabled-optimization -Wformat=2 -Wcast-align -Wcast-qual -Wfatal-errors -Wshadow=local")
# complete -Wshadow has too many false positive on constructors
# -Wmaybe-unitialized has many false positive: could need: -Wno-maybe-uninitialized

IF("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
  set(CMAKE_BUILD_TYPE "Debug")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS} -g3")
  IF(NOT CXX_OPTIM_LEVEL)
    SET(CXX_OPTIM_LEVEL "g")
  ENDIF()
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "relwithdebinfo")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS} -g3")
  IF(NOT CXX_OPTIM_LEVEL)
    SET(CXX_OPTIM_LEVEL "3")
  ENDIF()
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "release")
  set(CMAKE_BUILD_TYPE "Release")
  ## option -w disable all warnings
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -DNDEBUG")
  IF(NOT CXX_OPTIM_LEVEL)
    SET(CXX_OPTIM_LEVEL "3")
  ENDIF()
ELSE()
  
  MESSAGE(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}. Supported values: Debug, RelWithDebInfo, Release.")
ENDIF()


MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "CXX Optimization flag: -O${CXX_OPTIM_LEVEL}")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O${CXX_OPTIM_LEVEL}")

#IF("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  MESSAGE(STATUS "Full CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
#ENDIF()

# if("${CMAKE_BUILD_TYPE}" STREQUAL "Test")
#         include(CTest)
#   SET(CMAKE_BUILD_TYPE COVERAGE)
#   set( CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG}  --coverage -ftest-coverage -fprofile-arcs" )
# 
# endif()


# if(POLICY CMP0042)
#     cmake_policy(SET CMP0042 OLD)
# endif()

enable_testing()

find_package(PkgConfig)

set( CXX_COMPILER "${CMAKE_CXX_COMPILER}" )
set( CXX_FLAGS "${CMAKE_CXX_FLAGS}" )

# DEPRECATED?
#set( AWALI_PATH "${CMAKE_SOURCE_DIR}" )
#set( MAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}" )

set( AWALI_SHARE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" )
set( AWALI_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" )
set( AWALI_DYNLIB_PATH "${CMAKE_CURRENT_BINARY_DIR}/awali" )
set( AWALI_MODULE_SUFFIX ".${VRNAME}/modules")
set( AWALI_MODULE_PATH "$ENV{HOME}/.${VRNAME}/modules")

IF(NOT DEFINED AWALI_TMP_DIR)
  set( AWALI_TMP_DIR "/tmp/${VRNAME}" )
ENDIF()
#THIS WAS FOR DEV AWALI_TMP_DIR $ENV{HOME}/.${VRNAME}/tmp)
set( AWALI_DYN_CONFIG_HH_PATH "${CMAKE_BINARY_DIR}" )


#   IF (NOT DEFINED CMAKE_INSTALL_DIR)
#     set(INSTALL_DIR "/usr/local")
# #     message(STATUS "Installation of C++ layers will be in ${INSTALL_DIR}. Use option -DINSTALL_DIR=<path> to supersede.")
#   ELSE()
#     set(INSTALL_DIR "${CMAKE_INSTALL_DIR}")
#   ENDIF()
# ENDIF()
# 
# set( AWALI_INST_SHARE_PATH "${INSTALL_DIR}/share" )
# set( AWALI_INST_INCLUDE_PATH "${INSTALL_DIR}/include" )
# # set( AWALI_INST_SOURCE_PATH "${INSTALL_DIR}/share/${VRNAME}/src" )
# # set( AWALI_INST_CORA_PATH "${INSTALL_DIR}/bin" )
# set( AWALI_INST_DYNLIB_PATH "${INSTALL_DIR}/lib" )
# set( AWALI_INST_OTF_LIB_PATH "${AWALI_INST_DYNLIB_PATH}/${VRNAME}_otf" )


add_custom_target(check)


IF (NOT DEFINED DYN)
  SET(DYN "TRUE")
ENDIF()


IF(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${VRNAME}")
  include_directories("${CMAKE_CURRENT_BINARY_DIR}")
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
  add_subdirectory(awali)
ENDIF()

IF (NOT DEFINED CORA)
  SET(CORA "TRUE")
ENDIF()
IF(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cora")
  IF("${DYN}" AND "${CORA}")
    add_subdirectory(cora)
  ENDIF()
ENDIF()

IF (NOT DEFINED DOCUMENTATION)
  SET(DOCUMENTATION "TRUE")
ENDIF()
IF(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/awalidoc")
  IF(DOCUMENTATION)
    add_subdirectory(awalidoc)
  ENDIF()
ENDIF()

IF (NOT DEFINED FORCE_INSTALL_IN_PREFIX)
  SET(FORCE_INSTALL_IN_PREFIX "FALSE")
ENDIF()

IF (NOT DEFINED PY)
  SET(PY "TRUE")
ENDIF()

IF (NOT DEFINED DOT_BIN)
  find_program(DOT_BIN "dot")
  IF(${DOT_BIN} STREQUAL DOT_BIN-NOTFOUND )
    message (WARNING "Could not find \"dot\" binary (sometimes distributed as part as graphviz).  Display of automaton will be disabled.  If this binary is installed but was not found by automatic detection, you may specify its location with cmake option: -DDOT_BIN=<binary location>")
    UNSET(DOT_BIN)
  ENDIF()
ENDIF()



################################################################################
##                      COMPILATION OF PYTHON FROM CMAKE                      ##
##             (Done here in order not to pollute awalipy directory)          ##
################################################################################
IF(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/awalipy")
  IF("${DYN}" AND "${PY}")
    IF (NOT DEFINED PYTHON_BIN)
      find_program(PYTHON_BIN NAMES "python" "python3")
    ENDIF()
    IF( ${PYTHON_BIN} STREQUAL PYTHON_BIN-NOTFOUND )
        MESSAGE (FATAL_ERROR "Could NOT find \"python\" nor \"python3\" binary, required to compile Awalipy.  You may disable the compilation of Awalipy by adding option -DPY=FALSE to the cmake command.  If Python is installed in your system but was not found by automatic detection, you may specify it with cmake option -DPYTHON_BIN=<binary location>")
   ELSE()
      message (STATUS "Found Python Binary: ${PYTHON_BIN}")
      execute_process(
        COMMAND ${PYTHON_BIN} "-c" "import sys; print (str(sys.version_info.major)+'.'+str(sys.version_info.minor)+'.'+str(sys.version_info.micro))"
        RESULT_VARIABLE PYTHON_NOT_FOUND
        OUTPUT_VARIABLE PYTHON_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      IF (PYTHON_NOT_FOUND)
        message (FATAL_ERROR "${PYTHON_BIN} does not seem to be a valid python binary.")
      ELSE()
        message (STATUS "Python version: ${PYTHON_VERSION}")
        IF ("${PYTHON_VERSION}" VERSION_LESS "3" )
          message(WARNING "Python version is <3, which is not recommended for Awali.  If the wrong Python binary was found by automatic detection, you may specify it with cmake option -DPYTHON_BIN=<binary location>")
        ENDIF()
      ENDIF()
    ENDIF()


    IF (DEFINED CYTHON_BIN)
      MESSAGE(STATUS "CYTHON_BIN is no longer used.  Check that your python binary (currently $PYTHON_BIN, set up with PYTHON_BIN cmake variable) can import Cython.")
    ENDIF()

    # Looking for Cython
    execute_process(
        COMMAND ${PYTHON_BIN} -B -c "import Cython; print (Cython.__version__)" 
        RESULT_VARIABLE CYTHON_NOT_FOUND
        OUTPUT_VARIABLE CYTHON_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    IF (CYTHON_NOT_FOUND)
      MESSAGE(FATAL_ERROR "No module Cython found (associated with ${PYTHON_BIN})." )
    ELSE()
      MESSAGE(STATUS "Cython version associated with ${PYTHON_BIN}: ${CYTHON_VERSION}")
    ENDIF()

    

#       find_program(CYTHON_BIN NAMES "cython" "cython3")
#     ENDIF()
# 
#     IF( ${CYTHON_BIN} STREQUAL CYTHON_BIN-NOTFOUND )
#         MESSAGE (FATAL_ERROR "Could not find \"cython\" nor \"cython3\" binary, required to compile Awalipy.  You may disable the compilation of Awalipy by adding option -DPY=FALSE to the cmake command.  If cython is installed in your system but was not found by automatic detection, you may specify it with cmake option -DCYTHON_BIN=... ")
#    ELSE()
#      message (STATUS "Found Cython Binary: ${CYTHON_BIN}")
#     ENDIF()

    set (PYTHON_DIR "awalipy")
    set (CYTHON_SUBDIR "cython-src") 
    set (CYTHON_DIR "${PYTHON_DIR}/${CYTHON_SUBDIR}")
    set (CYTHON_BUILD_SUBDIR "build")
    set (BRIDGE_DYN2PY_DIR "${PYTHON_DIR}/bridge-to-dyn")

    #
    #
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/CMakeScripts/setup.py.in" "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_DIR}/setup.py")

    #
    #     add_custom_command(
    #             OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/cython-src/.touch_to_force_python_layer_recompilation.pyx
    # #             DEPENDS awalidyn ${PYTHON_EXTRA_DEP} ${SETUP_PY_IN}
    # #             COMMAND ${CMAKE_COMMAND} -E touch
    # #                     ${CMAKE_CURRENT_SOURCE_DIR}/cython-src/.touch_to_force_python_layer_recompilation.pyx
    # #             COMMENT "Forcing python library compilation"
    # #     )
    #
    #
    file(GLOB CYTHON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${CYTHON_DIR}/*.pyx")
    SET(CYTHON_SOURCES ${CYTHON_SOURCES} 
                       "${CMAKE_CURRENT_SOURCE_DIR}/${CYTHON_DIR}/automaton.h"
                       "${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_DIR}/awalipy_purepython_extra.py")
    file(GLOB BRIDGE_DYN2PY_SOURCES
      ${CMAKE_CURRENT_SOURCE_DIR}/${BRIDGE_DYN2PY_DIR}/*hh)
   #message(CYTHON_SOURCES:${CYTHON_SOURCES})
   #message(BRDIGE2DYN:${BRIDGE_DYN2PY_SOURCES})
   set (AWALI_CPP_TARGET "${PYTHON_DIR}/${CYTHON_BUILD_SUBDIR}/${CYTHON_SUBDIR}/awalipy.cpp")
    add_custom_target(awalipy ALL DEPENDS "${AWALI_CPP_TARGET}")
    add_custom_command(
        OUTPUT "${AWALI_CPP_TARGET}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
          "${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_DIR}/awalipy_purepython_extra.py"
          "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_DIR}/"
        COMMAND "${CMAKE_COMMAND}" -E create_symlink
            "${CMAKE_CURRENT_SOURCE_DIR}/${CYTHON_DIR}/"
            "${CMAKE_CURRENT_BINARY_DIR}/${CYTHON_DIR}"
        COMMAND ${CMAKE_COMMAND}
            -D "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            -D "PYTHON_BIN=${PYTHON_BIN}"
            -D "WORKING_DIR=${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_DIR}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/CMakeScripts/compile_python.cmake"
        COMMAND ${CMAKE_COMMAND} -E touch "${AWALI_CPP_TARGET}"
        DEPENDS awalidyn ${CYTHON_SOURCES} ${BRIDGE_DYN2PY_SOURCES}
        COMMENT "Building Awalipy"
    )
        IF (NOT DEFINED AWALIPY_INSTALL_PREFIX)
          IF ("${FORCE_INSTALL_IN_PREFIX}")
            set(AWALIPY_INSTALL_PREFIX "IN_PREFIX")
          ELSE()
            set(AWALIPY_INSTALL_PREFIX "AUTO")
          ENDIF()
        ENDIF()

        IF ("${AWALIPY_INSTALL_PREFIX}" STREQUAL "IN_PREFIX")
          set(AWALIPY_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
        ENDIF()

        IF ("${AWALIPY_INSTALL_PREFIX}" STREQUAL "AUTO")
          SET (PYINSTOPTION "")
        ELSE()
          SET (PYINSTOPTION "--prefix=${AWALIPY_INSTALL_PREFIX}")
        ENDIF()

        install ( CODE "execute_process(COMMAND ${PYTHON_BIN} setup.py build_ext install ${PYINSTOPTION} --record install_manifest.txt WORKING_DIRECTORY \"${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_DIR}\")" )


    add_custom_target ( uninstall-python
      COMMAND ${CMAKE_COMMAND}
        -D "DATA_TO_REMOVE=${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_DIR}/install_manifest.txt"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/CMakeScripts/remove_data.cmake" )

    add_dependencies(uninstall uninstall-python)





    add_custom_target(check-py DEPENDS awalipy)
    foreach(VAL 
                automaton-determinize
                automaton-algo-access
                automaton-binary-op
                automaton-edition
                examples
                ratexp-navigation
                ratexp-minimal-automaton
                transducer-edition
                state-names
                )

        add_custom_target(test-py-${VAL}
                COMMAND ${PYTHON_BIN} -B "${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_DIR}/tests/${VAL}.py"
                WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_DIR}"
                DEPENDS awalipy
        )

        add_dependencies(check-py test-py-${VAL})
    endforeach(VAL)

    add_dependencies(check check-py)

    #
    #
    foreach(VAL "a1_det")
      add_custom_target(copy-${VAL}
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${PYTHON_DIR}/tests/${VAL}.json"
                                        "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_DIR}" )
    endforeach(VAL)
    
# ls ~/.awali/modules | sed 's:^lib\(.*\).so$:\1:' | sed 's:,:_:g' | xclip -selection c
    add_dependencies(test-py-automaton-determinize copy-a1_det
      lal_char_b-accessible
      lal_char_b-automaton
      lal_char_b-context
      lal_char_b-determinize
      lal_char_b-factories
      lal_char_b_lal_char_b-are_equivalent
      lal_char_b-quotient
    )
    
#     add_dependencies(test-py-automaton-algo-access
# 
# 
#     )

    add_dependencies(test-py-ratexp-navigation
      lal_char_b-context
      lal_char_b-determinize
      lal_char_b_lal_char_b-are_equivalent
      lal_char_b-quotient
      lal_char_b-standard
      lal_char_q_lal_char_q-are_equivalent
      lal_char_z-context
      lal_char_z-determinize
#       lal_char_z_lal_char_q-promotion
      lal_char_z-quotient
      lal_char_z-standard
      lan-lal_char-_b-context
      lan-lal_char-_b_lan-lal_char-_b-product
      lan-lal_char-_b-proper
      lan-lal_char-_z-context
      lan-lal_char-_z_lan-lal_char-_z-product
      lan-lal_char-_z-proper
    )
    
    add_dependencies(test-py-ratexp-minimal-automaton
      lal_char_b-context
      lal_char_b-derivation
      lal_char_b-determinize
      lal_char_b-proper
      lal_char_b-quotient
      lal_char_b-standard
      lal_int_b-automaton
      lal_int_b-context
      lal_int_b-derivation
      lal_int_b-determinize
      lal_int_b-proper
      lal_int_b-quotient
      lal_int_b-standard
      lan-lal_char-_b-quotient
      lan-lal_int-_b-quotient
    )

    add_dependencies(test-py-automaton-binary-op
      lal_char_b-automaton
      lal_char_b-context
      lal_char_b-determinize
      lal_char_b-eval
      lal_char_b_lal_char_b-are_equivalent
      lal_char_b_lal_char_b-join
      lal_char_b_lal_char_b-product
      lal_char_b_lal_char_b-promotion
      lal_char_b-quotient
      lal_char_b-ratexp
      lal_char_b-standard
    )

    add_dependencies(test-py-automaton-edition
      lal_char_b-automaton
      lal_char_b-context
      lal_char_c-automaton
      lal_char_c-context
      lal_char_f2-automaton
      lal_char_f2-context
      lal_char_nn7-automaton
      lal_char_nn7-context
      lal_char_noo-automaton
      lal_char_noo-context
      lal_char_pmax-automaton
      lal_char_pmax-context
      lal_char_q-automaton
      lal_char_q-context
      lal_char_r-automaton
      lal_char_r-context
      lal_char_z-automaton
      lal_char_z-context
      lal_char_zmax-automaton
      lal_char_zmax-context
      lal_char_zmin-automaton
      lal_char_zmin-context
      lal_char_zz7-automaton
      lal_char_zz7-context
      lal_char_zz99-automaton
      lal_char_zz99-context
    )

    add_dependencies(test-py-transducer-edition
      lat-lan-lal_char-_lan-lal_char--_b-automaton
      lat-lan-lal_char-_lan-lal_char--_b-context
      lat-lan-lal_char-_lan-lal_char--_b-transducer
      lat-lan-lal_char-_lan-lal_char--_f2-automaton
      lat-lan-lal_char-_lan-lal_char--_f2-context
      lat-lan-lal_char-_lan-lal_char--_f2-transducer
      lat-lan-lal_char-_lan-lal_char--_pmax-automaton
      lat-lan-lal_char-_lan-lal_char--_pmax-context
      lat-lan-lal_char-_lan-lal_char--_pmax-transducer
      lat-lan-lal_char-_lan-lal_char--_q-automaton
      lat-lan-lal_char-_lan-lal_char--_q-context
      lat-lan-lal_char-_lan-lal_char--_q-transducer
      lat-lan-lal_char-_lan-lal_char--_r-automaton
      lat-lan-lal_char-_lan-lal_char--_r-context
      lat-lan-lal_char-_lan-lal_char--_r-transducer
      lat-lan-lal_char-_lan-lal_char--_c-automaton
      lat-lan-lal_char-_lan-lal_char--_c-context
      lat-lan-lal_char-_lan-lal_char--_c-transducer
      lat-lan-lal_char-_lan-lal_char--_z-automaton
      lat-lan-lal_char-_lan-lal_char--_z-context
      lat-lan-lal_char-_lan-lal_char--_zmax-automaton
      lat-lan-lal_char-_lan-lal_char--_zmax-context
      lat-lan-lal_char-_lan-lal_char--_zmax-transducer
      lat-lan-lal_char-_lan-lal_char--_zmin-automaton
      lat-lan-lal_char-_lan-lal_char--_zmin-context
      lat-lan-lal_char-_lan-lal_char--_zmin-transducer
      lat-lan-lal_char-_lan-lal_char--_z-transducer
      lat-lan-lal_char-_lan-lal_char--_zz7-automaton
      lat-lan-lal_char-_lan-lal_char--_zz7-context
      lat-lan-lal_char-_lan-lal_char--_zz7-transducer
      lat-lan-lal_char-_lan-lal_char--_zz99-automaton
      lat-lan-lal_char-_lan-lal_char--_zz99-context
      lat-lan-lal_char-_lan-lal_char--_zz99-transducer
      lat-lan-lal_char-_lan-lal_char--_nn7-automaton
      lat-lan-lal_char-_lan-lal_char--_nn7-context
      lat-lan-lal_char-_lan-lal_char--_nn7-transducer
      lat-lan-lal_char-_lan-lal_char--_noo-automaton
      lat-lan-lal_char-_lan-lal_char--_noo-context
      lat-lan-lal_char-_lan-lal_char--_noo-transducer
    )
    
    add_dependencies(test-py-examples
      lal_char_b-context
      lal_char_b-ratexp
      lal_char_c-context
      lal_char_c-ratexp
      lal_char_q-context
      lal_char_q-ratexp
      lal_char_r-context
      lal_char_r-ratexp
      lal_char_z-context
      lal_char_zmax-context
      lal_char_zmax-ratexp
      lal_char_zmin-context
      lal_char_zmin-ratexp
      lal_char_z-ratexp
      lal_int_z-context
      lal_int_z-ratexp
      lan-lal_char-_b-context
      lan-lal_char-_b-ratexp
      lat-lal_char_lal_char-_b-context
      lat-lal_char_lal_char-_b-transducer
      lat-lan-lal_char-_lan-lal_char--_b-context
      lat-lan-lal_char-_lan-lal_char--_b-transducer
      lat-lan-lal_char-_law_char-_b-context
      lat-lan-lal_char-_law_char-_b-transducer
    )

# ls ~/.awali/modules | sed 's:^lib\(.*\).so$:\1:' | sed 's:,:_:g' | xclip -selection c

  ENDIF() #PY=TRUE
ENDIF() #EXISTS awalipy dir



# Definition of: make recommended
add_custom_target( recommended 
  DEPENDS
  lal_char_b-accessible
  lal_char_b-automaton
  lal_char_b-context
  lal_char_b-derivation
  lal_char_b-determinize
  lal_char_b-factories
  lal_char_b_lal_char_b-product
  lal_char_b-output
  lal_char_b-partial_id
  lal_char_b-proper
  lal_char_b-quotient
  lal_char_b-ratexp
  lal_char_z-context
  lal_char_z-output
  lal_char_b_lal_char_b-product
  lan-lal_char-_b-context
  lan-lal_char-_b-output
  lan-lal_char-_b-proper
  lan-lal_char-_b-ratexp
  lat-lan-lal_char-_lan-lal_char--_b-context
  lat-lan-lal_char-_lan-lal_char--_b-output
  lat-lan-lal_char-_lan-lal_char--_b-transducer
  lat-lan-lal_char-_lal_char-_b_lat-lan-lal_char-_lal_char-_b-compose
)


add_custom_target(tarballs
  COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/make_pkg.sh" "v${VERSION}" "${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(upload-tarballs
  COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/upload_to_vaucanson-project.sh" "files.awali.org" "${VERSION_MAJOR}.${VERSION_MINOR}" "tarballs" "*.tgz"
  COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/upload_to_vaucanson-project.sh" "files.awali.org" "." "tarballs" "*.tgz"

)
