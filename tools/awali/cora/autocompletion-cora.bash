#!/bin/bash

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


out=/dev/null
# out=log.txt  # COMMENT/UNCOMMENT to activate/deactivate debug-information
               # writing to file log.txt


_cora()
{
  COMPREPLY=()
  curw=${COMP_WORDS[COMP_CWORD]}
  IFS=$'\n'
  echo \"${COMP_WORDS[@]}\" > ${out}
  echo ${COMP_CWORD} >> ${out}

  COMPREPLY=($(compgen -W '`__cora_autocompletion $((${#COMP_WORDS[@]}-1)) "${COMP_WORDS[@]}" "${COMP_CWORD}" 2>> ${out}`' -- ${curw}))
  echo \"${COMPREPLY[@]}\" >> ${out}
}

complete -o nospace -F _cora cora
