/* gngserver_repl.c -- read commands, evaluate them and print the results
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

#include "gngserver/common.h"

#ifdef HAVE_LIBREADLINE
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

#include "eval.h"
#include "gngserver_repl.h"

#ifndef BUFSIZ
#  define BUFSIZ 256
#endif

int is_interactive = 0;

static char *getline (FILE *in, const char *prompt);

int
source (GNGServer *gngserver, const char *path)
{
  FILE *stream;
  int result = GNGSERVER_OKAY;
  int save_interactive = is_interactive;

  GNGS_ASSERT (gngserver && path);
  
  is_interactive = 0;

  if ((stream = fopen (path, "rt")) == NULL) {
    gngserver_result_clear (gngserver);
    gngserver_result_append (gngserver, "cannot source \"", path, "\": ",
			     strerror (errno), NULL);
    result = GNGSERVER_ERROR;
  }
  else
    result =  evalstream (gngserver, stream);

  is_interactive = save_interactive;

  return result;
}

int
evalstream (GNGServer *gngserver, FILE *stream)
{
  int status = GNGSERVER_EXIT;
  char *line;

  GNGS_ASSERT (gngserver && stream);

  line = getline (stream, (char*)gngserverstate_get (gngserver, "PS1"));
  
  while (line) {
    status = evalline (gngserver, &line);
    
    switch (status) {
    case GNGSERVER_INCOMPLETE:
      {
	char *buf = xstrdup (line);
	int len = strlen (buf);
	
	line = getline (stream, (char*)gngserverstate_get (gngserver, "PS2"));
	len += strlen (line);
	
	buf = XREALLOC (char, buf, 2 + len);
	strcat (buf, "\n");
	strcat (buf, line);
	
	line = buf;
      }
      break;

    case GNGSERVER_EXIT:
      goto exit_gngserver;
      /*NOBREAK*/
      
    case GNGSERVER_ERROR:
      fprintf (stderr, "%s.\n", gngserver_result_get (gngserver));
      /*NOBREAK*/
      
    default:
      line = getline (stream, (char*)gngserverstate_get (gngserver, "PS1"));
      break;
    }
    
    gngserver_result_clear (gngserver);
  }
  
  if (is_interactive && !line)
    printf ("\n");
  
 exit_gngserver:
  return status;
}

int
evalline (GNGServer *gngserver, char **pline)
{
  int status = GNGSERVER_OKAY;
  char *command = *pline;
  
  while (status != GNGSERVER_INCOMPLETE && *command) {
    Tokens *tokens;
    
    status = tokenize (gngserver, &tokens, &command);
    
    if ((status == GNGSERVER_OKAY)
	&& tokens && tokens->argv && tokens->argv[0] && *tokens->argv[0])
      status = eval (gngserver, tokens);

    if (tokens) {
      free (*tokens->argv);
      XFREE (tokens->argv);
      XFREE (tokens);
    }
  }

  if (status != GNGSERVER_INCOMPLETE)
    *pline = command;
  
  return status;
}

static char *
getline (FILE *in, const char *prompt)
{
  int lim = BUFSIZ;
  int i = 0;
  int isdone = 0;

  static char *buf = NULL;	/* Always allocated and freed
				   from inside this function.  */
  XFREE (buf);

  if (is_interactive) {
    buf = (char *) readline ((char *) prompt);

#ifdef HAVE_ADD_HISTORY
    if (buf && *buf)
      add_history (buf);
#endif
      
    return buf;
  }
  
  buf = XMALLOC (char, lim);
      
  while (!isdone) {
    int c = getc (in);

    switch (c) {
    case EOF:
      isdone = EOF;
      break;

    case '\n':
      isdone = 1;
      break;
      
    default:
      if (i == lim) {
	lim *= 2;
	buf = XREALLOC (char, buf, lim);
      }
      buf[i++] = (char)c;
      break;
    }
  }
  buf[i] = 0;

  return isdone == EOF ? NULL : buf;
}
