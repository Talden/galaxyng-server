/* module.h -- dynamic module management: a wrapper for ltdl
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
/** @start 1 */
#ifndef GNGS_MODULE_H
#define GNGS_MODULE_H 1

#include <gngserver/common.h>
#include <gngserver/gngserver.h>

typedef void ModuleInit         (GNGServer *gngserver);
typedef void ModuleFinish       (GNGServer *gngserver);

extern const char *module_error (void);
extern int module_init          (void);
extern int module_load          (GNGServer *gngserver, const char *name);
extern int module_unload        (GNGServer *gngserver, const char *name);

#endif /* !GNGS_MODULE_H */
/** @end 1 */
