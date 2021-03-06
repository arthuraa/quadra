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

LINK.o=$(LINK.cc)

CLEAN+=config/depends.mk
DISTCLEAN+=config/config.mk ChangeLog.bak gmon.out
REALCLEAN+=ChangeLog

TARGETS+=Quadra.desktop

CXXFLAGS+=-Wall -pipe -Iinclude -Iskelton/include -Iimages -DLIBGAMESDIR=\"$(libgamesdir)\" -DDATAGAMESDIR=\"$(datagamesdir)\"

ARFLAGS=rcs

# FIXME: this should be the reverse, version.h should get values from
# this makefile using some -D options to the compiler.
MAJOR:=$(shell grep 'VERSION_MAJOR' include/version.h | cut -d" " -f3 | bc)
MINOR:=$(shell grep 'VERSION_MINOR' include/version.h | cut -d" " -f3 | bc)
PATCHLEVEL:=$(shell grep 'VERSION_PATCHLEVEL' include/version.h | cut -d" " -f3 | bc)
VERSION_EXTRA:=$(shell grep 'VERSION_EXTRA' include/version.h | cut -d" " -f3 | bc)
VERSION:=$(MAJOR).$(MINOR).$(PATCHLEVEL)$(VERSION_EXTRA)

CXXFLAGS+=-DVERSION="$(VERSION)" -DVERSION_MAJOR=$(MAJOR) -DVERSION_MINOR=$(MINOR) -DVERSION_PATCHLEVEL=$(PATCHLEVEL)

