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

FOREACH(f 
  "first-step.md"
) # Put file to upload to files.awali.org/gs

  add_custom_target("gs-dyn-upload-${f}"
    COMMAND "${SCRIPT_DIR}/upload_gs.sh" "${VERSION_MAJOR}.${VERSION_MINOR}" "${CMAKE_CURRENT_SOURCE_DIR}/${f}"
  )
  add_dependencies("upload-gs" "gs-dyn-upload-${f}")
ENDFOREACH(f)
