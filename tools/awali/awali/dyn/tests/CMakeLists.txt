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

foreach(VAL
        accessible
        automaton
        are-equivalent
        context-description
        derivation
        determinize
        doxygen
        eps filter
        eval
        factories
        fallbacktostring
        from
        heap
        json
        json_examples
	      lift
        minimize
        minimal-aut
        options
        output
        parse-json-v0
        product
        promote
        traits
        view
        cat-examples
       )

  #include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../..")
  # ${CMAKE_CURRENT_SOURCE_DIR} equals  <smth>/awali/dyn/tests and we want to
  # include <smth>

  add_executable(test-${VAL} EXCLUDE_FROM_ALL test-${VAL}.cc )
  target_link_libraries(test-${VAL} dl)
  target_link_libraries(test-${VAL} awalidyn)

  add_test(build-${VAL} "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target test-${VAL})

  IF (CHECK_MEMORY_LEAK)
    add_test(${VAL} ${MEMORY_CHECK_COMMAND} ./test-${VAL})
  ELSE()
    add_test(${VAL} ./test-${VAL})
  ENDIF()

  set_tests_properties(${VAL} PROPERTIES FIXTURES_REQUIRED build-${VAL})
  set_tests_properties(build-${VAL} PROPERTIES FIXTURES_SETUP build-${VAL})

endforeach(VAL)

add_dependencies(check check-dyn)
set_tests_properties(json_examples PROPERTIES FIXTURES_REQUIRED cat-examples FIXTURES_REQUIRED build-json_examples)

# set_tests_properties(accessible PROPERTIES DEPENDS b-context)


#ls ~/.awali/modules | sed 's:^lib\(.*\).so$:\1:' | sed 's:,:_:g'
# ls ~/.awali/modules | sed 's:^lib\(.*\).so$:\1:' | sed 's:,:_:g' | xclip -selection c
add_dependencies(check-dyn
awalidyn
lal_char_b-accessible
lal_char_b-automaton
lal_char_b-context
lal_char_b-derivation
lal_char_b-determinize
lal_char_b-eval
lal_char_b-factories
lal_char_b_lal_char_b-are_equivalent
lal_char_b_lal_char_b-join
lal_char_b_lal_char_b-product
lal_char_b_lal_char_b-promotion
# lal_char_b_lal_char_c-join
# lal_char_b_lal_char_c-promotion
# lal_char_b_lal_char_q-join
# lal_char_b_lal_char_q-promotion
# lal_char_b_lal_char_r-join
# lal_char_b_lal_char_r-promotion
# lal_char_b_lal_char_z-join
# lal_char_b_lal_char_z-promotion
# lal_char_b_lan-lal_char-_b-join
# lal_char_b_lan-lal_char-_b-promotion
# lal_char_b_lan-lal_char-_z-join
# lal_char_b_lan-lal_char-_z-promotion
lal_char_b-output
lal_char_b-proper
lal_char_b-quotient
lal_char_b-ratexp
lal_char_b-standard
lal_char_b-transpose
lal_char_c-automaton
lal_char_c-context
lal_char_c-eval
lal_char_c_lal_char_c-product
lal_char_c_lal_char_c-promotion
# lal_char_c_lal_char_q-join
# lal_char_c_lal_char_r-join
# lal_char_c_lal_char_z-join
lal_char_c-output
lal_char_f2-context
lal_char_n-context
lal_char_nn7-context
lal_char_nn7-output
lal_char_n-output
lal_char_q-automaton
lal_char_q-context
lal_char_q-eval
# lal_char_q_lal_char_c-promotion
lal_char_q_lal_char_q-product
lal_char_q_lal_char_q-promotion
# lal_char_q_lal_char_r-join
# lal_char_q_lal_char_r-promotion
# lal_char_q_lal_char_z-join
lal_char_q-output
lal_char_q-ratexp
lal_char_r-automaton
lal_char_r-context
lal_char_r-eval
# lal_char_r_lal_char_c-promotion
lal_char_r_lal_char_r-product
lal_char_r_lal_char_r-promotion
# lal_char_r_lal_char_z-join
lal_char_r-output
lal_char_z-automaton
lal_char_z-context
lal_char_z-determinize
lal_char_z-eval
# lal_char_z_lal_char_c-promotion
# lal_char_z_lal_char_q-promotion
# lal_char_z_lal_char_r-promotion
lal_char_z_lal_char_z-join
lal_char_z_lal_char_z-product
lal_char_z_lal_char_z-promotion
# lal_char_z_lan-lal_char-_b-join
lal_char_zmax-automaton
lal_char_zmax-context
lal_char_zmax-output
lal_char_zmin-automaton
lal_char_zmin-context
lal_char_zmin-output
lal_char_z-output
lal_char_z-quotient
lal_char_z-singleproduct
lal_char_zz3-context
lal_char_zz3-output
lal_int_b-context
lal_int_b-output
lal_int_b-ratexp
lal_int_n-context
lal_int_nn7-context
lal_int_nn7-output
lal_int_n-output
lal_int_q-context
lal_int_q-output
lal_int_r-context
lal_int_r-output
lal_int_z-automaton
lal_int_z-context
lal_int_zmin-context
lal_int_zmin-output
lal_int_z-output
lal_int_zz3-context
lal_int_zz3-output
lan-lal_char-_b-automaton
lan-lal_char-_b-context
lan-lal_char-_b-output
lan-lal_char-_b-proper
lan-lal_char-_b-quotient
lan-lal_char-_b-ratexp
lan-lal_char-_f2-context
lan-lal_char-_n-context
lan-lal_char-_nn7-context
lan-lal_char-_nn7-output
lan-lal_char-_n-output
lan-lal_char-_q-context
lan-lal_char-_q-output
lan-lal_char-_q-ratexp
lan-lal_char-_r-context
lan-lal_char-_r-output
lan-lal_char-_z-context
lan-lal_char-_zmin-context
lan-lal_char-_zmin-output
lan-lal_char-_zz3-context
lan-lal_char-_zz3-output
lan-lal_int-_b-context
lan-lal_int-_b-output
lan-lal_int-_n-context
lan-lal_int-_nn7-context
lan-lal_int-_nn7-output
lan-lal_int-_n-output
lan-lal_int-_q-context
lan-lal_int-_q-output
lan-lal_int-_r-context
lan-lal_int-_r-output
lan-lal_int-_z-context
lan-lal_int-_zmin-context
lan-lal_int-_zmin-output
lan-lal_int-_zz3-context
lan-lal_int-_zz3-output
lao_ratexpset-lal_char_b--output
lao_ratexpset-lal_char_q--output
lat-lal_char_lal_char-_b-automaton
lat-lal_char_lal_char-_b-context
lat-lal_char_lal_char-_b-output
lat-lal_char_lan-lal_int-_law_char-_b-context
lat-lal_char_lan-lal_int-_law_char-_b-output
lat-lal_char_lan-lal_int-_law_char-_n-context
lat-lal_char_lan-lal_int-_law_char-_nn7-context
lat-lal_char_lan-lal_int-_law_char-_nn7-output
lat-lal_char_lan-lal_int-_law_char-_n-output
lat-lal_char_lan-lal_int-_law_char-_q-context
lat-lal_char_lan-lal_int-_law_char-_q-output
lat-lal_char_lan-lal_int-_law_char-_r-context
lat-lal_char_lan-lal_int-_law_char-_r-output
lat-lal_char_lan-lal_int-_law_char-_z-context
lat-lal_char_lan-lal_int-_law_char-_zmin-context
lat-lal_char_lan-lal_int-_law_char-_zmin-output
lat-lal_char_lan-lal_int-_law_char-_zz3-context
lat-lal_char_lan-lal_int-_law_char-_zz3-output
lat-lan-lal_char-_lal_char-_b-context
lat-lan-lal_char-_lal_char-_b-output
lat-lan-lal_char-_lal_char-_n-context
lat-lan-lal_char-_lal_char-_nn7-context
lat-lan-lal_char-_lal_char-_nn7-output
lat-lan-lal_char-_lal_char-_n-output
lat-lan-lal_char-_lal_char-_q-context
lat-lan-lal_char-_lal_char-_q-output
lat-lan-lal_char-_lal_char-_r-context
lat-lan-lal_char-_lal_char-_r-output
lat-lan-lal_char-_lal_char-_zmin-context
lat-lan-lal_char-_lal_char-_zmin-output
lat-lan-lal_char-_lal_char-_zz3-context
lat-lan-lal_char-_lal_char-_zz3-output
lat-lan-lal_char-_lan-lal_char--_b-automaton
lat-lan-lal_char-_lan-lal_char--_b-context
lat-lan-lal_char-_lan-lal_char--_b-determinize
lat-lan-lal_char-_lan-lal_char--_b-output
lat-lan-lal_char-_lan-lal_char--_b-quotient
lat-lan-lal_char-_lan-lal_char--_b-standard
lat-lan-lal_char-_lan-lal_char--_n-context
lat-lan-lal_char-_lan-lal_char--_nn7-context
lat-lan-lal_char-_lan-lal_char--_nn7-output
lat-lan-lal_char-_lan-lal_char--_n-output
lat-lan-lal_char-_lan-lal_char--_q-context
lat-lan-lal_char-_lan-lal_char--_q-output
lat-lan-lal_char-_lan-lal_char--_r-context
lat-lan-lal_char-_lan-lal_char--_r-output
lat-lan-lal_char-_lan-lal_char--_z-context
lat-lan-lal_char-_lan-lal_char--_zmin-context
lat-lan-lal_char-_lan-lal_char--_zmin-output
lat-lan-lal_char-_lan-lal_char--_zz3-context
lat-lan-lal_char-_lan-lal_char--_zz3-output
lat-lan-lal_char-_law_char-_b-automaton
lat-lan-lal_char-_law_char-_b-context
lat-lan-lal_char-_law_char-_b-output
law_char_b-context
law_char_b-output
law_char_n-context
law_char_nn7-context
law_char_nn7-output
law_char_n-output
law_char_q-context
law_char_q-output
law_char_r-context
law_char_r-output
law_char_z-context
law_char_zmin-context
law_char_zmin-output
law_char_zz3-context
law_char_zz3-output
)
