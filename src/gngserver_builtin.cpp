/* gngserver_builtin.c -- bagngserver builtin commands loaded statically
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

#include "gngserver/module.h"
#include "gngserver_repl.h"

/* List of built in functions. */
#define builtin_functions               \
        BUILTIN(exit,           0, 1)   \
        BUILTIN(load,           1, 1)   \
        BUILTIN(source,         1, -1)  \
        BUILTIN(unload,         1, -1)


#define BUILTIN(name, min, max) \
        extern GNGSERVER_BUILTIN(CONC(builtin_, name));
builtin_functions
#undef BUILTIN

/* name to function map */
Builtin builtin_table[] = {

#define BUILTIN(name, min, max) \
        { STR(name), CONC(builtin_, name), (min), (max) },
  builtin_functions
#undef BUILTIN

  { NULL, NULL, -1, -1 }
};

BUILTIN_DECLARATION(exit)
{
  return GNGSERVER_EXIT;
}

BUILTIN_DECLARATION (load)
{
  int status = GNGSERVER_ERROR;

  if (module_load (gngserver, argv[1]) < 0) {
    gngserver_result_clear (gngserver);
    gngserver_result_append(gngserver, "module \"", argv[1], "\" not loaded: ",
			    module_error (), NULL);
  }
  else
    status = GNGSERVER_OKAY;

  return status;
}

BUILTIN_DECLARATION (source)
{
  int status = GNGSERVER_OKAY;
  int i;

  for (i = 1; status == GNGSERVER_OKAY && argv[i]; ++i)
    status = source (gngserver, argv[i]);

  return status;
}

BUILTIN_DECLARATION (unload)
{
  int status = GNGSERVER_ERROR;
  int i;

  for (i = 1; argv[i]; ++i) {
    if (module_unload (gngserver, argv[i]) != GNGSERVER_OKAY) {
      gngserver_result_clear (gngserver);
      gngserver_result_append (gngserver, "module \"", argv[1],
			       "\" not unloaded: ", module_error (), NULL);
    }
    else
      status = GNGSERVER_OKAY;
  }

  return status;
}
