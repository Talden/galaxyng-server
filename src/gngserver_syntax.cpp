/* gngserver_syntax.cpp -- syntax callbacks for gngserver shell
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

#define CHUNK_SIZE	256

/* List of builtin syntax. */
#define syntax_functions		\
	SYNTAX(escape,	"\\")		\
	SYNTAX(space,	" \f\n\r\t\v")	\
	SYNTAX(comment,	"#")		\
	SYNTAX(string,	"\"")		\
	SYNTAX(endcmd,	";")		\
	SYNTAX(endstr,	"")

int syntax_init   (GNGServer *gngserver);
int syntax_finish (GNGServer *gngserver, BufferIn *in, BufferOut *out);

#define SYNTAX(name, string)		\
	extern GNGSERVER_SYNTAX(CONC(syntax_, name));
syntax_functions
#undef SYNTAX

/* Syntax handler mappings. */
Syntax syntax_table[] = {

#define SYNTAX(name, string)		\
	{ CONC(syntax_, name), string },
  syntax_functions
#undef SYNTAX
  
  { NULL, NULL }
};

int
syntax_init (GNGServer *gngserver)
{
  int *pinstring = (int*)XMALLOC (int, 1);
  *pinstring = 0;
  
  gngserverstate_set (gngserver, "::syntax::instring", pinstring, free);
  
  return GNGSERVER_OKAY;
}

int
syntax_finish (GNGServer *gngserver, BufferIn *in, BufferOut *out)
{
  if (in->start && &in->buf.start[in->buf.i] != in->start) {
    /* Record the pending entry from the calling loop. */
    out->offv = XREALLOC (int, out->offv, 1 + out->offc);
    out->offv[out->offc] = out->argind;
    out->buf.start[out->buf.i] = 0;
    ++out->offc;
  }

  return GNGSERVER_OKAY;
}    

SYNTAX_DECLARATION (escape)
{
  if (in->buf.start[1 + in->buf.i] == '\n') {
    /* escaped newlines are treated as if they weren't there */
    if (in->start == in->buf.start + in->buf.i)
      in->start += 2;
    in->buf.i += 2;
  }
  else {
    /* skip the escape character, and copy the escaped character */
    ++in->buf.i;
    out->buf.start[out->buf.i++] = in->buf.start[in->buf.i++];
  }

  return GNGSERVER_CONTINUE;
}

SYNTAX_DECLARATION (space)
{
  int *pinstring = (int *)gngserverstate_get (gngserver, "::syntax::instring");

  if (*pinstring)
    /* just copy spaces inside strings */
    out->buf.start[out->buf.i++] = in->buf.start[in->buf.i++];
  else if (in->start == in->buf.start + in->buf.i) {
    /* ignore leading whitespace at the start of an arg */
    ++in->start;
    ++in->buf.i;
  }
  else {
    /* otherwise save the previous argument offset */
    out->offv = XREALLOC (int, out->offv, 1 + out->offc);
    out->offv[out->offc] = out->argind;
    out->buf.start[out->buf.i] = 0;
    out->argind = ++out->buf.i;
    ++out->offc;
    in->start = &in->buf.start[++in->buf.i];
  }

  return GNGSERVER_CONTINUE;
}

SYNTAX_DECLARATION (comment)
{
  /* advance to the end of the line. */
  while (in->buf.start[++in->buf.i] && in->buf.start[in->buf.i] != '\n')
    /*NOWORK*/;

  return GNGSERVER_CONTINUE;
}

SYNTAX_DECLARATION (string)
{
  int *pinstring = (int*)gngserverstate_get (gngserver, "::syntax::instring");

  /* Toggle the INSTRING flag, and advance past the quote mark. */
  *pinstring = !*pinstring;
  ++in->buf.i;

  return GNGSERVER_CONTINUE;
}

SYNTAX_DECLARATION (endcmd)
{
  int *pinstring = (int*)gngserverstate_get (gngserver, "::syntax::instring");

  if (!*pinstring) {
    ++in->buf.i;
    return GNGSERVER_OKAY;
  }

  out->buf.start[out->buf.i++] = in->buf.start[in->buf.i++];
  return GNGSERVER_CONTINUE;
}

SYNTAX_DECLARATION (endstr)
{
  int *pinstring = (int*)gngserverstate_get (gngserver, "::syntax::instring");

  if (*pinstring)
    return GNGSERVER_INCOMPLETE;

  return GNGSERVER_OKAY;
}
