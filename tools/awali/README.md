 Introduction to Awali
=======================

Awali is a C++ and Python generic library for weighted finite state machines.
Copyright 2016-2021 Sylvain Lombardy, Victor Marsault, Jacques Sakarovitch

Awali is a free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

The complete GNU General Public Licence Notice can be found in the
`COPYING.txt` file, in the same directory as this `README.md`.


Using Awali
-----------

Awali can be used for the while at four different levels:

- through the *Cora* binary: Cora is a programm that offers many commands to
  compute on automata and transducers.

- as a python module; the `python` directory contains the files to produce
  a module `awalipy` which can be imported into python, ipython or jupyter.

- as a simple C++ library; the `awali/dyn` directory contains headers that
  allow to use Awali without worrying about types. This library uses
  the algorithms of the template library which are compiled on-the-fly
  for the requested type.

- as a template library; the `awali/sttc` directory contains all the
  Awali template library, sometimes called the *static layer*.


History
-------

Awali results from a fork of Vaucanson 2.


Compilation and installation
----------------------------

The *standard* command-line to compile and install Awali are the following.

```
  cd /directory/where/this/README.md/is/located/
  mkdir _build
  cd _build
  cmake ..
  make
  make recommended
  sudo make install
```

This process might not be suitable for your situation.  Details on
requirements, compilation and installation are described in the `INSTALL.md`
file.


Examples and documentation
--------------------------

The documentation of Awali is mostly under construction.

### 1.  Getting started

The directory `awalidoc/GettingStarted` contain files that are a good start for
someone that wants to start using Awali.

The `dyn` subdirectory contains a `first-step.md` file that contains a first 
program and how to compile it.

The `py` and `cora` subdirectories contain jupyter-notebook documents.
Html versions of those documents may be found in the Awali website.


### 2. Examples

- Example programs written in C++ with the static layer of Awali may be found
  in `awali/sttc/examples` (relative to the directory where this `README.md` is
  located).  
  This directory contains an example `Makefile` to compile those programs.

- Example programs written in C++ with the dynamic layer of Awali way be found
  in `awali/dyn/examples` (relative to the directory where this `README.md` is
  located).  
  This directory contains an example `Makefile` to compile those programs.

- Example Python scripts using Awalipy may be found in `awalipy/examples`
  (relative to the directory where this `README.md` is located).


### 3. Documentation

- The C++-level documentation is written with doxygen. You may generate it
  by running the following command from the directory where the `cmake` command
  was executed.
  ```
    make doxygen
  ```
  The generated documentation is made of html pages located in directory
  `awalidoc/Doxygen/html` (relative to the location you executed the `make
  doxygen` command, hence probably `_build/awalidoc/Doxygen/html` relative to 
  the directory where this `README.md` is located).  
  The page `awalidoc/Doxygen/html/index.html` is a good way to start.

- The Python documentation is given as docstring directly in the code and be 
  accessed as usual from the `help(..)` or `..?` built-in features.

- The Cora documentation is given directly in the command line, a good start is:
  ```
    cora help
  ```


License
-------

Awali is a free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see http://www.gnu.org/licenses/.


Contacts
--------

* Sylvain Lombardy  
  LaBRI / Bordeaux INP  
  Sylvain.Lombardy@labri.fr  
  wwww.labri.fr/~slombard
  
* Victor Marsault  
  LIGM / Université Gustave Eiffel and CNRS  
  Victor.Marsault@u-pem.fr  
  https://victor.marsault.xyz  

*  Jacques Sakarovitch  
   IRIF / CNRS, University Paris Diderot and  
   Telecom Paris, IPP  
   Jacques.Sakarovitch@telecom-paris.fr  
   www.telecom-paris.fr/~jsaka  

