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

AUX_DIST		= $(ac_aux_dir)/config.guess \
			$(ac_aux_dir)/config.sub \
			$(ac_aux_dir)/install-sh \
			$(ac_aux_dir)/ltconfig \
			$(ac_aux_dir)/ltmain.sh \
			$(ac_aux_dir)/mdate-sh \
			$(ac_aux_dir)/missing \
			$(ac_aux_dir)/mkinstalldirs
AUX_DIST_EXTRA		= $(ac_aux_dir)/readline.m4 \
			$(ac_aux_dir)/sys_errlist.m4 \
			$(ac_aux_dir)/sys_siglist.m4

EXTRA_DIST		= bootstrap mkstamp

AUTOMAKE_OPTIONS 	= gnits
SUBDIRS 		= replace gngserver modules src tests doc
MAINTAINERCLEANFILES 	= Makefile.in aclocal.m4 configure config-h.in \
			stamp-h.in $(AUX_DIST)

ACLOCAL			= aclocal -I $(ac_aux_dir)

docdir			= $(top_builddir)/doc

html:
	@echo Making $@ in $(docdir)
	@cd $(docdir) && make $@

dist-hook:
	(cd $(distdir) && mkdir $(ac_aux_dir))
	for file in $(AUX_DIST) $(AUX_DIST_EXTRA); do \
	  cp $$file $(distdir)/$$file; \
	done
