/* eval.cpp -- create and dispatch command vectors
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
#include "gngserver/builtin.h"
#include "gngserver/Diagnostic.h"
#include "gngserver/eval.h"
#include "gngserver/gngserver.h"
#include "gngserver/syntax.h"

static void bufferin_init  (BufferIn *in, char *command);
static void bufferout_init (BufferOut *out, size_t lim);

int
eval (GNGServer *gngserver, Tokens *tokens)
{
  Builtin *builtin;
  
  GNGS_ASSERT (tokens && tokens->argv && tokens->argv[0]);

  builtin = builtin_find (gngserver, tokens->argv[0]);

  if (!builtin)
    builtin = builtin_find (gngserver, "unknown");

  if (!builtin) {
    gngserver_result_clear (gngserver);
    gngserver_result_append (gngserver, "command \"", tokens->argv[0],
			     "\" not found", NULL);
    return GNGSERVER_ERROR;
  }

  /* The table expresses valid counts of arguments, not including
     the command itself, hence the `-1'.  */
  if (tokens->argc -1 < builtin->min ||
      (tokens->argc -1> builtin->max && builtin->max != -1)) {
    gngserver_result_set (gngserver, "wrong number of arguments", 0);
    return GNGSERVER_ERROR;
  }

  return (*builtin->func) (gngserver, tokens->argc, tokens->argv);
}

int
untokenize (GNGServer *gngserver, char **pcommand, Tokens *tokens)
{
  char *command = NULL;
  int i, off;

  GNGS_ASSERT (tokens->argc);

  if (tokens->lim < 1) {
    for (tokens->lim = i = 0; tokens->argv[i]; ++i)
      tokens->lim += 1 + strlen (tokens->argv[i]);
  }

  command = (char*)XMALLOC (char, tokens->lim);
  
  for (off = i = 0; tokens->argv[i]; ++i) {
    sprintf (&command[off], "%s ", tokens->argv[i]);
    off += 1 + strlen (tokens->argv[i]);
  }
  command[off -1] = 0;
  
  *pcommand = command;

  return GNGSERVER_OKAY;
}

int
tokenize (GNGServer *gngserver, Tokens **ptokens, char **pcommand)
{
  BufferIn in;
  BufferOut out;
  List *head;
  int status = GNGSERVER_CONTINUE;

  GNGS_ASSERT (gngserver->syntax);

  bufferin_init (&in, *pcommand);
  bufferout_init (&out, in.buf.lim);

  /* Perform any user initialisation for syntax readers. */
  for (head = gngserver->syntax_init; head; head = head->next)
    (*(SyntaxInit *) head->userdata) (gngserver);
  std::cerr << "syntax init completed" << std::endl;
  /* Dispatch to handlers by syntax class of character, or
     simply copy from input to output by default. */
  while (status == GNGSERVER_CONTINUE) {
    SyntaxHandler *handler = syntax_handler(gngserver, in.buf.start[in.buf.i]);

    if (handler) {
      std::cerr << "we have a handler " << std::endl;
      
      status = (*handler) (gngserver, &in, &out);
    }
    else {
      std::cerr << "copying" << std::endl;
      out.buf.start[out.buf.i++] = in.buf.start[in.buf.i++];
    }
  }
  std::cerr << "handlers completed" << std::endl;
  /* Perform any client finalization for syntax reader. */
  for (head = gngserver->syntax_finish; head; head = head->next)
    (*(SyntaxFinish *) head->userdata) (gngserver, &in, &out);
  std::cerr << "syntax finishers completed" << std::endl;
  {
    /* Can't fill ARGV on the fly in case BUF moved during realloc. */
    Tokens *tokens = XMALLOC (Tokens, 1);
    
    tokens->argv = XMALLOC (char *, 1 + out.offc);

    for (tokens->argc = 0; tokens->argc < out.offc; ++tokens->argc)
      tokens->argv[tokens->argc] = out.buf.start + out.offv[tokens->argc];

    tokens->argv[tokens->argc] = NULL;
    tokens->lim  = out.buf.lim;

    if (!*tokens->argv)
      XFREE(out.buf.start);
    XFREE (out.offv);

    *pcommand += in.buf.i;
    *ptokens = tokens;
  }

  return status;
}

static void
bufferin_init (BufferIn *in, char *command)
{
  GNGS_ASSERT (in && command);
  
  in->buf.lim	= 1 + strlen (command);
  in->buf.start	= command;
  in->buf.i	= 0;

  in->start	= in->buf.start;
}

static void
bufferout_init (BufferOut *out, size_t lim)
{
  GNGS_ASSERT (out);
  
  out->buf.lim	= lim;
  out->buf.start= XMALLOC (char, lim);
  out->buf.i	= 0;
  
  out->offc	= 0;
  out->offv	= NULL;
  out->argind	= out->buf.i;
}
