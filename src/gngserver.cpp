/* gngserver.c -- read commands, evaluate them and print the results
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

#include <signal.h>

#include "gngserver/common.h"
#include "gngserver/module.h"
#include "gngserver_repl.h"

#ifndef BUFSIZ
#  define BUFSIZ 256
#endif

#define GNGSERVERRCFILE	".gngserverrc"

static int evalgngserverrc (GNGServer *gngserver);

int
main (int argc, char *const argv[], char *const envp[])
{
  int result = EXIT_SUCCESS;
  FILE *in = stdin;
  GNGServer *gngserver = gngserver_new ();
  Diagnostic *dia = new Diagnostic("GNGServer");
  
  /* initialise the module subsystem */
  if (module_init () != GNGSERVER_OKAY)
    dia->Fatal(cerr, "module initialisation failed");

  if (module_load (gngserver, NULL) != GNGSERVER_OKAY)
    dia->Fatal(cerr, "gngserver initialisation failed");

  /* initial symbols */
  gngserverstate_set (gngserver, "PS1", "] ", NULL);
  gngserverstate_set (gngserver, "PS2", "- ", NULL);
  for (; *envp; ++envp) {
    size_t delim = strcspn (*envp, "=");
    char *name = XMALLOC (char, 1 + delim);

    strncpy (name, *envp, delim);
    name[delim] = 0;

    gngserverstate_set (gngserver, name, *envp + delim +1, NULL);
  }
  gngserverstate_set (gngserver, "SHELL", argv[0], NULL);
  gngserverstate_set (gngserver, "GNGSERVER_VERSION", VERSION, NULL);
  
  /* evaluate the startup file */
  evalgngserverrc (gngserver);
  
  /* Interactive implies unbuffered output and ignore interrupts. */
  if (argc > 1 && argv[1][0] != '-')
    in = fopen (argv[1], "rt");
  else if (isatty(0)) {
    signal (SIGINT, SIG_IGN);
    setbuf (stdout, NULL);
    is_interactive = 1;
  }

  /* evaluate the input stream */
  evalstream (gngserver, in);

  exit (result);
}

static int
evalgngserverrc (GNGServer *gngserver)
{
  int result = GNGSERVER_OKAY;
  char *home = getenv ("HOME");
  char *gngserverrcpath, *separator = "";
  int len;

  if (!home)
    home = "";

  len = strlen (home);
  if (len && home[len -1] != '/')
    separator = "/";

  len += strlen (separator) + strlen (GNGSERVERRCFILE) + 1;

  gngserverrcpath = XMALLOC (char, len);
  sprintf (gngserverrcpath, "%s%s%s", home, separator, GNGSERVERRCFILE);

  if (access (gngserverrcpath, R_OK) == 0)
    result = source (gngserver, gngserverrcpath);

  return result;
}

