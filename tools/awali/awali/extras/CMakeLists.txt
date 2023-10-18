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


add_executable(awali_module_compiler EXCLUDE_FROM_ALL module-compiler.cc)
target_link_libraries(awali_module_compiler awalidyn)

add_custom_target(touch_module_directories
  COMMAND ${CMAKE_COMMAND} -E make_directory ${AWALI_MODULE_PATH}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${AWALI_TMP_DIR}
  )

add_custom_target(world DEPENDS awalidyn)

set_property(GLOBAL PROPERTY TARGET_MESSAGES OFF)

set(ALL_CONTEXTS "")

foreach(MOD accessible are_equivalent automaton context derivation determinize eliminate eval factor factories graph join output partial_id product proper quotient ratexp standard singleproduct promotion transducer transpose compose)
  add_custom_target(module_${MOD})
endforeach()

foreach(WS b n z q r c zmin zmax pmax f2 zz3 zz7 zz99 noo nn7 nn99 maxmin)
   add_custom_target(weightset_${WS})
   add_dependencies(world weightset_${WS})
endforeach(WS)

foreach(CTX 
        "lal_char_WS" 
        "lal_int_WS" 
        "lan<lal_char>_WS" 
        "lan<lal_int>_WS" 
        "law_char_WS"
        "lao_ratexpset<lal_char_WS>"
        "lat<lal_char,lal_char>_WS"
        "lat<lan<lal_char>,lal_char>_WS" 
        "lat<lal_char,lan<lal_char>>_WS" 
        "lat<lan<lal_char>,lan<lal_char>>_WS" 
        "lat<lal_char,lan<lal_char>,law_char>_WS"
        "lat<lal_char,lan<lal_int>,law_char>_WS"
        "lat<lan<lal_char>,law_char>_WS" 
)
  string (REPLACE "<" "-" ECTX "${CTX}")
  string (REPLACE ">" "-" ECTX "${ECTX}")
  string (REPLACE "," "_" ECTX "${ECTX}")
  foreach(WS b n z q r c zmin zmax pmax f2 zz3 zz7 zz99 noo nn7 nn99 maxmin)
    string (REPLACE "WS" "${WS}" ECTX2 "${ECTX}")
    string (REPLACE "WS" "${WS}" CTX2 "${CTX}")
    list(APPEND ALL_CONTEXTS "${CTX2}")
    foreach(MOD accessible determinize factories  partial_id standard 
                automaton transducer
                proper      transpose  context    quotient output
                ratexp     factor      derivation eval       graph    
                singleproduct)
      
      add_custom_target("${ECTX2}-${MOD}"
        COMMAND ./awali_module_compiler -d "${MOD}" "${CTX2}"
        VERBATIM
        DEPENDS awali_module_compiler awalidyn touch_module_directories
      )
      add_dependencies("weightset_${WS}" "${ECTX2}-${MOD}")
      add_dependencies("module_${MOD}"  "${ECTX2}-${MOD}")
    endforeach(MOD)

    foreach(MOD product are_equivalent compose promotion join)
      add_custom_target("${ECTX2}_${ECTX2}-${MOD}"
        COMMAND ./awali_module_compiler -d "${MOD}" "${CTX2}" "${CTX2}"
        VERBATIM
        DEPENDS awali_module_compiler awalidyn touch_module_directories
      )
      add_dependencies("weightset_${WS}" "${ECTX2}_${ECTX2}-${MOD}")
      add_dependencies("module_${MOD}"  "${ECTX2}_${ECTX2}-${MOD}")
    endforeach(MOD)
  endforeach(WS)
endforeach(CTX)


# foreach(CTX1 ${ALL_CONTEXTS})
#   string (REGEX REPLACE "[<>,]" "-" ECTX1  "${CTX1}")
#   foreach(CTX2 ${ALL_CONTEXTS})
#     string (REGEX REPLACE "[<>,]" "-" ECTX2  "${CTX2}")
#     if("${CTX1}" STRLESS_EQUAL "${CTX2}")
#       foreach(MOD join)
#         add_custom_target("${ECTX1}_${ECTX2}-${MOD}"
#           COMMAND ./awali_module_compiler -d "${MOD}" "${CTX1}" "${CTX2}"
#           VERBATIM
#           DEPENDS awali_module_compiler awalidyn touch_module_directories
#         )
#       endforeach()
#     endif()
#   endforeach()
# endforeach()

