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

add_executable(awali_module_compiler EXCLUDE_FROM_ALL module-compiler.cc)
target_link_libraries(awali_module_compiler awalidyn)

add_custom_target(touch_module_directories
  COMMAND ${CMAKE_COMMAND} -E make_directory ${AWALI_MODULE_PATH}
  COMMAND ${CMAKE_COMMAND} -E make_directory ${AWALI_TMP_DIR}
  )

add_custom_target(world DEPENDS awalidyn)

set_property(GLOBAL PROPERTY TARGET_MESSAGES OFF)

foreach(MOD accessible are_equivalent automaton context derivation determinize eliminate eval factor factories graph output partial_id product proper quotient ratexp standard singleproduct transducer transpose compose)
  add_custom_target(module_${MOD})
endforeach()

foreach(WS b z q r c zmin zmax pmax f2 zz3 zz7 zz99 noo nn7 nn99 maxmin)
   add_custom_target(weightset_${WS})
   add_dependencies(world weightset_${WS})
endforeach(WS)

foreach(CTX 
        "lal_char_WS" 
        "lal_int_WS" 
        "lan<lal_char>_WS" 
        "lan<lal_int>_WS" 
        "lao_ratexpset<lal_char_WS>"
        "lat<lal_char,lal_char>_WS"
        "lat<lal_char,lan<lal_char>,law_char>_WS"
        "lat<lal_char,lan<lal_int>,law_char>_WS"
        "lat<lan<lal_char>,lal_char>_WS" 
        "lat<lan<lal_char>,lan<lal_char>>_WS" 
        "lat<lan<lal_char>,law_char>_WS" 
)
  string (REPLACE "<" "-" ECTX "${CTX}")
  string (REPLACE ">" "-" ECTX "${ECTX}")
  string (REPLACE "," "_" ECTX "${ECTX}")
  foreach(WS b z q r c zmin zmax pmax f2 zz3 zz7 zz99 noo nn7 nn99 maxmin)
    string (REPLACE "WS" "${WS}" ECTX2 "${ECTX}")
    string (REPLACE "WS" "${WS}" CTX2 "${CTX}")
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

    foreach(MOD product are_equivalent compose)
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
# foreach(WS b z q r c zmin zmax pmax f2 zz3 zz7 zz99 noo nn7 nn99 maxmin)
#    add_custom_target(${WS})
#    add_dependencies(world ${WS})
# 
#   foreach(MOD accessible determinize factories  partial_id standard automaton
#               proper      transpose  context    quotient output
#               ratexp     factor      derivation eval       graph    singleproduct
#   )
# #     add_custom_target(lat<lal_char,lal_char>)
# 
#     add_custom_target(lat-lal_char_lan-lal_char-_law_char-_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d ${MOD} "lat<lal_char,lan<lal_char>,law_char>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn
#     )
# 
#     add_custom_target(lao_ratexpset-lal_char_${WS}--${MOD}
#       COMMAND ./awali_module_compiler -d ${MOD} "lao_ratexpset<lal_char_${WS}>"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn
#     )
# 
#     add_custom_target(lal_char_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d ${MOD} lal_char_${WS}
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn
#     )
# 
#     add_custom_target(lan-lal_int-_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d ${MOD} lan<lal_int>_${WS}
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn
#     )
# 
#     add_custom_target(lal_int_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d ${MOD} lal_int_${WS}
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn
#     )
# 
#     add_dependencies(${WS} "lal_char_${WS}-${MOD}")
#     add_dependencies(module_${MOD} "lal_char_${WS}-${MOD}")
# 
#   #   set_property(TARGET lal_char_${WS}-${MOD} PROPERTY TARGET_MESSAGES OFF)
# 
#     add_custom_target(lan-lal_char-_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d ${MOD} "lan<lal_char>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn
#     )
# 
#     add_dependencies(${WS} lan-lal_char-_${WS}-${MOD})
#     add_dependencies(module_${MOD} lan-lal_char-_${WS}-${MOD})
# 
#   endforeach(MOD)
# 
#   foreach(MOD are_equivalent product)
#     add_custom_target(lal_char_${WS}_lal_char_${WS}-${MOD}
#         COMMAND ./awali_module_compiler -d ${MOD} lal_char_${WS} lal_char_${WS}
#         VERBATIM
#         DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )    
#     add_custom_target(lal_char_${WS}_lan-lal_char-_${WS}-${MOD}
#         COMMAND ./awali_module_compiler -d ${MOD} "lal_char_${WS}" "lan<lal_char>_${WS}"
#         VERBATIM
#         DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
#     add_custom_target(lan-lal_char-_${WS}_lal_char_${WS}-${MOD}
#         COMMAND ./awali_module_compiler -d ${MOD} "lan<lal_char>_${WS}" "lan<lal_char>_${WS}"
#         VERBATIM
#         DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
#     add_custom_target(lan-lal_char-_${WS}_lan-lal_char-_${WS}-${MOD}
#         COMMAND ./awali_module_compiler -d ${MOD} "lan<lal_char>_${WS}" "lan<lal_char>_${WS}"
#         VERBATIM
#         DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
#     add_dependencies(${WS} lal_char_${WS}_lal_char_${WS}-${MOD})
#     add_dependencies(module_${MOD} lal_char_${WS}_lal_char_${WS}-${MOD})
#   endforeach(MOD)
# 
#   foreach(MOD context standard transducer automaton accessible transpose graph output determinize quotient)
# 
#     add_custom_target(lat-lan-lal_char-_law_char-_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d "${MOD}" "lat<lan<lal_char>,law_char>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
# 
#     add_custom_target(lat-lal_char_lal_char-_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d "${MOD}" "lat<lal_char,lal_char>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
# 
#     add_custom_target(lat-lan-lal_char-_lan-lal_char--_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d "${MOD}" "lat<lan<lal_char>,lan<lal_char>>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
# 
# #     add_custom_target(lat-lan-lal_char-_lal_char-_${WS}-${MOD}
# #       COMMAND ./awali_module_compiler -d "${MOD}" "lat<lan<lal_char>,lal_char>_${WS}"
# #       VERBATIM
# #       DEPENDS awali_module_compiler awalidyn touch_module_directories
# #     )
# 
#     add_dependencies(${WS} lat-lan-lal_char-_lan-lal_char--_${WS}-${MOD})
#     add_dependencies(module_${MOD} lat-lan-lal_char-_lan-lal_char--_${WS}-${MOD})
# 
#     add_custom_target(lat-lan-lal_char-_lal_char-_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d "${MOD}" "lat<lan<lal_char>,lal_char>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
#     add_custom_target(lat-lal_char-_lan-lal_char--_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d "${MOD}" "lat<lal_char,lan<lal_char>>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
# 
#     add_custom_target(lat-lal_char_lan-lal_int-_law_char-_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d "${MOD}" "lat<lal_char,lan<lal_int>,law_char>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
#   endforeach(MOD)


#   foreach(MOD compose)
#     add_custom_target(lat-lan-lal_char-_lan-lal_char--_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d "${MOD}" "lat<lan<lal_char>,lan<lal_char>>_${WS}" "lat<lan<lal_char>,lan<lal_char>>_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
#   endforeach(MOD)
#   foreach(MOD eliminate)
#     add_custom_target(lao_${WS}-${MOD}
#       COMMAND ./awali_module_compiler -d "${MOD}" "lao_${WS}"
#       VERBATIM
#       DEPENDS awali_module_compiler awalidyn touch_module_directories
#     )
#   endforeach()
# endforeach(WS)
