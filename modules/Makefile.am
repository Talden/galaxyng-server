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

INCLUDES        = -I$(top_builddir) -I$(top_srcdir) \
                -I$(top_builddir)/gngserver -I$(top_srcdir)/gngserver \
                -I$(top_builddir)/src -I$(top_srcdir)/src

pkglib_LTLIBRARIES = unknown.la

pkglib_LTLIBRARIES += cd.la dollar.la eval.la sqrt.la

cd_la_SOURCES = cd.cpp
cd_la_LDFLAGS = -no-undefined -module -avoid-version
cd_la_LIBADD  = $(top_builddir)/gngserver/libgngserver.la

dollar_la_SOURCES = dollar.cpp
dollar_la_LDFLAGS = -no-undefined -module -avoid-version
dollar_la_LIBADD  = $(top_builddir)/gngserver/libgngserver.la

eval_la_SOURCES = eval.cpp
eval_la_LDFLAGS = -no-undefined -module -avoid-version
eval_la_LIBADD  = $(top_builddir)/gngserver/libgngserver.la

sqrt_la_SOURCES = sqrt.cpp
sqrt_la_LDFLAGS = -no-undefined -module -avoid-version
sqrt_la_LIBADD  = $(top_builddir)/gngserver/libgngserver.la -lm

unknown_la_SOURCES = unknown.cpp
unknown_la_LDFLAGS = -no-undefined -module -avoid-version
unknown_la_LIBADD  = $(top_builddir)/gngserver/libgngserver.la
