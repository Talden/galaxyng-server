/* eval.cpp -- implement the eval dynamically loadable builtin
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

#define builtin_table	eval_LTX_builtin_table

/* Generate prototype. */
GNGSERVER_BUILTIN(builtin_eval);

Builtin builtin_table[] = {
  { "eval", builtin_eval, 1, -1 },
  { 0, 0, -1, -1 }
};

BUILTIN_DECLARATION (eval)
{
  int status = GNGSERVER_ERROR;
  Tokens *tokens;
  char *command;

  {
    Tokens tmp;
    tmp.argv = (char **) &argv[1];
    tmp.argc = argc -1;
    tmp.lim  = 0;
  
    status = untokenize (gngserver, &command, &tmp);
  }

  if (status == GNGSERVER_OKAY)
    status = tokenize (gngserver, &tokens, &command);

  if (status == GNGSERVER_INCOMPLETE) {
    gngserver_result_set (gngserver, "incomplete command", -1);
    status = GNGSERVER_ERROR;
    goto error;
  }

  status = eval (gngserver, tokens);

 error:
  free (*tokens->argv);
  XFREE (tokens->argv);
  XFREE (tokens);
  XFREE (command);
  
  return status;
}
