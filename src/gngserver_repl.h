/* gngserver_repl.h -- read commands, evaluate them and print the results
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

#ifndef REPL_H
#define REPL_H 1

#include <gngserver/common.h>
#include <gngserver/gngserver.h>

extern int is_interactive;

extern int source     (GNGServer *gngserver, const char *path);
extern int evalstream (GNGServer *gngserver, FILE *stream);
extern int evalline   (GNGServer *gngserver, char **pline);

#endif /* !REPL_H */

