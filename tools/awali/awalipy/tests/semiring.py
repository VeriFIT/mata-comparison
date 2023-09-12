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

import sys, os
cmd_folder=os.getcwd()
if cmd_folder not in sys.path:
    sys.path.insert(0, cmd_folder)


semirings =   [  'B',                    'Z',         'F2',               \
                 'Z-min-plus',                      'Z-max-plus', \
                 'R',                             'R-max-prod', \
                 'Q' ,                           'C',\
                 'Z/7Z',                   'Z/99Z', \
                 'N-oo', \
                 'N7'
              ]
#first value is semiring 0's, second is semiring 1's
values_list = [ ['0','1','1','1','1'],  ['0','1','-10','-1','15'], ['0','1','1','1','1'],  \
                ['oo','0','-10','2','15'],  ['-oo','0','-10','2','15']  ,\
                ['0','1','0.32','-3.1415','10'],  ['0','1','4','2.7646','3.22222'],\
                ['0','1','3/2','-1/2','4' ] ,     ['0','1','2+i','3+i','1-2i'],\
                ['0','1','2','4','5'],  ['0','1','12','88','31'],
                ['0','1','2','oo','3'],
                ['0','1','2','4','5']

              ]
# semiring addition of the last two values
add_list=     [ '1',                     '14',                      '1', \
                                                                    #this addition in F2 is false on purpose
                '2',                                '15', \
                '6.8585',                         '3.22222', \
                '7/2',                            '4-i',\
                '2',                     '20',
                'oo',
                '7'
              ]
n_sr= len(semirings)



global semiring, values, added, test_succeeded, sr_i

name = None
values = None
added = None
sr_i = -42

def reset():
    global sr_i
    sr_i=-1

reset()

def next():
    global name, values, added, test_succeeded, sr_i
    sr_i= sr_i+1
    if (sr_i==n_sr):
      return False
    name = semirings[sr_i]
    values = values_list[sr_i]
    added = add_list[sr_i]
    return True
