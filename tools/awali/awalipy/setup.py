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

from distutils.core import setup, Extension
from Cython.Build import cythonize
import os;

#os.environ["CC"] = "${CMAKE_CXX_COMPILER}"

##### A hack to remove the pesky -W-strict-prototypes argument which makes
##### g++ issue a warning: the flag makes no sense for compiling c++.
from distutils.command.build_ext import build_ext
from distutils.sysconfig import customize_compiler

class hacked_build_ext(build_ext):
    def build_extensions(self):
        customize_compiler(self.compiler)
        try:
            self.compiler.compiler_so.remove("-Wstrict-prototypes")
        except (AttributeError, ValueError):
            pass
        build_ext.build_extensions(self)
##### (The following line, lower in the file, must also be deleted
#####     cmdclass = {'build_ext': hacked_build_ext},
##### )
##### End of Hack

cython_module = \
    Extension(
        'awalipy',
        sources=['cython-src/awalipy.pyx'],
       include_dirs=['..'],
#                      '${CMAKE_BINARY_DIR}', ],
#        library_dirs=['${AWALI_INST_DYNLIB_PATH}', '${AWALI_DYNLIB_PATH}'],
        libraries=['awalidyn'],
        language="c++",
        extra_compile_args=["-std=c++11", "-O3"],
#        runtime_library_dirs=['${AWALI_INST_DYNLIB_PATH}', '${AWALI_DYNLIB_PATH}'],
    )
#cython_module.cython_c_in_temp = True

setup(
    cmdclass = {'build_ext': hacked_build_ext},
    name = "awalipy",
    version = '0.4',
    ext_modules =  cythonize(cython_module),
    py_modules=['awalipy_purepython_extra'],
)
