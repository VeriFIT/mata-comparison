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
  "GettingStartedWithCora"
  "CoraDoc-WeightsIA"
  "CoraDoc-WeightsIB"
) # Put jupyter file names in the previous list without the ".ipynb"

#Command to generate html from ipynb
  add_custom_target("gs-cora-${f}"
    COMMAND "${NBCONVERT}" "--to=html" "--output-dir=${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${f}.ipynb")
  add_dependencies(gs "gs-cora-${f}")

#Command to upload html and ipyn files to files.awali.org
  add_custom_target("gs-cora-upload-${f}"
    COMMAND "${SCRIPT_DIR}/upload_gs.sh" "${VERSION_MAJOR}.${VERSION_MINOR}" "${CMAKE_CURRENT_SOURCE_DIR}/${f}.ipynb" "${CMAKE_CURRENT_BINARY_DIR}/${f}.html"
    DEPENDS "gs-cora-${f}"
  )
  add_dependencies("upload-gs" "gs-cora-upload-${f}")
ENDFOREACH(f)
