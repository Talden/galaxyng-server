/* unknown.cpp -- implement the unknown dynamically loadable builtin
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

#include <sys/types.h>
#include <sys/wait.h>
#include <gngserver/module.h>

#define builtin_table   unknown_LTX_builtin_table

static char *path_find  (const char *command);
static int path_execute (GNGServer *gngserver, const char *path,
			 char *const argv[]);

/* Generate prototype. */
GNGSERVER_BUILTIN (builtin_unknown);

Builtin builtin_table[] = {
  { "unknown", builtin_unknown, 0, -1 },
  { 0, 0, -1, -1 }
};

BUILTIN_DECLARATION(unknown)
{
  char *path = path_find (argv[0]);
  int status = GNGSERVER_ERROR;

  if (!path)
    gngserver_result_append (gngserver, "command \"", argv[0], "\" not found",
			     NULL);
  else if (path_execute (gngserver, path, argv) != GNGSERVER_OKAY)
    gngserver_result_append (gngserver, "command \"", argv[0],"\" failed: ",
			     strerror (errno), NULL);
  else
    status = GNGSERVER_OKAY;

  return status;
}

static char *
path_find (const char *command)
{
  char *path = xstrdup (command);
  
  if (*command == '/') {
    if (access (command, X_OK) < 0)
      goto notfound;
  }
  else {
    char *PATH = getenv ("PATH");
    char *pbeg, *pend;
    size_t len;
    
    for (pbeg = PATH; *pbeg != '\0'; pbeg = pend) {
      pbeg += strspn (pbeg, ":");
      len = strcspn (pbeg, ":");
      pend = pbeg + len;
      path = XREALLOC (char, path, 2 + len + strlen(command));
      *path = '\0';
      strncat (path, pbeg, len);
      if (path[len -1] != '/') strcat (path, "/");
      strcat (path, command);
      
      if (access (path, X_OK) == 0)
	break;
    }
    
    if (*pbeg == '\0')
      goto notfound;
  }
  
  return path;
  
 notfound:
  XFREE (path);
  return NULL;
}  

static int
path_execute (GNGServer *gngserver, const char *path, char *const argv[])
{
  int status = GNGSERVER_OKAY;        /* assume successful for now */
  pid_t pid;

  pid = fork ();
  switch (pid) {
  case -1:                    /* fork failed */
    status = GNGSERVER_ERROR;
    break;

  case 0:
    execv (path, argv);
    _exit (1);                /* execv in child failed */
    break;

  default:                    /* wait for child to exit */
    {
      int stat;
      if (waitpid (pid, &stat, 0) == pid) {
	if (WIFSIGNALED (stat)
	    || (WIFEXITED (stat) && (WEXITSTATUS (stat) != 0)))
	  status = GNGSERVER_ERROR;
      }
      else
	status = GNGSERVER_ERROR;
    }
    break;
  }
  
  return status;
}
