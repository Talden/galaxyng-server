/* module.cpp -- dynamic module management: a wrapper for ltdl
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
#include "gngserver/eval.h"
#include "gngserver/ltdl.h"
#include "gngserver/module.h"
#include "gngserver/gngserver.h"

#ifndef GNGS_MODULE_PATH_ENV
#  define GNGS_MODULE_PATH_ENV   "GNGS_MODULE_PATH"
#endif

static int unload_ltmodule (lt_dlhandle module, lt_ptr_t data);
static int userdata_address_compare (List *elt, void *match);

static char multi_init_error[]
            = "module loader initialised more than once";
static char no_builtin_table_error[]
            = "module has no builtin or syntax table";
static char builtin_unload_error[]
            = "builtin table failed to unload";
static char syntax_unload_error[]
            = "syntax table failed to unload";
static char module_not_found_error[]
            = "no such module";
static char module_not_unloaded_error[]
            = "module not unloaded";

static const char *last_error = NULL;

const char *
module_error (void)
{
  return last_error;
}

int
module_init (void)
{
  static int initialized = 0;
  int errors = 0;

  /* Only perform the initialisation once. */
  if (!initialized) {
    /* ltdl should use the same mallocation as us. */
    lt_dlmalloc = (lt_ptr_t (*) (size_t)) xmalloc;
    lt_dlfree = (void (*) (lt_ptr_t)) free;

    /* Make sure preloaded modules are initialised. */
    LTDL_SET_PRELOADED_SYMBOLS();

    last_error = NULL;

    /* Call ltdl initialization function. */
    errors = lt_dlinit();


    /* Set up the module search directories. */
    if (errors == 0) {
      const char *path = getenv (GNGS_MODULE_PATH_ENV);
      if (path != NULL)
	errors = lt_dladdsearchdir(path);
      
    }

    if (errors == 0)
      errors = lt_dladdsearchdir(MODULE_PATH);

    
    if (errors != 0)
      last_error = lt_dlerror ();

    ++initialized;

    return errors ? GNGSERVER_ERROR : GNGSERVER_OKAY;
    }

  last_error = multi_init_error;
  return GNGSERVER_ERROR;
}

int
module_load (GNGServer *gngserver, const char *name)
{
  lt_dlhandle module;
  Builtin *builtin_table;
  Syntax  *syntax_table;
  int status = GNGSERVER_OKAY;

  std::cerr << "> module_load(\"";
  if (name)
    std::cerr << name << "\")" << std::endl;
  else
    std::cerr << "\")" << std::endl;

  last_error = NULL;

  module = lt_dlopenext (name);

  if (module) {
    builtin_table = (Builtin *) lt_dlsym(module, "builtin_table");
    syntax_table = (Syntax *)lt_dlsym(module, "syntax_table");

    std::cerr << "builtin_table: " << builtin_table << std::endl;
    std::cerr << "syntax_table: " << syntax_table << std::endl;
    if (!builtin_table && !syntax_table) {
      lt_dlclose(module);
      last_error = no_builtin_table_error;
      module = NULL;
    }
  }

  if (module) {
    ModuleInit *init_func
      = (ModuleInit *) lt_dlsym (module, "module_init");
    if (init_func)
      (*init_func) (gngserver);
  }

  if (module) {
    SyntaxFinish *syntax_finish
      = (SyntaxFinish *) lt_dlsym (module, "syntax_finish");
    SyntaxInit *syntax_init
      = (SyntaxInit *) lt_dlsym (module, "syntax_init");

    if (syntax_finish)
      gngserver->syntax_finish = list_cons (list_new ((void*)syntax_finish),
					    gngserver->syntax_finish);
    if (syntax_init)
      gngserver->syntax_init = list_cons (list_new ((void*)syntax_init),
					  gngserver->syntax_init);
  }

  std::cerr << "  module: " << module << "  builtin_table: " << builtin_table
	    << "  syntax_table: " << syntax_table << std::endl;

  if (module) {
    if (builtin_table) {
      std::cerr << "  calling builtin_install(" << gngserver << ", "
		<< builtin_table << ")" << std::endl;

      status = builtin_install (gngserver, builtin_table);
    }

    if (syntax_table && status == GNGSERVER_OKAY) {
      std::cerr << "  calling syntax_install(" << gngserver << ", "
		<< module << ", " << syntax_table << ")" << std::endl;
      status = syntax_install (gngserver, module, syntax_table);
    }
    std::cerr << "> module_load // " << status << std::endl;
    return status;
  }

  last_error = lt_dlerror();
  if (!last_error)
    last_error = module_not_found_error;

  std::cerr << "> module_load // ERROR" << std::endl;
  return GNGSERVER_ERROR;
}

struct unload_data {GNGServer *gngserver; const char *name; };

int
module_unload (GNGServer *gngserver, const char *name)
{
  struct unload_data data;

  last_error = NULL;

  data.gngserver = gngserver;
  data.name = name;

  /* Stopping might be an error, or we may have unloaded the module. */
  if (lt_dlforeach (unload_ltmodule, (lt_ptr_t) &data) != 0)
    if (!last_error)
      return GNGSERVER_OKAY;

  if (!last_error)
    last_error = module_not_found_error;
    
  return GNGSERVER_ERROR;
}

/* This callback returns `0' if the module was not yet found.
   If there is an error, LAST_ERROR will be set, otherwise the
   module was successfully unloaded. */
static int
unload_ltmodule (lt_dlhandle module, void *data)
{
  struct unload_data *unload = (struct unload_data *) data;
  const lt_dlinfo *module_info = lt_dlgetinfo (module);

  if ((unload == NULL)
      || (unload->name == NULL)
      || (module_info == NULL)
      || (module_info->name == NULL)
      || (strcmp (module_info->name, unload->name) != 0))
    {
      /* No match, return 0 to keep searching */
      return 0;
    }
  
  if (module) {
    /* Fetch the addresses of the entrypoints into the module. */
    Builtin *builtin_table
      = (Builtin*) lt_dlsym (module, "builtin_table");
    Syntax *syntax_table
      = (Syntax *) lt_dlsym (module, "syntax_table");
    void *syntax_init_address
      = (void *) lt_dlsym (module, "syntax_init");
    void **syntax_finish_address
      = (void **) lt_dlsym (module, "syntax_finish");
    List *stale;

    /* Remove all references to these entry points in the internal
       data structures, before actually unloading the module. */
    stale = list_remove (&unload->gngserver->syntax_init,
			 syntax_init_address, userdata_address_compare);
    XFREE (stale);
        
    stale = list_remove (&unload->gngserver->syntax_finish,
			 syntax_finish_address, userdata_address_compare);
    XFREE (stale);

    if (builtin_table &&
	builtin_remove (unload->gngserver, builtin_table) != GNGSERVER_OKAY) {
      last_error = builtin_unload_error;
      module = NULL;
    }

    if (syntax_table &&
	GNGSERVER_OKAY != syntax_remove (unload->gngserver, module,
					 syntax_table)) {
      last_error = syntax_unload_error;
      module = NULL;
    }
  }
  
  if (module) {
    ModuleFinish *finish_func
      = (ModuleFinish *) lt_dlsym (module, "module_finish");

    if (finish_func)
      (*finish_func) (unload->gngserver);
  }

  if (module) {
    if (lt_dlclose (module) != 0)
      module = NULL;
  }

  /* No errors?  Stop the search! */
  if (module)
    return 1;
      
  /* Find a suitable diagnostic. */
  if (!last_error)
    last_error = lt_dlerror();
  if (!last_error)
    last_error = module_not_unloaded_error;
      
  /* Error diagnosed.  Stop the search! */
  return -1;
}

static int
userdata_address_compare (List *elt, void *match)
{
  return (int) ((char*)elt->userdata - (char*)match);
}
