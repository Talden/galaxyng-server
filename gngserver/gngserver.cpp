/* gngserver.cpp -- create and maintain gngserver ADT
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
#include "gngserver.h"

#define CHUNK_SIZE	256

static GNGServerState *gngserverstate_find(GNGServer *gngserver,
					   const char *key);
	
GNGServer *
gngserver_new (void)
{
  GNGServer *memory = XCALLOC (GNGServer, 1);
  return memory;
}

GNGServer *
gngserver_delete (GNGServer *stale)
{
  GNGSERVER_ASSERT (stale);

  free (stale->result);
  free (stale);

  return 0;
}

int
gngserverstate_set(GNGServer *gngserver, const char *key, void *value,
		   void (*remove) (void *value))
{
  GNGServerState *state = gngserverstate_find (gngserver, key);

  if (state) {
    if (state->remove)
      (*state->remove) (state->data);
  }
  else {
    state = XMALLOC (GNGServerState, 1);

    state->next = gngserver->state;
    gngserver->state = state;

    state->key = (const char *) xstrdup (key);
  }

  state->data = value;
  state->remove = remove;

  return 0;
}

void *
gngserverstate_get(GNGServer *gngserver, const char *key)
{
  GNGServerState *state = gngserverstate_find (gngserver, key);
  return state ? state->data : NULL;
}

int
gngserverstate_clear(GNGServer *gngserver, const char *key)
{
  GNGServerState *state = gngserver->state;
  GNGServerState *stale = NULL;

  if (!state)
    return -1;

  if (strcmp (state->key, key) == 0) {
    stale = state;
    gngserver->state = state->next;
  }
  else {
    for (; state->next; state = state->next)
      if (strcmp (state->next->key, key) == 0) {
	stale = state->next;
	state->next = stale->next;
	break;
      }
  }

  if (!stale)
    return -1;

  if (stale->remove)
    (*stale->remove) (stale->data);
  XFREE (stale->key);
  XFREE (stale);

  return 0;
}

GNGServer *
gngserver_result_clear (GNGServer *gngserver)
{
  GNGS_ASSERT (gngserver);

  gngserver->len = 0;
  if (gngserver->result)
    gngserver->result[0] = 0;
  else
    gngserver->result = 0;
  
  return gngserver;
}

const char *
gngserver_result_get (GNGServer *gngserver)
{
  return gngserver->result;
}

GNGServer *
gngserver_result_set (GNGServer *gngserver, const char *value, size_t len)
{
  GNGS_ASSERT (gngserver);

  if (len < 0)
    len = strlen (value);
  
  gngserver_result_clear (gngserver);
  
  if (len < CHUNK_SIZE) {
    gngserver->lim = CHUNK_SIZE;
    gngserver->result = XREALLOC (char, gngserver->result, gngserver->lim);
  }
  
  return gngserver_result_append (gngserver, value, NULL);
}

GNGServer *
gngserver_result_append (GNGServer *gngserver, const char *value, ...)
{
  va_list ap;
  const char *argument = value;
  
  GNGSERVER_ASSERT (gngserver);
  
  if (!value) return gngserver;

  va_start (ap, value);
  while (argument)
    {
      size_t len = strlen (argument);
      
      if (gngserver->len + len >= gngserver->lim)
	{
	  gngserver->lim += CHUNK_SIZE;
	  gngserver->result = XREALLOC (char, gngserver->result, gngserver->lim);
	}

      strncpy (&gngserver->result[gngserver->len], argument, len);
      gngserver->len += len;
      gngserver->result[gngserver->len] = 0;

      argument = va_arg (ap, const char *);
    }
  va_end (ap);

  return gngserver;
}

static GNGServerState *
gngserverstate_find (GNGServer *gngserver, const char *key)
{
  GNGServerState *state;

  for (state = gngserver->state; state; state = state->next)
    if (strcmp (state->key, key) == 0)
      break;

  return state;
}
