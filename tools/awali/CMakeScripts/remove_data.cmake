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

file(STRINGS ${DATA_TO_REMOVE} list)
foreach(line IN LISTS list)
    MESSAGE("Removing file: ${line}")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E remove ${line} RESULT_VARIABLE X)
    IF(X)
      MESSAGE(FATAL_ERROR "Cannot remove file \"${line}\", probably due to insufficient access rights.")
    ENDIF()
endforeach()
