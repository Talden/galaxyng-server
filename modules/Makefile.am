## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright 2005 Kenneth D. Weinert 
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

MAINTAINERCLEANFILES = Makefile.in

INCLUDES        = -I$(top_builddir) -I$(top_srcdir) \
                -I$(top_builddir)/sic -I$(top_srcdir)/sic \
                -I$(top_builddir)/src -I$(top_srcdir)/src

CPPFLAGS = -O3 -Wall -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -shared -fpic

EGP_SRC = ExampleGenericPlugin.cpp
EGP_OBJ = $(EGP_SRC:.cpp=.o)
EGP_SO  = ExampleGenericPlugin.so

modules : $(EGP_SO)
	$(CPP) $(CPPFLAGS) -o $(EGP_SO)
