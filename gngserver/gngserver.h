/* gngserver.h -- create and maintain GNGServer ADT
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

#if !defined(GNGS_SERVER_H)
#define GNGS_SERVER_H 1

#include <gngserver/common.h>
#include <gngserver/error.h>

enum {
  GNGS_ERROR = -1,
  GNGS_OKAY = 0,
  GNGS_INCOMPLETE,
  GNGS_BREAK,
  GNGS_CONTINUE,
  GNGS_EXIT
};

struct builtintab;

typedef struct statedata {
  struct statedata *next;	/* so they can be chained */
  const char *key;		/* used as a key to find the right data */
  void *data;			/* associated state data */
  void (*destroy) (void *data);
} GNGServerState;

typedef struct gngserver {
  char *result;			/* result string */
  size_t len;			/* bytes used by result field */
  size_t lim;			/* bytes allocated to result field */
  struct builtintab *builtins;	/* tables of builtin functions */
  GNGServerState *state;	/* state data from syntax extensions */
} GNGServer;

extern GNGServer *gngserver_new(void);
extern GNGServer *gngserver_delete(GNGServer *stale);

extern int gngserverstate_set(GNGServer *gngserver, const char *key,
			      void *value, void (*destroy) (void *value));
extern void *gngserverstate_get(GNGServer *gngserver, const char *key);
extern int gngserverstate_clear(GNGServer *gngserver, const char *key);

extern GNGServer *gngserver_result_clear(GNGServer *gngserver);
extern const char *gngserver_result_get(GNGServer *gngserver);
extern GNGServer *gngserver_result_set(GNGServer *gngserver, const char *value,
				       size_t len);
extern GNGServer *gngserver_result_append (GNGServer *gngserver,
					   const char *value, ...);

#endif /* !GNGS_SIC_H */
