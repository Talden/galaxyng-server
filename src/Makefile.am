## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright 2004 Kenneth D. Weinert
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

INCLUDES	= -I$(top_builddir) -I$(top_builddir)/gngserver \
		-I$(top_srcdir) -I$(top_srcdir)/gngserver

if WITH_READLINE
LIBADD_READLINE = -lreadline
else
LIBADD_READLINE =
endif

noinst_HEADERS	= gngserver_repl.h

bin_PROGRAMS	= gngserver
gngserver_LDADD	= $(top_builddir)/gngserver/libgngserver.la $(LIBADD_READLINE) @LIBADD_DL@
gngserver_SOURCES	= gngserver.cpp gngserver_builtin.cpp gngserver_repl.cpp gngserver_syntax.cpp
gngserver_LDFLAGS	= -no-undefined -dlpreopen force -dlopen self  \
		-dlopen ${top_builddir}/modules/cd.la \
		-dlopen ${top_builddir}/modules/dollar.la \
		-dlopen ${top_builddir}/modules/eval.la \
		-dlopen ${top_builddir}/modules/sqrt.la \
		-dlopen ${top_builddir}/modules/unknown.la
