/* builtin.h -- installing and removing gngserver builtin commands
   Copyright 2004 Kenneth D. Weinert
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef BUILTIN_H
#define BUILTIN_H 1

#include <gngserver/common.h>
#include <gngserver/gngserver.h>
#include <gngserver/eval.h>

/* Prototype Generator. */
#define GNGSERVER_BUILTIN(name)       				\
	int name (GNGServer *gngserver, int argc, char *const argv[])

/* Declaration generator. */
#define BUILTIN_DECLARATION(name)				\
	int														\
	CONC(builtin_, name) (gngserver, argc, argv)			\
         GNGServer *gngserver;										\
	     int argc;						\
	     char *const argv[];

typedef int (*builtin_handler)(GNGServer *gngserver, int argc, char *const argv[]);

typedef struct {
	const char *name;
	builtin_handler func;
	int min, max;
} Builtin;

typedef struct builtintab BuiltinTab;

extern Builtin *builtin_find(GNGServer *gngserver, const char *name);
extern int builtin_install(GNGServer *gngserver, Builtin *table);
extern int builtin_remove(GNGServer *gngserver, Builtin *table);

#endif /* !BUILTIN_H */
