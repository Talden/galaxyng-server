## readline.m4 -- provide and handle --with-readline configure option
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

# serial 1 GNGS_WITH_READLINE

AC_DEFUN(GNGS_WITH_READLINE,
[AC_ARG_WITH(readline,
[  --with-readline         compile with the system readline library],
[if test x"${withval-no}" != no; then
  gngs_save_LIBS=$LIBS
  AC_CHECK_LIB(readline, readline)
  if test x"${ac_cv_lib_readline_readline}" = xno; then
    AC_MSG_ERROR(libreadline not found)
  fi
  LIBS=$gngs_save_LIBS
fi])
AM_CONDITIONAL(WITH_READLINE, test x"${with_readline-no}" != xno)
])
