/* eval.h -- create and dispatch command vectors
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

#ifndef EVAL_H
#define EVAL_H 1

#include <gngserver/common.h>
#include <gngserver/gngserver.h>

/* A special property of TOKENS structures is that all of the pointers
   in ARGV point into a single block of memory which are freed all at
   once by freeing the first element. */
typedef struct {
	int  argc;			/* number of elements in ARGV */
	char **argv;			/* array of pointers to elements */
	size_t lim;			/* number of bytes allocated */
} Tokens;

extern int eval(GNGServer *gngserver, Tokens *tokens);
extern int untokenize(GNGServer *gngserver, char **pcommand, Tokens *tokens);
extern int tokenize(GNGServer *gngserver, Tokens **ptokens, char **pcommand);


#endif /* !EVAL_H */
