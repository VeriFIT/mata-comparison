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

IF (NOT DEFINED AWALICPP_INSTALL_PREFIX)
  set(AWALICPP_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
ENDIF()

IF (NOT DEFINED AWALICPP_HEADER_INSTALL_PREFIX)
  set(AWALICPP_HEADER_INSTALL_PREFIX "${AWALICPP_INSTALL_PREFIX}/include")
ENDIF()

IF (NOT DEFINED AWALICPP_DYNLIB_INSTALL_DIR)
  set(AWALICPP_DYNLIB_INSTALL_DIR "${AWALICPP_INSTALL_PREFIX}/lib")
ENDIF()

IF (NOT DEFINED AWALICPP_SHARE_INSTALL_PREFIX)
    set(AWALICPP_SHARE_INSTALL_PREFIX "${AWALICPP_INSTALL_PREFIX}/share")
ENDIF()

IF (NOT DEFINED AWALICPP_MODULE_INSTALL_DIR)
  set(AWALICPP_MODULE_INSTALL_DIR "${AWALICPP_INSTALL_PREFIX}/lib/awali_modules")
ENDIF()

IF (NOT DEFINED DOT_BIN)
  find_program(DOT_BIN "dot")
  IF(${DOT_BIN} STREQUAL DOT_BIN-NOTFOUND)
    message (WARNING "Could not find \"dot\" binary (sometimes distributed as part as graphviz).  Display of automaton will be disabled.")
    UNSET(DOT_BIN)
  ENDIF()
ENDIF()


set( AWALIDYN_SOURCES "dyn/awalidyn.cc" )


################################################################################
# COMMON
################################################################################
FOREACH(F
        enum_utils
        parse_exception
        qfraction
        json_ast
        )
  set(AWALIDYN_SOURCES common/${F}.cc ${AWALIDYN_SOURCES})
ENDFOREACH()

################################################################################
# COMMON/JSON
################################################################################
FOREACH(F
        node
        visitor
        equality
        parser
        smart_printer
        )
  set(AWALIDYN_SOURCES common/json/${F}.cc ${AWALIDYN_SOURCES})
ENDFOREACH()

################################################################################
# CORE
################################################################################
FOREACH(F
        any
        automaton
        context
        context_description
        context_description_deprecated
        ratexp
        transducer
       )
  set(AWALIDYN_SOURCES dyn/core/${F}.cc ${AWALIDYN_SOURCES})
ENDFOREACH()


################################################################################
# LOADING
################################################################################
FOREACH(F
        compile
        handler
        locations
        parse_ctx
       )
  set(AWALIDYN_SOURCES dyn/loading/${F}.cc ${AWALIDYN_SOURCES})
ENDFOREACH()


################################################################################
# ALGOS
################################################################################
FOREACH(F
        aliases
        random
        sys
        tdc_circulation
        tdc_eval
       )
  set(AWALIDYN_SOURCES dyn/algos/${F}.cc ${AWALIDYN_SOURCES} )
ENDFOREACH()


################################################################################
# OPTIONS
################################################################################
FOREACH(F options option)
  set(AWALIDYN_SOURCES dyn/options/${F}.cc ${AWALIDYN_SOURCES})
ENDFOREACH()


################################################################################
# MODULES
################################################################################
FOREACH(F
        accessible
        are_equivalent
        automaton
        derivation
        determinize
        eliminate
        eval
        factor
        factories product
        graph
        output
        partial_id
        promotion
        proper
        quotient
        ratexp
        singleproduct
        standard context
        transducer
        transpose
        words
       )
  set(AWALIDYN_SOURCES dyn/modules/${F}.cc ${AWALIDYN_SOURCES} )
ENDFOREACH()



add_subdirectory(common)
add_subdirectory(dyn)
add_subdirectory(sttc/tests)
add_subdirectory(extras)



add_library (awalidyn SHARED "${AWALIDYN_SOURCES}")

target_link_libraries(awalidyn dl)
target_link_libraries(awalidyn "-rdynamic")








###############################################################################
##                           INSTALLATION COMMANDS                           ##
###############################################################################


## Library. ###################################################################
install( TARGETS "awalidyn" DESTINATION "${AWALICPP_DYNLIB_INSTALL_DIR}" )


# Headers. ####################################################################
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/sttc"
                   "${CMAKE_CURRENT_SOURCE_DIR}/dyn"
                   "${CMAKE_CURRENT_SOURCE_DIR}/common"
                   "${CMAKE_CURRENT_SOURCE_DIR}/utils"
            DESTINATION "${AWALICPP_HEADER_INSTALL_PREFIX}/awali"
            FILES_MATCHING
                PATTERN "*.hh"
                PATTERN "*.hxx" )
install( FILES "${CMAKE_CURRENT_SOURCE_DIR}/dyn.hh"
         DESTINATION "${AWALICPP_HEADER_INSTALL_PREFIX}/awali")



# Source code. ################################################################
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/sttc"
                   "${CMAKE_CURRENT_SOURCE_DIR}/dyn"
                   "${CMAKE_CURRENT_SOURCE_DIR}/common"
                   "${CMAKE_CURRENT_SOURCE_DIR}/utils"
            DESTINATION "${AWALICPP_SHARE_INSTALL_PREFIX}/awali/src/awali"
            FILES_MATCHING PATTERN "*.cc" )

# Example automata. ###########################################################
install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/automata"
                   "${CMAKE_CURRENT_SOURCE_DIR}/ratexps"
            DESTINATION "${AWALICPP_SHARE_INSTALL_PREFIX}/awali" )

## Executing ldconfig in order for awalidyn to be found by the c++ linker ######
find_program(LDCONFIG_BIN "ldconfig")
IF(NOT ${LDCONFIG_BIN} STREQUAL "LDCONFIG_BIN-NOTFOUND")
  install(CODE "MESSAGE(\"Running ${LDCONFIG_BIN}.\")")
  install( CODE "execute_process(COMMAND \"${LDCONFIG_BIN}\")" )
ENDIF()

## OTF libraries. #############################################################

install( DIRECTORY "${AWALI_MODULE_PATH}/"   # Trailing "/" is significant
         DESTINATION "${AWALICPP_MODULE_INSTALL_DIR}"
         OPTIONAL )

###############################################################################
##                          UNINSTALLATION COMMANDS                          ##
###############################################################################

# Note (VM): uninstallation is centralised here since sttc and dyn do not
# know how to remove themselves.
#
# Note (VM): This will leave out several directories and subdirectories.

if(TARGET uninstall)
else()
  add_custom_target (uninstall
    COMMAND "${CMAKE_COMMAND}"
        -D "DATA_TO_REMOVE=${CMAKE_BINARY_DIR}/install_manifest.txt"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeScripts/remove_data.cmake"
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${AWALICPP_MODULE_INSTALL_DIR}"
  )
endif()

add_dependencies(uninstall cleanlib)
