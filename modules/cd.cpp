/* cd.cpp -- implement the `cd'  dynamically loadable builtin
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

#include <gngserver/module.h>

#define builtin_table	cd_LTX_builtin_table

/* Generate prototype. */
GNGSERVER_BUILTIN(builtin_cd);

Builtin builtin_table[] = {
  { "cd", builtin_cd, 0, 1 },
  { 0, 0, -1, -1 }
};

BUILTIN_DECLARATION(cd)
{
  const char *destination = argv[1];
  int status = GNGSERVER_ERROR;

  if (!destination)
    destination = (const char*)gngserverstate_get (gngserver, "HOME");
  
  if  (!destination)
      gngserver_result_set (gngserver, "unbound variable \"HOME\"", 0);
  else if (chdir (destination) < 0)
    {
      gngserver_result_clear (gngserver);
      gngserver_result_append (gngserver, destination, ": ", strerror (errno), NULL);
    }
  else
    status = GNGSERVER_OKAY;

  return status;
}
