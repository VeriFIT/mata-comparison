Compilation and Installation of Awali
=======================================

Content:
--------

- Build Requirements
- Compilation
- Installation
- Where files are installed
- Customizing compilation and installation
  - List of CMake variables
  - Common use-cases

Compilation and Installation of Awali
=====================================

Build Requirements
------------------

Awali uses the C++11 standard; it has been tested with g++ 4.8, 4.9 and Clang++ 6.0,
It should compile with higher versions.
It may probably be used only on Linux or BSD OS (including MacOS).

* GNU Compiler Collection (GCC): `http://gcc.gnu.org/`

* CMake: Compilation process uses CMake : `https://cmake.org/`

* Cython: Used for the compilation of the Python front-end of Awali (module `awalipy`)

* Graphviz: `http://www.research.att.com/sw/tools/graphviz/`
The `display` command in Cora uses the export of Awali
into Dot format and launches the `dotty` command from the
Graphviz_ suite:

* Doxygen: `http://doxygen.org`
Doxygen is used to generate the C++ reference documentation.

* ncurses library
ncurses is used for automaton edition in the `cora` command-line interface to Awali

Compilation
-----------

In any directory (preferably NOT in the awali directory itself), write:

If the tarball has been downloaded and decompressed in `some/local/directory` then run the following commands

    cd /directory/where/this/README.md/is/located/
    mkdir _build
    cd _build
    cmake ..
    make
    make recommended
    sudo make install
		

Installation
------------

We recommend you to install Awali by running the following command

	sudo make install


To reverse this operation execute:

	sudo make uninstall


Where files are installed:
--------------------------

NB: in the descriptions below, strings that are between `${..}` are CMake variables.
We refer the reader to the next section for explanation of how to
set up such variables to custom values.

**Headers of the dynamic and static libraries**
(ie: `{awali,dyn}/**/*.{hh,hxx}`)
are installed in `/usr/local/include/awali/`.
This location is defined by CMake expression:
`${AWALICPP_HEADER_INSTALL_PREFIX}/awali`


**Sources of dynamic modules**
(ie: `{awali,dyn}/**/*.cc`) are installed in
`/usr/local/share/awali/`.
This location is defined by CMake expression:
`${AWALICPP_SHARE_INSTALL_PREFIX}/awali`


**Examples** (ie: `share/automata/*` ) are installed in
`/usr/local/share/awali/automata`.
This location is defined by CMake expression:
`${AWALICPP_SHARE_INSTALL_PREFIX}/awali/automata`.


**Dynamic Library** (ie: `libawalidyn.so` ) is installed in
`/usr/local/lib/`.
This location is governed by CMake expression:
`${AWALICPP_DYNLIB_INSTALL_DIR}`.


**Static libraries** (that were compiled prior to installation) are installed in
`/usr/local/lib/awali_modules/`.
This location is governed by CMake expression
`${AWALICPP_MODULES_INSTALL_DIR}`.


**Cora executable** is installed in  `/usr/local/bin/`.
This location is defined by CMake expression
`${CORA_BINARY_INSTALL_DIR}`.


**Cora completion**

  - executable is installed in  `/usr/local/bin/`
  - bash-script in the `bash_completion.d` directory, if it could be found.
  - This
    behaviour may be changed using the CMake variable
    `CORA_BASH_COMPL_INSTALL_DIR`.
  - zsh autocompletion file in the "site-functions" directory if it could be found.
This behaviour may be changed using the CMake variable
    `CORA_ZSH_COMPL_INSTALL_DIR`.


**Python library** is installed using "setup" from "distutils.core" hence is
installed at different locations depending on the OS.

The location may be forced using CMake variable `AWALIPY_INSTALL_PREFIX`.
The version of Python used is governed by CMake variable `PYTHON_BIN`
(and `CYTHON_BIN`, which should be consistent with `PYTHON_BIN`).


Customizing compilation and installation.
-----------------------------------------

The general way to customize compilation and/or installation of Awali is to assign values to CMake variable.
This is done by an option `-D<variable>=<value>`
to `cmake` command-line.
For instance, in order to bind variable `XXXX`
to value `YYYY` and variable `ZZZZ` to value "123456",
the `cmake` command is as follows:

    cmake -DXXXX="YYYY" -DZZZZ="123456" ..

(instead of the plain `cmake ..` )

**List of CMake Variables**

- `CMAKE_INSTALL_PREFIX` governs where all the components of Awali will be installed;
its default value is `/usr/local/bin`.

- `AWALICPP_INSTALL_PREFIX` governs where the static and dynamic layers will be installed; its default value is:
`${CMAKE_INSTALL_PREFIX}` .

- `AWALICPP_HEADER_INSTALL_DIR` governs where the headers of the dynamic and static libraries will be installed;
its default value is `${AWALICPP_INSTALL_PREFIX}/lib`.

- `AWALICPP_DYNLIB_INSTALL_DIR` is the location of the dynamic library;
its default value is `${AWALICPP_INSTALL_PREFIX}/lib`.

- `AWALICPP_SHARE_INSTALL_PREFIX` governs where the source of the dynamic modules and the examples automata;
its default value is `${AWALICPP_INSTALL_PREFIX}/share`.

- `AWALICPP_MODULE_INSTALL_DIR` governs where are the static modules compiled prior to installation;
its default value is `${AWALICPP_INSTALL_PREFIX}/lib/awali_modules`

- `CORA_INSTALL_PREFIX` governs where components of Cora will be installed;
its default value is: `${CMAKE_INSTALL_PREFIX}`.

- `CORA_BINARY_INSTALL_DIR` governs where the `cora` binary and the
autocompletion binary for `cora` will be installed;
its default value is `${CORA_INSTALL_PREFIX}/bin`.

- `CORA_BASH_COMPL_DIR` tells where to install the completion directive for `co`ra under bash shell.
It defaults to `AUTO` meaning that CMake will look for the appropriate directory; the search may fail issueing a warning.
The value `IN_PREFIX` means that the installation will take place in
`${CORA_INSTALL_PREFIX}/etc/bash_completion.d`; bash then has to be configured to look for completion file in that directory.
Other acceptable values are absolute paths.

- `CORA_ZSH_COMPL_DIR` is similar to `CORA_BASH_COMPL_DIR` but for the zsh shell (`IN_PREFIX` is effectively equivalent to
`${CORA_INSTALL_PREFIX}/share/zsh/site-functions`).

- `AWALIPY_INSTALL_PREFIX` governs where the Python module Awalipy will be installed.

    - Its default value is `AUTO` unless variable `FORCE_INSTALL_IN_PREFIX` is set to `TRUE`,
in which case its default value is `IN_PREFIX`.

    - Value `IN_PREFIX` is effectively replaced by
`${CMAKE_INSTALL_PREFIX}` before compiling/installing Awalipy.

    - If set to `AUTO`, the setup from distutils.core module decides where to install the module;
otherwise, the option
`--prefix=${AWALIPY_INSTALL_PREFIX}` is given to setup.

- `CORA` governs whether to compile and install Cora;
defaults to `TRUE`.

- `PY` governs whether or not to compile and install the Python layer; defaults to `TRUE`.

- `PYTHON_BIN` provides the python binary to use for compiling and installing awalipy;
defaults to the result provided by the command `python`.

- `CYTHON_BIN` is similar to `PYTHON_BIN` but for the `cython` binary.

- `FORCE_INSTALL_IN_PREFIX` is a shorthand variable to disable all
smart/automatic installation;
setting `FORCE_INSTALL_IN_PREFIX` to `TRUE` effectively sets `AWALIPY_INSTALL_PREFIX`, `CORA_BASH_COMPL_DIR` and
`CORA_ZSH_COMPL_DIR` to `IN_PREFIX`.


**Common use-cases**

**Q**: How to make the system install in `/opt` directory?
**A**: Use option:  `-DCMAKE_INSTALL_PREFIX=/opt`

**Q**: How to make a user install `in /home/jdoe/awali`?
**A**: Use option:

    -DCMAKE_INSTALL_PREFIX=/home/jdoe/awali -DFORCE_INSTALL_IN_PREFIX=TRUE

**Q**: How to install in a custom bash completion directory
`/some/nonstandard/directory`?
**A**: Use option `-DCORA_BASH_COMPL_DIR=/some/nonstandard/directory`.

**Q**: How to disable compilation and installation of the Python layer?
**A**: Use option `-DPY=FALSE`.

**Q**: How to disable compilation and installation of Cora?
**A**: Use option `-DCORA=FALSE`.

**Q**: How to make Awalipy available from Python3 (resp. Python2)?
**A**: Awali is compiled and installed for the version of Python corresponding to the binaries named `python` and `cython`.
Depending on your system, it may be either Python 2 or 3.
You may force installation to use by manually setting the `python` and `cython` binaries.
The options provided to cmake would look like this:

    -DPYTHON_BIN=python3 -DCYTHON_BIN=cython3

Note that the strings on the right of the `=` sign  are OS-dependent.
They should be binaries in the path or absolute path to binaries (e.g., `/usr/bin/python3.5m`).





