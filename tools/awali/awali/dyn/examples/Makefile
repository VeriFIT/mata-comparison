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


# NB: This Makefile is supposed to work with an Awali that is not installed.
# *It is not very robust by nature; use with caution.*
# It requires the library to be compiled in a directory named `_build`, next
# to the `awali` directory containing `dyn.hh` file.
# Otherwise, the CFLAGS and LDFLAGS variables must be modified accordingly.
#
# Usage: `make myprogram` if the C++ code is in file  `myprogram.cc`


DST_DIR?=.
CC=c++
PATH_TO_ROOT?=../../..
CMAKE_PATH_FROM_ROOT?=_build
CMAKE_PATH?=$(PATH_TO_ROOT)/$(CMAKE_PATH_FROM_ROOT)

CFLAGS=-std=c++11 -I$(PATH_TO_ROOT) -I$(CMAKE_PATH)/awali
LDFLAGS=-Wl,-rpath,$(CMAKE_PATH)/awali -L$(CMAKE_PATH)/awali -lncurses -lawalidyn

PROGRAMS=\
	io-file\
	io-stream\
	make-url-validator


all: ${PROGRAMS}

% : %.cc
	echo $(LDFLAGS)
	$(CC) $(CFLAGS) -o $(DST_DIR)/$@ $^ $(LDFLAGS)

