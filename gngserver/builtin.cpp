/* builtin.cpp -- installing and removing sic builtin commands
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "common.h"
#include "builtin.h"
#include "eval.h"

struct builtintab {
	struct builtintab *next;
	Builtin *table;
};

int
builtin_install (GNGServer *gngserver, Builtin *table)
{
	int status = GNGSERVER_ERROR;
  
	if (table) {
		BuiltinTab *memory = XMALLOC (BuiltinTab, 1);
		memory->next = gngserver->builtins;
		memory->table = table;
		gngserver->builtins = memory;
		status = GNGSERVER_OKAY;
    }

	return status;
}

int
builtin_remove (GNGServer *gngserver, Builtin *table)
{
	int status = GNGSERVER_ERROR;
  
	if (gngserver->builtins && table) {
		BuiltinTab *stale = NULL;

		if (gngserver->builtins->table == table) {
			stale = gngserver->builtins;
			gngserver->builtins = gngserver->builtins->next;
			status = GNGSERVER_OKAY;
		}
		else {
			BuiltinTab *p;
	  
			for (p = gngserver->builtins; p->next; p = p->next)
				if (p->next->table == table) {
					stale = p->next;
					p->next = p->next->next;
					status = GNGSERVER_OKAY;
					break;
				}
		}

		XFREE (stale);
    }
  
	return status;
}

Builtin *
builtin_find (GNGServer *gngserver, const char *name)
{
	if (gngserver->builtins) {
		BuiltinTab *p;
		for (p = gngserver->builtins; p; p = p->next) {
			int i;
			for (i = 0; p->table[i].name; ++i) {
				if (strcmp (p->table[i].name, name) == 0)
					return &p->table[i];
			}
		}
    }

	return NULL;
}