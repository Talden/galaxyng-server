/* syntax.h -- create and administer syntax tables
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

#ifndef SYNTAX_H
#define SYNTAX_H 1

#include <gngserver/common.h>
#include <gngserver/ltdl.h>

struct gngserver;

/* Prototype Generator. */
#define GNGSERVER_SYNTAX(name)					\
	int name (GNGServer *gngserver, BufferIn *in, BufferOut *out)
			
/* Declaration generator. */
#define SYNTAX_DECLARATION(name)				\
	int							\
	CONC(syntax_, name) (struct gngserver *gngserver, 			\
			     BufferIn *in,			\
			     BufferOut *out)

typedef struct {
  char *start;			/* start of buffered string */
  size_t i;			/* offset to next unused char */
  size_t lim;			/* upper limit for I */
} Buffer;

typedef struct {
  Buffer buf;			/* the buffer details */
  char *start;			/* start of the current command */
} BufferIn;

typedef struct {
  Buffer buf;			/* the buffer details */
  int *offv;			/* vector of offsets */
  size_t offc;			/* count of offsets in OFFV */
  size_t argind;		/* offset to start of current argument */
} BufferOut;

typedef int SyntaxHandler (struct gngserver *gngserver,
			   BufferIn *in, BufferOut *out);
typedef int SyntaxInit    (struct gngserver *gngserver);
typedef int SyntaxFinish  (struct gngserver *gngserver,
			   BufferIn *in, BufferOut *out);

typedef struct syntaxnode SyntaxTable;

typedef struct syntax {
  SyntaxHandler *handler;
  char *ch;
} Syntax;

extern int syntax_install (struct gngserver *gngserver, lt_dlhandle module,
			   Syntax *table);
extern int syntax_remove  (struct gngserver *gngserver, lt_dlhandle module,
			   Syntax *table);
extern SyntaxHandler *syntax_handler (struct gngserver *gngserver, int ch);

#endif /* !SYNTAX_H */
