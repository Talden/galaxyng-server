/* xmalloc.c -- memory management with out of memory reporting
   Copyright (C) 2000 Gary V. Vaughan
  
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

#ifndef WITH_DMALLOC

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "common.h"
#include "Diagnostic.h"

///\brief allocate memory and clear it with existence check
void *
xcalloc (size_t num, size_t size)
{

#if HAVE_CALLOC
  void *memory = calloc (num, size);
  if (!memory) {
    Diagnostic *dia = new Diagnostic("xcalloc");
    dia->Fatal (cerr, "Memory exhausted");
  }
#else
  void *memory = xmalloc (num * size);
  bzero (new, num * size);
#endif

  return memory;
}
     
///\brief allocate memory with existence check
void *
xmalloc (size_t num)
{
  void *memory = malloc (num);

  if (!memory) {
    Diagnostic *dia = new Diagnostic("xmalloc");
    dia->Fatal (cerr, "Memory exhausted");
  }

  return memory;
}

///\brief reallocate memory with existence check
void *
xrealloc (void *p, size_t num)
{
  void *memory;

  if (!p)
    return xmalloc (num);

  memory = realloc (p, num);

  if (!memory) {
    Diagnostic *dia = new Diagnostic("xrealloc");
    dia->Fatal (cerr, "Memory Exhausted");
  }

  return memory;
}

#endif /* !WITH_DMALLOC */
