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

import inspect

def run_from_jupyter_notebook():
    try:
        __IPYTHON__
        cfg = get_ipython().config
        if 'IPKernelApp' in cfg:
            return True
        else:
            return False
    except NameError:
        return False


deprecation_warning = True

def disable_deprecation_warning():
    global deprecation_warning 
    deprecation_warning = False

def _deprecated(caller,since=None,replace="the method with the same name"):
    global deprecation_warning
    if not deprecation_warning:
        return
    s= "[Awalipy] Function '{}' is deprecated".format(caller)
    if since is not None:
        s += " since version " + since + "."
    else:
        s += "."
    if replace is not None:
        s+= "  Use {} instead.".format(replace)
    print(s)


