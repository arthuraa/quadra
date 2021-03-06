# Quadra - a multiplayer action puzzle game
# Copyright (C) 2000  Pierre Phaneuf
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
# 02111-1307, USA.
#
# $Id$

WADDER_OBJECTS:=$(patsubst %.cpp,%.o,$(wildcard skelton/tools/wadder/*.cpp))
WADDER_OBJECTS+= \
	skelton/common/resfile.o \
	skelton/common/resmanager.o \
	skelton/common/reswriter.o \
	skelton/common/stringtable.o \
	skelton/svgalib/error.o \
	skelton/svgalib/res.o

CLEAN+=$(WADDER_OBJECTS) skelton/tools/wadder/wadder

CXXDEPS+=$(wildcard skelton/tools/wadder/*.cpp)

